#include "PST4NetServer.hpp"
#include <thread>
#include <iostream>

using namespace PST4;
using namespace RakNet;
using namespace std;
using namespace chrono;

NetworkServer::NetworkServer(unsigned long port) :
	running(true),
	port(port)
{
	cout << "Starting PST4::NetworkServer on port " << port << '\n';
	peer = RakPeerInterface::GetInstance();
	sd = SocketDescriptor(port, nullptr);
	peer->Startup(100, &sd, 1);
	peer->SetMaximumIncomingConnections(100);
	cout << "Server started. Can handle " << 100 << " clients\n";
}

NetworkServer::~NetworkServer()
{
	peer->Shutdown(500);
	RakPeerInterface::DestroyInstance(peer);
}

void NetworkServer::run()
{
	while (running) tick();
}

void NetworkServer::receivePackets()
{
	for (packet = peer->Receive(); packet;
		peer->DeallocatePacket(packet), packet = peer->Receive())
		processPacket();
}

void NetworkServer::sendPackets()
{
	for (auto clientPair = connectedClients.begin(); clientPair != connectedClients.end(); ++clientPair)
	{
		auto client = clientPair->second.get();
		headPosePacket head(client->getSessionId(), client->getHeadPos(), client->getHeadOrient());
		peer->Send(reinterpret_cast<char*>(&head), sizeof head, LOW_PRIORITY, UNRELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

void NetworkServer::processPacket()
{
	if (packet->data[0] > ID_USER_PACKET_ENUM) processGameMessage();
	else switch (packet->data[0])
	{
	case ID_NEW_INCOMING_CONNECTION:
		handleNewClient();
		break;
	case ID_DISCONNECTION_NOTIFICATION:
		handleClientDisconected();
		break;
	default:
		cerr << "Received packet with unimplemented ID = " << unsigned(packet->data[0]) << '\n';
	}
}

void NetworkServer::processGameMessage()
{
	auto strAddress = packet->systemAddress.ToString();
	auto client = connectedClients.at(strAddress).get();
	//cout << "Game message from client : " << strAddress << " ";
	switch (packet->data[0])
	{
	case ID_PST4_MESSAGE_ECHO:
		cout << "ECHO message : " << reinterpret_cast<echoPacket*>(packet->data)->message << "\n";
		break;
	case ID_PST4_MESSAGE_HEARTBEAT:
		cout << "Received heartbeat, ";
		cout << "time since last : " << duration_cast<seconds>(steady_clock::now() - client->getLastHreatbeatTimePoint()).count() << " sec.\n";
		client->heartbeat();
		cout << "last known head pose is : " << client->getHeadPos() << " " << client->getHeadOrient() << '\n';
		break;
	case ID_PST4_MESSAGE_HEAD_POSE:
	{
		auto headPose{ reinterpret_cast<headPosePacket*>(packet->data) };
		client->setHeadPose(headPose->absPos, headPose->absOrient);
		//cout << "head pose for " << client->getSessionId() << " : " << client->getHeadPos() << " " << client->getHeadOrient() << '\n';
	}
	break;
	default:
		cerr << "Received unimplemented GameMessage ID = " << unsigned(packet->data[0]) << " From " << packet->systemAddress.ToString() << '\n';
	}
}

void NetworkServer::handleNewClient()
{
	cout << "New client connected\n";
	std::string addrStr = packet->systemAddress.ToString();
	connectedClients[addrStr] = make_unique<GameClient>(packet->systemAddress);

	serverToClientIdPacket s2cID(connectedClients.at(addrStr)->getSessionId());

	peer->Send(reinterpret_cast<char*>(&s2cID), sizeof s2cID, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, false);
}

void NetworkServer::handleClientDisconected()
{
	cout << "Client disconnected\n";
	connectedClients.erase(packet->systemAddress.ToString());
}

void NetworkServer::tick()
{
	this_thread::sleep_for(milliseconds(5));
	receivePackets();
	sendPackets();
}
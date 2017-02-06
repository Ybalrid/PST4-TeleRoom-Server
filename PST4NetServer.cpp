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

		handPosePacket hand(client->getSessionId(), false);
		if (client->getLastHandPoseGood())
			hand = handPosePacket(client->getSessionId(), client->getLeftHandPos(), client->getLeftHandOrient(), client->getRightHandPos(), client->getrightHandOrient());
		peer->Send(reinterpret_cast<char*>(&hand), sizeof hand, LOW_PRIORITY, UNRELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
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
	case ID_CONNECTION_LOST: //Log the lost conection and let execute the next item to handle the disconnect
		std::cout << "lost connection with " << packet->systemAddress.ToString() << std::endl;
	case ID_DISCONNECTION_NOTIFICATION:
		handleClientDisconected();
		break;
	case ID_SND_RECEIPT_ACKED:
	{
		auto client = connectedClients[packet->systemAddress.ToString()].get();
		uint32_t ack;
		memcpy(&ack, packet->data + 1, 4);
		if (client->getAckFromServer() == ack)
		{
			std::cout << "Remote acked session id";
		}
	}break;
	default:
		cerr << "Received packet with unimplemented ID = " << unsigned(packet->data[0]) << '\n';
	}
}

void NetworkServer::processGameMessage()
{
	auto strAddress = packet->systemAddress.ToString();
	auto client = connectedClients.at(strAddress).get();
	uint32_t ack;
	//cout << "Game message from client : " << strAddress << " ";
	switch (packet->data[0])
	{
	case ID_PST4_MESSAGE_VOICE_BUFFER:
	{
		voicePacket* voice = reinterpret_cast<voicePacket*>(packet->data);
		//Calculate actual packet size:
		size_t size = voice->dataLen + sizeof voice->type + sizeof voice->sessionId + sizeof voice->frameSizes + sizeof voice->dataLen;
		peer->Send(reinterpret_cast<char*>(packet->data), size, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	case ID_PST4_MESSAGE_SESSION_ID:
	{
		serverToClientIdPacket s2cID(client->getSessionId());
		ack = peer->Send(reinterpret_cast<char*>(&s2cID), sizeof s2cID, IMMEDIATE_PRIORITY, RELIABLE_ORDERED_WITH_ACK_RECEIPT, 0, packet->systemAddress, false);
		client->setAckNumber(ack);
		break;
	}
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
	case ID_PST4_MESSAGE_HAND_POSE:
	{
		auto handPose{ reinterpret_cast<handPosePacket*>(packet->data) };
		client->setLastHandPoseGood(handPose->hasHands);
		if (client->getLastHandPoseGood()) client->setHandPose(handPose->leftPos, handPose->leftOrient, handPose->rightPos, handPose->rightOrient);
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

	auto ack = peer->Send(reinterpret_cast<char*>(&s2cID), sizeof s2cID, IMMEDIATE_PRIORITY, RELIABLE_ORDERED_WITH_ACK_RECEIPT, 0, packet->systemAddress, false);
	connectedClients[addrStr]->setAckNumber(ack);
}

void NetworkServer::handleClientDisconected()
{
	cout << "Client disconnected\n";
	auto sysAddressStr = packet->systemAddress.ToString();
	auto endedSessionId = connectedClients[sysAddressStr]->getSessionId();
	connectedClients.erase(sysAddressStr);

	sessionEndedPacket sessionEnded(endedSessionId);
	peer->Send(reinterpret_cast<char*>(&sessionEnded), sizeof sessionEnded, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkServer::tick()
{
	this_thread::sleep_for(milliseconds(5));
	receivePackets();
	sendPackets();
}
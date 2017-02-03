#include "PST4NetServer.hpp"
#include <thread>
#include <iostream>

using namespace PST4;

NetworkServer::NetworkServer(unsigned long port) :
	running(true),
	port(port)
{
	std::cout << "Starting PST4::NetworkServer on port " << port << '\n';
	peer = RakNet::RakPeerInterface::GetInstance();
	sd = RakNet::SocketDescriptor(port, nullptr);
	peer->Startup(100, &sd, 1);
	peer->SetMaximumIncomingConnections(100);
	std::cout << "Server started. Can handle " << 100 << " clients\n";
}

NetworkServer::~NetworkServer()
{
	peer->Shutdown(500);
	RakNet::RakPeerInterface::DestroyInstance(peer);
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
		std::cerr << "Received packet with unimplemented ID = " << unsigned(packet->data[0]) << '\n';
	}
}

void NetworkServer::processGameMessage()
{
	std::cout << "Game message from client : " << packet->systemAddress.ToString() << " ";
	switch (packet->data[0])
	{
	case ID_PST4_MESSAGE_ECHO:
		std::cout << "ECHO message : " << reinterpret_cast<echoPacket*>(packet->data)->message << "\n";
		break;
	case ID_PST4_MESSAGE_HEARTBEAT:
		std::cout << "Received heartbeat, should have sent ACK now\n";
		break;
	default:
		std::cerr << "Received unimplemented GameMessage ID = " << packet->data[0] << " From " << packet->systemAddress.ToString() << '\n';
	}
}

void NetworkServer::handleNewClient()
{
	std::cout << "New client connected\n";
}

void NetworkServer::handleClientDisconected()
{
	std::cout << "Client disconnected\n";
}

void NetworkServer::tick()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	receivePackets();
}
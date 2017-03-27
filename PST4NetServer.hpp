#pragma once

#include <exception>
#include <stdexcept>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <BitStream.h>
#include <PST4Packets.hpp>
#include <unordered_map>
#include <memory>
#include <chrono>

#include "Client.hpp"
#include "DynamicObject.hpp"


namespace PST4
{
	class NetworkServer
	{
	public:
		static constexpr size_t MAX_CLIENTS = 10;
		NetworkServer(unsigned long port);
		~NetworkServer();
		void run();
	private:
		void receivePackets();
		void sendPackets();
		void processPacket();
		void handleNewClient();
		void handleClientDisconected();

		void processGameMessage();

		void tick();
		bool running;
		unsigned long port;
		RakNet::RakPeerInterface* peer;
		RakNet::SocketDescriptor sd;
		RakNet::Packet* packet;

		std::unordered_map<std::string, std::unique_ptr<PST4::GameClient>> connectedClients;
		std::unordered_map<std::string, std::unique_ptr<PST4::DynamicObject>> dynamicObjects;

	};
}
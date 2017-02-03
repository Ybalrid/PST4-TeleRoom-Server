#pragma once

#include <exception>
#include <stdexcept>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <PST4Packets.hpp>

namespace PST4
{
	class NetworkServer
	{
	public:
		NetworkServer(unsigned long port);
		~NetworkServer();
		void run();
	private:
		void receivePackets();
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
	};
}

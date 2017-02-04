#pragma once

#include <RakPeerInterface.h>
#include <string>
#include <iostream>
#include <chrono>

#include <PST4Packets.hpp>

namespace PST4
{
	class GameClient
	{
	public:
		GameClient(RakNet::SystemAddress address);
		~GameClient();

		void heartbeat();

		std::string getAddress() const;
		RakNet::SystemAddress& getSystemAddress();
		std::chrono::steady_clock::time_point& getLastHreatbeatTimePoint();

		size_t getSessionId() const;

		void setHeadPose(Vect3f p, Quatf o);

		Quatf getHeadOrient() const;
		Vect3f getHeadPos() const;

		void setAckNumber(uint32_t ackFromServer);
		uint32_t getAckFromServer();

	private:
		std::chrono::time_point<std::chrono::steady_clock> lastHeartbeatTimePoint;
		RakNet::SystemAddress distantAddress;
		const size_t sessionId;

		Vect3f headPos;
		Quatf headOrient;
		uint32_t ackSessionId;

		bool clientOk;
	};
}
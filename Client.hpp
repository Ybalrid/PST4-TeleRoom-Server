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
		void setHandPose(Vect3f leftp, Quatf leftq, Vect3f rightp, Quatf rightq);

		Quatf getHeadOrient() const;
		Vect3f getHeadPos() const;

		Vect3f getLeftHandPos() const;
		Vect3f getRightHandPos() const;

		Quatf getLeftHandOrient() const;
		Quatf getrightHandOrient() const;

		void setAckNumber(uint32_t ackFromServer);
		uint32_t getAckFromServer() const;

		void setLastHandPoseGood(bool state);
		bool getLastHandPoseGood() const;

		bool isPhysicsMaster() const;
		void setPhysicsMaster(bool master = true);

	private:
		std::chrono::time_point<std::chrono::steady_clock> lastHeartbeatTimePoint;
		RakNet::SystemAddress distantAddress;
		const size_t sessionId;

		Vect3f headPos, leftHandPos, rightHandPos;
		Quatf headOrient, leftHandOrient, rightHandOrient;
		uint32_t ackSessionId;

		bool clientOk;
		bool lastHandPoseGood;
		bool physicsMaster;
	};
}
#include "Client.hpp"

PST4::GameClient::GameClient(RakNet::SystemAddress address) :
	distantAddress(address),
	sessionId(std::hash<std::string>()(distantAddress.ToString())),
	clientOk{ false },
	lastHandPoseGood(false),
	physicsMaster(false)
{
	std::cout << "Creating GameClient for " << getAddress() << '\n';
	lastHeartbeatTimePoint = std::chrono::steady_clock::now();
}

PST4::GameClient::~GameClient()
{
	std::cout << "Destructing GameClient for " << getAddress() << '\n';
}

void PST4::GameClient::heartbeat()
{
	lastHeartbeatTimePoint = std::chrono::steady_clock::now();
}

std::string PST4::GameClient::getAddress() const
{
	return distantAddress.ToString();
}

RakNet::SystemAddress& PST4::GameClient::getSystemAddress()
{
	return distantAddress;
}

std::chrono::steady_clock::time_point& PST4::GameClient::getLastHreatbeatTimePoint()
{
	return lastHeartbeatTimePoint;
}

size_t PST4::GameClient::getSessionId() const
{
	return sessionId;
}

void PST4::GameClient::setHeadPose(Vect3f p, Quatf o)
{
	headPos = p;
	headOrient = o;
}

void PST4::GameClient::setHandPose(Vect3f leftp, Quatf leftq, Vect3f rightp, Quatf rightq)
{
	leftHandPos = leftp;
	rightHandPos = rightp;
	leftHandOrient = leftq;
	rightHandOrient = rightq;
}

PST4::Quatf PST4::GameClient::getHeadOrient() const
{
	return headOrient;
}

PST4::Vect3f PST4::GameClient::getHeadPos() const
{
	return headPos;
}

PST4::Vect3f PST4::GameClient::getLeftHandPos() const
{
	return leftHandPos;
}

PST4::Vect3f PST4::GameClient::getRightHandPos() const
{
	return rightHandPos;
}

PST4::Quatf PST4::GameClient::getLeftHandOrient() const
{
	return leftHandOrient;
}

PST4::Quatf PST4::GameClient::getrightHandOrient() const
{
	return rightHandOrient;
}

void PST4::GameClient::setAckNumber(uint32_t ackFromServer)
{
	ackSessionId = ackFromServer;
}

uint32_t PST4::GameClient::getAckFromServer() const
{
	return ackSessionId;
}

void PST4::GameClient::setLastHandPoseGood(bool state)
{
	lastHandPoseGood = state;
}

bool PST4::GameClient::getLastHandPoseGood() const
{
	return lastHandPoseGood;
}

bool PST4::GameClient::isPhysicsMaster() const
{
	return physicsMaster;
}

void PST4::GameClient::setPhysicsMaster(bool master)
{
	if (master)
		std::cout << "Client " << sessionId << " is physics master now\n";
	physicsMaster = master;
}

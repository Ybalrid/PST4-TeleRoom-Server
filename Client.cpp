#include "Client.hpp"

PST4::GameClient::GameClient(RakNet::SystemAddress address) :
	distantAddress(address),
	sessionId(std::hash<std::string>()(distantAddress.ToString()))
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

PST4::Quatf PST4::GameClient::getHeadOrient() const
{
	return headOrient;
}

PST4::Vect3f PST4::GameClient::getHeadPos() const
{
	return headPos;
}
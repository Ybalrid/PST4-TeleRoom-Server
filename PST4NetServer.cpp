#include "PST4NetServer.hpp"

using namespace PST4;

NetworkServer::NetworkServer(unsigned long port) :
	running(true),
	port(port)
{
}

void NetworkServer::run()
{
	while (running) tick();
}

void NetworkServer::tick()
{
}
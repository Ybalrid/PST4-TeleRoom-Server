#pragma once

namespace PST4
{
	class NetworkServer
	{
	public:
		NetworkServer(unsigned long port);
		void run();
	private:
		void tick();
		bool running;
		unsigned long port;
	};
}
#include <iostream>
#include <string>

#include "PST4NetServer.hpp"
#include <memory>

using namespace std;

void pause(const string& message = "Press enter to continue...")
{
	cout << message << '\n';
	cin.get();
}

int main(int argc, char* argv[])
{
	//Just for debug : display all the args!
	for (auto i{ 0U }; i < unsigned(argc); ++i)
		cout << "arg : " << i << " : " << argv[i] << '\n';

	auto port = 42420UL;

	//If more than one arg, 2nd is the port number to listen to incoming TCP
	if (argc > 1)
		port = stoul(argv[1]);

	cout << "Net port to use is : " << port << '\n';

	//Start server here
	auto Server = make_unique<PST4::NetworkServer>(port);
	Server->run();

#ifdef _WIN32
	pause();
#endif
	return 0;
}
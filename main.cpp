#include <iostream>
#include <string>

using namespace std;

void pause(const string& message = "Press enter to continue...")
{
	cout << message << '\n';
	cin.get();
}

int main(int argc, char* argv[])
{
	//Just for debug : display all the args!
	for (auto i{ 0U }; i < argc; ++i)
		cout << "arg : " << i << " : " << argv[i] << '\n';

	auto port = 4242UL;

	//If more than one arg, 2nd is the port number to listen to incoming TCP
	if (argc > 1)
		port = stoul(argv[1]);

	cout << "Net port to use is : " << port << '\n';

	//Start server here

#ifdef _WIN32
	pause();
#endif
	return 0;
}
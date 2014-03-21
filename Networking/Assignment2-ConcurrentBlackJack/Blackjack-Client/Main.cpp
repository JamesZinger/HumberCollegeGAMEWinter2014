#include "BlackjackClient.h"

#include <iostream>
#include <limits>
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	string name, hostname, port;
	int iPort;
	cout << "Please enter your name: ";
	cin >> name;
	cout << endl;

	cout << "Please enter the host name of the black jack server: ";
	cin >> hostname;
	cout << endl;

	cout << "Please enter the port of the game you wish to join: ";
	cin >> port;
	cout << endl;
	iPort = atoi( port.c_str() );


	while ( true )
	{
		if (iPort > 0 && iPort < USHRT_MAX)
		{
			break;
		}
		else
		{
			port.clear();
			cout << "Invalid Port, Please enter a port value between 0-65535: ";
			cin >> port;
			cout << endl;
			iPort = atoi( port.c_str() );
		}
	}


	BlackjackClient* client = new BlackjackClient(name, hostname, iPort);

	client->Run();

	delete client;

	return 0;

}
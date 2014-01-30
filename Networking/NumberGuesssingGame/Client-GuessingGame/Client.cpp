#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8081"

using namespace std;

using std::cout;
using std::cin;

int __cdecl main( void )
{
	// Get Server Location
	cout << "Enter the server location (hostname/ip:port):\n";
	
	string hostName;

	int port = -1;

	do
	{

		string s;

		stringstream hoststream, sportstream;

		cin >> s;

		int colonIndex = -1;

		for ( int i = 0; i < s.length(); i++ )
		{
			if ( s[ i ] == ':' )
			{
				colonIndex = i;
				hostName = "";
				for ( int j = 0; j < colonIndex; j++ )
				{
					hoststream << s[ j ];
				}

				for ( int j = colonIndex + 1; j < s.length(); j++ )
				{
					sportstream << s[ j ];
				}

				hostName = hoststream.str();

				if ( !( sportstream >> port ) )
				{
					cout << "Invaild port name. Cannot be parsed to a number.\n";
					continue;
				}

				break;
			}
		}

		if ( colonIndex == -1 )
		{
			cout << "Could not find the colon delimiter, Please use the (hostname/ip:port) format:\n";
			continue;
		}

		break;

	} while ( true );

	cout << "Port: " << port << "\n";


	cout << "Host Name: " << hostName << "\n";
	
	char s [20];

	cin >> s;

	return 0;
}
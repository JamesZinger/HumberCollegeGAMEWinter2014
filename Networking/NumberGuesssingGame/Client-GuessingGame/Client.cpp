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

		if ( port == -1 )
		{
			continue;
		}

		break;

	} while ( true );

	cout << "Port: " << port << "\n";

	cout << "Host Name: " << hostName << "\n";

	SOCKET ListenSocket = INVALID_SOCKET,
		ClientSocket = INVALID_SOCKET;

	WSADATA wsaData;

	struct addrinfo *result = NULL,
		hints;

	char recvbuf[ DEFAULT_BUFLEN ];

	int iResult, iSendResult;

	iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

	if ( iResult != 0 )
	{
		printf( "WSAStartup Failed: %d\n", iResult );
		return 1;
	}

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo( NULL, DEFAULT_PORT, &hints, &result );

	if ( iResult != 0 )
	{
		printf( "getaddrinfo failed: %d\n", iResult );
		WSACleanup();
		return 1;
	}

	ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );

	if ( ListenSocket == INVALID_SOCKET )
	{
		printf( "socket failed: %ld\n", WSAGetLastError() );
		freeaddrinfo( result );
		WSACleanup();
		return 1;
	}

	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen );
	if ( iResult == SOCKET_ERROR )
	{
		printf( "bind failed: %d\n", WSAGetLastError() );
		freeaddrinfo( result );
		closesocket( ListenSocket );
		WSACleanup();
		return 1;
	}

	freeaddrinfo( result );

	iResult = listen( ListenSocket, SOMAXCONN );
	if ( iResult == SOCKET_ERROR )
	{
		printf( "listen failed: %d\n", WSAGetLastError() );
		closesocket( ListenSocket );
		WSACleanup();
		return 1;
	}

	char s[ 20 ];

	cin >> s;

	return 0;
}
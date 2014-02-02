#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

#define DEFAULT_BUFLEN 10000
#define DEFAULT_PORT "8081"

using namespace std;

using std::cout;
using std::cin;

int __cdecl main( void )
{
	// Get Server Location
	cout << "Enter the server location (hostname/ip:port):\n";

	string hostName;
	string fullHostName;
	string sPort;
	int port = -1;

	do
	{

		stringstream hoststream, sportstream, sPortStream;

		cin >> fullHostName;

		int colonIndex = -1;

		for ( int i = 0; i < fullHostName.length(); i++ )
		{
			if ( fullHostName[ i ] == ':' )
			{
				colonIndex = i;
				hostName = "";
				for ( int j = 0; j < colonIndex; j++ )
				{
					hoststream << fullHostName[ j ];
				}

				for ( int j = colonIndex + 1; j < fullHostName.length(); j++ )
				{
					sportstream << fullHostName[ j ];
					sPortStream << fullHostName[ j ];
				}

				hostName = hoststream.str();

				sPort = sPortStream.str();

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

	WSADATA wsaData;

	SOCKET ConnectSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	char sendbuf[ DEFAULT_BUFLEN ];

	char recvbuf[ DEFAULT_BUFLEN ];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

	if ( iResult != 0 )
	{
		printf( "WSAStartup failed: %d\n", iResult );
		return 1;
	}

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo( hostName.c_str(), sPort.c_str(), &hints, &result );
	if ( iResult != 0 )
	{
		printf( "getaddrinfo failed: %d\n", iResult );
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for ( ptr = result; ptr != NULL; ptr = ptr->ai_next )
	{
		// Create a SOCKET for connecting to server
		ConnectSocket = socket( ptr->ai_family, ptr->ai_socktype,
								ptr->ai_protocol );
		if ( ConnectSocket == INVALID_SOCKET )
		{
			printf( "Error at socket(): %ld\n", WSAGetLastError() );
			freeaddrinfo( result );
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen );
		if ( iResult == SOCKET_ERROR )
		{
			closesocket( ConnectSocket );
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	//freeaddrinfo( result );

	if ( ConnectSocket == INVALID_SOCKET )
	{
		printf( "Unable to connect to server!\n" );
		WSACleanup();
		return 1;
	}

	cout << "Enter 's' to Start or 't' to Quit\n";

	string in = "";

	cin >> in;

	int res = strcmp( in.c_str(), "s" );

	if ( res == 0 )
	{
		strcpy( sendbuf, "GET /NumberGuessing/s \n" );
		strcat( sendbuf, "Host: localhost\n\n" );
	}
	else
	{
		res = strcmp( in.c_str(), "t" );

		if ( res == 0 )
		{
			strcpy( sendbuf, "GET /NumberGuessing/t \n" );
			strcat( sendbuf, "Host: localhost\n\n" );
		}
	}


	// Send an initial buffer
	iResult = send( ConnectSocket, sendbuf, (int)strlen( sendbuf ), 0 );
	if ( iResult == SOCKET_ERROR )
	{
		printf( "send failed: %d\n", WSAGetLastError() );
		closesocket( ConnectSocket );
		WSACleanup();
		return 1;
	}

	printf( "Bytes Sent: %ld\n", iResult );

	string cmd;

	do
	{

		int length = strlen( sendbuf );

		iResult = recv( ConnectSocket, recvbuf, recvbuflen - 1, 0 );
		if ( iResult > 0 )
		{
			printf( "Bytes received: %d\n", iResult );
			recvbuf[ iResult ] = '\0';
			printf( "%s\n", recvbuf );
		}
		else if ( iResult == 0 )
		{
			printf( "Connection closed\n" );
			break;
		}
		else
			printf( "recv failed: %d\n", WSAGetLastError() );

		do
		{
			cin >> cmd;

			if ( cmd[ 0 ] == 'q' )
			{
				strcpy( sendbuf, "GET /NumberGuessing/q \n" );
				strcat( sendbuf, "Host: localhost\n\n" );
			}
			else
			{
				stringstream ss;

				ss << cmd;

				int number = -1;

				if ( !( ss >> number ) )
				{
					cout << "Not a vaild Command. Please Enter Either a Number or a 'q' to Quit\n";
					continue;
				}

				if ( number >= 0 && number <= 100 )
				{

					string nMsg = "GET /NumberGuessing/n=";
					nMsg.append( cmd );
					nMsg.append( " \n" );
					strcpy( sendbuf, nMsg.c_str() );
					strcat( sendbuf, "Host: localhost\n\n" );
				}
				else
				{
					cout << "Number is not Between 0 and 100. Please Enter a Number Between the Range or 'q' to Quit\n";
					continue;
				}
			}

			int iResult2 = send( ConnectSocket, sendbuf, (int)strlen( sendbuf ), 0 );
			if ( iResult2 == SOCKET_ERROR )
			{
				printf( "send failed: %d\n", WSAGetLastError() );
				closesocket( ConnectSocket );
				WSACleanup();
				return 1;
			}

			printf( "Bytes Sent: %ld\n", iResult2 );

			break;

		} while ( true );




	} while ( iResult > 0 );

	// cleanup
	closesocket( ConnectSocket );
	WSACleanup();
	getchar();

	char s[ 20 ];

	cin >> s;

	return 0;
}
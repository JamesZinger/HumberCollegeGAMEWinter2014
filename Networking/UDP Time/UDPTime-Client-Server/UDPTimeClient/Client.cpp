#include <ws2tcpip.h>
#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <iostream>
#include <time.h>
#include <stdint.h>
#pragma comment(lib,"ws2_32.lib")

using std::string;
using std::cin;
using std::cout;

const int CLIENT_PORT = 50001; /*the client port number*/

#define BUFFER_SIZE 1024

int main( int argc, char* argv[] )
{

	struct sockaddr_in serverAddr;				//server address
	struct sockaddr_in clientAddr;				//client address
	char buf[ BUFFER_SIZE ];					//buffer the message send and receive
	WSADATA wsaData;							//windows socket data
	SOCKET sd;									//socket descriptor
	struct hostent;								//the name of the host
	unsigned short serv_port;					//the port on the server to connect to
	int clientSock;
	const int len = sizeof( struct sockaddr );	
	// Clear the Structs for the client and server addresses
	memset( (void *)&serverAddr, '\0', sizeof( struct sockaddr_in ) );
	memset( (char*)&clientAddr, '\0', sizeof( struct sockaddr_in ) );

#pragma region Argument Verification

	if ( argc != 3 )
	{
		fprintf( stderr, "Incorrect amount of inputs. Actual: %i Expected: %i", argc, 3 );
		exit( 1 );
	}

	bool isLocalHost = false;

	if ( inet_addr( argv[ 1 ] ) == INADDR_NONE )
	{

		if ( strcmp( argv[ 1 ], "localhost" ) != 0 )
		{
			fprintf( stderr, "bad ip address %s\n", argv[ 1 ] );
			exit( 1 ); /* error */
		}

		isLocalHost = true;
	}


	if ( sscanf( argv[ 2 ], "%u", &serv_port ) != 1 )
	{
		fprintf( stderr, "bad port number %s\n", argv[ 2 ] );
		exit( 1 );
	}

#pragma endregion

#pragma region Windows Connection

	if ( WSAStartup( 0x0202, &wsaData ) != 0 )
	{
		if ( WSAStartup( 0x0101, &wsaData ) != 0 )
		{

			fprintf( stderr, "Could not open Windows connection.\n" );
			exit( 1 );
		}
	}

#pragma endregion

#pragma region Socket Creation


	// Server Connection Establishment
	{
		// Attempt to make the socket
		sd = socket( AF_INET, SOCK_DGRAM, 0 );
		if ( sd == INVALID_SOCKET )
		{
			fprintf( stderr, "socket creating failed\n" );
			WSACleanup();
			exit( 1 );
		}

		// Setup Server Struct
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons( serv_port );

		{
			addrinfo hints = { sizeof( addrinfo ) };
			hints.ai_flags = AI_ALL;
			hints.ai_family = PF_INET;
			hints.ai_protocol = 4; //IPPROTO_IPV4

			addrinfo* pResult = NULL;
			int errcode = getaddrinfo( argv[ 1 ], NULL, &hints, &pResult );
			if ( errcode != 0 )
				return ERROR;

			serverAddr.sin_addr.s_addr = *( (uint32_t*)& ( ( (sockaddr_in*)pResult->ai_addr )->sin_addr ) );
		}
	}

	// Client Info Fetching and Port Binding
	{
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_addr.s_addr = INADDR_ANY;
		clientAddr.sin_port = htons( (u_short)CLIENT_PORT );

		if ( bind( clientSock, (LPSOCKADDR)&clientAddr, sizeof( struct sockaddr ) ) < 0 )
		{/*bind a client address and port*/
			fprintf( stderr, "bind failed\n" );
			exit( 1 );
		}
	}
#pragma endregion

	while ( 1 )
	{
		memset( buf, 0, sizeof( buf ) );
		printf( "\nClient--->: " );
		gets( buf );
		sendto( clientSock, buf, strlen( buf ), 0, (LPSOCKADDR)&serverAddr, len );
		if ( strcmp( buf, "end" ) == 0 ) break; /* send "end" to exit loop */

		memset( buf, 0, sizeof( buf ) );
		recvfrom( clientSock, buf, sizeof( buf ), 0, (LPSOCKADDR)&serverAddr, &len );

		// moderator note: This code is a buffer overflow attack waiting to happen

		if ( strcmp( buf, "end" ) == 0 ) break; /* receive "end" to exit loop */
		if ( strcmp( buf, "close" ) == 0 ) return 1; /* receive "close" to close both */
		printf( "\n--->From the server: " );
		printf( "%s", buf );
	}

	closesocket( clientSock );
	WSACleanup();

	string s;

	cin >> s;

	return 0;
}
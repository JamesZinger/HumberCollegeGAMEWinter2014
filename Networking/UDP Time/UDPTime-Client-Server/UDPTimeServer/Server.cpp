#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

using std::string;
using std::cout;

const int SERVER_PORT = 10001; /*the default serverport number*/
int requests = 0; /*the number client connect*/

int main( int argc, char* argv[] )
{
	struct sockaddr_in serverAddr; /*server address*/
	struct sockaddr_in clientAddr; /*client address*/

	int serverSock; /*server socket, and client sock*/
	char buf[ 100 ]; /*buffer the message send and receive*/
	int len = sizeof( struct sockaddr );
	char ip[ 15 ]; /*client address*/
	int serverPort, clientPort; /*client and server ports*/

#pragma region Server Socket Setup

	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData ); /*windows socket startup */

	memset( (char*)&serverAddr, 0, sizeof( serverAddr ) );
	serverAddr.sin_family = AF_INET; /*set server address protocol family*/
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if ( argc > 1 )
	{
		serverPort = atoi( argv[ 1 ] );
	}
	else
	{
		serverPort = SERVER_PORT;
	}

	if ( serverPort > 0 )
		serverAddr.sin_port = htons( (u_short)serverPort );/*set port*/
	else
	{
		fprintf( stderr, "bad port number %s\n", argv[ 1 ] );
		exit( 1 );
	}

	serverSock = socket( PF_INET, SOCK_DGRAM, 0 );/*create a socket*/
	if ( serverSock < 0 )
	{
		fprintf( stderr, "socket creating failed\n" );
		exit( 1 );
	}

	if ( bind( serverSock, (LPSOCKADDR)&serverAddr, len ) < 0 )
	{/*bind a server address and port*/
		fprintf( stderr, "bind failed\n" );
		exit( 1 );
	}

#pragma endregion

	printf( "\nServer is set for nonblocking mode.\n" );

	// Set Non Blocking Mode, specified via last parameter
	// 0 Disabled 
	// !0 Enabled 
	unsigned long int nonBlockingMode = 1;
	int result = ioctlsocket( serverSock, FIONBIO, &nonBlockingMode );

	if ( result )
	{
		fprintf( stderr, "ioctlsocket failed\n" );
		exit( 1 );
	}

	while ( 1 )
	{
		// Setup the Client Address ( IP & port )
		struct sockaddr_in clientAddr;
		// this information will be filled in when recvfrom executes

		// Check for messages
		FD_SET set;
		timeval timeVal;

		FD_ZERO( &set );
		FD_SET( serverSock, &set );

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		//  	    printf("\nServer is waiting to something enters into the receive buffer!\n");
		result = select( FD_SETSIZE, &set, NULL, NULL, &timeVal );

		if ( result == 0 )
		{
			Sleep( 300 );
			continue;
		}

		if ( result == SOCKET_ERROR )
		{
			fprintf( stderr, "select error\n" );
			exit( 1 );
		}

		result = 0;

		memset( buf, 0, sizeof( buf ) );
		recvfrom( serverSock, buf, sizeof( buf ), 0, (LPSOCKADDR)&clientAddr, &len );

		requests++;
		strcpy( ip, inet_ntoa( clientAddr.sin_addr ) );
		clientPort = ntohs( (u_short)clientAddr.sin_port );

		fprintf( stdout, "\nA client come from ip:%s port:%d .\nThis server has been contacted %d time%s\n", ip, clientPort, requests, requests == 1 ? "." : "s." );

		printf( "Command Recieved: %s", buf );
		printf( "\n" );

		string sReturn;

		if ( strcmp( buf, "GET TIME" ) == 0 )
		{
			time_t current = time( NULL );
			sReturn = asctime( localtime( &current ) );
		}
		else
		{
			printf( "Invaild Request\n" );
			sReturn = "Invaild Request";
		}

		memset( buf, 0, sizeof( buf ) );
		cout << "Returning: " << sReturn << "\n";
		memcpy( buf, sReturn.c_str(), sReturn.length() );
		sendto( serverSock, buf, sizeof( buf ), 0, (LPSOCKADDR)&clientAddr, len );

		memset( buf, 0, sizeof( buf ) );

	}
}
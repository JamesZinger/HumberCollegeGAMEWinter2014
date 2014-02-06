#include <ws2tcpip.h>
#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <iostream>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")

using std::string;
using std::cin;
using std::cout;

const int CLIENT_PORT = 50001; /*the client port number*/

#define BUFFER_SIZE 1024

int main( int argc, char* argv[] )
{

	struct sockaddr_in serverAddr;	//server address
	struct sockaddr_in clientAddr;	//client address
	int clientSock;					//client sock
	char buf[ BUFFER_SIZE ];		//buffer the message send and receive
	int serverPort;					//protcol port
	WSADATA wsaData;				//windows socket data
	SOCKET sd;						//socket descriptor
	string hostname;				//the name of the host
	int serv_port;					//the port on the server to connect to
	int len = sizeof( struct sockaddr );

	WSAStartup( 0x0202, &wsaData ); /*windows socket startup */

	memset( (char*)&clientAddr, 0, sizeof( clientAddr ) );
	clientAddr.sin_family = AF_INET; /*set client address protocol family*/
	clientAddr.sin_addr.s_addr = INADDR_ANY;
	clientAddr.sin_port = htons( (u_short)CLIENT_PORT ); /*set client port*/

	serverAddr.sin_family = AF_INET;

	if ( argc > 2 )
	{
		serverAddr.sin_addr.s_addr = inet_addr( hostname.c_str() );/*get the ip address*/
		if ( serverAddr.sin_addr.s_addr == INADDR_NONE )
		{
			fprintf( stderr, "bad ip address %s\n", hostname.c_str() );
			exit( 1 );
		}

		serverPort = atoi( port.c_str() );
		if ( serverPort > 0 )
			serverAddr.sin_port = htons( (u_short)serverPort );/*get the port*/
		else
		{
			fprintf( stderr, "bad port number %s\n", port.c_str() );
			exit( 1 );
		}
	}
	else
	{
		fprintf( stderr, "input the ip address and port number" );
		exit( 1 );
	}

	clientSock = socket( PF_INET, SOCK_DGRAM, 0 );/*create a socket*/
	if ( clientSock < 0 )
	{
		fprintf( stderr, "socket creating failed\n" );
		exit( 1 );
	}

	if ( bind( clientSock, (LPSOCKADDR)&clientAddr, len ) < 0 )
	{/*bind a client address and port*/
		fprintf( stderr, "bind failed\n" );
		exit( 1 );
	}

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
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <time.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"

char* buildHttpResponse( char* msg );
void getCommandValue( char* msg, char* command, int* value );

int __cdecl main( void )
{
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET,
		ClientSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		hints;

	char recvbuf[ DEFAULT_BUFLEN ];

	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( iResult != 0 )
	{
		printf( "WSAStartup failed: %d\n", iResult );
		return 1;
	}

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo( NULL, DEFAULT_PORT, &hints, &result );
	if ( iResult != 0 )
	{
		printf( "getaddrinfo failed: %d\n", iResult );
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
	if ( ListenSocket == INVALID_SOCKET )
	{
		printf( "socket failed: %ld\n", WSAGetLastError() );
		freeaddrinfo( result );
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
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

	printf( "Server is up and running!\n" );

	iResult = listen( ListenSocket, SOMAXCONN );
	if ( iResult == SOCKET_ERROR )
	{
		printf( "listen failed: %d\n", WSAGetLastError() );
		closesocket( ListenSocket );
		WSACleanup();
		return 1;
	}


	// Since HTTP is a connectionless protocole, for every
	// send-receive cycle we should "accept" the comming
	// connection from the client
	char command;
	int rndValue, guessedValue, numGuess;
	bool running = true;
	char* msg;

	// Accept a client socket
	ClientSocket = accept( ListenSocket, NULL, NULL );
	if ( ClientSocket == INVALID_SOCKET )
	{
		printf( "accept failed: %d\n", WSAGetLastError() );
		closesocket( ListenSocket );
		WSACleanup();
		return 1;
	}

	do
	{
		

		iResult = recv( ClientSocket, recvbuf, recvbuflen, 0 );
		if ( iResult > 0 )
		{
			printf( "Bytes received: %d\n", iResult );
			recvbuf[ iResult ] = '\0';
			getCommandValue( recvbuf, &command, &guessedValue );

			if ( command == 's' || command == 't' )
			{// the client sent a start or end command
				if ( command == 's' )
				{
					msg = buildHttpResponse( "Game has started make a guess between 1 and 100." );

					srand( (unsigned)time( NULL ) );
					rndValue = rand() % 100 + 1;
					numGuess = 0;
				}
				else if ( command == 't' )
				{
					msg = buildHttpResponse( "The game is terminated. Thank you for playing the game." );
					running = false;
				}
			}
			else
			{ // the client sent a number
				numGuess++;
				char* tmp = new char[ DEFAULT_BUFLEN ];
				if ( guessedValue == rndValue )
				{
					sprintf( tmp, "Congratulation! Your guess was right. The number of guesses is: %d", numGuess );
				}
				else if ( guessedValue > rndValue )
				{
					sprintf( tmp, "Your number is greater than my number. The number of guesses is: %d", numGuess );
				}
				else if ( guessedValue < rndValue )
				{
					sprintf( tmp, "Your number is less than my number. The number of guesses is: %d", numGuess );
				}
				msg = buildHttpResponse( tmp );
				delete[] tmp;
			}

			iSendResult = send( ClientSocket, msg, strlen( msg ), 0 );
			if ( iSendResult == SOCKET_ERROR )
			{
				printf( "send failed: %d\n", WSAGetLastError() );
				closesocket( ClientSocket );
				WSACleanup();
				return 1;
			}
		}

		if ( running == false )
			printf( "Connection closing...\n" );

	} while ( running );

	// shutdown the connection since we're done
	iResult = shutdown( ClientSocket, SD_SEND );
	if ( iResult == SOCKET_ERROR )
	{
		printf( "shutdown failed: %d\n", WSAGetLastError() );
		closesocket( ClientSocket );
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket( ClientSocket );
	WSACleanup();
	getchar();
	return 0;
}

char* buildHttpResponse( char* msg )
{
	char* result = new char[ DEFAULT_BUFLEN ];
	strcpy( result, "HTTP/1.0 200 OK\n" );
	strcat( result, "Date: Fri, 31 Dec 2010 23:59:59 GMT\n" );
	strcat( result, "Content-Type: text/html\n" );
	strcat( result, "Content-Length: " );
	sprintf( result, "%s %d\n\n%s", result, strlen( msg ), msg );

	return result;
}

void getCommandValue( char* msg, char* command, int* value )
{
	sscanf( msg, "GET /NumberGuessing/%c", command );

	if ( command[ 0 ] == 'c' )
	{
		sscanf( msg, "GET /NumberGuessing/c=%c", command );
		*value = -1;
	}
	else
	{
		sscanf( msg, "GET /NumberGuessing/n=%d", value );
	}
}
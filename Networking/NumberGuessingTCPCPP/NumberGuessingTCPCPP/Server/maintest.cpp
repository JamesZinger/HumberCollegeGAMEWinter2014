#define BUF_LEN 512
#define _CRT_SECURE_NO_WARNINGS

#include "ClassServer.h"
#include <WS2tcpip.h>
#include <stdio.h>
#include <time.h>


char* buildHttpResponse( char* msg );
void getCommandValue( char* msg, char* command, int* value );

int __cdecl main( void )
{
	char header[ BUF_LEN ];
	char recvbuf[ BUF_LEN ];
	char command;
	char* msg;

	bool running = true;

	int iResult, guessedValue, numGuess, rndValue, iSendResult;

	strcpy( header, "HTTP/1.0 200 OK\n" );
	strcat( header, "Content-Type: text/html\n" );
	strcat( header, "Content-Length: %d\n\n%s" );

	ClassServer *serverSocket = new ClassServer( "8080" );

	if ( serverSocket->isConnected() )
	{
		do
		{
			if ( serverSocket->AcceptConnection() )
			{
				serverSocket->RecieveData( recvbuf, BUF_LEN - 1, &iResult );
				if ( iResult > 0 )
				{
					printf( "Bytes received: %d\n", iResult );
					recvbuf[ iResult ] = '\0';
					getCommandValue( recvbuf, &command, &guessedValue );


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
					else if ( command == 'n' )
					{ // the client sent a number
						numGuess++;
						char* tmp = new char[ BUF_LEN ];
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

					serverSocket->SendData( msg, strlen( msg ), &iSendResult );


				}
			}
		} while ( running );
	}

	delete serverSocket;

}

char* buildHttpResponse( char* msg )
{
	char* result = new char[ BUF_LEN ];
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
	else if ( command[ 0 ] == 'n' )
	{
		sscanf( msg, "GET /NumberGuessing/n=%d", value );
	}
	else
	{
		*value = -1;
	}
}
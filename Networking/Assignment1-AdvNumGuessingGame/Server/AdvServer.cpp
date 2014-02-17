#pragma comment(lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <time.h>

#include "NumberGuessingGame.h"

#define SERVER_PORT 8081
#define RECV_BUF_SIZE 200


// TODO Remove this if it doesnt prevent building
//#define _CRT_SECURE_NO_WARNING

using std::string;
using std::cout;
using std::vector;
using std::stringstream;

struct ipAddr
{
	char b1;
	char b2;
	char b3;
	char b4;

};

inline bool operator==( const ipAddr& i1, const ipAddr i2 )
{
	if ( i1.b1 == i2.b1 &&
		 i1.b2 == i2.b2 &&
		 i1.b3 == i2.b3 &&
		 i1.b4 == i2.b4 )
		 return true;

	return false;
}

struct connectionStamp
{
	ipAddr addr;
	struct tm timestamp;
	NumberGuessingGame* game;
};

struct threadStruct
{
	SOCKET socket;
	connectionStamp stamp;
};

vector<connectionStamp> vStamps;


#pragma region function declairations

/**
 * Gets a command from a web request
 * @param	msg	If non-null, the message.
 * @return	The command.
 **/
char getCommand( char* msg );

/**
 * Builds an HTTP response based on a message you want to send.
 * @param	msg			  	The message you want to send.
 * @param [out]	output	The output string which is the HTTP request.
 **/
void buildHttpResponse( string msg, string& output );

/**
 * Gets a number from a web request.
 * @param	msg	The message which is the web request.
 * @return	The number from request.
 **/
int getNumberFromRequest( char* msg );

#pragma endregion

// our thread for recving commands. 
DWORD WINAPI receive_cmds( LPVOID lpParam )
{
	cout << "thread created\r\n";

	threadStruct* input = (threadStruct*)lpParam;

	// set our socket to the socket passed in as a parameter   
	SOCKET current_client = input->socket;

	connectionStamp stamp = input->stamp;

	delete input;

	input = NULL;

	DWORD timeout = 0;

	setsockopt( current_client, SOL_SOCKET, SO_RCVTIMEO, (char*)( &timeout ), sizeof( int ) );

	// buffer to hold our recived data
	char buf[ RECV_BUF_SIZE ];

	// for error checking 
	int res;

	// The game the thread will use
	NumberGuessingGame* game = NULL;

	connectionStamp* rStamp;

	for ( int i = 0; i < vStamps.size(); i++ )
	{
		if ( stamp.addr == vStamps[ i ].addr )
		{
			rStamp = &( vStamps[ i ] );
			game = rStamp->game;
			break;
		}
	}

	res = recv( current_client, buf, sizeof( buf ), MSG_WAITALL ); // recv cmds



	Sleep( 10 );

	if ( res < 0 )
	{
		cout << "Closing thread id: " << GetCurrentThreadId() << "\n";
		closesocket( current_client );
		ExitThread( 0 );
	}

	char command = getCommand( buf );

	cout << "recieved command '" << command << "'\n";
	cout << "on thread id: " << GetCurrentThreadId() << "\n";

	string ssend;
	stringstream ss;

	/// An attempted number guess or restart the game
	// Commands requireing the game to not be null
	if ( command == 'n' || command == 'r' )
	{

		if ( game == NULL )
		{
			ss = stringstream();
			ss << "No game is running currently for you\n";
			ss << "Please reconnect to fix this issue\n";
		}

		/// The game is not null
		else
		{
			if ( command == 'n' )
			{

				if ( game->isGameFinished() )
				{
					ss = stringstream();
					ss << "Cannot guess on a finished game\n";
					ss << "Please send command 'r' to restart the game\n";
				}
				else
				{
					int value = getNumberFromRequest( buf );

					game->GuessNumber( value, ssend );

					ss << ssend;
				}

			}

			if ( command == 'r' )
			{
				ss = stringstream();

				ss << "Attempting to restart the game...\n";

				if ( game->RestartGame() )
				{
					ss << "Restart Sucessful!\n";
				}
				else
				{
					ss << "Something has gone wrong. Please reconnect\n";
				}
			}
		}
	}

	/// Attempt to start the game
	else if ( command == 's' )
	{
		ss = stringstream();

		if ( game != NULL )
		{
			ss << "A game is already running.\n";
			ss << "Please either restart the game with command 'r' or end it with 't'\n";
		}

		else
		{
			game = new NumberGuessingGame();

			rStamp->game = game;

			ss << "Game Sucesfully created\n";
		}

	}

	/// Attempt to termiante the game
	else if ( command == 't' )
	{
		ss = stringstream();
		ss << "Terminating Connection\n";
		ss << "Thanks for playing!\n";

		buildHttpResponse( ss.str(), ssend );

		Sleep( 10 );

		send( current_client, ssend.c_str(), ssend.length(), 0 );

		Sleep( 10 );

		if ( game != NULL )
		{
			delete game;
		}

		closesocket( current_client );
		ExitThread( 0 );
	}

	else if ( command == 'l' )
	{
		ss << "IP\t\t\tDATE\t\tTIME\n";
		ss << "==========================\n";

		for ( int i = 0; i < vStamps.size(); i++ )
		{
			connectionStamp stamp = vStamps[ i ];

			ss << (int)( stamp.addr.b1 ) << '.' << (int)( stamp.addr.b2 ) << '.';
			ss << (int)( stamp.addr.b3 ) << '.' << (int)( stamp.addr.b4 ) << "\t";

			ss << stamp.timestamp.tm_mday << '-' << stamp.timestamp.tm_mon << '-' << 1900 + stamp.timestamp.tm_year << '\t';

			ss << stamp.timestamp.tm_hour << ':' << stamp.timestamp.tm_min << ':' << stamp.timestamp.tm_sec << '\n';
		}
	}

	/// Handle an unrecognized command
	else
	{
		ss = stringstream();
		ss << "Command " << command << " doesn't exist.\n";
		ss << "Possible commands you can use:\n";
		ss << "s\tStarts the game\n";
		ss << "t\tEnds the game\n";
		ss << "r\tRestarts the game\n";
		ss << "n=?\tAttempts to guess the number '?'\n";
	}

	Sleep( 10 );

	buildHttpResponse( ss.str(), ssend );

	send( current_client, ssend.c_str(), ssend.length(), 0 );

	/// Clear the recv buffer
	memset( buf, '\0', RECV_BUF_SIZE );

	Sleep( 10 );

}

int main()
{
	cout << "Starting up multi-threaded TCP server by KOrUPt\r\n";

	cout << "Modified by James Zinger\r\n";

	// our masterSocket(socket that listens for connections)
	SOCKET sock;

	// for our thread
	DWORD thread;

	WSADATA wsaData;
	sockaddr_in server;

	// start winsock
	int ret = WSAStartup( 0x101, &wsaData ); // use highest version of winsock avalible

	if ( ret != 0 )
	{
		cout << "Failed to start up winodws socket. Error Code: " << WSAGetLastError();
		exit( 1 );
	}

	// fill in winsock struct ... 
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( SERVER_PORT ); // listen on telnet port 23

	// create our socket
	sock = socket( AF_INET, SOCK_STREAM, 0 );

	if ( sock == INVALID_SOCKET )
	{
		cout << "Socket Failed to Create\r\nyty";
		exit( 2 );
	}

	// bind our socket to a port(port 123) 
	if ( bind( sock, (sockaddr*)&server, sizeof( server ) ) != 0 )
	{
		cout << "Failed to bind to socket: " << sock << "\r\n";
		exit( 3 );
	}


	// socket that we sneds/recv data on
	SOCKET client;

	sockaddr_in from;
	int fromlen = sizeof( from );
	// listen for a connection  
	if ( listen( sock, 5 ) != 0 )
	{
		cout << "Failed to Listen from socket: " << sock << "\r\n";
		exit( 4 );
	}

	// loop forever 
	while ( true )
	{
		// accept connections
		client = accept( sock, ( struct sockaddr* )&from, &fromlen );
		printf( "Client is connected\r\n" );

		char s[ 30 ];

		string s2;

		s2 = inet_ntop( from.sin_family, (void*)( &( from.sin_addr ) ), s, 30 );

		cout << s2 << '\n';

		connectionStamp stamp;

		stamp.addr.b1 = from.sin_addr.S_un.S_un_b.s_b1;
		stamp.addr.b2 = from.sin_addr.S_un.S_un_b.s_b2;
		stamp.addr.b3 = from.sin_addr.S_un.S_un_b.s_b3;
		stamp.addr.b4 = from.sin_addr.S_un.S_un_b.s_b4;

		time_t t = time( NULL );

		stamp.timestamp = *( localtime( &t ) );
		stamp.game = NULL;

		vStamps.push_back( stamp );

		threadStruct* info = new threadStruct();

		info->stamp = stamp;
		info->socket = client;

		// create our recv_cmds thread and parse client socket as a parameter
		CreateThread( NULL, 0, receive_cmds, (LPVOID)info, 0, &thread );
	}

	for ( int i = 0; i < vStamps.size(); i++ )
	{
		delete vStamps[ i ].game;
	}

	vStamps.clear();

	// shutdown winsock
	closesocket( sock );
	WSACleanup();

	// exit
	return 0;
}

#pragma region Helper Functions Definitions

char getCommand( char* msg )
{
	char tmp = '\0';
	sscanf_s( msg, "GET /NumberGuessing/%c", &tmp );

	if ( tmp == 'c' )
	{
		sscanf_s( msg, "GET /NumberGuessing/c=%c", &tmp );
	}

	return tmp;
}

void buildHttpResponse( string msg, string& output )
{
	stringstream ss = stringstream();

	ss << "HTTP/1.0 200 OK\n";
	ss << "Date: Fri, 31 Dec 2010 23:59:59 GMT\n";
	ss << "Content-Length: " << msg.length() << "\n\n";
	ss << msg;

	output = ss.str();
}

int getNumberFromRequest( char* msg )
{
	int tmp = 0;
	sscanf_s( msg, "GET /NumberGuessing/n=%d", &tmp );
	return tmp;
}

#pragma endregion
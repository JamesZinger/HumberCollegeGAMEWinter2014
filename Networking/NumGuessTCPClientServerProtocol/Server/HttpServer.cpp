#include "HttpServer.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::stringstream;

HttpServer::HttpServer( string port )
{
	struct addrinfo* result = NULL, hints;
	int iResult, iSendResult;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	WSADATA wsaData;

	iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

	if ( iResult != 0 )
	{
		cout << "WSA Startup failed, Error Code: " << iResult << "\n";
		system( "Pause" );
		exit( 1 );
	}

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo( NULL, port.c_str(), &hints, &result );
	if ( iResult != 0 )
	{
		cout << "getaddrinfo failed, Error Code: " << iResult << "\n";
		WSACleanup();
		system( "Pause" );
		exit( 1 );
	}

	ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
	if ( ListenSocket == INVALID_SOCKET )
	{
		cout << "socket creation failed, Error Code: " << WSAGetLastError() << "\n";
		freeaddrinfo( result );
		WSACleanup();
		system( "Pause" );
		exit( 1 );
	}

	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen );
	if (iResult == SOCKET_ERROR)
	{

		cout << "Bind failed, Error Code: " << WSAGetLastError() << "\n";
		freeaddrinfo( result );
		ServerDisconnect();
		system( "Pause" );
		exit( 1 );
	}

	freeaddrinfo( result );

	if ( _debugging )
		cout << "Server is up and running!\n";

	iResult = listen( ListenSocket, SOMAXCONN );
	if (iResult == SOCKET_ERROR)
	{
		cout << "listen failed, Error Code: " << WSAGetLastError() << '\n';
		ServerDisconnect();
		system( "Pause" );
		exit( 1 );
	}

	RCBuffer = (char*)malloc( sizeof(char)* BUFFER_SIZE );

	ListenPort = atoi( port.c_str() );
}

HttpServer::~HttpServer()
{
	ClientDisconnect();
	ServerDisconnect();
	free( RCBuffer );
}

bool HttpServer::AcceptConnection()
{
	if ( ListenSocket == INVALID_SOCKET )
	{
		cout << "Listening Socket is not configured\n";
		ServerDisconnect();
		return false;
	}

	ClientSocket = accept( ListenSocket, NULL, NULL );

	if(ClientSocket == INVALID_SOCKET)
	{
		cout << "Failed to accept connection, Error Code: " << WSAGetLastError() << '\n';
		ServerDisconnect();
		return false;
	}

	return true;
}

bool HttpServer::SendHttpMsg( string* msg, int *sentLength )
{
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "No client to send to\n";
		return false;
	}
	BuiltHttpResponse( msg );
	*sentLength = send( ClientSocket, msg->c_str(), msg->length(), 0 );
	if (*sentLength == SOCKET_ERROR)
	{
		cout << "Send failed, Error Code: " << WSAGetLastError() << '\n';
		ClientDisconnect();
		return false;
	}

	return true;

}

bool HttpServer::RecieveData( string& result, int *receivedLength )
{
	if ( ClientSocket == INVALID_SOCKET )
	{
		printf( "No client is attached. \n" );
		return false;
	}
	memset( RCBuffer, '\0', BUFFER_SIZE );
	*receivedLength = recv( ClientSocket, RCBuffer, BUFFER_SIZE, 0 );
	if ( *receivedLength == 0 )
	{
		printf( "Connection closing...\n" );
		ClientDisconnect();
		return false;
	}
	else  if ( *receivedLength < 0 )
	{
		printf( "recv failed: %d\n", WSAGetLastError() );
		ClientDisconnect();
		return false;
	}

	result = RCBuffer;

	return true;
}

void HttpServer::ClientDisconnect()
{
	closesocket( ClientSocket );
	WSACleanup();
}

void HttpServer::ServerDisconnect()
{
	closesocket( ListenSocket );
	WSACleanup();
}

void HttpServer::BuiltHttpResponse(string* msg)
{
	stringstream ss = stringstream();
	ss << "HTTP/1.0 200 OK\n";
	ss << "Date: Fri, 31 Dec 2010 23:59:59 GMT\n";
	ss << "Content-Type: text/html\n";
	ss << "Content-Length: " << msg->length() << "\n\n";
	ss << ( *msg );

	msg->clear();
	msg->append( ss.str() );

	return;

}

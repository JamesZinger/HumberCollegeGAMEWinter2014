///////////////////////////////////////////////////////////
//  TCPServer.cpp
//  Implementation of the Class TCPServer
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "TCPServer.h"

#include <sstream>
#include <iostream>

using std::stringstream;
using std::cout;
using std::endl;
using boost::interprocess::string;

TCPServer::TCPServer(Protocol* proto, int port ) : Server( proto, port )
{
	SOCKET listenSocket = INVALID_SOCKET;

	addrinfo hints, *addrResult;
	ZeroMemory( &hints, sizeof( hints ) );

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	stringstream ss;

	ss << port;

	string s = string(ss.str().c_str());

	int iResult = getaddrinfo( NULL, s.c_str(), &hints, &addrResult );

	if ( iResult != 0 )
	{
		cout << "getaddrinfo failed, Error Code: " << iResult << endl;
		WSACleanup();
		system( "pause" );
		exit( iResult );
	}

	listenSocket = socket( addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol );
	if ( listenSocket == INVALID_SOCKET )
	{
		int errorCode = WSAGetLastError();
		cout << "failed to create socket, Error code: " << errorCode << endl;
		freeaddrinfo( addrResult );
		WSACleanup();
		system( "pause" );
		exit( errorCode );
	}

	///  0 = Blocking
	/// !0 = Non-Blocking
	unsigned long mode = 0;

	iResult = ioctlsocket( listenSocket, FIONBIO, &mode );
	if ( iResult != NO_ERROR )
	{
		cout << "could not set the socket to non-blocking mode, Error Code: " << iResult << endl;
		freeaddrinfo( addrResult );
		closesocket( listenSocket );
		WSACleanup();
		system( "pause" );
		exit( iResult );
	}

	iResult = bind( listenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen );
	if ( iResult == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();
		cout << "bind failed, Error code: " << errorCode << endl;
		freeaddrinfo( addrResult );
		closesocket( listenSocket );
		WSACleanup();
		system( "pause" );
		exit( errorCode );
	}

	iResult = listen( listenSocket, SOMAXCONN );
	if ( iResult == SOCKET_ERROR )
	{
		cout << "could not listen, Error code: " << iResult << endl;
		freeaddrinfo( addrResult );
		closesocket( listenSocket );
		WSACleanup();
		system( "pause" );
		exit( iResult );
	}

	freeaddrinfo( addrResult );

	ListenSocket( listenSocket );
}


TCPServer::~TCPServer()
{

}


SOCKET TCPServer::AcceptConnection(sockaddr_in* addrPtr /* = nullptr */)
{
	if ( ListenSocket() == INVALID_SOCKET )
	{
		cout << "Server is not configured to listen" << endl;
		closesocket( ListenSocket() );
		WSACleanup();
		system( "pause" );
		exit( 1 );
	}
	SOCKET client;
	if (addrPtr == nullptr)
	{
		client = accept( ListenSocket(), NULL, NULL );
	}
	else
	{
		int i = sizeof( *addrPtr );
		client = accept( ListenSocket(), (sockaddr*)addrPtr, &i );
	}

	

	if ( client == INVALID_SOCKET )
	{
		if ( Debugging() )
		{
			cout << "Accept failed, Error code: " << WSAGetLastError() << endl;
		}
	}

	return client;
}

void TCPServer::SendMessageOverNetwork( SOCKET MessageSocket, boost::interprocess::string& Message )
{
	if (MessageSocket == INVALID_SOCKET)
	{
		cout << "Socket is not valid" << endl;
		return;
	}
	int iResult = send( MessageSocket, Message.c_str(), Message.length(), 0 );
	
	if (Debugging())
	{
		cout << "Sending Message: " << endl;
		cout << Message << endl << endl;
	}

	if (iResult == SOCKET_ERROR)
	{
		cout << "Failed to send message, Error code: " << WSAGetLastError() << endl;
		closesocket( MessageSocket );
		return;
	}
}

int TCPServer::RecieveMessage( SOCKET MessageSocket, char* Buffer, int BufferLength, boost::interprocess::string* out )
{
	if ( out == nullptr )
		out = new boost::interprocess::string();

	string s;
	if (MessageSocket == INVALID_SOCKET)
	{
		cout << "Socket is not valid" << endl;
		return -1;
	}
	int iResult = recv( MessageSocket, Buffer, BufferLength, 0 );
	if (iResult < 0)
	{
		int errorCode = WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
		{
			return 0;
		}
		cout << "Failed to receive message, Error code: " << WSAGetLastError() << endl;
		closesocket( MessageSocket );
		return -1;
	}

	out->assign(Buffer);

	return 1;	
}
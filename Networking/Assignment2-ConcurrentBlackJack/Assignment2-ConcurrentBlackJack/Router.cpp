///////////////////////////////////////////////////////////
//  Router.cpp
//  Implementation of the Class Router
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "Router.h"
#include "TCPGameServer.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

Router::Router()
{
	m_gameServer = nullptr;
}


Router::~Router()
{

}


void Router::RoutingThreadFunc( SOCKET Request, TCPGameServer* server )
{
	m_threadID = GetCurrentThreadId();

	char* Buff = (char*)malloc( 1024 );

	memset( Buff, '\0', 1024 );

	std::string* recievedMessage = server->RecieveMessage( Request, Buff, 1024 );

	delete Buff;

	if ( server->Debugging() )
	{
		cout << "Received Message: " << endl;
		cout << *recievedMessage << endl << endl;
	}

	if ( recievedMessage->length() == 0 )
	{
		closesocket( Request );

		if ( server->Debugging() )
		{
			cout << "Closing connection" << endl;
		}
		delete this;
		return;
	}

	HandleRequest( Request, *recievedMessage );

	delete recievedMessage;

	std::string* message = new std::string( "Got It" );

	server->SendMessage( Request, *message );

	Sleep( 100 );

	delete this;
}


const char Router::DetermineRequestContext( string& Message )
{
	char context;

	stringstream ss = stringstream();

	ss << Message;

	string firstLine;

	std::getline( ss, firstLine, '\n' );

	const string http = "HTTP";

	if ( Message.length() < http.length() )
	{
		if ( GameServer()->Debugging() )
		{
			cout << "Cannot determine command, Command: " << Message << endl;
		}
		return '\0';
	}

	if ( firstLine.compare( firstLine.length() - http.length(), http.length(), http ) == 0 )
	{
		//Is an Http Command
		context = 'h';
	}
	else
	{
		//Likely a game command
		context = 'g';
	}

	return '\0';
}

void Router::HandleRequest( const SOCKET client, string& recieveString )
{
	//Find out what game the request is for.

	string s = recieveString;

	char context = DetermineRequestContext( s );

	//Send the correct message to the Game's Queue

	switch ( context )
	{
		case 'g':

			break;

		case 'h':

			break;

		case '\0':

			break;

		default:
			break;
	}

	return;
}

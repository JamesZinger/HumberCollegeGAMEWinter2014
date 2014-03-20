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

	GameServer( server );

	char* Buff = (char*)malloc( 1024 );

	memset( Buff, '\0', 1024 );

	std::string* recievedMessage = GameServer()->RecieveMessage( Request, Buff, 1024 );

	delete Buff;

	if ( GameServer()->Debugging() )
	{
		cout << "Received Message: " << endl;
		cout << *recievedMessage << endl << endl;
	}

	if ( recievedMessage->length() == 0 )
	{
		closesocket( Request );

		if ( GameServer()->Debugging() )
		{
			cout << "Closing connection" << endl;
		}
		delete this;
		return;
	}

	server->GetProtocol()->HandleRequest( Request, *recievedMessage, server );

	delete recievedMessage;

	Sleep( 100 );

	delete this;
}

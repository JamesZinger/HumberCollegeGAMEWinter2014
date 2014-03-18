#include "TCPGameServer.h"

#include <sstream>
#include <boost/thread.hpp>
#include <iostream>

void TCPGameServer::Run()
{
	while ( true )
	{
		SOCKET client = AcceptConnection();
		if ( client != INVALID_SOCKET )
		{
			Router* R = new Router();
			boost::thread* t = new boost::thread( &Router::RoutingThreadFunc, R, client, this );
			ThreadGroup()->add_thread( t );
			
		}
		else if ( client == INVALID_SOCKET )
		{
			Sleep( 100 );
		}
	}
}

void TCPGameServer::AddPlayer( SOCKET clientSocket )
{
	std::pair<SOCKET, Player*> PlayerSock( clientSocket, new Player( 1024 ) );
	Players().insert( PlayerSock );
}

const Player* TCPGameServer::GetPlayer( SOCKET clientSocket )
{
	return Players().at( clientSocket );
}

#include "TCPGameServer.h"

#include <sstream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>

void TCPGameServer::Run()
{
	while ( true )
	{
		connectionStamp stamp;
		sockaddr_in from;

		SOCKET client = AcceptConnection( &from );
		if ( client != INVALID_SOCKET )
		{
			stamp.addr.b1 = from.sin_addr.S_un.S_un_b.s_b1;
			stamp.addr.b2 = from.sin_addr.S_un.S_un_b.s_b2;
			stamp.addr.b3 = from.sin_addr.S_un.S_un_b.s_b3;
			stamp.addr.b4 = from.sin_addr.S_un.S_un_b.s_b4;

			stamp.initalConnectionTime = boost::posix_time::second_clock::local_time();

			std::pair<SOCKET, connectionStamp> sockStamp( client, stamp );
			ActiveConnections().insert( sockStamp );

			Router* R = new Router();
			boost::thread* t = new boost::thread( &Router::RoutingThreadFunc, R, client, this );

		}
		else if ( client == INVALID_SOCKET )
		{
			Sleep( 100 );
		}
	}
}

void TCPGameServer::AddPlayer( SOCKET sock, Player* player )
{
	std::pair<SOCKET, Player*> PlayerSock( sock, player );
	Players()->insert( PlayerSock );
}
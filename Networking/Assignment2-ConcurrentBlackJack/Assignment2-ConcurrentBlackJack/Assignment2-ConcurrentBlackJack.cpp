// Assignment2-ConcurrentBlackJack.cpp : Defines the entry point for the console application.
//

#pragma comment(lib, "ws2_32.lib")

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include "TCPGameServer.h"
#include "BlackjackProtocol.h"
#include "BlackjackGame.h"

using std::cout;
using std::endl;

void CreateServer( Protocol* proto, int port )
{
	TCPGameServer::m_instance = new TCPGameServer( proto, port );
}

int main( int argc, char* argv[] )
{
	Blackjack::BlackjackProtocol* proto = new Blackjack::BlackjackProtocol();
	CreateServer(proto);
	TCPGameServer* server = TCPGameServer::Instance();

	server->Debugging( true );

	server->Run();

	system( "pause" );

	return 0;
}

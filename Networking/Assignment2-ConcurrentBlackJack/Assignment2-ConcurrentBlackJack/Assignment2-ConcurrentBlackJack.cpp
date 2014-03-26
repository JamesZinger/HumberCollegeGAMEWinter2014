// Assignment2-ConcurrentBlackJack.cpp : Defines the entry point for the console application.
//

#pragma comment(lib, "ws2_32.lib")

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <vector>

#include "TCPGameServer.h"
#include "BlackjackProtocol.h"
#include "BlackjackGame.h"

using std::cout;
using std::endl;
using std::vector;

int main(int argc, char* argv[])
{
	Blackjack::BlackjackProtocol* proto = new Blackjack::BlackjackProtocol();
	TCPGameServer* server = new TCPGameServer( proto, 8282 );
	
	server->Debugging( true );

	server->Run();

	system( "pause" );

	return 0;
}




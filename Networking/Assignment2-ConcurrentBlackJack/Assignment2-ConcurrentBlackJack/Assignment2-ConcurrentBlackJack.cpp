// Assignment2-ConcurrentBlackJack.cpp : Defines the entry point for the console application.
//

#pragma comment(lib, "ws2_32.lib")

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <vector>

#include "TCPGameServer.h"
#include "BlackjackGame.h"

using std::cout;
using std::endl;
using std::vector;

int main(int argc, char* argv[])
{
	vector<string> names;
	names.push_back( "test" );
	Blackjack::BlackjackGame *game = new Blackjack::BlackjackGame(names);
	TCPGameServer *server = new TCPGameServer(game);

	server->Debugging( true );

	server->Run();

	system( "pause" );

	return 0;
}




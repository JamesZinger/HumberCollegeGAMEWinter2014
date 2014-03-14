// Assignment2-ConcurrentBlackJack.cpp : Defines the entry point for the console application.
//

#pragma comment(lib, "ws2_32.lib")

#include "TCPGameServer.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{



	TCPGameServer* server = new TCPGameServer();

	server->Run();

	system( "pause" );

	return 0;
}




///////////////////////////////////////////////////////////
//  TCPGameServer.h
//  Implementation of the Class TCPGameServer
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include "TCPServer.h"
#include "Router.h"

#include <concurrent_unordered_map.h>
#include <boost/interprocess/containers/vector.hpp>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using boost::interprocess::vector;
using Concurrency::concurrent_unordered_map;

class Game;
class Player;

/// <summary>	A TCP game server that runs a type of game. </summary>
/// <remarks>	James, 2014-03-15. </remarks>

class TCPGameServer : public TCPServer
{
public:
	TCPGameServer( Protocol* proto, int port = 8282 ) : TCPServer( proto, port )
	{
		m_games = boost::interprocess::vector<Game*>();
		m_players = concurrent_unordered_map<SOCKET, Player*>();
	}



	virtual void Run();

	vector<Game*>			Games()		{ return m_games; }
	concurrent_unordered_map<SOCKET, Player*>	Players()	{ return m_players; }

	void			AddPlayer( SOCKET clientSocket, Player* player );

	const Player*	GetPlayer( SOCKET clientSocket ) { m_players.at( clientSocket ); }

protected:

	void Games		( vector<Game*> val )			{ m_games = val; }
	void Players	( concurrent_unordered_map<SOCKET, Player*> val )	{ m_players = val; }


private:
	vector<Game*>								m_games;
	concurrent_unordered_map<SOCKET, Player*>	m_players;

};

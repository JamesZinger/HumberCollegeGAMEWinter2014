///////////////////////////////////////////////////////////
//  TCPGameServer.h
//  Implementation of the Class TCPGameServer
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__TCP_GAME_SERVER_H__)
#define __TCP_GAME_SERVER_H__

#include "TCPServer.h"
#include "Game.h"
#include "Router.h"

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using Concurrency::concurrent_unordered_map;
using Concurrency::concurrent_vector;

/// <summary>	A TCP game server that runs a type of game. </summary>
/// <remarks>	James, 2014-03-15. </remarks>


class TCPGameServer : public TCPServer
{
public:
	TCPGameServer( Game* game, int port = 8282 ) : TCPServer(port)
	{
		m_gameTemplate = game;
		m_games = concurrent_vector<Game*>();
	}

	virtual ~TCPGameServer()
	{
		delete m_gameTemplate;
	}

	virtual void Run();

	concurrent_vector<Game*> Games()	const { return m_games; }
	Game* GameTemplate()							const { return m_gameTemplate; }

	void			AddPlayer	( SOCKET clientSocket );
	const Player*	GetPlayer	( SOCKET clientSocket );
protected:
	void Games( concurrent_vector<Game*> val ) { m_games = val; }
	concurrent_unordered_map<SOCKET, Player*> Players() const { return m_players; }
	void Players( concurrent_unordered_map<SOCKET, Player*> val ) { m_players = val; }	

private:
	concurrent_vector<Game*> m_games;
	Game* m_gameTemplate;
	concurrent_unordered_map<SOCKET, Player*> m_players;
	
};
#endif // !defined(__TCP_GAME_SERVER_H__)

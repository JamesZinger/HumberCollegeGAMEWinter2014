///////////////////////////////////////////////////////////
//  BlackjackGame.h
//  Implementation of the Class BlackjackGame
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include "Game.h"
#include "Deck.h"
#include "House.h"
#include "BlackjackPlayer.h"
#include "BlackjackProtocol.h"

#include <boost/interprocess/containers/vector.hpp>
#include <iostream>
#include <ctime>

using std::cout;
using std::endl;
using std::ostream;
using std::time;
using boost::interprocess::vector;

namespace Blackjack
{

	class BlackjackGame : public Game
	{
	public:
		BlackjackGame( BlackjackPlayer* InitalPlayer, int maxPlayers = 2);

		virtual ~BlackjackGame();

		//plays the game of blackjack    
		void Play();

		bool AddPlayer( BlackjackPlayer* player );
		void RemovePlayer( BlackjackPlayer* player );
		int MaxPlayers() const { return m_maxPlayers; }

		vector<BlackjackPlayer*>& Players()	{ return m_players; }

		

	protected:

		virtual void GameThreadFunc( TCPGameServer* server );

		void Players( vector<BlackjackPlayer*> val ) { m_players = val; }
		void MaxPlayers( int val ) { m_maxPlayers = val; }
	private:
		Deck m_Deck;
		House m_House;

		vector<BlackjackPlayer*> m_players;

		bool isGameEmpty();

		int m_maxPlayers;
		
	};
}

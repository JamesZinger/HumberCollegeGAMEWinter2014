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
		BlackjackGame( BlackjackPlayer* InitalPlayer );

		virtual ~BlackjackGame();

		//plays the game of blackjack    
		void Play();

		void AddPlayer( BlackjackPlayer* player );
		void RemovePlayer( BlackjackPlayer* player );

		vector<BlackjackPlayer*> Players()	{ return m_players; }



	protected:

		virtual void GameThreadFunc( TCPGameServer* server );

		void Players( vector<BlackjackPlayer*> val ) { m_players = val; }

	private:
		Deck m_Deck;
		House m_House;

		vector<BlackjackPlayer*> m_players;

		bool isGameEmpty();

	};
}

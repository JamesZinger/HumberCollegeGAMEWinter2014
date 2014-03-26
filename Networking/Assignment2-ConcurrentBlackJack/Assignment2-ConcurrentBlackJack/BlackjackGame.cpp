///////////////////////////////////////////////////////////
//  BlackjackGame.cpp
//  Implementation of the Class BlackjackGame
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "BlackjackGame.h"
#include <boost/thread.hpp>

namespace Blackjack
{
	BlackjackGame::BlackjackGame( BlackjackPlayer* player )
	{
		srand( time( 0 ) );    //seed the random number generator
		m_Deck.Populate();
		m_Deck.Shuffle();
		Players()->push_back( player );
	}

	BlackjackGame::~BlackjackGame()
	{
	}

	void BlackjackGame::Play()
	{

		//deal initial 2 cards to everyone
		concurrent_vector<BlackjackPlayer*>::iterator pPlayer;
		for ( int i = 0; i < 2; ++i )
		{
			for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
				m_Deck.Deal( **pPlayer );
			m_Deck.Deal( m_House );
		}

		//hide house's first card
		m_House.FlipFirstCard();

		//display everyone's hand
		for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
			cout << *pPlayer << endl;
		cout << m_House << endl;

		//deal additional cards to players
		for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
			m_Deck.AdditionalCards( **pPlayer );

		//reveal house's first card
		m_House.FlipFirstCard();
		cout << endl << m_House;

		//deal additional cards to house
		m_Deck.AdditionalCards( m_House );

		if ( m_House.IsBusted() )
		{
			//everyone still playing wins
			for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
				if ( !( (*pPlayer)->IsBusted() ) )
					(*pPlayer)->Win();
		}
		else
		{
			//compare each player still playing to house
			for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
				if ( !( (*pPlayer)->IsBusted() ) )
				{
					if ( (*pPlayer)->GetTotal() > m_House.GetTotal() )
						(*pPlayer)->Win();
					else if ( (*pPlayer)->GetTotal() < m_House.GetTotal() )
						(*pPlayer)->Lose();
					else
						(*pPlayer)->Push();
				}
		}

		//remove everyone's cards
		for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
			(*pPlayer)->Clear();
		m_House.Clear();
	}

	void BlackjackGame::GameThreadFunc(TCPGameServer* server)
	{
		ThreadID( GetCurrentThreadId() );
		GameServer(server);

		while ( true )
		{
			Play();
			if (Players()->size() == 0)
			{
				break;
			}
		}
	}

}

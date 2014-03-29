///////////////////////////////////////////////////////////
//  BlackjackGame.cpp
//  Implementation of the Class BlackjackGame
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "BlackjackGame.h"
#include "TCPGameServer.h"
#include <boost/thread.hpp>

namespace Blackjack
{
	BlackjackGame::BlackjackGame( BlackjackPlayer* player, int maxPlayers)
	{
		srand( time( NULL ) );    //seed the random number generator
		m_Deck.Populate();
		m_Deck.Shuffle();
		
		m_Deck.Deal( m_House );

		//hide house's first card
		m_House.FlipFirstCard();

		AddPlayer( player );

		MaxPlayers(maxPlayers);

		TCPGameServer::Instance()->Games().push_back(this);
	}

	BlackjackGame::~BlackjackGame()
	{
	}

	void BlackjackGame::Play()
	{
		if ( isGameEmpty() )
			return;

		while ( true )
		{
			if ( isGameEmpty() )
				return;

			bool isPlayersReady = true;
			for (auto it = Players().begin(); it != Players().end(); ++it)
			{
				
			}
			
		}

		//display everyone's hand
		//for ( pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
		//	cout << *pPlayer << endl;
		//cout << m_House << endl;

		//deal additional cards to players
		for ( auto pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
			m_Deck.AdditionalCards( **pPlayer );

		//reveal house's first card
		m_House.FlipFirstCard();
		cout << endl << m_House;

		//deal additional cards to house
		m_Deck.AdditionalCards( m_House );

		if ( m_House.IsBusted() )
		{
			//everyone still playing wins
			for ( auto pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
				if ( !( ( *pPlayer )->IsBusted() ) )
					( *pPlayer )->Win();
		}
		else
		{
			//compare each player still playing to house
			for ( auto pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
				if ( !( ( *pPlayer )->IsBusted() ) )
				{
					if ( ( *pPlayer )->GetTotal() > m_House.GetTotal() )
						( *pPlayer )->Win();
					else if ( ( *pPlayer )->GetTotal() < m_House.GetTotal() )
						( *pPlayer )->Lose();
					else
						( *pPlayer )->Push();
				}
		}

		//remove everyone's cards
		for ( auto pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer )
			( *pPlayer )->Clear();
		m_House.Clear();
	}

	void BlackjackGame::GameThreadFunc( TCPGameServer* server )
	{
		ThreadID( GetCurrentThreadId() );
		GameServer( server );


		Play();

	}

	bool BlackjackGame::AddPlayer( BlackjackPlayer* player )
	{
		if (Players().size() < MaxPlayers())
		{
			Players().push_back( player );
			Deck().Deal( *player );

			return true;
		}
		return false;	
	}

	void BlackjackGame::RemovePlayer( BlackjackPlayer* player )
	{

		for ( auto it = Players().begin(); it != Players().end(); ++it )
		{
			if ( ( *it ) == player )
			{
				Players().erase( it );
			}
		}
	}

	bool BlackjackGame::isGameEmpty()
	{
		return (Players().size() == 0);
	}

}

///////////////////////////////////////////////////////////
//  BlackjackGame.cpp
//  Implementation of the Class BlackjackGame
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "BlackjackGame.h"
namespace Blackjack
{
	BlackjackGame::BlackjackGame( const vector<string>& names )
	{
		//create a vector of players from a vector of names       
		vector<string>::const_iterator pName;
		for ( pName = names.begin(); pName != names.end(); ++pName )
			m_Players.push_back( BlackjackPlayer( *pName ) );

		srand( time( 0 ) );    //seed the random number generator
		m_Deck.Populate();
		m_Deck.Shuffle();
	}

	BlackjackGame::~BlackjackGame()
	{
	}

	void BlackjackGame::Play()
	{
		//deal initial 2 cards to everyone
		vector<BlackjackPlayer>::iterator pPlayer;
		for ( int i = 0; i < 2; ++i )
		{
			for ( pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer )
				m_Deck.Deal( *pPlayer );
			m_Deck.Deal( m_House );
		}

		//hide house's first card
		m_House.FlipFirstCard();

		//display everyone's hand
		for ( pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer )
			cout << *pPlayer << endl;
		cout << m_House << endl;

		//deal additional cards to players
		for ( pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer )
			m_Deck.AdditionalCards( *pPlayer );

		//reveal house's first card
		m_House.FlipFirstCard();
		cout << endl << m_House;

		//deal additional cards to house
		m_Deck.AdditionalCards( m_House );

		if ( m_House.IsBusted() )
		{
			//everyone still playing wins
			for ( pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer )
				if ( !( pPlayer->IsBusted() ) )
					pPlayer->Win();
		}
		else
		{
			//compare each player still playing to house
			for ( pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer )
				if ( !( pPlayer->IsBusted() ) )
				{
					if ( pPlayer->GetTotal() > m_House.GetTotal() )
						pPlayer->Win();
					else if ( pPlayer->GetTotal() < m_House.GetTotal() )
						pPlayer->Lose();
					else
						pPlayer->Push();
				}
		}

		//remove everyone's cards
		for ( pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer )
			pPlayer->Clear();
		m_House.Clear();
	}

	//function prototypes
	ostream& operator<<( ostream& os, const Card& aCard );
	ostream& operator<<( ostream& os, const GenericPlayer& aGenericPlayer );

	ostream& operator<<( ostream& os, const Card& aCard )
	{
		const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9",
			"10", "J", "Q", "K" };
		const string SUITS[] = { "c", "d", "h", "s" };

		if ( aCard.m_IsFaceUp )
			os << RANKS[ aCard.m_Rank ] << SUITS[ aCard.m_Suit ];
		else
			os << "XX";

		return os;
	}

	//overloads << operator so a GenericPlayer object can be sent to cout
	ostream& operator<<( ostream& os, const GenericPlayer& aGenericPlayer )
	{
		os << aGenericPlayer.m_Name << ":\t";

		vector<Card*>::const_iterator pCard;
		if ( !aGenericPlayer.m_Cards.empty() )
		{
			for ( pCard = aGenericPlayer.m_Cards.begin();
				  pCard != aGenericPlayer.m_Cards.end(); ++pCard )
				  os << *( *pCard ) << "\t";

			if ( aGenericPlayer.GetTotal() != 0 )
				cout << "(" << aGenericPlayer.GetTotal() << ")";
		}
		else
		{
			os << "<empty>";
		}

		return os;
	}

}

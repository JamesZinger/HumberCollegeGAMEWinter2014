///////////////////////////////////////////////////////////
//  BlackjackPlayer.cpp
//  Implementation of the Class BlackjackPlayer
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "BlackjackPlayer.h"


namespace Blackjack
{
	BlackjackPlayer::BlackjackPlayer( const string& name ) : GenericPlayer( name )
	{
	}

	BlackjackPlayer::~BlackjackPlayer()
	{
	}

	bool BlackjackPlayer::IsHitting() const
	{
		cout << m_Name << ", do you want a hit? (Y/N): ";
		char response;
		cin >> response;
		return ( response == 'y' || response == 'Y' );
	}

	void BlackjackPlayer::Win() const
	{
		cout << m_Name << " wins.\n";
	}

	void BlackjackPlayer::Lose() const
	{
		cout << m_Name << " loses.\n";
	}

	void BlackjackPlayer::Push() const
	{
		cout << m_Name << " pushes.\n";
	}
}



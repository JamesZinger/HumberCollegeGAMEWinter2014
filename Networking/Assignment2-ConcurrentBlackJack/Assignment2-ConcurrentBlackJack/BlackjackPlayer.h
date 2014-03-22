///////////////////////////////////////////////////////////
//  BlackjackPlayer.h
//  Implementation of the Class BlackjackPlayer
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include "Player.h"
#include "GenericPlayer.h"

#include <string>
#include <iostream>

using std::string;
using std::cin;

namespace Blackjack
{
	class BlackjackPlayer : public GenericPlayer , public Player
	{

	public:
		BlackjackPlayer( const string& name = "" );
		virtual ~BlackjackPlayer();

		//returns whether or not the player wants another hit       
		virtual bool IsHitting() const;

		//announces that the player wins
		void Win() const;

		//announces that the player loses
		void Lose() const;

		//announces that the player pushes
		void Push() const;

	};
}
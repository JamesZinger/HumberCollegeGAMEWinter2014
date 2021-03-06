///////////////////////////////////////////////////////////
//  BlackjackPlayer.h
//  Implementation of the Class BlackjackPlayer
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include "Player.h"
#include "GenericPlayer.h"
#include "MessageInput.h"


#include <iostream>
#include <boost/interprocess/containers/vector.hpp>

using std::cin;

namespace Blackjack
{

	enum PlayerState
	{
		Game,
		Lobby,
		Shutdown
	};

	class BlackjackPlayer : public GenericPlayer, public Player
	{

	public:
		BlackjackPlayer();
		virtual ~BlackjackPlayer();

		//returns whether or not the player wants another hit       
		virtual bool IsHitting() const;

		//announces that the player wins
		void Win() const;

		//announces that the player loses
		void Lose() const;

		//announces that the player pushes
		void Push() const;

		Blackjack::PlayerState State() const { return m_state; }


	protected:

		virtual void HandleMessage( boost::interprocess::string Message );
		virtual void Init();

		virtual void HandleCreateGameRequest	( MessageInput*	input );
		virtual void HandleJoinGameRequest		( MessageInput*	input );
		virtual void HandleDisconnectRequest	( MessageInput*	input );
		virtual void HandleRefreshRequest		( MessageInput*	input );
		virtual void HandleInvaildRequest		( boost::interprocess::string error );

		void State( Blackjack::PlayerState val ) { m_state = val; }

	private:
		PlayerState m_state;

		MessageInput* ConstructMessageInput		( boost::interprocess::vector< boost::interprocess::string > lines );
		MessageInput* ConstructGameInput		( boost::interprocess::vector< boost::interprocess::string > lines );


	};
}
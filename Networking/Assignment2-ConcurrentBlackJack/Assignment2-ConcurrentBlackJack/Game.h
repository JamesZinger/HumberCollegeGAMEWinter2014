///////////////////////////////////////////////////////////
//  Game.h
//  Implementation of the Class Game
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include "MessageInput.h"
#include "Player.h"
#include <concurrent_vector.h>

class Game
{
public:

	Game();
	virtual ~Game();
	
	void EnqueueMessage(MessageInput *msg);

#pragma region Getters

	unsigned long									ThreadID()		const	{ return m_threadID; }
	Concurrency::concurrent_queue<MessageInput*>*	InputQueue()			{ return &m_inputQueue; }
	Concurrency::concurrent_vector<Player*>*		Players()				{ return &m_players; }

#pragma endregion

protected:

#pragma region Setters
	
	void ThreadID( unsigned long val ) { m_threadID = val; }
	void Players( Concurrency::concurrent_vector<Player*> val ) { m_players = val; }

#pragma endregion

private:
	Concurrency::concurrent_queue<MessageInput*>	m_inputQueue;
	unsigned long									m_threadID;
	Concurrency::concurrent_vector<Player*>			m_players;
	
	

};
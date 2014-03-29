///////////////////////////////////////////////////////////
//  Player.h
//  Implementation of the Class Player
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include "MessageOutput.h"
#include <concurrent_queue.h>
#include <WinSock2.h>


class Game;
namespace boost
{
	class thread;
}

using boost::thread;
using Concurrency::concurrent_queue;

class Player
{

public:
	Player(int BufferLength = 1024);
	virtual ~Player();

	virtual void PlayerThreadFunc	( const boost::interprocess::string name, SOCKET Client );
	virtual void EnqueueMessage		( MessageOutput* Message );

#pragma region Getters

	Game*							GetGame()			const { return m_game; }
	SOCKET							Socket()			const { return m_socket; }
	unsigned long					ThreadID()			const { return m_threadID; }
	boost::interprocess::string&	Name()				const { return *m_name; }
	int								InputBufferLength()	const { return m_inputBufferLength; }
	bool							ShuttingDown()		const { return m_shuttingDown; }

#pragma endregion

protected:

	virtual int RecieveMessage				();
	virtual void SendNetworkMessage			();

	virtual void HandleMessage( boost::interprocess::string message )	= 0;
	virtual void Init()									= 0;

#pragma region Setters

	// Setters
	void SetGame		( Game* val )							{ m_game = val; }
	void Socket			( SOCKET val )							{ m_socket = val;  }
	void ThreadID		( unsigned long val )					{ m_threadID = val; }
	void Name			( boost::interprocess::string* val )	{ m_name = val; }
	void ShuttingDown	( bool val )							{ m_shuttingDown = val; }

#pragma endregion

private:
	Game*								m_game;
	char*								m_inputArray;
	const int							m_inputBufferLength;
	SOCKET								m_socket;
	unsigned long						m_threadID;
	concurrent_queue<MessageOutput*>	m_messageQueue;
	boost::interprocess::string*		m_name;
	bool								m_shuttingDown;
	
	bool SendMessageOverSocket( boost::interprocess::string& Message, int* SendLength );
	
};
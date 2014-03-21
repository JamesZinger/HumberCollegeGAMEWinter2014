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
using std::string;
using Concurrency::concurrent_queue;

class Player
{

public:
	Player(int BufferLength);
	virtual ~Player();

	virtual void PlayerThreadFunc( const string& name );
	virtual void EnqueueMessage( MessageOutput* Message );

#pragma region Getters

	Game*			GetGame()			const { return m_game; }
	SOCKET			Socket()			const { return m_socket; }
	unsigned long	ThreadID()			const { return m_threadID; }
	string*			Name()				const { return m_name; }
	int				InputBufferLength()	const { return m_inputBufferLength; }

#pragma endregion

protected:

	virtual int RecieveMessage();
	virtual void SendNetworkMessage();

#pragma region Setters

	// Setters
	void SetGame	( Game* val )			{ m_game = val; }
	void Socket		( SOCKET val )			{ m_socket = val;  }
	void ThreadID	( unsigned long val )	{ m_threadID = val; }
	void Name		( string* val )			{ m_name = val; }

#pragma endregion

private:
	Game*								m_game;
	char*								m_inputArray;
	const int							m_inputBufferLength;
	SOCKET								m_socket;
	unsigned long						m_threadID;
	concurrent_queue<MessageOutput*>	m_messageQueue;
	string*								m_name;

	bool SendMessageOverSocket( std::string& Message, int* SendLength );
	void HandleMessage( std::string message );
};
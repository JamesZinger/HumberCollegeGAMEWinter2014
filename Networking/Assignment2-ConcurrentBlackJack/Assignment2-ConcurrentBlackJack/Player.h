///////////////////////////////////////////////////////////
//  Player.h
//  Implementation of the Class Player
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__PLAYER_H__)
#define __PLAYER_H__

#include "MessageOutput.h"
#include <concurrent_queue.h>
#include <WinSock2.h>

class Game;

class Player
{

public:
	Player();
	virtual ~Player();
	Game* Getgame();
	SOCKET GetSocket();
	int GetThreadID();

protected:
	void Setgame(Game* newVal);
	void SetSocket(SOCKET newVal);
	void SetThreadID(int newVal);
	bool RecieveMessage(std::string* Message);
	bool SendMessage(std::string& Message, int* SendLength);

	MessageOutput m_outputQueue;


private:
	Game *m_game;
	char* m_inputArray;
	SOCKET m_socket;
	int m_threadID;
	Concurrency::concurrent_queue<MessageOutput*> m_messageQueue;


};
#endif // !defined(__PLAYER_H__)

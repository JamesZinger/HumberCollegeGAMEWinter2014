///////////////////////////////////////////////////////////
//  Game.h
//  Implementation of the Class Game
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__GAME_H__)
#define __GAME_H__

#include "MessageInput.h"
#include "Player.h"

#include <vector>
#include <concurrent_queue.h>

class Game
{

public:
	Player* m_Player;

	Game();
	virtual ~Game();
	int GetthreadID();

protected:
	void SetthreadID(int newVal);

private:
	Concurrency::concurrent_queue<MessageInput*> m_inputQueue;
	std::vector<Player*> m_playersvector;
	int m_threadID;

};
#endif // !defined(__GAME_H__)

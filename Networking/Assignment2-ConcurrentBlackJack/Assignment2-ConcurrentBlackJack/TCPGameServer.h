///////////////////////////////////////////////////////////
//  TCPGameServer.h
//  Implementation of the Class TCPGameServer
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__TCP_GAME_SERVER_H__)
#define __TCP_GAME_SERVER_H__

#include "TCPServer.h"
#include "Game.h"

#include <vector>

template <class T>
class TCPGameServer : public TCPServer
{
	T Game;
public:
	TCPGameServer();
	virtual ~TCPGameServer();

	virtual void Run();

private:
	std::vector<Game*> m_Games;

};
#endif // !defined(__TCP_GAME_SERVER_H__)

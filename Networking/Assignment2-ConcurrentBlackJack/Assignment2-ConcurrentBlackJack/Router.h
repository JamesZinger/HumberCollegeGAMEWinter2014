///////////////////////////////////////////////////////////
//  Router.h
//  Implementation of the Class Router
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include <WinSock2.h>

#include "Protocol.h"

class TCPGameServer;


class Router
{
public:
	Router();
	virtual ~Router();

	void RoutingThreadFunc( SOCKET Request, TCPGameServer* parentServer );

#pragma region Getters

	unsigned long	ThreadID()		const { return m_threadID; }
	TCPGameServer*	GameServer()	const { return m_gameServer; }

#pragma endregion 

protected:

#pragma region Setters
	
	void ThreadID	( unsigned long val )	{ m_threadID = val; }
	void GameServer	( TCPGameServer* val )	{ m_gameServer = val; }

#pragma endregion 

private:

	unsigned long m_threadID;
	TCPGameServer* m_gameServer;

};
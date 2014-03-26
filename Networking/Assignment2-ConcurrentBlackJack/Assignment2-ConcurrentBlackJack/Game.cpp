///////////////////////////////////////////////////////////
//  Game.cpp
//  Implementation of the Class Game
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "Game.h"




Game::Game()
{
	ThreadID(0);
}


Game::~Game()
{

}

void Game::EnqueueMessage( MessageInput *msg )
{
	m_inputQueue.push(msg);
}

///////////////////////////////////////////////////////////
//  Player.cpp
//  Implementation of the Class Player
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "Player.h"


Player::Player()
{

}


Player::~Player()
{

}


Game* Player::Getgame()
{

	return  m_game;
}


SOCKET Player::GetSocket()
{

	return  m_socket;
}


int Player::GetThreadID()
{

	return  m_threadID;
}


void Player::Setgame(Game* newVal)
{
	m_game = newVal;
}


void Player::SetSocket(SOCKET newVal)
{
	m_socket = newVal;
}


void Player::SetThreadID(int newVal)
{
	m_threadID = newVal;
}


bool Player::RecieveMessage(std::string* Message)
{

	return false;
}


bool Player::SendMessage(std::string& Message, int* SendLength)
{

	return false;
}
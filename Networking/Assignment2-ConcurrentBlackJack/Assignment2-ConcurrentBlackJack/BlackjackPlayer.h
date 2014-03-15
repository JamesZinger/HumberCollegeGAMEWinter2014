///////////////////////////////////////////////////////////
//  BlackjackPlayer.h
//  Implementation of the Class BlackjackPlayer
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__BLACK_JACK_PLAYER_H__)
#define __BLACK_JACK_PLAYER_H__

#include "Player.h"

class BlackjackPlayer : public Player
{

public:
	BlackjackPlayer();
	virtual ~BlackjackPlayer();
	std::string getName();
	void setName(std::string newVal);

private:
	std::string m_name;

};
#endif // !defined(__BLACK_JACK_PLAYER_H__)

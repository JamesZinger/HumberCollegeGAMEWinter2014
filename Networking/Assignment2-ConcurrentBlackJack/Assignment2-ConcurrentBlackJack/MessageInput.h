#pragma once
#include <string>

#include "Player.h"

class MessageInput
{

public:
	MessageInput();
	virtual ~MessageInput();

	std::string m_message;
	Player *m_player;
	
};

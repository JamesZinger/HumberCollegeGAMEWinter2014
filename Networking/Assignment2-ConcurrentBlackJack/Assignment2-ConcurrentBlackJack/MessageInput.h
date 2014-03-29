#pragma once

#include <boost/interprocess/containers/string.hpp>

class Player;

class MessageInput
{

public:
	MessageInput();
	virtual ~MessageInput();

	boost::interprocess::string m_message;
	Player *m_player;
	
};

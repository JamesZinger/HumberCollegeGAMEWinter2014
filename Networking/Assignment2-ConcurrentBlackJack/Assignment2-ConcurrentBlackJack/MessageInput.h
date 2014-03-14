///////////////////////////////////////////////////////////
//  MessageInput.h
//  Implementation of the Class MessageInput
//  Created on:      14-Mar-2014 5:35:37 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__MESSAGE_INPUT_H__)
#define __MESSAGE_INPUT_H__

#include <string>

#include "Player.h"


class MessageInput
{

public:
	MessageInput();
	virtual ~MessageInput();
	std::string *m_message;
	Player *m_player;

};
#endif // !defined(__MESSAGE_INPUT_H__)

///////////////////////////////////////////////////////////
//  MessageOutput.h
//  Implementation of the Class MessageOutput
//  Created on:      14-Mar-2014 5:35:51 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include <string>

class MessageOutput
{

public:
	MessageOutput();
	virtual ~MessageOutput();

	std::string * Message() { return m_message; }
	void Message( std::string * val ) { m_message = val; }

private:
	std::string *m_message;
	
	
};
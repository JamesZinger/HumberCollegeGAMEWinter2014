///////////////////////////////////////////////////////////
//  MessageOutput.h
//  Implementation of the Class MessageOutput
//  Created on:      14-Mar-2014 5:35:51 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__MESSAGE_OUTPUT_H__)
#define __MESSAGE_OUTPUT_H__

#include <string>

class MessageOutput
{

public:
	MessageOutput();
	virtual ~MessageOutput();
	std::string *m_message;

};
#endif // !defined(__MESSAGE_OUTPUT_H__)

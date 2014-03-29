///////////////////////////////////////////////////////////
//  MessageOutput.h
//  Implementation of the Class MessageOutput
//  Created on:      14-Mar-2014 5:35:51 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once

#include <boost/interprocess/containers/string.hpp>

class MessageOutput
{

public:
	MessageOutput();
	virtual ~MessageOutput();

	boost::interprocess::string& Message() { return m_message; }
	void Message( boost::interprocess::string val ) { m_message = val; }

private:
	boost::interprocess::string m_message;
	
	
};
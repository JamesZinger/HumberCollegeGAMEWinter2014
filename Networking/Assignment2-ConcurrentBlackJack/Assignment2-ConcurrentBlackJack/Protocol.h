///////////////////////////////////////////////////////////
//  Protocol.h
//  Implementation of the Class Protocol
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__PROTOCOL_H__)
#define __PROTOCOL_H__

#include <string>

class Protocol
{

public:
	Protocol();
	virtual ~Protocol();
	virtual bool DeserializeMessage(Protocol protocol, std::string* message);
	virtual bool SerializeMessage(std::string Message, Protocol protocol);

};
#endif // !defined(__PROTOCOL_H__)

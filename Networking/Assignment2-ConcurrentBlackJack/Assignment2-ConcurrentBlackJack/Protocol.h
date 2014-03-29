///////////////////////////////////////////////////////////
//  Protocol.h
//  Implementation of the Class Protocol
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once


#include <WinSock2.h>
#include <boost/interprocess/containers/string.hpp>

class Server;

class Protocol
{
public:
	virtual void HandleRequest( const SOCKET client, boost::interprocess::string& recieveString, const Server* server ) = 0;
};
///////////////////////////////////////////////////////////
//  TCPServer.h
//  Implementation of the Class TCPServer
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__TCP_SERVER_H__)
#define __TCP_SERVER_H__

#include "Server.h"

class TCPServer : public Server
{

public:
	TCPServer();
	virtual ~TCPServer();
	void CreateRoutingThread(SOCKET connection);

private:
	bool AcceptConnection();

};
#endif // !defined(__TCP_SERVER_H__)

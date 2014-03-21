///////////////////////////////////////////////////////////
//  TCPServer.h
//  Implementation of the Class TCPServer
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__TCP_SERVER_H__)
#define __TCP_SERVER_H__

#include "Server.h"
#include <WS2tcpip.h>

class TCPServer : public Server
{

public:
	
	TCPServer(Protocol* proto, int port = 8282);
	virtual ~TCPServer();
	virtual void Run() = 0;
	virtual int RecieveMessage( SOCKET MessageSocket, char* Buffer, int BufferLength, std::string* out );
	virtual void SendMessageOverNetwork	( SOCKET MessageSocket, std::string& Message );

protected:

	SOCKET AcceptConnection();
};
#endif // !defined(__TCP_SERVER_H__)

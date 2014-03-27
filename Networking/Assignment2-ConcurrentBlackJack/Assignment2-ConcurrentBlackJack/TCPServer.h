#pragma once
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
	virtual SOCKET AcceptConnection(sockaddr_in* addrPtr = nullptr);


};

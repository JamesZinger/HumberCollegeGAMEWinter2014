///////////////////////////////////////////////////////////
//  Server.h
//  Implementation of the Class Server
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__SERVER_H__)
#define __SERVER_H__

#include <WinSock2.h>
#include <string>

class Server
{
public:
	Server();
	Server(const int port);
	virtual ~Server();
	unsigned short GetListenPort();
	SOCKET GetListenSocket();
	bool isConnected();
	boolean IsDebugging();
	virtual void Run() = 0;
	
protected:
	void SetDebugging(boolean newVal);
	void SetListenPort(unsigned short newVal);
	void SetListenSocket(SOCKET newVal);

private:
	boolean m_debugging;
	unsigned short m_listenPort;
	SOCKET m_listenSocket;
	char *m_recieveBuffer;

};
#endif // !defined(__SERVER_H__)

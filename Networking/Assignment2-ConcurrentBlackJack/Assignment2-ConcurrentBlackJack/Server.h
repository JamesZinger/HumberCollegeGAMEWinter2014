///////////////////////////////////////////////////////////
//  Server.h
//  Implementation of the Class Server
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once 

#include <WinSock2.h>
#include <string>

class Protocol;

namespace boost
{
	class thread_group;
}

struct ipAddr
{
	char b1;
	char b2;
	char b3;
	char b4;

};

inline bool operator==( const ipAddr& i1, const ipAddr i2 )
{
	if ( i1.b1 == i2.b1 &&
		 i1.b2 == i2.b2 &&
		 i1.b3 == i2.b3 &&
		 i1.b4 == i2.b4 )
		 return true;

	return false;
}

/// <summary>	A generic server class. </summary>
class Server
{
public:
	Server( Protocol* proto, const int port = 8282 );
	virtual ~Server();

	virtual void			Run						() = 0;
	virtual void			SendMessageOverNetwork	( SOCKET MessageSocket, std::string& Message ) = 0;
	virtual std::string*	RecieveMessage			( SOCKET MessageSocket, char* Buffer, int BufferLength ) = 0;

	unsigned short			ListenPort()	const { return m_listenPort; }
	SOCKET					ListenSocket()	const { return m_listenSocket; }
	bool					isConnected()	const { return ( m_listenSocket != INVALID_SOCKET ); }
	boolean					Debugging()		const { return m_debugging; }
	WSAData*				WsaData()		const { return m_wsaData; }
	boost::thread_group*	ThreadGroup()	const { return m_threadGroup; }
	Protocol*				GetProtocol()	const { return m_protocol; }

	void Debugging( boolean val ) { m_debugging = val; }


protected:

	void ListenPort		( unsigned short val )			{ m_listenPort = val; }
	void ListenSocket	( SOCKET val )					{ m_listenSocket = val; }
	void WsaData		( WSAData* val )				{ m_wsaData = val; }
	void ThreadGroup	( boost::thread_group* val )	{ m_threadGroup = val; }
	void SetProtocol	( Protocol* val )				{ m_protocol = val; }

private:
	boolean					m_debugging;
	unsigned short			m_listenPort;
	SOCKET					m_listenSocket;
	WSAData*				m_wsaData;
	boost::thread_group*	m_threadGroup;
	Protocol*				m_protocol;
	
	
};
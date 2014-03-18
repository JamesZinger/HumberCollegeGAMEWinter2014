///////////////////////////////////////////////////////////
//  Server.h
//  Implementation of the Class Server
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once 

#include <WinSock2.h>
#include <string>

namespace boost
{
	class thread_group;
}

/// <summary>	A generic server class. </summary>
class Server
{
public:
	Server( const int port = 8282 );
	virtual ~Server();

	virtual void			Run				() = 0;
	virtual void			SendMessage		( SOCKET MessageSocket, std::string& Message ) = 0;
	virtual std::string*	RecieveMessage	( SOCKET MessageSocket, char* Buffer, int BufferLength ) = 0;

	unsigned short			ListenPort()	const { return m_listenPort; }
	SOCKET					ListenSocket()	const { return m_listenSocket; }
	bool					isConnected()	const { return ( m_listenSocket != INVALID_SOCKET ); }
	boolean					Debugging()		const { return m_debugging; }
	WSAData*				WsaData()		const { return m_wsaData; }
	boost::thread_group*	ThreadGroup()	const { return m_threadGroup; }

	void Debugging( boolean val ) { m_debugging = val; }


protected:

	void ListenPort		( unsigned short val )			{ m_listenPort = val; }
	void ListenSocket	( SOCKET val )					{ m_listenSocket = val; }
	void WsaData		( WSAData* val )				{ m_wsaData = val; }
	void ThreadGroup	( boost::thread_group* val )	{ m_threadGroup = val; }

private:
	boolean					m_debugging;
	unsigned short			m_listenPort;
	SOCKET					m_listenSocket;
	WSAData*				m_wsaData;
	boost::thread_group*	m_threadGroup;

};
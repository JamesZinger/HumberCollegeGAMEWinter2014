///////////////////////////////////////////////////////////
//  Server.h
//  Implementation of the Class Server
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#pragma once 

#include <WinSock2.h>
#include <boost/date_time.hpp>
#include <concurrent_unordered_map.h>
#include <boost/interprocess/containers/string.hpp>

class Protocol;

using Concurrency::concurrent_unordered_map;

struct ipAddr
{
	char b1;
	char b2;
	char b3;
	char b4;

};

struct connectionStamp
{
	ipAddr addr;
	boost::posix_time::ptime initalConnectionTime;
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

	virtual void	Run						() = 0;
	virtual void	SendMessageOverNetwork	( SOCKET MessageSocket, boost::interprocess::string& Message ) = 0;
	virtual int		RecieveMessage			( SOCKET MessageSocket, char* Buffer, int BufferLength, boost::interprocess::string* out ) = 0;

	unsigned short		ListenPort()		const	{ return m_listenPort; }
	SOCKET				ListenSocket()		const	{ return m_listenSocket; }
	bool				isConnected()		const	{ return ( m_listenSocket != INVALID_SOCKET ); }
	boolean				Debugging()			const	{ return m_debugging; }
	WSAData*			WsaData()			const	{ return m_wsaData; }
	Protocol*			GetProtocol()		const	{ return m_protocol; }
	
	concurrent_unordered_map<SOCKET, connectionStamp>&	ActiveConnections()			{ return m_activeConnections; }

	void				Debugging( boolean val ) { m_debugging = val; }

protected:

	void ListenPort			( unsigned short val )		{ m_listenPort = val; }
	void ListenSocket		( SOCKET val )				{ m_listenSocket = val; }
	void WsaData			( WSAData* val )			{ m_wsaData = val; }
	void SetProtocol		( Protocol* val )			{ m_protocol = val; }

	void ActiveConnections	( concurrent_unordered_map<SOCKET, connectionStamp> val )	{ m_activeConnections = val; }

private:
	boolean				m_debugging;
	unsigned short		m_listenPort;
	SOCKET				m_listenSocket;
	WSAData*			m_wsaData;
	Protocol*			m_protocol;

	concurrent_unordered_map<SOCKET, connectionStamp>	m_activeConnections;

};


#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <concurrent_queue.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

using std::string;
using Concurrency::concurrent_queue;
using boost::thread;

class BlackjackClient
{
public:

	BlackjackClient( string name, string hostname, int port, int bufferLength = 1024);
	~BlackjackClient();

	bool SendData		( string& message, int* sentLength = NULL );
	int	 RecieveData	( string* message );
	void SendShutdown	();
	void Disconnect		();
	void Run			();

	void NetworkThreadFunc();

	bool		isConnected()	const { return ( m_clientSocket != INVALID_SOCKET ); }
	string		Name()			const { return m_name; }
	SOCKET		ClientSocket()	const { return m_clientSocket; }
	char*		InputBuffer()	const { return m_inputBuffer; }
	const int	BufferLength()	const { return m_bufferLength; }
	int			Port()			const { return m_port; }
	string		Hostname()		const { return m_hostname; }
	WSADATA		WsaData()		const { return m_wsaData; }
	bool		ShuttingDown()	const { return m_isShuttingDown; }

protected:

	void ClientSocket	( SOCKET val )	{ m_clientSocket = val; }
	void Name			( string val )	{ m_name = val; }
	void InputBuffer	( char* val )	{ m_inputBuffer = val; }
	void Hostname		( string val )	{ m_hostname = val; }
	void Port			( int val )		{ m_port = val; }
	void WsaData		( WSADATA val )	{ m_wsaData = val; }
	void ShuttingDown	( bool val )	{ m_isShuttingDown = val; }

	void HandleMessage	( string& message );
	void Update			();

private:

	string						m_name;
	string						m_hostname;
	int							m_port;
	SOCKET						m_clientSocket;
	char*						m_inputBuffer;
	const int					m_bufferLength;
	WSADATA						m_wsaData;
	concurrent_queue<string*>	m_inputQueue;
	concurrent_queue<string*>	m_outputQueue;
	thread*						m_networkingThread;
	bool						m_isShuttingDown;
};


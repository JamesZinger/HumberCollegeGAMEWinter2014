#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>

using Concurrency::concurrent_queue;
using Concurrency::concurrent_unordered_map;
using boost::thread;


enum PlayerCommands
{
	Connect = 1,
	JoinGame,
	LeaveGame,
	Hit,
	Stay,
	Refresh,
	CreateGame,
	Disconnect
};

enum PlayerState
{
	Game,
	Lobby,
	Shutdown
};

static const boost::interprocess::string SERVER_HEADER = boost::interprocess::string( "BlackJackServer Protocol 1.0" );
static const boost::interprocess::string CLIENT_HEADER = boost::interprocess::string( "BlackJackClient Protocol 1.0" );

class BlackjackClient
{
public:

	BlackjackClient( boost::interprocess::string name, boost::interprocess::string hostname, int port, int bufferLength = 1024 );
	~BlackjackClient();

	bool SendData		( boost::interprocess::string& message, int* sentLength = NULL );
	int	 RecieveData	( boost::interprocess::string* message );

	void SendShutdown();
	void DisconnectFromServer();
	void Run();
	void NetworkThreadFunc();


	bool							isConnected()	const { return ( m_clientSocket != INVALID_SOCKET ); }
	boost::interprocess::string		Name()			const { return m_name; }
	SOCKET							ClientSocket()	const { return m_clientSocket; }
	char*							InputBuffer()	const { return m_inputBuffer; }
	const int						BufferLength()	const { return m_bufferLength; }
	int								Port()			const { return m_port; }
	boost::interprocess::string		Hostname()		const { return m_hostname; }
	WSADATA							WsaData()		const { return m_wsaData; }
	bool							ShuttingDown()	const { return m_isShuttingDown; }

	static concurrent_unordered_map<PlayerCommands, std::string>&	CommandMap()	{ return m_commandMap; }
	static concurrent_unordered_map<std::string, PlayerState>&		StateMap()		{ return m_stateMap; }

	static boost::interprocess::string BuildBlackJackRequest( PlayerCommands command, boost::interprocess::string* extraInfo = nullptr );

protected:

	void ClientSocket		( SOCKET val )							{ m_clientSocket = val; }
	void Name				( boost::interprocess::string val )		{ m_name = val; }
	void InputBuffer		( char* val )							{ m_inputBuffer = val; }
	void Hostname			( boost::interprocess::string val )		{ m_hostname = val; }
	void Port				( int val )								{ m_port = val; }
	void WsaData			( WSADATA val )							{ m_wsaData = val; }
	void ShuttingDown		( bool val )							{ m_isShuttingDown = val; }

	void HandleMessage			( boost::interprocess::string& message );
	void Update();
	void HandleGameMessage		( std::vector<boost::interprocess::string> lines );
	void HandleLobbyMessage		( std::vector<boost::interprocess::string> lines );

	static void StateMap	( concurrent_unordered_map<std::string, PlayerState> val )		{ m_stateMap = val; }
	static void CommandMap	( concurrent_unordered_map<PlayerCommands, std::string> val )	{ m_commandMap = val; }
private:

	boost::interprocess::string						m_name;
	boost::interprocess::string						m_hostname;
	int												m_port;
	SOCKET											m_clientSocket;
	char*											m_inputBuffer;
	const int										m_bufferLength;
	WSADATA											m_wsaData;
	concurrent_queue<boost::interprocess::string*>	m_inputQueue;
	concurrent_queue<boost::interprocess::string*>	m_outputQueue;
	thread*											m_networkingThread;
	bool											m_isShuttingDown;

	static concurrent_unordered_map<PlayerCommands, std::string>	m_commandMap;
	static concurrent_unordered_map<std::string, PlayerState>		m_stateMap;
	static concurrent_unordered_map<std::string, PlayerCommands>	m_inputMap;

};


#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <vector>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

using std::string;
using Concurrency::concurrent_queue;
using Concurrency::concurrent_unordered_map;
using boost::thread;
using std::vector;

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

static const string SERVER_HEADER = string( "BlackJackServer Protocol 1.0" );
static const string CLIENT_HEADER = string( "BlackJackClient Protocol 1.0" );

class BlackjackClient
{
public:

	BlackjackClient( string name, string hostname, int port, int bufferLength = 1024 );
	~BlackjackClient();

	bool SendData( string& message, int* sentLength = NULL );
	int	 RecieveData( string* message );
	void SendShutdown();
	void DisconnectFromServer();
	void Run();

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

	static concurrent_unordered_map<PlayerCommands, string>*	CommandMap() { return &m_commandMap; }
	static concurrent_unordered_map<string, PlayerState>*		StateMap() { return &m_stateMap; }

	static string BuildBlackJackRequest( PlayerCommands command, void* extraInfo );

protected:

	void ClientSocket( SOCKET val ) { m_clientSocket = val; }
	void Name( string val ) { m_name = val; }
	void InputBuffer( char* val ) { m_inputBuffer = val; }
	void Hostname( string val ) { m_hostname = val; }
	void Port( int val ) { m_port = val; }
	void WsaData( WSADATA val ) { m_wsaData = val; }
	void ShuttingDown( bool val ) { m_isShuttingDown = val; }

	void HandleMessage( string& message );
	void Update();
	void HandleGameMessage( vector<string> lines );
	void HandleLobbyMessage( vector<string> lines );

	static void StateMap( concurrent_unordered_map<string, PlayerState> val ) { m_stateMap = val; }
	static void CommandMap( concurrent_unordered_map<PlayerCommands, string> val ) { m_commandMap = val; }
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

	static concurrent_unordered_map<PlayerCommands, string>		m_commandMap;
	static concurrent_unordered_map<string, PlayerState>		m_stateMap;


};


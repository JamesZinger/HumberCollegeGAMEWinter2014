#include "BlackjackClient.h"

#include <iostream>
#include <sstream>
#include <WS2tcpip.h>
#include <boost\assign.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\foreach.hpp>

using std::cin;
using std::cout;
using std::endl;
using std::stringstream;
using boost::split;
using boost::trim;
using boost::is_any_of;
using boost::interprocess::string;


BlackjackClient::BlackjackClient( string name, string hostname, int port, int bufferLength ) : m_bufferLength( bufferLength )
{
	Name( name );
	ClientSocket( INVALID_SOCKET );
	m_inputBuffer = (char*)malloc( bufferLength );
	Hostname( hostname );
	Port( port );
	ShuttingDown( false );

	addrinfo	*result = nullptr,
		*ptr = nullptr,
		hints;

	int iResult;

	iResult = WSAStartup( MAKEWORD( 2, 2 ), &WsaData() );
	if ( iResult != 0 )
	{
		cout << "WSAStartup failed, Error Code: " << iResult << endl;
		system( "Pause" );
		exit( -1 );
	}

	memset( &hints, '\0', sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	stringstream ss;
	string s;
	ss << port;

	s = ss.str().c_str();

	iResult = getaddrinfo( hostname.c_str(), s.c_str(), &hints, &result );
	if ( iResult != 0 )
	{
		cout << "Getaddrinfo failed, Error code: " << iResult << endl;
		WSACleanup();
		system( "Pause" );
		exit( -2 );
	}

	for ( ptr = result; ptr != NULL; ptr = ptr->ai_next )
	{
		m_clientSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
		if ( m_clientSocket == INVALID_SOCKET )
		{
			cout << "Error creating socket, WSA Error code: " << WSAGetLastError() << endl;
			freeaddrinfo( result );
			WSACleanup();
			system( "Pause" );
			exit( -2 );
		}


		iResult = connect( m_clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen );
		if ( iResult == SOCKET_ERROR )
		{
			int errorCode = WSAGetLastError();
			if ( errorCode == WSAEWOULDBLOCK )
			{
				break;
			}
			closesocket( m_clientSocket );
			ClientSocket( INVALID_SOCKET );
			continue;
		}
		break;
	}

	u_long iMode = 1;
	ioctlsocket( m_clientSocket, FIONBIO, &iMode );

	freeaddrinfo( result );

	if ( ClientSocket() == INVALID_SOCKET )
	{
		cout << "Unable to connect to server" << endl;
		WSACleanup();
		system( "Pause" );
		exit( -3 );
	}

}

BlackjackClient::~BlackjackClient()
{
	DisconnectFromServer();
	delete[] m_inputBuffer;
}

bool BlackjackClient::SendData( string& message, int* sentLength /*= NULL */ )
{
	if ( ClientSocket() == INVALID_SOCKET )
	{
		cout << "Cannot send, No client is attached" << endl;
		return false;
	}
	const char* msg = message.c_str();
	int i = send( ClientSocket(), msg, message.length(), 0 );

	if ( sentLength != nullptr )
		*sentLength = i;

	if ( i == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode == WSAEWOULDBLOCK )
		{
			return false;
		}
		cout << "Sending failed, WSA Error code: " << errorCode << endl;
		DisconnectFromServer();
		return false;
	}

	return true;
}

int BlackjackClient::RecieveData( string* message )
{
	if ( ClientSocket() == INVALID_SOCKET )
	{
		cout << "No client is attached." << endl;
		return -1;
	}
	memset( InputBuffer(), '\0', m_bufferLength );
	int receivedLength = recv( ClientSocket(), InputBuffer(), m_bufferLength, 0 );
	if ( receivedLength == 0 )
	{
		cout << "Connection closing..." << endl;
		DisconnectFromServer();
		return -1;
	}
	else  if ( receivedLength < 0 )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode == WSAEWOULDBLOCK )
		{
			return 0;
		}
		cout << "recv failed, WSA Error code: " << errorCode << endl;
		DisconnectFromServer();
		return -1;
	}

	*message = string( InputBuffer() );

	return 1;
}

void BlackjackClient::SendShutdown()
{
	int iResult = shutdown( ClientSocket(), SD_SEND );
	if ( iResult == SOCKET_ERROR )
	{
		cout << "Shutdown Failed, WSA Error code: " << WSAGetLastError() << endl;
		DisconnectFromServer();
	}
}

void BlackjackClient::DisconnectFromServer()
{
	closesocket( ClientSocket() );
	WSACleanup();
	system( "pause" );
}

void BlackjackClient::Run()
{
	m_networkingThread = new thread( &BlackjackClient::NetworkThreadFunc, this );
	{
		string s = BuildBlackJackRequest( Connect, &Name() );

		m_outputQueue.push( new string( s ) );

		boost::posix_time::milliseconds time( 200 );
		boost::this_thread::sleep( time );

		s = BuildBlackJackRequest( Refresh );
		m_outputQueue.push( new string( s ) );
	}
	// Game loop
	while ( !ShuttingDown() )
	{
		Update();

	}
}

void BlackjackClient::NetworkThreadFunc()
{
	while ( true )
	{
		// Send messages
		if ( !m_outputQueue.empty() )
		{
			string* msg;
			if ( m_outputQueue.try_pop( msg ) )
			{
				if ( msg->compare( "SHUTDOWN" ) == 0 )
					SendShutdown();

				else

					SendData( *msg );

				delete msg;
			}
		}

		// Receive messages
		{
			string msg = string();
			int retCode = RecieveData( &msg );
			if ( retCode == 1 )
			{
				string* msg2 = new string( msg.c_str() );
				const char* msg3 = msg2->c_str();
				
				HandleMessage( *msg2 );
				delete msg2;

			}
			else if ( retCode == 0 )
			{

			}
			else if ( retCode == -1 )
			{

				break;
			}

		}

		if (ShuttingDown())
		{
			break;
		}
	}
}

void BlackjackClient::HandleMessage( string& message )
{
	cout << "Received Message: " << endl;
	cout << message << endl;
	cout << endl;

	std::vector<string> lines;
	split( lines, message, boost::is_any_of( "\n" ) );

	if ( lines.size() < 2 )
		return;


	if ( lines[ 0 ].compare( SERVER_HEADER ) != 0 )
		return;


	std::vector<std::string> clientStateLine;
	split( clientStateLine, lines[ 1 ], boost::is_any_of( "=" ) );

	if ( clientStateLine.size() < 2 )
		return;

	for (unsigned int i = 0; i < clientStateLine.size(); i++)
	{
		trim(clientStateLine[i]);
	}
	
	PlayerState state;
	try
	{
		std::string s = std::string(clientStateLine[1].c_str());
		state = StateMap().at( s );
	}
	catch ( const std::out_of_range& oor )
	{
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		std::cerr << "Tried to lookup: " << clientStateLine[1] << endl ;
	}

	switch ( state )
	{
	case Game:
		HandleGameMessage( lines );
		break;
	case Lobby:
		HandleLobbyMessage( lines );
		break;
	case Shutdown:
		ShuttingDown( true );
		exit(0);
		break;
	default:
		break;
	}
}

void BlackjackClient::Update()
{
	boost::posix_time::milliseconds time( 200 );
	boost::this_thread::sleep( time );

	string input;
	cout << "Please Enter a command: ";
	cin >> input;

	std::vector<string> contexts;
	split(contexts, input, is_any_of("_"));

	if (contexts.size() == 0)
	{
		return;
	}
	PlayerCommands command;
	try
	{
		command = m_inputMap.at( std::string( contexts[ 0 ].c_str() ) );
	}
	catch ( const std::out_of_range& oor )
	{
		cout << "Invalid Command, Please enter one of the following:" << endl;
		for (auto it = m_inputMap.begin(); it != m_inputMap.end(); ++it)
		{
			cout << it->first << endl;
		}
		return;
	}
	string output;
	if (command == JoinGame)
	{
		if (contexts.size() != 2)
		{
			cout << "Please enter a game number to join with the join command with Join_#" << endl;
			return;
		}
		output = BuildBlackJackRequest( command, &contexts[ 1 ] );
	}
	else
		output = BuildBlackJackRequest( command );

	m_outputQueue.push( new string( output ) );

	if (command == Disconnect)
	{
		ShuttingDown( true );
		delete this;
		exit( 0 );
	}
}

string BlackjackClient::BuildBlackJackRequest( PlayerCommands command, string* extraInfo )
{
	stringstream stringBuilder;
	stringBuilder << CLIENT_HEADER << endl;
	stringBuilder << "Command = " << CommandMap().at( command ) << endl;
	switch ( command )
	{
		case Connect:
			stringBuilder << "Name = " << *( (string*)extraInfo ) << endl;
			break;
		case JoinGame:
			stringBuilder << "RoomNumber = " << *( (string*)extraInfo ) << endl;
			break;
		default:
			break;
	}

	stringBuilder << endl;

	return string(stringBuilder.str().c_str());
}

void BlackjackClient::HandleLobbyMessage( std::vector<string> lines )
{

	if ( lines.size() < 3 )
	{
		return;
	}

	int NumberGames;
	{
		std::vector<string> contexts;
		split( contexts, lines[ 2 ], boost::is_any_of( "=" ) );
		if ( contexts.size() != 2 )
		{
			return;
		}

		BOOST_FOREACH( string context, contexts )
		{
			trim( context );
		}

		NumberGames = atoi( contexts[ 1 ].c_str() );
	}

	//Check if the number of lines is representative of the number of games

	if ( lines.size() < (unsigned)( 3 + ( NumberGames * 2 ) ) )
	{
		return;
	}

	int lineindex = 2;
	for ( int i = 0; i < NumberGames; i++ )
	{
		cout << endl;
		cout << endl;

		lineindex++;

		{
			std::vector<string> vsGameNumber;
			split( vsGameNumber, lines[ lineindex ], is_any_of( "=" ) );

			if ( vsGameNumber.size() < 2 )
				return;

			BOOST_FOREACH( string context, vsGameNumber )
			{
				trim( context );
			}

			int gameNumber = atoi( vsGameNumber[ 1 ].c_str() );
			cout << "Game Number: " << gameNumber << "\t\t\t";
		}
	
		lineindex++;
		{

			std::vector<string> vsNumberPlayers;
			split( vsNumberPlayers, lines[ lineindex ], is_any_of( "=" ) );

			if ( vsNumberPlayers.size() < 2 )
				return;
			
			BOOST_FOREACH( string context, vsNumberPlayers )
			{
				trim( context );
			}

			int NumPlayers = atoi( vsNumberPlayers[ 1 ].c_str() );

			cout << "Number of Players: " << NumPlayers << " / 2" << endl;
		}
	}

	cout << endl;
}

void BlackjackClient::HandleGameMessage( std::vector<string> lines )
{
	if ( lines.size() < 4 )
	{
		return;
	}

	int ClientPlayerNum, NumberPlayers;

	{
		std::vector<string> line;
		split( line, lines[ 2 ], is_any_of( "=" ) );

		if ( line.size() < 2 )
		{
			return;
		}

		BOOST_FOREACH( string context, line )
		{
			trim( context );
		}

		ClientPlayerNum = atoi( line[ 1 ].c_str() );
	}

	{
		std::vector<string> line;
		split( line, lines[ 3 ], is_any_of( "=" ) );

		if ( line.size() < 2 )
		{
			return;
		}

		BOOST_FOREACH( string context, line )
		{
			trim( context );
		}

		NumberPlayers = atoi( line[ 1 ].c_str() );
	}

	if ( lines.size() < (unsigned int)(4 + ( NumberPlayers * 3 )) )
	{
		return;
	}

	int lineindex = 3;

	for ( int i = 0; i < NumberPlayers; i++ )
	{
		string PlayerName;
		int PlayerKnowTotal;
		bool isStaying;

		lineindex++;

		{
			std::vector<string> line;
			split( line, lines[ lineindex ], is_any_of( "=" ) );

			if ( line.size() < 2 )
				return;

			BOOST_FOREACH( string l, line )
			{
				trim( l );
			}

			PlayerName = line[ 1 ];
		}

		lineindex++;

		{
			std::vector<string> line;
			split( line, lines[ lineindex ], is_any_of( "=" ) );

			if ( line.size() < 2 )
				return;

			BOOST_FOREACH( string l, line )
			{
				trim( l );
			}

			PlayerKnowTotal = atoi( line[ 1 ].c_str() );
		}

		lineindex++;

		{
			std::vector<string> line;
			split( line, lines[ lineindex ], is_any_of( "=" ) );

			if ( line.size() < 2 )
				return;

			BOOST_FOREACH( string l, line )
			{
				trim( l );
			}

			if ( line[ 1 ].compare( "True" ) )
			{
				isStaying = true;
			}
			else if ( line[ 1 ].compare( "False" ) )
			{
				isStaying = false;
			}
			else
			{
				return;
			}
		}

		cout << endl;

		cout << PlayerName << "\t\t" << PlayerKnowTotal << "\t\t" << "Staying: " << isStaying << endl;
	}

	cout << endl;
}

concurrent_unordered_map<PlayerCommands, std::string> BlackjackClient::m_commandMap = boost::assign::map_list_of
	( PlayerCommands::Connect,		std::string( "Connect" )		)
	( PlayerCommands::Hit,			std::string( "Hit" )			)
	( PlayerCommands::JoinGame,		std::string( "JoinGame" )		)
	( PlayerCommands::LeaveGame,	std::string( "LeaveGame" )		)
	( PlayerCommands::Refresh,		std::string( "Refresh" )		)
	( PlayerCommands::Stay,			std::string( "Stay" )			)
	( PlayerCommands::CreateGame,	std::string( "CreateGame" )		)
	( PlayerCommands::Disconnect,	std::string( "Disconnect" )		)
	;

concurrent_unordered_map<std::string, PlayerState> BlackjackClient::m_stateMap = boost::assign::map_list_of
	( std::string( "Game" ),		PlayerState::Game		)
	( std::string( "Lobby" ),		PlayerState::Lobby		)
	( std::string( "Shutdown" ),	PlayerState::Shutdown	)
	;

concurrent_unordered_map<std::string, PlayerCommands> BlackjackClient::m_inputMap = boost::assign::map_list_of
	( std::string( "Create" ),		PlayerCommands::CreateGame	)
	( std::string( "Disconnect" ),	PlayerCommands::Disconnect	)
	( std::string( "Hit" ),			PlayerCommands::Hit			)
	( std::string( "Join" ),		PlayerCommands::JoinGame	)
	( std::string( "Leave" ),		PlayerCommands::LeaveGame	)
	( std::string( "Refresh" ),		PlayerCommands::Refresh		)
	( std::string( "Stay" ),		PlayerCommands::Stay		)
	;
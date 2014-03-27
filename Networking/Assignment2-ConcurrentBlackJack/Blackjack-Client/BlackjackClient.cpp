#include "BlackjackClient.h"

#include <iostream>
#include <sstream>
#include <WS2tcpip.h>
#include <boost\assign.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\foreach.hpp>

using std::cout;
using std::endl;
using std::stringstream;
using boost::split;
using boost::trim;
using boost::is_any_of;



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

	s = ss.str();

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
			cout << "Failed to connect, Trying other possible connections, Error code: " << WSAGetLastError() << endl;
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

	int i = send( ClientSocket(), message.c_str(), message.length(), 0 );

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
	}
	// Game loop
	while ( !ShuttingDown() )
	{
		if ( !m_inputQueue.empty() )
		{
			string* msg = nullptr;
			if ( m_inputQueue.try_pop( msg ) )
			{
				HandleMessage( *msg );
				delete msg;

			}
		}

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
				m_inputQueue.push( msg2 );
			}
			else if ( retCode == 0 )
			{

			}
			else if ( retCode == -1 )
			{

				break;
			}

		}
	}
}

void BlackjackClient::HandleMessage( string& message )
{
	cout << "Received Message: " << endl;
	cout << message << endl;
	cout << endl;

	vector<string> lines;
	split( lines, message, boost::is_any_of( "\n" ) );

	if ( lines.size() < 2 )
		return;


	if ( lines[ 0 ].compare( SERVER_HEADER ) != 0 )
		return;


	vector<string> clientStateLine;
	split( clientStateLine, lines[ 1 ], boost::is_any_of( "=" ) );

	if ( clientStateLine.size() < 2 )
		return;

	BOOST_FOREACH( string s, clientStateLine )
	{
		trim( s );
	}

	PlayerState state = StateMap()->at( clientStateLine[ 1 ] );

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
			break;
		default:
			break;
	}
}

void BlackjackClient::Update()
{

}

string BlackjackClient::BuildBlackJackRequest( PlayerCommands command, void* extraInfo )
{
	stringstream stringBuilder;
	stringBuilder << CLIENT_HEADER << endl;
	stringBuilder << "Command = " << CommandMap()->at( command ) << endl;
	switch ( command )
	{
		case Connect:
			stringBuilder << "Name = " << *( (string*)extraInfo ) << endl;
			break;
		case JoinGame:
			stringBuilder << "RoomNumber = " << *( (int*)extraInfo ) << endl;
			break;
		default:
			break;
	}

	stringBuilder << endl;

	return stringBuilder.str();
}

void BlackjackClient::HandleLobbyMessage( vector<string> lines )
{

	if ( lines.size() < 3 )
	{
		return;
	}

	int NumberGames;
	{
		vector<string> contexts;
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

	if ( lines.size() < ( 3 + ( NumberGames * 2 ) ) )
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
			vector<string> vsGameNumber;
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

			vector<string> vsNumberPlayers;
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

void BlackjackClient::HandleGameMessage( vector<string> lines )
{
	if ( lines.size() < 4 )
	{
		return;
	}

	int ClientPlayerNum, NumberPlayers;

	{
		vector<string> line;
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
		vector<string> line;
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

	if ( lines.size() < 4 + ( NumberPlayers * 3 ) )
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
			vector<string> line;
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
			vector<string> line;
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
			vector<string> line;
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

concurrent_unordered_map<PlayerCommands, string> BlackjackClient::m_commandMap = boost::assign::map_list_of
( PlayerCommands::Connect, string( "Connect" ) )
( PlayerCommands::Hit, string( "Hit" ) )
( PlayerCommands::JoinGame, string( "JoinGame" ) )
( PlayerCommands::LeaveGame, string( "LeaveGame" ) )
( PlayerCommands::Refresh, string( "Refresh" ) )
( PlayerCommands::Stay, string( "Stay" ) )
( PlayerCommands::CreateGame, string( "CreateGame" ) )
( PlayerCommands::Disconnect, string( "Disconnect" ) )
;

concurrent_unordered_map<string, PlayerState> BlackjackClient::m_stateMap = boost::assign::map_list_of
( string( "Game" ), PlayerState::Game )
( string( "Lobby" ), PlayerState::Lobby )
( string( "Shutdown" ), PlayerState::Shutdown )
;
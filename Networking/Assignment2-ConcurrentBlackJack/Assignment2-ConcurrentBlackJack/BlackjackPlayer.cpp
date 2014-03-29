///////////////////////////////////////////////////////////
//  BlackjackPlayer.cpp
//  Implementation of the Class BlackjackPlayer
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "BlackjackPlayer.h"
#include "BlackjackProtocol.h"
#include "BlackjackGame.h"
#include "TCPGameServer.h"
#include <boost/algorithm/string.hpp>
#include <sstream>

using boost::interprocess::string;
using std::stringstream;

namespace Blackjack
{
	BlackjackPlayer::BlackjackPlayer() : GenericPlayer()
	{
		m_state = PlayerState::Lobby;
		Name( new boost::interprocess::string() );
	}

	BlackjackPlayer::~BlackjackPlayer()
	{
	}


	void BlackjackPlayer::HandleMessage( string Message )
	{
		const char* msg = Message.c_str();
		vector<string> lines;
		boost::split( lines, Message, boost::is_any_of( "\n" ) );

		if ( lines.size() < 2 )
		{
			HandleInvaildRequest( "Invalid Message Size" );
			return;
		}

		if ( lines[ 0 ].compare( CLIENT_HEADER ) != 0 )
		{
			if ( TCPGameServer::Instance()->Debugging() )
			{
				cout << "Header is invalid: " << endl;
				cout << lines[ 0 ] << " != " << CLIENT_HEADER << endl;
			}
			HandleInvaildRequest( "Invalid Header" );
			return;
		}

		{
			vector<string> commandLine;
			boost::split( commandLine, lines[ 1 ], boost::is_any_of( "=" ) );

			if ( commandLine.size() != 2 )
			{
				HandleInvaildRequest( "Invalid command line Size" );
				return;
			}

			boost::trim( commandLine[ 0 ] );
			boost::trim( commandLine[ 1 ] );

			if ( commandLine[ 0 ].compare( "Command" ) != 0 )
			{
				return;
			}

			auto command = BlackjackProtocol::CommandMap().find( std::string( commandLine[ 1 ].c_str() ) );
			if ( command == BlackjackProtocol::CommandMap().end() )
			{
				HandleInvaildRequest( "Invalid command" );
				return;
			}

			PlayerCommands com = command->second;

			switch ( com )
			{
			case Blackjack::Connect:
				HandleInvaildRequest( "Cannot connect if already connected" );
				return;

			case Blackjack::JoinGame:
				HandleJoinGameRequest( ConstructMessageInput( lines ) );
				break;

			case Blackjack::LeaveGame:
				(*GetGame()).EnqueueMessage( ConstructGameInput( lines ) );
				break;

			case Blackjack::Hit:
				(*GetGame()).EnqueueMessage( ConstructGameInput( lines ) );
				break;

			case Blackjack::Stay:
				(*GetGame()).EnqueueMessage( ConstructGameInput( lines ) );
				break;

			case Blackjack::Refresh:
				HandleRefreshRequest( ConstructMessageInput( lines ) );
				break;

			case Blackjack::CreateGame:
				HandleCreateGameRequest( ConstructMessageInput( lines ) );
				break;

			case Blackjack::Disconnect:
				HandleDisconnectRequest( ConstructMessageInput( lines ) );
				ShuttingDown(true);
				break;

			default:
				HandleInvaildRequest( "wHAT DAFUQ Happened?!?!?!?!?!?!?!" );
				return;
				break;
			}
		}
	}

	void BlackjackPlayer::HandleCreateGameRequest( MessageInput* input )
	{
		BlackjackGame* game = new BlackjackGame( this );
		m_state = PlayerState::Game;

		MessageOutput* msgout = new MessageOutput();

		string s = BlackjackProtocol::BuildServerResponse( this, TCPGameServer::Instance() );

		msgout->Message( s );

		EnqueueMessage( msgout );
	}

	void BlackjackPlayer::HandleJoinGameRequest( MessageInput* input )
	{
		const char* msg = input->m_message.c_str();

		TCPGameServer* server = TCPGameServer::Instance();
		vector<std::string> contexts;
		boost::split( contexts, input->m_message, boost::is_any_of( "=" ) );

		if ( contexts.size() != 2 )
		{
			HandleInvaildRequest( "Invalid operation" );
			return;
		}

		for ( int i = 0; i < contexts.size(); i++ )
		{
			boost::trim( contexts[ i ] );
		}

		int roomNumber = atoi( contexts[ 1 ].c_str() );

		if (roomNumber <= 0 || roomNumber > server->Games().size())
		{
			HandleInvaildRequest("Room does not exist");
			return;
		}
		BlackjackGame& game = *((BlackjackGame*)(server->Games()[roomNumber - 1]));
		if (!game.AddPlayer(this))
		{
			HandleInvaildRequest("Game is full.");
			return;
		}

		m_state = PlayerState::Game;
		MessageOutput* msgout = new MessageOutput();
		SetGame(&game);
		string s = BlackjackProtocol::BuildServerResponse( this, TCPGameServer::Instance() );

		msgout->Message( s );

		EnqueueMessage( msgout );
	}

	void BlackjackPlayer::HandleDisconnectRequest( MessageInput* input )
	{
		m_state = Shutdown;
		if (GetGame() != nullptr)
		{
			((BlackjackGame*)(GetGame()))->RemovePlayer(this);
		}
		string s = BlackjackProtocol::BuildServerResponse(this, TCPGameServer::Instance() );
		MessageOutput* msgout = new MessageOutput();
		msgout->Message( s );
		EnqueueMessage( msgout );
	}

	void BlackjackPlayer::HandleRefreshRequest( MessageInput* input )
	{
		MessageOutput* msgout = new MessageOutput();

		string s = BlackjackProtocol::BuildServerResponse( this, TCPGameServer::Instance() );

		msgout->Message( s );

		EnqueueMessage( msgout );
	}

	void BlackjackPlayer::HandleInvaildRequest( string error )
	{

		MessageOutput* out = new MessageOutput();
		out->Message( error );
		EnqueueMessage( out );
	}

	MessageInput* BlackjackPlayer::ConstructMessageInput( vector<string> lines )
	{

		vector<string> extraLines;
		for ( unsigned int i = 2; i < lines.size(); i++ )
		{
			extraLines.push_back( lines[ i ] );
		}

		MessageInput* in = new MessageInput();
		in->m_message = boost::join( extraLines, "\n" );
		in->m_player = this;
		return in;

	}

	MessageInput* BlackjackPlayer::ConstructGameInput( vector<string> lines )
	{
		vector<string> extraLines;
		for ( unsigned int i = 1; i < lines.size(); i++ )
		{
			extraLines.push_back( lines[ i ] );
		}

		MessageInput* in = new MessageInput();
		in->m_message = boost::join( extraLines, "\n" );
		in->m_player = this;
		return in;
	}

	void BlackjackPlayer::Win() const
	{

	}

	void BlackjackPlayer::Lose() const
	{

	}

	void BlackjackPlayer::Push() const
	{

	}

	bool BlackjackPlayer::IsHitting() const
	{
		return false;
	}

	void BlackjackPlayer::Init()
	{

	}


}



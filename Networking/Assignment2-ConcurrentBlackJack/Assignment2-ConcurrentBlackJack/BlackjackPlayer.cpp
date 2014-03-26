///////////////////////////////////////////////////////////
//  BlackjackPlayer.cpp
//  Implementation of the Class BlackjackPlayer
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "BlackjackPlayer.h"
#include "BlackjackProtocol.h"
#include "BlackjackGame.h"
#include <boost/algorithm/string.hpp>


namespace Blackjack
{
	BlackjackPlayer::BlackjackPlayer() : GenericPlayer()
	{
		m_state = PlayerState::Lobby;
	}

	BlackjackPlayer::~BlackjackPlayer()
	{
	}

	bool BlackjackPlayer::IsHitting() const
	{
		cout << m_Name << ", do you want a hit? (Y/N): ";
		char response;
		cin >> response;
		return ( response == 'y' || response == 'Y' );
	}

	void BlackjackPlayer::Win() const
	{
		cout << m_Name << " wins.\n";
	}

	void BlackjackPlayer::Lose() const
	{
		cout << m_Name << " loses.\n";
	}

	void BlackjackPlayer::Push() const
	{
		cout << m_Name << " pushes.\n";
	}

	void BlackjackPlayer::Init()
	{
		m_Name = Name();
	}

	void BlackjackPlayer::HandleMessage( string Message )
	{

		vector<string> lines;
		boost::split( lines, Message, boost::is_any_of( "\n" ) );

		if ( lines.size() < 2 )
		{
			HandleInvaildRequest( "Invalid Message Size" );
			return;
		}

		if ( lines[ 0 ].compare( SERVER_HEADER ) != 0 )
		{
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
				HandleInvaildRequest( "Invalid command request" );
				return;
			}

			auto command = BlackjackProtocol::CommandMap()->find( commandLine[ 1 ] );
			if ( command == BlackjackProtocol::CommandMap()->end() )
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
				GetGame().EnqueueMessage( ConstructGameInput( lines ) );
				break;

			case Blackjack::Hit:
				GetGame().EnqueueMessage( ConstructGameInput( lines ) );
				break;

			case Blackjack::Stay:
				GetGame().EnqueueMessage( ConstructGameInput( lines ) );
				break;

			case Blackjack::Refresh:
				HandleRefreshRequest( ConstructMessageInput( lines ) );
				break;

			case Blackjack::CreateGame:
				HandleCreateGameRequest( ConstructMessageInput( lines ) );
				break;

			case Blackjack::Disconnect:
				HandleDisconnectRequest( ConstructMessageInput( lines ) );
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

	}

	void BlackjackPlayer::HandleJoinGameRequest( MessageInput* input )
	{

	}

	void BlackjackPlayer::HandleDisconnectRequest( MessageInput* input )
	{

	}

	void BlackjackPlayer::HandleRefreshRequest( MessageInput* input )
	{

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


}



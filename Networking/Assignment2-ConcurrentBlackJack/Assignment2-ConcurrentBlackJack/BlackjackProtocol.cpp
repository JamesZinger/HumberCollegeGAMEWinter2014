#include "BlackjackProtocol.h"
#include "BlackjackGame.h"
#include "TCPGameServer.h"
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

//using std::string;
using boost::interprocess::string;
using boost::is_any_of;
using boost::split;

namespace Blackjack
{

	string BlackjackProtocol::BuildServerResponse( BlackjackPlayer* PlayerContext, TCPGameServer* server )
	{
		stringstream stringBuilder;

		stringBuilder << SERVER_HEADER << endl;

		std::string s = m_clientStateMap.at( PlayerContext->State() );

		stringBuilder << "ClientState = " << s << endl;

		switch ( PlayerContext->State() )
		{
		case PlayerState::Game:
		{
			BlackjackGame& game = *(BlackjackGame*)PlayerContext->GetGame();
			int number = 0;
			bool found = false;
			for ( auto it = game.Players().begin(); it != game.Players().end(); ++it )
			{
				number++;
				if ( PlayerContext == *it )
				{
					found = true;
					stringBuilder << "ClientPlayerNum = " << number << endl;
					break;
				}
			}

			if ( !found )
			{
				stringBuilder << "ClientPlayerNum = " << "SERVER_ERROR" << endl;
			}

			stringBuilder << "NumberPlayers = " << game.Players().size() << endl;

			for ( vector<BlackjackPlayer*>::iterator it = game.Players().begin(); it != game.Players().end(); ++it )
			{
				stringBuilder << "PlayerName = " << (*it)->Name() << endl;
				stringBuilder << "PlayerKnownTotal = " << 0 << endl;
				stringBuilder << "Staying = " << "False" << endl;
			}
		}
			break;

		case PlayerState::Lobby:
		{
			server->Games().size();
			stringBuilder << "NumberGame = " << server->Games().size() << endl;
			int i = 1;
			for ( auto it = server->Games().begin(); it != server->Games().end(); ++it )
			{
				stringBuilder << "GameNumber = " << ( i ) << endl;
				stringBuilder << "NumberPlayers = " << ((BlackjackGame*)*it)->Players().size() << endl;
				i++;
			}
		}
			break;

		case PlayerState::Shutdown:
			break;
		}

		return string(stringBuilder.str().c_str());
	}

	BlackjackProtocol::BlackjackProtocol()
	{
	}

	BlackjackProtocol::~BlackjackProtocol()
	{

	}

	void BlackjackProtocol::HandleRequest( const SOCKET client, string& recieveString, const Server* server )
	{
		//Find out what game the request is for.

		TCPGameServer* gameServer = (TCPGameServer*)server;

		string s = recieveString;
		string sendString;
		stringstream ss;
		char context = DetermineRequestContext( s );

		//Send the correct message to the Game's Queue

		switch ( context )
		{
		case 'g':

			HandleGameRequest( client, recieveString, gameServer );
			return;

		case 'h':
			HandleHttpRequest( client, recieveString, gameServer, ss );
			break;

		case '\0':
			ss << "Invalid Message" << endl;
			break;

		default:
			ss << "Something has gone very wrong" << endl;
			break;
		}

		gameServer->SendMessageOverNetwork( client, string(ss.str().c_str()) );
		closesocket( client );

		return;
	}

	const char BlackjackProtocol::DetermineRequestContext( const string& Message )
	{
		char context = '\0';

		stringstream ss = stringstream();

		ss << Message;

		string firstLine;

		{
			vector<string> lines;

			split( lines, ss.str(), is_any_of( "\n" ) );

			firstLine = lines[0];
		}

		const string http = "HTTP";
		const string BJPFirstLine = "BlackJackClient Protocol 1.0";

		if ( Message.length() < http.length() )
		{
			return context;
		}
		if ( firstLine.compare( BJPFirstLine ) == 0 )
		{
			context = 'g';
		}
		else if ( firstLine.compare( firstLine.length() - ( http.length() + 5 ), http.length(), http ) == 0 )
		{
			//Is an HTTP Command
			context = 'h';
		}
		else
			context = '\0';

		return context;
	}

	void BlackjackProtocol::HandleHttpRequest( SOCKET client, string request, TCPGameServer* server, stringstream& ss )
	{
		if ( server->Players().empty() )
		{
			ss << "No players are currently connected" << "<br />";
		}

		else
		{
			ss << "Listing Players: " << "<br />";

			for ( auto it = server->Players().begin(); it != server->Players().end(); ++it )
			{
				ss << ( *it ).second->Name() << "<br />";
			}
		}

		ss << endl;
	}

	void BlackjackProtocol::HandleGameRequest( SOCKET client, string request, TCPGameServer* server )
	{
		vector<string> lines;
		boost::split( lines, request, boost::is_any_of( "\n" ) );

		if ( lines.size() < 3 )
		{
			return;
		}



		vector<string> context;
		boost::split( context, lines[ 1 ], boost::is_any_of( "=" ) );



		if ( context.size() < 2 )
		{
			return;
		}

		boost::trim( context[ 0 ] );
		boost::trim( context[ 1 ] );

		if ( context[ 0 ].compare( "Command" ) != 0 )
		{
			return;
		}
		std::string s = std::string(context[1].c_str());
		auto command = CommandMap().find( s );
		auto something = CommandMap().end();
		if ( command == something )
		{
			return;
		}
		if ( command->second != PlayerCommands::Connect )
		{
			if (server->Debugging())
			{
				cout << "initial command is not equal to Connect. Command = " << command->second;
			}
			return;
		}

		vector<string> names;
		boost::split( names, lines[ 2 ], boost::is_any_of( "=" ) );

		if ( names.size() < 2 )
		{
			return;
		}

		boost::trim( names[ 0 ] );
		boost::trim( names[ 1 ] );

		Blackjack::BlackjackPlayer* p = new Blackjack::BlackjackPlayer();
		boost::thread* t = new boost::thread( &Player::PlayerThreadFunc, p, names[ 1 ], client );
	}



#pragma region Static Construction


	concurrent_unordered_map<std::string, PlayerCommands> BlackjackProtocol::m_commandMap = boost::assign::map_list_of
		( std::string( "Connect" ), PlayerCommands::Connect )
		( std::string( "Hit" ), PlayerCommands::Hit )
		( std::string( "JoinGame" ), PlayerCommands::JoinGame )
		( std::string( "LeaveGame" ), PlayerCommands::LeaveGame )
		( std::string( "Refresh" ), PlayerCommands::Refresh )
		( std::string( "Stay" ), PlayerCommands::Stay )
		( std::string( "CreateGame" ), PlayerCommands::CreateGame )
		( std::string( "Disconnect" ), PlayerCommands::Disconnect )
		;

	concurrent_unordered_map<std::string, AdminCommands> BlackjackProtocol::m_adminCommandMap = boost::assign::map_list_of
		( std::string( "List" ), AdminCommands::ListConnections )
		;

	concurrent_unordered_map<PlayerState, std::string> BlackjackProtocol::m_clientStateMap = boost::assign::map_list_of
		( PlayerState::Game, std::string("Game") )
		( PlayerState::Lobby, std::string("Lobby") )
		( PlayerState::Shutdown, std::string("Shutdown") )
		;

#pragma endregion

}
#include "BlackjackProtocol.h"
#include "BlackjackGame.h"
#include <vector>
#include <iterator>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

using std::vector;

namespace Blackjack
{

	BlackjackProtocol::BlackjackProtocol()
	{

#pragma region Player Command Setup

		std::pair<string, PlayerCommands> s;

		s = std::make_pair( "Connect", PlayerCommands::Connect );
		m_commandMap.insert( s );

		s = std::make_pair( "Hit", PlayerCommands::Hit );
		m_commandMap.insert( s );

		s = std::make_pair( "JoinGame", PlayerCommands::JoinGame );
		m_commandMap.insert( s );

		s = std::make_pair( "LeaveGame", PlayerCommands::LeaveGame );
		m_commandMap.insert( s );

		s = std::make_pair( "Refresh", PlayerCommands::Refresh );
		m_commandMap.insert( s );

		s = std::make_pair( "Stay", PlayerCommands::Stay );
		m_commandMap.insert( s );

		s = std::make_pair( "CreateGame", PlayerCommands::CreateGame );
		m_commandMap.insert( s );

		s = std::make_pair( "Disconnect", PlayerCommands::Disconnect );
		m_commandMap.insert( s );

#pragma endregion

#pragma region Admin Command Setup

		std::pair<string, AdminCommands> a;

		a = std::make_pair( "List", AdminCommands::ListConnections );
		m_adminCommandMap.insert( a );

#pragma endregion

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

		gameServer->SendMessageOverNetwork( client, ss.str() );

		closesocket( client );

		return;
	}

	const char BlackjackProtocol::DetermineRequestContext( const string& Message )
	{
		char context = '\0';

		stringstream ss = stringstream();

		ss << Message;

		string firstLine;

		std::getline( ss, firstLine, '\n' );
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

		concurrent_unordered_map<string, PlayerCommands>::const_iterator command = CommandMap()->find( context[ 1 ] );
		auto something = CommandMap()->end();
		if ( command == something )
		{
			return;
		}
		if ( command->second != PlayerCommands::Connect )
		{
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

		Player* p = new Player();
		boost::thread* t = new boost::thread( &Player::PlayerThreadFunc, p, names[ 1 ], client );
	}

}
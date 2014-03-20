#include "BlackjackProtocol.h"
#include "BlackjackGame.h"
#include <sstream>
#include "Server.h"
#include "TCPGameServer.h"

#include <concurrent_unordered_map.h>
#include <boost/thread.hpp>

using std::stringstream;
using Concurrency::concurrent_unordered_map;

namespace Blackjack
{

	BlackjackProtocol::BlackjackProtocol()
	{
		
		BlackJackGame( new BlackjackGame() );
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

	const char BlackjackProtocol::DetermineRequestContext(const string& Message)
	{
		char context = '\0';

		stringstream ss = stringstream();

		ss << Message;

		string firstLine;

		std::getline( ss, firstLine, '\n' );

		const string http = "HTTP";

		if ( Message.length() < http.length() )
		{
			return context;
		}

		if ( firstLine.compare( firstLine.length() - (http.length() + 5), http.length(), http ) == 0 )
		{
			//Is an Http Command
			context = 'h';
		}
		else
		{
			//Likely a game command
			context = 'g';
		}

		return context;
	}

	void BlackjackProtocol::HandleHttpRequest( SOCKET client, string request, TCPGameServer* server, stringstream& ss )
	{
		ss << "Admin Command Received" << endl;

		ss << "Listing Players: " << endl;
		{
			//concurrent_unordered_map<SOCKET, Player*>::iterator it;
			auto it = server->Players().begin();
			it != server->Players().end();
			it++;
			for ( auto it = server->Players().begin(); it != server->Players().end(); it++ )
			{
				ss << (*it).second->Name() << endl;
			}
		}
		ss << endl;
	}

	void BlackjackProtocol::HandleGameRequest( SOCKET client, string request, TCPGameServer* server )
	{
		string msg = "Not implemented yet";
		server->SendMessageOverNetwork( client, msg );
		closesocket( client );
		return;


	}

}
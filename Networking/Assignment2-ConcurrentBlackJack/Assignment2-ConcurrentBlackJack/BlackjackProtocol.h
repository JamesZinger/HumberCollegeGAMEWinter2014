#pragma once

#include "Protocol.h"
#include <sstream>
#include "BlackjackPlayer.h"
#include <concurrent_unordered_map.h>

class TCPGameServer;

namespace Blackjack
{
	class BlackjackGame;

	using std::string;
	using std::stringstream;
	using Concurrency::concurrent_unordered_map;

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

	enum AdminCommands
	{
		ListConnections
	};

	static const string SERVER_HEADER = string( "BlackJackServer Protocol 1.0" );
	static const string CLIENT_HEADER = string( "BlackJackClient Protocol 1.0" );

	class BlackjackProtocol :
		public Protocol
	{
	public:
		BlackjackProtocol();
		~BlackjackProtocol();

		virtual void HandleRequest( const SOCKET client, string& recieveString, const Server* server );
		static string BuildServerResponse( BlackjackPlayer* PlayerContext, TCPGameServer* server );

#pragma region Getters

		static concurrent_unordered_map<string, PlayerCommands>		CommandMap()		{ return m_commandMap; }
		static concurrent_unordered_map<string, AdminCommands>		AdminCommandMap()	{ return m_adminCommandMap; }
		static concurrent_unordered_map<PlayerState, string>		ClientStateMap()		{ return m_clientStateMap; }


#pragma endregion

	protected:

		void HandleHttpRequest( SOCKET client, string request, TCPGameServer* server, stringstream& ss );
		void HandleGameRequest( SOCKET client, string request, TCPGameServer* server );

#pragma region Setters

		static void CommandMap( concurrent_unordered_map<string, PlayerCommands> val )		{ m_commandMap = val; }
		static void AdminCommandMap( concurrent_unordered_map<string, AdminCommands> val )	{ m_adminCommandMap = val; }
		static void ClientStateMap( concurrent_unordered_map<PlayerState, string> val )		{ m_clientStateMap = val; }

#pragma endregion

	private:

		const char DetermineRequestContext( const string& Message );

		static concurrent_unordered_map<string, PlayerCommands>		m_commandMap;
		static concurrent_unordered_map<string, AdminCommands>		m_adminCommandMap;
		static concurrent_unordered_map<PlayerState, string>			m_clientStateMap;
	};

}

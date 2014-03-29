#pragma once

#include "Protocol.h"
#include <sstream>
#include "BlackjackPlayer.h"
#include <concurrent_unordered_map.h>

class TCPGameServer;

namespace Blackjack
{
	class BlackjackGame;

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

	static const boost::interprocess::string SERVER_HEADER = boost::interprocess::string( "BlackJackServer Protocol 1.0" );
	static const boost::interprocess::string CLIENT_HEADER = boost::interprocess::string( "BlackJackClient Protocol 1.0" );

	class BlackjackProtocol :
		public Protocol
	{
	public:
		BlackjackProtocol();
		~BlackjackProtocol();

		virtual void HandleRequest( const SOCKET client, boost::interprocess::string& recieveString, const Server* server );
		static boost::interprocess::string BuildServerResponse( BlackjackPlayer* PlayerContext, TCPGameServer* server );

#pragma region Getters

		static concurrent_unordered_map<std::string, PlayerCommands>&	CommandMap()		{ return m_commandMap; }
		static concurrent_unordered_map<std::string, AdminCommands>&	AdminCommandMap()	{ return m_adminCommandMap; }
		static concurrent_unordered_map<PlayerState, std::string>&		ClientStateMap()	{ return m_clientStateMap; }


#pragma endregion

	protected:

		void HandleHttpRequest( SOCKET client, boost::interprocess::string request, TCPGameServer* server, stringstream& ss );
		void HandleGameRequest( SOCKET client, boost::interprocess::string request, TCPGameServer* server );

#pragma region Setters

		static void CommandMap( concurrent_unordered_map<std::string, PlayerCommands> val )			{ m_commandMap = val; }
		static void AdminCommandMap( concurrent_unordered_map<std::string, AdminCommands> val )		{ m_adminCommandMap = val; }
		static void ClientStateMap( concurrent_unordered_map<PlayerState, std::string> val )		{ m_clientStateMap = val; }

#pragma endregion

	private:

		const char DetermineRequestContext( const boost::interprocess::string& Message );

		static concurrent_unordered_map<std::string, PlayerCommands>	m_commandMap;
		static concurrent_unordered_map<std::string, AdminCommands>		m_adminCommandMap;
		static concurrent_unordered_map<PlayerState, std::string>		m_clientStateMap;
	};

}

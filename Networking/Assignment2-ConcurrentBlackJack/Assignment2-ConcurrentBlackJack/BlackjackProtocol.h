#pragma once

#include "Protocol.h"
#include <sstream>
#include "TCPGameServer.h"

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

	class BlackjackProtocol :
		public Protocol
	{
	public:
		BlackjackProtocol();
		~BlackjackProtocol();

		virtual void HandleRequest( const SOCKET client, string& recieveString, const Server* server );

#pragma region Getters

		concurrent_unordered_map<string, PlayerCommands>*	CommandMap()				{ return &m_commandMap;	}
		concurrent_unordered_map<string, AdminCommands>*	AdminCommandMap()			{ return &m_adminCommandMap; }

#pragma endregion

	protected:

		void HandleHttpRequest( SOCKET client, string request, TCPGameServer* server, stringstream& ss );
		void HandleGameRequest( SOCKET client, string request, TCPGameServer* server );

#pragma region Setters
		
		void CommandMap			( concurrent_unordered_map<string, PlayerCommands> val )	{ m_commandMap = val; }
		void AdminCommandMap	( concurrent_unordered_map<string, AdminCommands> val )		{ m_adminCommandMap = val; }

#pragma endregion

	private:

		const char DetermineRequestContext(const string& Message);

		concurrent_unordered_map<string, PlayerCommands>	m_commandMap;
		concurrent_unordered_map<string, AdminCommands>		m_adminCommandMap;

	};

}

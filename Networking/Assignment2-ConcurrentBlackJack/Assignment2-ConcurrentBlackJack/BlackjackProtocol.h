#pragma once
#include "Protocol.h"
#include <sstream>
#include "TCPGameServer.h"

namespace Blackjack
{
	class BlackjackGame;

	using std::string;
	using std::stringstream;

	class BlackjackProtocol :
		public Protocol
	{
	public:
		BlackjackProtocol();
		~BlackjackProtocol();

		virtual void HandleRequest( const SOCKET client, string& recieveString, const Server* server );

		BlackjackGame* BlackJackGame() const { return m_blackJackGame; }
		

	protected:
		void BlackJackGame( BlackjackGame* val ) { m_blackJackGame = val; }

		void HandleHttpRequest( SOCKET client, string request, TCPGameServer* server, stringstream& ss );
		void HandleGameRequest( SOCKET client, string request, TCPGameServer* server );
	private:
		const char DetermineRequestContext(const string& Message);
		BlackjackGame* m_blackJackGame;

	};

}

#pragma once
#include "Protocol.h"
namespace Blackjack
{
	using std::string;

	class BlackjackProtocol :
		public Protocol
	{
	public:
		BlackjackProtocol();
		~BlackjackProtocol();

		virtual void HandleRequest( const SOCKET client, string& recieveString );
	};

}

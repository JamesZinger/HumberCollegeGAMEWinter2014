#include "HttpTCPGuessingGame.h"


HttpTCPGuessingGame::HttpTCPGuessingGame() : NumberGuessingGame()
{
	stringstream ss = stringstream();
	ss << DEFAULT_PORT;
	server = new HttpServer( ss.str() );
}

HttpTCPGuessingGame::HttpTCPGuessingGame( int port ) : NumberGuessingGame()
{

	stringstream ss = stringstream();
	ss << port;
	server = new HttpServer( ss.str() );
}

HttpTCPGuessingGame::~HttpTCPGuessingGame()
{
	delete server;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Starts the main game loop. </summary>
///
/// <remarks>	James, 2014-03-05. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
void HttpTCPGuessingGame::Run()
{
	while (true)
	{
		if (!server->AcceptConnection())
		{
			continue;
		}

		

		while ( !isGameFinished() )
		{
			GameCycle();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	One tick of the game loop. </summary>
///
/// <remarks>	James, 2014-03-05. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
void HttpTCPGuessingGame::GameCycle()
{	
	string s = string();
	int recvLength = 0;
	if ( !server->RecieveData( &s, &recvLength ) )
	{
		cout << "RecieveData failed, RC length: " << recvLength << '\n';
		system( "Pause" );
		exit( 1 );
	}

	HttpCommand c = DetermineCommand( s );

	switch ( c )
	{
		case HttpTCPGuessingGame::start:
			
			break;
		case HttpTCPGuessingGame::terminate:

			break;
		case HttpTCPGuessingGame::guessnumber:

			break;
		default:
			cout << "Something has gone terribly terribly wrong\n";
			system( "Pause" );
			exit( 1 );
			break;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Determine the command from an http reqest. </summary>
///
/// <remarks>	James, 2014-03-05. </remarks>
///
/// <param name="s">				[in,out] The string&amp; to process. </param>
/// <param name="numberGuessed">	[in,out] Number of guessed. </param>
///
/// <returns>	A HttpTCPGuessingGame::HttpCommand reflecting the command. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
HttpTCPGuessingGame::HttpCommand HttpTCPGuessingGame::DetermineCommand( string& s, int& numberGuessed )
{
	numberGuessed = -1;



	return HttpCommand::start;
}


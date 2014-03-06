#include "HttpTCPGuessingGame.h"


HttpTCPGuessingGame::HttpTCPGuessingGame()
{
	stringstream ss = stringstream();
	ss << DEFAULT_PORT;
	server = new HttpServer( ss.str() );
	game = NULL;
}

HttpTCPGuessingGame::HttpTCPGuessingGame( int port )
{

	stringstream ss = stringstream();
	ss << port;
	server = new HttpServer( ss.str() );
	game = NULL;
}

HttpTCPGuessingGame::~HttpTCPGuessingGame()
{
	delete server;
	if ( game != NULL )
		delete game;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Starts the main game loop. </summary>
///
/// <remarks>	James, 2014-03-05. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
void HttpTCPGuessingGame::Run()
{
	while ( true )
	{
		if ( !server->AcceptConnection() )
		{
			continue;
		}



		while ( GameCycle() ){}

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	One tick of the game loop. </summary>
///
/// <remarks>	James, 2014-03-05. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
bool HttpTCPGuessingGame::GameCycle()
{
	string RCString = string();
	stringstream RXSS = stringstream();

	bool shouldContinue = true;

	int recvLength = 0;
	if ( !server->RecieveData( &RCString, &recvLength ) )
	{
		cout << "RecieveData failed, RC length: " << recvLength << '\n';
		system( "Pause" );
		exit( 1 );
	}

	int guess;
	HttpCommand request = DetermineCommand( RCString, guess );

	switch ( request )
	{
		case HttpTCPGuessingGame::start:
			if ( game == NULL )
			{
				game = new NumberGuessingGame();
				RXSS << "Game successfully created. You may now guess a number with n=?\n";
			}
			else
			{
				RXSS << "Game is already running. Please either terminate or guess a number with c=t or n=?\n";
			}
			break;


		case HttpTCPGuessingGame::terminate:
			if ( game != NULL )
			{
				RXSS << "Game successfully terminated. Thanks for Playing!\n";
				shouldContinue = false;
				delete game;
				game = NULL;
			}
			else
				RXSS << "Cannot terminate a game that is not in progress. please start a new game with c=s\n";

			break;


		case HttpTCPGuessingGame::guessnumber:
			if (game != NULL)
			{
				string s;
				game->GuessNumber( guess, s);
				RXSS << s;
			}
			else
			{
				RXSS << "No game is currently running please start a new game with c=s\n";
			}
			break;


		case HttpCommand::restart:
			if ( game != NULL )
			{
				game->RestartGame();
				RXSS << "Game Successfully Restarted\n";
			}
			else
			{
				RXSS << "No game is currently in progress, please start a new game with c=s\n";
			}
			break;

		case HttpCommand::none:
			RXSS << "Command not recognized. Please use one of the following:\n";
			RXSS << "c=s\t\t\tStart a New Game\n";
			RXSS << "c=t\t\t\tEnd a running game\n";
			RXSS << "c=r\t\t\tRestart a running game\n";
			RXSS << "n=?\t\t\tGuess a number for a running game\n";
			break;


		default:
			cout << "Something has gone terribly terribly wrong\n";
			system( "Pause" );
			exit( 1 );
			break;
	}

	string RXString = RXSS.str();
	int sentLength = 0;
	if (!server->SendHttpMsg( &RXString, &sentLength))
	{
		cout << "Failed to send Message, RX Length: " << sentLength << '\n';
		system( "Pause" );
		exit( 1 );
	}

	return shouldContinue;
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
HttpTCPGuessingGame::HttpCommand HttpTCPGuessingGame::DetermineCommand( string& msg, int& numberGuessed )
{
	numberGuessed = -1;

	char *context = '\0';
	char *context2 = '\0';

	HttpCommand httpcom = HttpCommand::none;

	sscanf_s( msg.c_str(), "GET /NumberGuessing/%c=%c", context );

	switch ( context[ 0 ] )
	{
		case 'c':

			sscanf_s( msg.c_str(), "GET /NumberGuessing/c=%c", context2 );

			switch ( context2[ 0 ] )
			{
				case 's':
					httpcom = HttpCommand::start;
					break;
				case 't':
					httpcom = HttpCommand::terminate;
					break;
				case 'r':
					httpcom = HttpCommand::restart;
					break;

				default:
					cout << "Request second context not recognized, second context: " << context2 << '\n';
					break;
			}

			break;

		case 'n':
			httpcom = HttpCommand::guessnumber;
			sscanf_s( msg.c_str(), "GET /NumberGuessing/n=%d", &numberGuessed );
			break;

		default:
			cout << "Context not recognized, Context: " << context[ 0 ] << '\n';
			break;
	}

	return httpcom;
}


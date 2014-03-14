#include "HttpTCPGuessingGame.h"
#include <string>

using std::string;

struct inputFormat
{
	string port;
};

HttpTCPGuessingGame *game;

int main( int argc, char** argv )
{
	if ( !( argc == 2 || argc == 1 ) )
	{
		cout << "Invalid amount of command arguments!\nEither use no arguments or just the port\nNumber of arguments: " << argc << '\n';
		system( "Pause" );
		exit( 1 );
	}

	inputFormat in = inputFormat();

	if ( argc == 2 )
	{
		in.port = argv[ 1 ];
		int port = atoi( in.port.c_str() );

		if ( port < 1 || port > 65535 )
		{
			if ( port < 1 )
				cout << "Invalid Port. Port cannot be less than 1\n";
			else
				cout << "Invalid Port. Port cannot be greater that 65535";

			system( "Pause" );
			exit( 1 );
		}


		game = new HttpTCPGuessingGame( port );
	}
	else
	{
		game = new HttpTCPGuessingGame();
	}

	game->Run();

	delete game;

}
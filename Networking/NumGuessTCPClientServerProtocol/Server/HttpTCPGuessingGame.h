#pragma once
#include "NumberGuessingGame.h"
#include "HttpServer.h"

class HttpTCPGuessingGame :
	public NumberGuessingGame
{

private:

	HttpServer* server;
	static const int DEFAULT_PORT = 8282;

protected:

	enum HttpCommand
	{
		start = 1,
		terminate,
		guessnumber
	};

	HttpCommand DetermineCommand( string& s, int& numberGuessed );


	void GameCycle();

	

public:

#pragma region ctor & dtor

	HttpTCPGuessingGame();
	HttpTCPGuessingGame( int ListenPort );
	~HttpTCPGuessingGame();

#pragma endregion 


	void Run();


#pragma region Getter and Setters

	const int DefaultPort() const { return DEFAULT_PORT; }

#pragma endregion Getter and Setters

};


#pragma once
#include "HttpServer.h"
#include "NumberGuessingGame.h"
class HttpTCPGuessingGame
{

private:

	HttpServer* server;
	NumberGuessingGame* game;
	static const int DEFAULT_PORT = 8282;

protected:

	enum HttpCommand
	{
		none = 0,
		start,
		terminate,
		guessnumber,
		restart
	};

	HttpCommand DetermineCommand( string& s, int& numberGuessed );


	bool GameCycle();

	

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


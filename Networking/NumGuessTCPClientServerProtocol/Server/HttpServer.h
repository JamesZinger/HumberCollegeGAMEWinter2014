#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string>

using std::string;

class HttpServer
{
private:
	SOCKET ListenSocket;
	SOCKET ClientSocket;

	bool _debugging;

	const int BUFFER_SIZE = 1024;

	char* RCBuffer;

	void BuiltHttpResponse(string* msg);

	int ListenPort;

public:
	HttpServer( string port );
	~HttpServer();

	bool AcceptConnection();

	bool SendHttpMsg( string* msg, int *sentLength );
	bool RecieveData( string* result, int *receivedLength );

	void ClientDisconnect();
	void ServerDisconnect();

	bool isConnected() { return ( ListenSocket != INVALID_SOCKET ); }


#pragma region Properties

	bool Debugging() const { return _debugging; }
	void Debugging( bool val ) { _debugging = val; }

	SOCKET GetServerSocket() { return ListenSocket; }
	SOCKET GetClientSocket() { return ClientSocket; }

#pragma endregion 
};


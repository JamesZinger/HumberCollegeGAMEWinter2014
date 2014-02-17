#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>

class ClassServer
{
public:
	ClassServer( char *port );

	~ClassServer()
	{
		ClientDisconnect();
		ServerDisconnect();
	}

	bool AcceptConnection();

	bool SendData( char *buf, int bufLength, int *sentLength );
	bool RecieveData( char *buf, int bufLength, int *receivedLength );

	void ClientDisconnect();
	void ServerDisconnect();

	bool isConnected() { return ( ListenSocket != INVALID_SOCKET ); }

	SOCKET GetServerSocket() { return ListenSocket; }
	SOCKET GetClientSocket() { return ClientSocket; }

private:
	SOCKET ListenSocket;
	SOCKET ClientSocket;

};
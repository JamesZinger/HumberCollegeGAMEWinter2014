#include "TCPClient.h"
#include <ws2tcpip.h>


GenTCPClientSocket::GenTCPClientSocket(char *hostName, char *port)
{
	 struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;
	int iResult, iSendResult;
	
	 ClientSocket = INVALID_SOCKET;

	 WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	 if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
	 }	   

 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	// Resolve the server address and port
	iResult = getaddrinfo(hostName, port, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}
	
	  // Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ClientSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		if (ClientSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect( ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);
	
	if (ClientSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

}

bool GenTCPClientSocket::SendData(char *buf, int length, int *sentLength)
{
	if (ClientSocket == INVALID_SOCKET) {
		printf("No client is attached. \n");
		return false;
	}
	*sentLength = send( ClientSocket, buf, length, 0 );
	if (*sentLength == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		Disconnect();
		return false;
	}

	return true;
}

bool GenTCPClientSocket::RecieveData(char *buf, int length, int *receivedLength)
{
	if (ClientSocket == INVALID_SOCKET) {
		printf("No client is attached. \n");
		return false;
	}
	*receivedLength = recv(ClientSocket, buf, length, 0);
	if (*receivedLength == 0) {
		printf("Connection closing...\n");
		Disconnect();
		return false;
	}
	else  if (*receivedLength < 0) {
		printf("recv failed: %d\n", WSAGetLastError());
		Disconnect();
		return false;
	}

	return true;

}

void GenTCPClientSocket::ShutdownSend()
{
	int iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		Disconnect();
	}
}

void GenTCPClientSocket::Disconnect()
{
	closesocket(ClientSocket);
	WSACleanup();
}



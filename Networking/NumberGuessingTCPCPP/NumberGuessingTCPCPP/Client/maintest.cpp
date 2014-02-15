#define BUF_LEN 512
#define PORT "8080"

#include "TCPClient.h"
#include <stdio.h>

int __cdecl main(int argc, char **argv) 
{
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

	char sendbuf[BUF_LEN];
	char recvbuf[BUF_LEN];
	int sentLength, receivedLenght;

	strcpy(sendbuf, "GET / HTTP/1.1\n");
	strcat(sendbuf, "Host: localhost\n\n");

	GenTCPClientSocket *clientSocket = new GenTCPClientSocket(argv[1], PORT);

	clientSocket->SendData(sendbuf, strlen(sendbuf), &sentLength);
	clientSocket->ShutdownSend();

    do {
	    clientSocket->RecieveData(recvbuf, BUF_LEN - 1, &receivedLenght);
		if ( receivedLenght > 0 ) {
            printf("Bytes received: %d\n", receivedLenght);
			recvbuf[receivedLenght] = '\0';
			printf("%s", recvbuf);
		}
        else if ( receivedLenght == 0 )
            printf("Connection closed\n");

    } while( receivedLenght > 0 );	


    delete clientSocket;

	getchar();
}
#define BUF_LEN 512

#include "GenTCPServerSocket.h"
#include <stdio.h>


int __cdecl main(void) 
{
	 char header[BUF_LEN];
	 char recvbuf[BUF_LEN];

	 int iResult;
	 strcpy(header, "HTTP/1.0 200 OK\n");
     strcat(header, "Content-Type: text/html\n");
     strcat(header, "Content-Length: %d\n\n%s");
	
	 GenTCPServerSocket *serverSocket = new GenTCPServerSocket("8080");

	 if (serverSocket->isConnected()) {
		 do {
			 if (serverSocket->AcceptConnection()) {
				serverSocket->RecieveData(recvbuf, BUF_LEN - 1, &iResult);
				if (iResult > 0)  {
					recvbuf[iResult] = '\0';
					printf(recvbuf);
				    sprintf(recvbuf, header, 
						strlen("<html><body> <h1> Server is working! </h1> </body> </html>"),
						       "<html><body> <h1> Server is working! </h1> </body> </html>");

					serverSocket->SendData(recvbuf, strlen(recvbuf), &iResult);
				}
			 }
		 }while (iResult > 0);
	 }
	 
	 delete serverSocket;

}
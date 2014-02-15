#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>

class GenTCPClientSocket
{
   public:
      GenTCPClientSocket(char *hostName, char *port);

      ~GenTCPClientSocket() 
	  { 
		  Disconnect(); 
	  }

      bool SendData(char *buf, int bufLength, int *sentLength);
      bool RecieveData(char *buf, int bufLength, int *receivedLength);
      
	  void Disconnect();
	  void ShutdownSend();

	  bool isConnected() { return (ClientSocket != INVALID_SOCKET); }

      SOCKET GetSocket() { return ClientSocket; }

   private:
      SOCKET ClientSocket;

};
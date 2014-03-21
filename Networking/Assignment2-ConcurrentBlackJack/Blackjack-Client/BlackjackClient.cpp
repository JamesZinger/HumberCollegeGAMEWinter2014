#include "BlackjackClient.h"

#include <iostream>
#include <sstream>
#include <WS2tcpip.h>

using std::cout;
using std::endl;
using std::stringstream;

BlackjackClient::BlackjackClient( string name, string hostname, int port, int bufferLength ) : m_bufferLength( bufferLength )
{
	Name( name );
	ClientSocket( INVALID_SOCKET );
	m_inputBuffer = (char*)malloc( bufferLength );
	Hostname( hostname );
	Port( port );
	ShuttingDown( false );

	addrinfo	*result = nullptr,
		*ptr = nullptr,
		hints;

	int iResult;

	iResult = WSAStartup( MAKEWORD( 2, 2 ), &WsaData() );
	if ( iResult != 0 )
	{
		cout << "WSAStartup failed, Error Code: " << iResult << endl;
		system( "Pause" );
		exit( -1 );
	}

	memset( &hints, '\0', sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	stringstream ss;
	string s;
	ss << port;

	s = ss.str();

	iResult = getaddrinfo( hostname.c_str(), s.c_str(), &hints, &result );
	if ( iResult != 0 )
	{
		cout << "Getaddrinfo failed, Error code: " << iResult << endl;
		WSACleanup();
		system( "Pause" );
		exit( -2 );
	}

	for ( ptr = result; ptr != NULL; ptr = ptr->ai_next )
	{
		m_clientSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
		if ( m_clientSocket == INVALID_SOCKET )
		{
			cout << "Error creating socket, WSA Error code: " << WSAGetLastError() << endl;
			freeaddrinfo( result );
			WSACleanup();
			system( "Pause" );
			exit( -2 );
		}


		iResult = connect( m_clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen );
		if ( iResult == SOCKET_ERROR )
		{
			int errorCode = WSAGetLastError();
			if ( errorCode == WSAEWOULDBLOCK )
			{
				break;
			}
			cout << "Failed to connect, Trying other possible connections, Error code: " << WSAGetLastError() << endl;
			closesocket( m_clientSocket );
			ClientSocket( INVALID_SOCKET );
			continue;
		}
		break;
	}

	u_long iMode = 1;
	ioctlsocket( m_clientSocket, FIONBIO, &iMode );

	freeaddrinfo( result );

	if ( ClientSocket() == INVALID_SOCKET )
	{
		cout << "Unable to connect to server" << endl;
		WSACleanup();
		system( "Pause" );
		exit( -3 );
	}

}

BlackjackClient::~BlackjackClient()
{
	Disconnect();
	delete[] m_inputBuffer;
}

bool BlackjackClient::SendData( string& message, int* sentLength /*= NULL */ )
{
	if ( ClientSocket() == INVALID_SOCKET )
	{
		cout << "Cannot send, No client is attached" << endl;
		return false;
	}

	int i = send( ClientSocket(), message.c_str(), message.length(), 0 );

	if ( sentLength != nullptr )
		*sentLength = i;

	if ( i == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode == WSAEWOULDBLOCK )
		{
			return false;
		}
		cout << "Sending failed, WSA Error code: " << errorCode << endl;
		Disconnect();
		return false;
	}

	return true;
}

int BlackjackClient::RecieveData( string* message )
{
	if ( ClientSocket() == INVALID_SOCKET )
	{
		cout << "No client is attached." << endl;
		return -1;
	}
	memset( InputBuffer(), '\0', m_bufferLength );
	int receivedLength = recv( ClientSocket(), InputBuffer(), m_bufferLength, 0 );
	if ( receivedLength == 0 )
	{
		cout << "Connection closing..." << endl;
		Disconnect();
		return -1;
	}
	else  if ( receivedLength < 0 )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode == WSAEWOULDBLOCK )
		{
			return 0;
		}
		cout << "recv failed, WSA Error code: " << errorCode << endl;
		Disconnect();
		return - 1;
	}

	*message = string( InputBuffer() );

	return 1;
}

void BlackjackClient::SendShutdown()
{
	int iResult = shutdown( ClientSocket(), SD_SEND );
	if ( iResult == SOCKET_ERROR )
	{
		cout << "Shutdown Failed, WSA Error code: " << WSAGetLastError() << endl;
		Disconnect();
	}
}

void BlackjackClient::Disconnect()
{
	closesocket( ClientSocket() );
	WSACleanup();
	system( "pause" );
}

void BlackjackClient::Run()
{
	m_networkingThread = new thread( &BlackjackClient::NetworkThreadFunc, this );
	{
		stringstream ss;
		ss << "BlackJackClient Protocol 1.0" << endl;
		ss << "Command = Connect" << endl;
		ss << "Name = " << Name() << endl;
		ss << endl;
		m_outputQueue.push( new string( ss.str() ) );
	}
	// Game loop
	while ( !ShuttingDown() )
	{
		if ( !m_inputQueue.empty() )
		{
			string* msg = nullptr;
			if ( m_inputQueue.try_pop( msg ) )
			{
				HandleMessage( *msg );
				delete msg;

			}
		}

		Update();
	}
}

void BlackjackClient::NetworkThreadFunc()
{
	while ( true )
	{
		// Send messages
		if ( !m_outputQueue.empty() )
		{
			string* msg;
			if ( m_outputQueue.try_pop( msg ) )
			{
				if ( msg->compare( "SHUTDOWN" ) == 0 )
					SendShutdown();

				else
					SendData( *msg );

				delete msg;
			}
		}

		// Receive messages
		{
			string msg = string();
			int retCode = RecieveData( &msg );
			if ( retCode == 1 )
			{
				string* msg2 = new string( msg.c_str() );
				m_inputQueue.push( msg2 );
			}
			else if (retCode == 0)
			{
				
			}
			else if (retCode == -1)
			{
				
				break;
			}

		}
	}
}

void BlackjackClient::HandleMessage( string& message )
{
	cout << "Received Message: " << endl;
	cout << message << endl;
	cout << endl;

	if (message == "SHUTDOWN")
	{
		ShuttingDown( true );
	}
	else
	{
		cout << "Sending Message: " << endl;
		string* retMessage = new string( "Got It!" );
		m_outputQueue.push( retMessage );
		string* shutdownMessage = new string( "SHUTDOWN" );
		m_outputQueue.push( shutdownMessage );
	}
}

void BlackjackClient::Update()
{

}
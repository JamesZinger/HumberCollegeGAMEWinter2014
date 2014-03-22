///////////////////////////////////////////////////////////
//  Player.cpp
//  Implementation of the Class Player
//  Created on:      14-Mar-2014 5:29:06 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "Player.h"
#include "Game.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>


Player::Player( int BufferLength /*= 1024*/ ) : m_inputBufferLength( BufferLength )
{
	m_inputArray = new char[ BufferLength ];
	memset(m_inputArray, '\0', BufferLength);
	SetGame( NULL );
	Socket( INVALID_SOCKET );
	ThreadID( -1 );
	Name( NULL );
}


Player::~Player()
{
	delete m_inputArray;
	closesocket( m_socket );
	delete m_name;
	while ( !m_messageQueue.empty() )
	{
		MessageOutput* msg = NULL;
		m_messageQueue.try_pop( msg );
		if ( msg != NULL )
			delete msg;
	}
}

int Player::RecieveMessage()
{
	if ( Socket() == INVALID_SOCKET )
	{
		printf( "No client is attached. \n" );
		return 0;
	}

	ZeroMemory( m_inputArray, InputBufferLength() );

	int RecvLength = 0;

	RecvLength = recv( Socket(), m_inputArray, InputBufferLength(), 0 );

	if ( RecvLength == 0 )
	{

		printf( "Connection closing...\n" );
		closesocket( Socket() );
		return -1;
	}
	else  if ( RecvLength < 0 )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode == WSAEWOULDBLOCK )
		{
			return 0;
		}
		printf( "recv failed: %d\n", errorCode );
		closesocket( Socket() );
		return -1;
	}

	return 1;
}

void Player::SendNetworkMessage()
{
	if ( m_messageQueue.empty() )
	{
		boost::posix_time::milliseconds time( 200 );
		boost::this_thread::sleep( time );
		return;
	}

	MessageOutput* message = new MessageOutput();

	if ( !m_messageQueue.try_pop( message ) )
	{
		delete message;
		return;
	}

	std::string msg = *message->Message();

	int MessageLength;

	SendMessageOverSocket( msg, &MessageLength );

	delete message;

	return;
}

void Player::PlayerThreadFunc( const string name, SOCKET Client )
{
	{
		char* buffer = new char[ name.length() + 1 ];
		strcpy_s( buffer, name.length() + 1, name.c_str() );
		Name( new string( buffer ) );
	}

	ThreadID( GetCurrentThreadId() );

	Socket( Client );

	while ( true )
	{
		SendNetworkMessage();
		int retCode = RecieveMessage();
		switch ( retCode )
		{
			case -1:
				Socket( INVALID_SOCKET );
				break;
			case 1:
				HandleMessage( string( m_inputArray ) );
				break;
		};

		if ( Socket() == INVALID_SOCKET )
		{
			break;
		}
	}

	delete this;
}

bool Player::SendMessageOverSocket( std::string& Message, int* SendLength )
{
	if ( Socket() == INVALID_SOCKET )
	{
		return false;
	}

	*SendLength = send( Socket(), Message.c_str(), Message.length(), 0 );

	if ( *SendLength == SOCKET_ERROR )
	{
		std::cout << "SOCKET ERROR, WSA Error code: " << WSAGetLastError() << std::endl;
		closesocket( Socket() );
		Socket( INVALID_SOCKET );
		return false;
	}

	return true;
}

void Player::EnqueueMessage( MessageOutput* Message )
{
	MessageOutput* msg = &( *Message );
	m_messageQueue.push( msg );
}

void Player::HandleMessage( std::string message )
{

}


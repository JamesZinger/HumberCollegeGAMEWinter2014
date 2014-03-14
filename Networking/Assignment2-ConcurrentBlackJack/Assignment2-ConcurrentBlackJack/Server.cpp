///////////////////////////////////////////////////////////
//  Server.cpp
//  Implementation of the Class Server
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#include "Server.h"




Server::Server()
{

}


Server::Server(const int port)
{

}


Server::~Server()
{

}


unsigned short Server::GetListenPort()
{

	return  m_listenPort;
}


SOCKET Server::GetListenSocket()
{

	return  m_listenSocket;
}


bool Server::isConnected()
{

	return  (m_listenSocket != INVALID_SOCKET);
}


boolean Server::IsDebugging()
{

	return  m_debugging;
}


void Server::SetDebugging(boolean newVal)
{

}


void Server::SetListenPort(unsigned short newVal)
{

}


void Server::SetListenSocket(SOCKET newVal)
{

}
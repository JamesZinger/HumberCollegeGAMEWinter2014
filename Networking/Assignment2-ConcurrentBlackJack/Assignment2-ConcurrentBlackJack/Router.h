///////////////////////////////////////////////////////////
//  Router.h
//  Implementation of the Class Router
//  Created on:      14-Mar-2014 5:29:07 PM
//  Original author: James
///////////////////////////////////////////////////////////

#if !defined(__ROUTER_H__)
#define __ROUTER_H__

#include <WinSock2.h>

class Router
{

public:
	Router();
	virtual ~Router();
	void RoutingThreadFunc(SOCKET Request);
	int threadID();

private:
	int m_threadID;

};
#endif // !defined(__ROUTER_H__)

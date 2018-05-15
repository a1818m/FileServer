#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "afos/AfSocket.h"
#include "afos/AfThread.h"
class TcpTaskMonitor;

class TcpServer : public AfThread
{
public:
	int StartServer(int port);
	void StopServer();

	virtual int ThreadMain();

private:
	bool m_quitflag;
	AfTcpSocket m_worksocket;
};


#endif
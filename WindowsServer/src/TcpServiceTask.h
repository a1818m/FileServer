#ifndef _TCP_SERVER_TASK_H
#define _TCP_SERVER_TASK_H

#include "afos/AfThread.h"
#include "afos/AfSocket.h"

class TcpServiceTask :public AfThread
{
public:
	TcpServiceTask(AfTcpSocket m_client);
	virtual~TcpServiceTask();
	int StartTask();
	bool alive();
	std::string GetClientAddr();
	virtual int ThreadMain();
public:
	virtual int Service()
	{
		return 0;	
	};
protected:
	bool m_alive;
	AfSockAddr m_addr;
	AfTcpSocket m_servicesock;
};

#endif

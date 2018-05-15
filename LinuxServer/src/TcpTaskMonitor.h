#ifndef _TCP_TASK_MONITOR_H
#define _TCP_TASK_MONITOR_H

#include "TcpServiceTask.h"
#include "../afos/AfMutex.h"
#include <list>
using namespace std;
class TcpServiceTask;
class TcpTaskMonitor:public AfThread
{
public:
	int monitor(TcpServiceTask* task);
	int StartMonitor();
	//µ¥ÀýÄ£Ê½
	static TcpTaskMonitor* i();
	virtual int ThreadMain();
private:
	TcpTaskMonitor();
	~TcpTaskMonitor();
private:
	 list<TcpServiceTask*> m_listtask;
	 AfMutex m_mutex;
	 bool m_quitflag;
};
#endif

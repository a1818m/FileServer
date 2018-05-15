#include "TcpTaskMonitor.h"
TcpTaskMonitor::TcpTaskMonitor()
{

}
TcpTaskMonitor::~TcpTaskMonitor()
{

}
int TcpTaskMonitor::StartMonitor()
{
	m_quitflag = false;
	Run();
	return 0;
}
TcpTaskMonitor* TcpTaskMonitor:: i()
{
	static TcpTaskMonitor p;
	return  &p;
}

int TcpTaskMonitor::monitor(TcpServiceTask* task)
{
	m_mutex.Lock();
	m_listtask.push_back(task);
	m_mutex.Unlock();
	return 0;
}

int TcpTaskMonitor::ThreadMain()
{
	while(!m_quitflag)
	{
		m_mutex.Lock();

		//遍历链表，查看停止的线程
		for(list<TcpServiceTask* >::iterator i = m_listtask.begin();
			i != m_listtask.end();)
		{
			TcpServiceTask* task = *i;
			if (task->alive())
			{
				i++;
			}
			else
			{
				printf("已回收服务线程(client=%s)\n",task->GetClientAddr().c_str());
				Join(task);
				i = m_listtask.erase(i);
				delete task;
			}
		}
		m_mutex.Unlock();
		AfThread::Sleep(1);
	}
	return 0;
}
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

		//���������鿴ֹͣ���߳�
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
				printf("�ѻ��շ����߳�(client=%s)\n",task->GetClientAddr().c_str());
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
#include "TcpServer.h"
#include "TestServiceTask.h"
#include "TcpTaskMonitor.h"
#include "MyFtpService.h"


int TcpServer::StartServer(int port)
{
	//�󶨷������˿�
	if(m_worksocket.Open(AfSockAddr(8888),true)<0)
	{
		printf("Port Open False: \n");
		return -11;
	}
	//�����ͻ�������
	if(m_worksocket.Listen()<0)
	{
		return -22;
	}
	//�����̹߳�����
	TcpTaskMonitor::i()->StartMonitor();
	printf("Server listening on: %d \n",port);
	//�����������߳�
	m_quitflag = false;
	Run();
	return 0;
}

void TcpServer::StopServer()
{
	m_quitflag = true;
	m_worksocket.Close();
	AfThread::Join(this);
}

int TcpServer::ThreadMain()
{
	printf("Server Start Work: \n");
	while(!m_quitflag)
	{
		//�½������׽���
		AfTcpSocket p;
		if(m_worksocket.Accept(&p)<0)
		{
			printf("Accept Client False : \n");
			break;
		}
		//�½�һ����socket����
		TcpServiceTask* task = new MyFtpService(p);
		task->StartTask();
		TcpTaskMonitor::i() ->monitor(task);
	}

	//�رչ����׽��� ���߳�
	return 0;
}
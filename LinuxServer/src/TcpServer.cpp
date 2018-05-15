#include "TcpServer.h"
#include "TestServiceTask.h"
#include "TcpTaskMonitor.h"
#include "MyFtpService.h"


int TcpServer::StartServer(int port)
{
	//绑定服务器端口
	if(m_worksocket.Open(AfSockAddr(8888),true)<0)
	{
		printf("Port Open False: \n");
		return -11;
	}
	//监听客户端请求
	if(m_worksocket.Listen()<0)
	{
		return -22;
	}
	//启动线程管理器
	TcpTaskMonitor::i()->StartMonitor();
	printf("Server listening on: %d \n",port);
	//启动服务器线程
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
		//新建服务套接字
		AfTcpSocket p;
		if(m_worksocket.Accept(&p)<0)
		{
			printf("Accept Client False : \n");
			break;
		}
		//新建一个从socket处理
		TcpServiceTask* task = new MyFtpService(p);
		task->StartTask();
		TcpTaskMonitor::i() ->monitor(task);
	}

	//关闭工作套接字 主线程
	return 0;
}
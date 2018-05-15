#include "TcpServiceTask.h"

TcpServiceTask::TcpServiceTask(AfTcpSocket m_client)
{
	m_servicesock = m_client;
	m_alive = false;
}

TcpServiceTask::~TcpServiceTask()
{
	
}

int TcpServiceTask::StartTask()
{
	m_servicesock.GetPeerAddr(m_addr);
	printf("客户端信息地址: %s \n",m_addr.GetIp_str().c_str());
	m_alive = true;
	Run();
	return 0;
}


bool TcpServiceTask::alive()
{
	return m_alive;
}

std::string TcpServiceTask::GetClientAddr()
{
	return m_addr.GetIp_str();
}

int TcpServiceTask::ThreadMain()
{
	//具体业务
	Service();

	//回收
	m_alive = false;
	m_servicesock.Close();
	printf("服务线程已经退出(client=%s)\n",GetClientAddr().c_str());
	return 0;
}


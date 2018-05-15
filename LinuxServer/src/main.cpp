#include "TcpServer.h"

int main()
{
	TcpServer server_sock;
	server_sock.StartServer(8888);
	
	while (1)
	{
		AfThread::Sleep(1);
	}
	return 0;
}
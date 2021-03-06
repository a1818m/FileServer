#ifndef _TCP_SENDER_H
#define _TCP_SENDER_H

#include "afos/AfSocket.h"

class TcpSend
{
public:
	TcpSend(AfTcpSocket sock):m_sock(sock)
	{
	}

	int send(short type,short length,const void* data)
	{
		unsigned char header[4];
		putUnit16(header,type);
		putUnit16(header+2,length);
		m_sock.Send(header,4);
		
		if(length <= 0) return 0;
		return m_sock.Send(data,length);
	}

private:
	void putUnit16(unsigned char* buf,unsigned short val)
	{
		buf[0] = (unsigned char)(val>>8);
		buf[1] = (unsigned char)val;
	}

private:
	AfTcpSocket m_sock;
};

#endif
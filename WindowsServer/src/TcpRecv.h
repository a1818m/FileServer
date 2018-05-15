#ifndef _TCP_RECVER_H
#define _TCP_RECVER_H

#include "afos/AfSocket.h"

class TcpRecv
{
public:
	TcpRecv(AfTcpSocket sock):m_sock(sock)
	{
		m_buffsize = 1024*12;
		data = new char[m_buffsize];
	}

	~TcpRecv()
	{
		delete[] data;
	}
	void clear()
	{
		type = 0;
		length = 0;
	}
	int recv()
	{
		//����ͷ��
		unsigned char head[4];
		if(4 !=recvN(m_sock,(char* )head,4))
		{
			return -11;
		}
		//ת�����ݸ�ʽ
		type = getUni16(head);
		length = getUni16(head+2);
		if(length > m_buffsize)
		{
			return -12;  //���ݹ���
		}
		//���ݲ�����0�ֽ�
		if(length == 0)
		{
			return 0;
		}

		int n = recvN(m_sock,data,length);
		if(n != length)
		{
			return -1;  //�������ݺ�Ԥ���Ĳ�һ��
		}
		data[length] = 0; //ĩβ��0
		return n;
	}

private:
	int recvN(AfTcpSocket s, char* buf,int len)
	{
		int gotData = 0;
		while(gotData<len)
		{
			int n = s.Recv((char*)buf+gotData,len-gotData);
			if(n <= 0)
			{
				return -1;
			}
			gotData += n;
		}
		return gotData;
	}

	unsigned short getUni16(unsigned char* buf)
	{
		return(buf[0]<<8)+buf[1];
	}
public:
	AfTcpSocket m_sock;
	int m_buffsize;
	unsigned short type;
	unsigned short length;
	char* data;
};

#endif
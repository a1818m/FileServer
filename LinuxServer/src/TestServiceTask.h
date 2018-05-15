#ifndef _TEST_SERVER_TASK_H
#define _TEST_SERVER_TASK_H

#include "TcpServiceTask.h"
#include "TcpSend.h"
#include "TcpRecv.h"
#include "../jsoncpp/json.h"
#include <stdio.h>
#include "MsgDefine.h"


class TestServiceTask :public TcpServiceTask
{
public:
	TestServiceTask(AfTcpSocket m_client)
		:TcpServiceTask(m_client)
	{
	}
	//��������
	int on_login(const string& jsonrep)
	{
		Json::Reader reader;
		Json::Value	resp;
		if(!reader.parse(jsonrep,resp,false))
			throw string("bad json format!\n");
		string username = resp["username"].asString();
		string password = resp["password"].asString();
		if(username != "a1818m")
			throw string("username is false");
		if(password != "123456")
			throw string("password is false");
		return 0;
	}
	//����ķ�������
	int Service()      
	{
		//������Ϣ
		TcpSend sd(m_servicesock);
		TcpRecv rv(m_servicesock);
		while(1)
		{
			//��������
			rv.clear();
			int n = rv.recv();
			printf("n: %d\n",n);
			if(n < 0)
			{
				break;
			}
			//��������
			string jsonrep(rv.data,rv.length);
			int errorCode = 0;
			string reason = "OK";
			if(rv.type == MSG_LOGIN)
			{
				try{
					on_login(rv.data);
				}
				catch(string e)
				{
					errorCode = -1;
					reason = e;
				}
			}
			else
			{
				errorCode = -1;
				reason = "δ֪����";
			}

			//����ظ�����
			Json::Value req;
			Json::FastWriter writer;
			req["errorCode"] = errorCode;
			req["reason"] = reason;
			string jsonresp = writer.write(req);
			sd.send(0,jsonresp.length(),jsonresp.c_str());
		}
		return 0;
	}
	
};

#endif

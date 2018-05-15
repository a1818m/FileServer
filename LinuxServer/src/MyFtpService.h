#ifndef _MY_FTP_SERVICE_H
#define _MY_FTP_SERVICE_H

#include "TcpServiceTask.h"
#include "TcpSend.h"
#include "TcpRecv.h"
#include "FileUtils.h"
#include <stdio.h>

#include "../jsoncpp/json.h"
#include "MsgDefine.h"

class MyFtpService : public TcpServiceTask
{
public:
	MyFtpService(AfTcpSocket client)
		:TcpServiceTask(client)
	{
	#ifdef _WIN32
		m_homeDir = "F:/Project"; 
		m_path = "/"; 
	#else
            m_homeDir = "/home/mytest/project/";  
      #endif
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
		if(username != "aaa")
			throw string("username is false");
		if(password != "111")
			throw string("password is false");
		return 0;
	}

	int on_cd(TcpSend& snd,Json::Value &jrep,Json::Value& jresult)
	{
		//��������
		string path = jrep["path"].asString();
		if(path[0] != '/')
			throw string("Path must start with '/' !");
		m_path = path;  //���浱ǰ·��

		printf("cd %.......\n",m_path.c_str());
		return 0;
	}

	int on_ls(TcpSend& snd,Json::Value &jrep,Json::Value& jresult)
	{
		//��������
		printf("ls %s%s ...\n", m_homeDir.c_str(), m_path.c_str());
		
		FileEntrylist files = FileUtils::ls(m_homeDir+m_path);
		//����Ӧ��Json
		for(FileEntrylist::iterator iter = files.begin();
			iter != files.end();iter++)
		{
			FileEntry& entry = *iter;
			Json::Value jobj;
			jobj["fileName"] = entry.fileName;
			jobj["fileType"] = entry.fileType;
			jobj["isDir"] = entry.isDir;
			jobj["fileSize"] = entry.fileSize;
			jresult.append(jobj);
		}
		return 0;
	}

	int on_get(TcpSend& snd,Json::Value& jreq,Json::Value& jresult)
	{
		//��ȷ���ļ��Ƿ����
		string fileName = jreq["fileName"].asString();
		#ifdef _WIN32
		string fullFilePath = FileUtils::toWindowsPath(m_homeDir+m_path)+fileName;
		#else
            string fullFilePath = m_homeDir+fileName;
		#endif
		FILE* fp = fopen(fullFilePath.c_str(),"rb");
		if(fp == NULL)
			throw string("���������ش���: �ļ��޷���!");

		//ȷʵ��ϣ�������Ϣ
		quickReply(snd,0,"OK");

		//�ļ��Ĵ���
		printf("�����ļ�: %s \n", fullFilePath.c_str());
		int numOfBytes = 0;
		int bufsize = 1024;
		char* buf = new char[bufsize];
		while(!feof(fp))
		{
			int n =fread(buf,1,bufsize,fp);
			if(n<0) break;       //ϵͳ����
			if(n == 0) continue; //�ļ�æµ���ȡ��ĩβ��
			if(snd.send(ACK_DATA_CONTINUE,n,buf)<0)
			{
				//��������ʧ�ܣ�����Ͽ�
				break;
			}
		}
		fclose(fp);
		snd.send(ACK_DATA_FINISH,0,NULL);
		printf("�������: %s \n", fullFilePath.c_str());
		return 1;
	}
	int on_put(TcpSend& snd,Json::Value& jreq,Json::Value& jresult)
	{
		//ȷ���ļ��Ƿ����
		string fileName = jreq["fileName"].asString();
		#ifdef _WIN32
		string fullFilePath = FileUtils::toWindowsPath(m_homeDir+m_path)+fileName;
		#else
            string fullFilePath = m_homeDir+fileName;
		#endif
		FILE* fp = fopen(fullFilePath.c_str(),"wb");
		if(fp == NULL)
			throw string("������·������: �ļ��޷���!");

		//ȷʵ��ϣ�������Ϣ
		quickReply(snd,0,"OK");

		//�����ļ�
		TcpRecv rcv(m_servicesock);
		int numOfBytes = 0;
		while(true)
		{
			// ��������
			int rc = rcv.recv();
			if(rc < 0)
				throw string("failed to recv response! \n");

			if(rcv.type == ACK_DATA_FINISH) break;
			if(rcv.type == ACK_DATA_CONTINUE)
			{
				// д���ļ�
				fwrite(rcv.data, 1, rcv.length, fp);
				numOfBytes += rcv.length;
				printf(".");
			}
		}
		fclose(fp);

		printf("�������,��%d�ֽ�.\n", numOfBytes);
		return 1;
	}
	void quickReply(TcpSend& snd,int errorCode,const string& reason)
	{
		Json::Value resp;
		Json::FastWriter writer;
		resp["errorCode"] = errorCode;
		resp["reason"] = reason;
		std::string jsonresp = writer.write(resp);
		snd.send(0, jsonresp.length(), jsonresp.c_str());
	}
	virtual int Service()
	{
		//��������
		TcpSend snd(m_servicesock);
		TcpRecv rcv(m_servicesock);
		while(true)
		{
			rcv.clear();
			int n = rcv.recv();
			//���ղ������ݺ󣬾��˳���ѭ��
			if(n < 0) break;

			//���������ݷ����ַ���
			string jsonstr(rcv.data,rcv.length); 
			//�������ַ���
			Json::Reader reader;
			Json::Value jreq;	
			if(!reader.parse(jsonstr,jreq,false))
			{
				//����ʧ�ܣ�����Json���ݷ���Ӧ��
				Json::Value resp;
				Json::FastWriter writer;
				resp["errorCode"] = -1;
				resp["reason"] = "request bad json format!";
				std::string jsonresp = writer.write(resp);
				snd.send(0,jsonresp.length(),jsonresp.c_str()); 
			}
			
			//����Ӧ��Json���ݣ��Լ�����Ӧ����������
			Json::Value jresult;
			int errorCode = 0;
			string reason = "OK";
			int rc = 0;
			//��������
			if(rcv.type == MSG_CD)
			{
				try{
					on_cd(snd,jreq,jresult);
				}
				catch(string e)
				{
					errorCode = -1;
					reason = e;
				}
			}
			else if(rcv.type == MSG_LIST)
			{
				try{
					on_ls(snd, jreq, jresult);
				}catch(string e)
				{
					errorCode = -1;
					reason = e;
				}
			}
			else if(rcv.type == MSG_GET)
			{
				try{
					rc = on_get(snd, jreq, jresult);
				}catch(string e)
				{
					errorCode = -1;
					reason = e;
				}
			}
			else if(rcv.type == MSG_PUT)
			{
				try{
					rc = on_put(snd, jreq, jresult);
				}catch(string e)
				{
					errorCode = -1;
					reason = e;
				}
			}
			else if(rcv.type == MSG_LOGIN)
			{
				try{
					rc = on_login(rcv.data);
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

			//����Json���ݣ����ظ�
			if(rc<=0)  //����ڷ����ļ��������Ѿ��ظ��ˣ��Ǵ�ʱ���ûظ���
			{
				Json::Value resp;
				Json::FastWriter writer;
				resp["errorCode"] = errorCode;
				resp["reason"] = reason;
				resp["result"] = jresult;
				std::string jsonresp = writer.write(resp);
				snd.send(0,jsonresp.length(),jsonresp.c_str());
			}
		}
		return 0;
	}
private:
	string m_homeDir;
	string m_path;
};
#endif

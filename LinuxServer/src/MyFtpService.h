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
	
	//解析接收
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
		//解析请求
		string path = jrep["path"].asString();
		if(path[0] != '/')
			throw string("Path must start with '/' !");
		m_path = path;  //保存当前路径

		printf("cd %.......\n",m_path.c_str());
		return 0;
	}

	int on_ls(TcpSend& snd,Json::Value &jrep,Json::Value& jresult)
	{
		//解析请求
		printf("ls %s%s ...\n", m_homeDir.c_str(), m_path.c_str());
		
		FileEntrylist files = FileUtils::ls(m_homeDir+m_path);
		//构造应答Json
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
		//先确认文件是否存在
		string fileName = jreq["fileName"].asString();
		#ifdef _WIN32
		string fullFilePath = FileUtils::toWindowsPath(m_homeDir+m_path)+fileName;
		#else
            string fullFilePath = m_homeDir+fileName;
		#endif
		FILE* fp = fopen(fullFilePath.c_str(),"rb");
		if(fp == NULL)
			throw string("服务器返回错误: 文件无法打开!");

		//确实完毕，回馈消息
		quickReply(snd,0,"OK");

		//文件的传送
		printf("下载文件: %s \n", fullFilePath.c_str());
		int numOfBytes = 0;
		int bufsize = 1024;
		char* buf = new char[bufsize];
		while(!feof(fp))
		{
			int n =fread(buf,1,bufsize,fp);
			if(n<0) break;       //系统出错
			if(n == 0) continue; //文件忙碌或读取到末尾了
			if(snd.send(ACK_DATA_CONTINUE,n,buf)<0)
			{
				//发送数据失败，网络断开
				break;
			}
		}
		fclose(fp);
		snd.send(ACK_DATA_FINISH,0,NULL);
		printf("发送完毕: %s \n", fullFilePath.c_str());
		return 1;
	}
	int on_put(TcpSend& snd,Json::Value& jreq,Json::Value& jresult)
	{
		//确认文件是否存在
		string fileName = jreq["fileName"].asString();
		#ifdef _WIN32
		string fullFilePath = FileUtils::toWindowsPath(m_homeDir+m_path)+fileName;
		#else
            string fullFilePath = m_homeDir+fileName;
		#endif
		FILE* fp = fopen(fullFilePath.c_str(),"wb");
		if(fp == NULL)
			throw string("服务器路径错误: 文件无法打开!");

		//确实完毕，回馈消息
		quickReply(snd,0,"OK");

		//接收文件
		TcpRecv rcv(m_servicesock);
		int numOfBytes = 0;
		while(true)
		{
			// 接收数据
			int rc = rcv.recv();
			if(rc < 0)
				throw string("failed to recv response! \n");

			if(rcv.type == ACK_DATA_FINISH) break;
			if(rcv.type == ACK_DATA_CONTINUE)
			{
				// 写入文件
				fwrite(rcv.data, 1, rcv.length, fp);
				numOfBytes += rcv.length;
				printf(".");
			}
		}
		fclose(fp);

		printf("接收完毕,共%d字节.\n", numOfBytes);
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
		//接收请求
		TcpSend snd(m_servicesock);
		TcpRecv rcv(m_servicesock);
		while(true)
		{
			rcv.clear();
			int n = rcv.recv();
			//接收不到数据后，就退出死循环
			if(n < 0) break;

			//将接收数据放入字符串
			string jsonstr(rcv.data,rcv.length); 
			//解析该字符串
			Json::Reader reader;
			Json::Value jreq;	
			if(!reader.parse(jsonstr,jreq,false))
			{
				//解析失败，构造Json数据返回应答
				Json::Value resp;
				Json::FastWriter writer;
				resp["errorCode"] = -1;
				resp["reason"] = "request bad json format!";
				std::string jsonresp = writer.write(resp);
				snd.send(0,jsonresp.length(),jsonresp.c_str()); 
			}
			
			//构造应答Json数据，以及定义应答数据类型
			Json::Value jresult;
			int errorCode = 0;
			string reason = "OK";
			int rc = 0;
			//分析数据
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
				reason = "未知请求";
			}

			//构造Json数据，并回复
			if(rc<=0)  //如果在发送文件函数中已经回复了，那此时不用回复了
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

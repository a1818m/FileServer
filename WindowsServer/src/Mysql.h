#ifndef _MYSQL_H
#define _MYSQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "libmysql")

class DataSource
{
public:
	DataSource();
	~DataSource();

public:
	int start();  //��ʼ�������ݿ�
	int end();    //�������ݿ�
	int insert(string password,string username);  //�����û�����
	int select(string password,string username);  //�˶��û�����

private:
	MYSQL conn;
};



#endif

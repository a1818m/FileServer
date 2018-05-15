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
	int start();  //开始连接数据库
	int end();    //结束数据库
	int insert(string password,string username);  //插入用户资料
	int select(string password,string username);  //核对用户资料

private:
	MYSQL conn;
};



#endif

#include "Mysql.h"

DataSource::DataSource()
{
}

DataSource::~DataSource()
{
}

int DataSource::start()
{
	//��ʼ��
	mysql_library_init(0, NULL, NULL);
	mysql_init(&conn);

	//����mysql
	char loclAddress[] = "127.0.0.1";
	if(NULL == mysql_real_connect(&conn,loclAddress,"root","123456","example",0,NULL,0))
	{
		printf("failed to connect to database: %s\n",mysql_error(&conn));
		return -1;
	}
	printf("���ݿ����ӳɹ�");
	return 0;
}

int DataSource::end()
{
	mysql_close(&conn);
	mysql_library_end();
	return 0;
}


int DataSource::insert(string password,string username)
{
	char user[10];
	strcpy_s(user,username.c_str());

	int passw;
	passw = atoi(password.c_str());

	//����sql���
	char sql[256];
	sprintf_s(sql,
		"  INSERT INTO `user`"
		"        (`passward`,`user`)"
		"         VALUES "
		"(`%d`,`%s`)"
		,passw
		,user
		);

	//ִ��sql���
	int ret = mysql_query(&conn, sql);
	if(ret != 0)
	{
		printf("error: %s \n", mysql_error(&conn));	
	}
	return 0;
}

int DataSource::select(string password,string username)
{

	char user[10];
	strcpy_s(user,username.c_str());

	int passw;
	passw = atoi(password.c_str());

	char sql[256];
	sprintf_s(sql,
		"  SELECT *FROM `user_pass_word` "
		"         WHERE(password = %d)"
		"         AND(user = '%s')"
		,passw
		,user
		);

	//ִ��sql���
	int ret = mysql_query(&conn, sql);
	if(ret != 0)
	{
		printf("error: %s \n", mysql_error(&conn));		
	}

	MYSQL_RES * result = mysql_store_result(&conn);
	my_ulonglong num_rows = mysql_num_rows(result);
	if(num_rows == NULL)
	{
		printf("�û���ƥ��ʧ��");
		return -1;
	}
	return 0;
}
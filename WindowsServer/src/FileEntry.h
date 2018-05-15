#ifndef _FILE_ENTRY_H
#define _FILE_ENTRY_H

#include <string>
#include <list>
using namespace std;

struct FileEntry
{
	FileEntry():isDir(false)
	{

	}
	string fileName;     //�ļ��� 
	string fileType;     //�ļ�����
	bool isDir;	         //�Ƿ�ΪĿ¼
	long long fileSize;  //�ļ���С
};
typedef list<FileEntry> FileEntrylist;  

#endif
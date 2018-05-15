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
	string fileName;     //文件名 
	string fileType;     //文件类型
	bool isDir;	         //是否为目录
	long long fileSize;  //文件大小
};
typedef list<FileEntry> FileEntrylist;  

#endif
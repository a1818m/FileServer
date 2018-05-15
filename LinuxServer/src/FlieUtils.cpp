
#include "FileUtils.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>

#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#endif

string FileUtils::toWindowsPath (const string& path)
{
	/* 把/ 换成 末尾加\  */
	string d = path;
	for(int i=0; i<d.length() ;i++)
		if(d[i] == '/') d[i] = '\\';
	if(d[d.length()-1] != '\\') 	
		d.append("\\");
	return d;
}

FileEntrylist FileUtils::ls(const string& dir)
{
#ifdef _WIN32
	string d = toWindowsPath(dir);

	FileEntrylist result;

	char filter[256] = {0};
	sprintf(filter,"%s",d.c_str());

	WIN32_FIND_DATA info;
	HANDLE hFind = FindFirstFile(filter, &info);
	while(hFind != INVALID_HANDLE_VALUE)
	{
		FileEntry entry;
		entry.fileName = info.cFileName;
		entry.fileType = info.cAlternateFileName;

		if(entry.fileName != "."&& entry .fileName != "..")
		{
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				entry.isDir = true;
			entry.fileSize = info.nFileSizeHigh;
			entry.fileSize = (entry.fileSize <<32) + info.nFileSizeLow;
			result.push_back(entry);
		}
		if(! FindNextFile(hFind, &info)) break;
	}
	FindClose(hFind);
	return result;
#else
	char path[256] = {0};
	sprintf(path,"%s",dir.c_str());

	FileEntrylist result;

	//打开目录
	DIR *d;
	if(!(d = opendir(path)))
	{
		printf("error opendir %s\n",path);
	}

	//如果打开成功，则循环读取目录
	struct dirent *ent = readdir(d);
	while(NULL != ent)
	{
		FileEntry entry;
		entry.fileName = ent->d_name;
		entry.fileType = ent->d_type;
		entry.isDir = false;
		if(entry.fileName != "."&& entry .fileName != "..")
		{
			struct stat buf;
			stat(entry.fileName.c_str(),&buf);
			entry.fileSize = buf.st_size;
			result.push_back(entry);
		}
		ent = readdir(d);
	}
	return result;
#endif
}


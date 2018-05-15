
#include "FileUtils.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>


string FileUtils::toWindowsPath (const string& path)
{
	string d = path;
	for(int i=0; i<d.length() ;i++)
		if(d[i] == '/') d[i] = '\\';
	if(d[d.length()-1] != '\\') 	
		d.append("\\");
	return d;
}

FileEntrylist FileUtils::ls(const string& dir)
{
	string d = toWindowsPath(dir);

	FileEntrylist result;

	char filter[256] = {0};
	sprintf(filter,"%s*.*",d.c_str());

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
}
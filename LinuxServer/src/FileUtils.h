
#ifndef _File_Utils_h
#define _File_Utils_h

#include "FileEntry.h"

class FileUtils
{
public:
	/* ��б��ת�ɷ�б�� */
	static string toWindowsPath (const string& path);


	static FileEntrylist ls(const string& dir);

};


#endif

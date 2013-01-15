#include "StdAfx.h"
#include "Utility.h"

CString GetUTF8Str( CString &str )
{
	CString r(L"");

	// transform post string to MBCS type
	char* buff = NULL;
	int buffSize = 0;
	buffSize = WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), NULL, 0, 0, 0);
	if(buffSize > 0)
	{
		buff = new char[buffSize + 1];
		ZeroMemory(buff, (buffSize + 1)*sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), buff, buffSize, 0, 0);
		for(int i = 0; i < buffSize; i++)
		{
			r.AppendFormat(L"%%%02X", (unsigned char)buff[i]);
		}
		delete []buff;
	}

	return r;
}


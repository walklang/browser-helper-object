#pragma once

#include "afxinet.h"

class CTestFunction
{
public:
	static CString GetWideCharResponse(CHttpFile* pHttpFile);
	static CString GetWideChar(LPCSTR multiByte);
	static LPSTR GetMultiByte(LPCWSTR wideChar);
	static int GetMultiByteLength(LPCWSTR wideChar);
};

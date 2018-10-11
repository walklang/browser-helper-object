#include "StdAfx.h"
#include "TestFunction.h"

CString CTestFunction::GetWideCharResponse(CHttpFile* pHttpFile){
	DWORD dwResponseLength;
	LPSTR lpszResponse;
	CString strResponse;

	dwResponseLength=(DWORD)pHttpFile->GetLength();
	while (0 != dwResponseLength){
		lpszResponse = (LPSTR)malloc(dwResponseLength + 1);
		lpszResponse[dwResponseLength] = '\0';
		pHttpFile->Read(lpszResponse, dwResponseLength);
		strResponse += GetWideChar((LPCSTR&)lpszResponse);
		free(lpszResponse);
		dwResponseLength=(DWORD)pHttpFile->GetLength();
	}

	return strResponse;
}

CString CTestFunction::GetWideChar(LPCSTR multiByte){
	CString strWideChar;
	LPWSTR lpwStr;
	int multiByteLength;	
	
	multiByteLength=MultiByteToWideChar(CP_UTF8,NULL,multiByte,-1,NULL,0);

	lpwStr=(LPWSTR)new wchar_t[multiByteLength];
	MultiByteToWideChar(CP_UTF8,0,multiByte,-1,lpwStr,multiByteLength);
	strWideChar=lpwStr;

	delete[] lpwStr;

	return strWideChar;
}

LPSTR CTestFunction::GetMultiByte(LPCWSTR wideChar){
	LPSTR lpStr;	
	int wideCharLength;

	wideCharLength=WideCharToMultiByte(CP_UTF8,NULL,wideChar,-1,NULL,0,NULL,FALSE);

	lpStr=(LPSTR)new char[wideCharLength];
	WideCharToMultiByte(CP_UTF8,0,wideChar,-1,lpStr,wideCharLength,NULL,NULL);

	return lpStr;
}

int CTestFunction::GetMultiByteLength(LPCWSTR wideChar){
	int wideCharLength;

	wideCharLength=WideCharToMultiByte(CP_UTF8,NULL,wideChar,-1,NULL,0,NULL,FALSE);

	return wideCharLength-1;
}
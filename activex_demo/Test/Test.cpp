// Test.cpp : Implementation of CTestApp and DLL registration.

#include "stdafx.h"
#include "Test.h"

#include "CatHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTestApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x7BE248BC, 0x79A9, 0x4606, { 0x8A, 0xDA, 0x15, 0x9A, 0x43, 0xEF, 0x2D, 0x20 } };

const CATID CATID_SafeForScripting =
		{0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};
const CATID CATID_SafeForInitializing  =
		{0x7dd95802,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};
const GUID CDECL BASED_CODE _ctlid =
		{0xd3b60ec7,0xf23e,0x4c90,{0xbc,0x5e,0x8d,0x30,0xcb,0x9c,0xab,0xf4}};

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CTestApp::InitInstance - DLL initialization

BOOL CTestApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CTestApp::ExitInstance - DLL termination

int CTestApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	if(FAILED(CreateComponentCategory(CATID_SafeForScripting,L"Controls that are safely scriptable"))){
		return ResultFromScode(SELFREG_E_CLASS);
	}

	if(FAILED(CreateComponentCategory(CATID_SafeForInitializing,L"Controls safely initializable from persistent data"))){
		return ResultFromScode(SELFREG_E_CLASS);
	}

	if(FAILED(RegisterCLSIDInCategory(_ctlid, CATID_SafeForScripting))){
		return ResultFromScode(SELFREG_E_CLASS);
	}

	if(FAILED(RegisterCLSIDInCategory(_ctlid, CATID_SafeForInitializing))){
        return ResultFromScode(SELFREG_E_CLASS);
	}

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

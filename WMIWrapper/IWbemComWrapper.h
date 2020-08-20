#pragma once

#include <sstream>
#include <stdexcept>

#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

static bool g_comInitialized = false;

class IWbemComWrapper
{
private:
	std::stringstream m_eMessage;
	IWbemLocator* m_pLoc;
	IWbemServices* m_pSvc;

public:
	IWbemComWrapper();
	~IWbemComWrapper();

	HRESULT ConnectServer(const BSTR wmiNamespacePath);
	HRESULT SetSecurity();
	HRESULT GetObject(const BSTR strObjectPath, IWbemClassObject** ppObject);
	HRESULT PutInstance(IWbemClassObject* instance);
};

HRESULT InitComLibrary();
inline void FreeComLibrary() {
	if (g_comInitialized) {
		CoUninitialize();
	}
}
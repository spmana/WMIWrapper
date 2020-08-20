#include "IWbemComWrapper.h"

IWbemComWrapper::IWbemComWrapper() {
	m_pLoc = NULL;
	m_pSvc = NULL;

	if (!g_comInitialized) {
		m_eMessage << "Com Libary must be initialized first";
		throw std::runtime_error(m_eMessage.str().c_str());
	}

	HRESULT hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID*)&m_pLoc);

	if (FAILED(hres)) {
		m_eMessage << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< std::hex << hres;
		throw std::runtime_error(m_eMessage.str().c_str());
	}
}

IWbemComWrapper::~IWbemComWrapper()
{
	if (m_pLoc) {
		m_pLoc->Release();
	}

	if (m_pSvc) {
		m_pSvc->Release();
	}
}

HRESULT IWbemComWrapper::ConnectServer(const BSTR wmiNamespacePath) {
	return m_pLoc->ConnectServer(
		wmiNamespacePath,
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&m_pSvc
	);
}

HRESULT IWbemComWrapper::SetSecurity() {
	return CoSetProxyBlanket(
		m_pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);
}

HRESULT IWbemComWrapper::GetObject(const BSTR strObjectPath, IWbemClassObject** ppObject) {
	return m_pSvc->GetObjectW(
		strObjectPath,
		WBEM_FLAG_RETURN_WBEM_COMPLETE,
		NULL,
		ppObject,
		NULL);
}

HRESULT IWbemComWrapper::PutInstance(IWbemClassObject* instance) {
	return m_pSvc->PutInstance(instance, WBEM_FLAG_CREATE_OR_UPDATE, NULL, NULL);
}

HRESULT InitComLibrary() {
	HRESULT hres;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hres)) {
		g_comInitialized = true;

		hres = CoInitializeSecurity(
			NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_DEFAULT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE,
			NULL
		);
	}

	return hres;
}
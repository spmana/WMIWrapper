#include "BaseIWbemInstance.h"

BaseIWbemInstance::BaseIWbemInstance(const wchar_t* className, IWbemComWrapper& iWbem) {
	m_instance = NULL;
	m_classObject = NULL;

	HRESULT hres = iWbem.GetObject(_bstr_t(className), &m_classObject);
	if (FAILED(hres)) {
		m_eMessage << "GetObject Failed. Error code = 0x"
			<< std::hex << hres;
		throw std::runtime_error(m_eMessage.str().c_str());
	}

	hres = m_classObject->SpawnInstance(0, &m_instance);
	if (FAILED(hres)) {
		m_eMessage << "SpawnInstance Failed. Error code = 0x"
			<< std::hex << hres;
		throw std::runtime_error(m_eMessage.str().c_str());
	}
}

BaseIWbemInstance::~BaseIWbemInstance() {
	if (m_classObject) {
		m_classObject->Release();
	}

	if (m_instance) {
		m_instance->Release();
	}
}

/* For documentation please refer to https://docs.microsoft.com/en-us/windows/win32/api/wbemcli/nf-wbemcli-iwbemclassobject-put*/
HRESULT BaseIWbemInstance::EditProperty(const LPCWSTR propName, 
	const OLECHAR* propValue, CIMTYPE_ENUMERATION propType) {

	VARIANT v;
	VariantInit(&v);
	V_VT(&v) = VT_BSTR;
	V_BSTR(&v) = SysAllocString(propValue);

	HRESULT hres = m_instance->Put(propName, 0, &v, propType);
	VariantClear(&v);
	return hres;
}

HRESULT BaseIWbemInstance::Save(IWbemComWrapper &iWbem) {
	return iWbem.PutInstance(m_instance);
}
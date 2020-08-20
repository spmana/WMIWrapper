#pragma once

#include <sstream>
#include <stdexcept>
#include "IWbemComWrapper.h"

class BaseIWbemInstance {
private:
	// const uint8_t creatorSID[16] = { 1,2,0,0,0,0,0,5,32,0,0,0,32,2,0,0 };

	IWbemClassObject* m_classObject;
	IWbemClassObject* m_instance;

	std::stringstream m_eMessage;

public:
	BaseIWbemInstance(const wchar_t* className, IWbemComWrapper &iWbem);
	~BaseIWbemInstance();

	HRESULT EditProperty(const LPCWSTR propName, 
		const OLECHAR* propValue, CIMTYPE_ENUMERATION propType);

	HRESULT Save(IWbemComWrapper& iWbem);
};
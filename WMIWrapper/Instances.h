#pragma once

#include "BaseIWbemInstance.h"

class __EventFilter : public BaseIWbemInstance {
public:
	__EventFilter(OLECHAR* name, OLECHAR* wmiNamespace, OLECHAR* query,
		OLECHAR* queryLang, IWbemComWrapper& iWbem) : BaseIWbemInstance(L"__EventFilter", iWbem) {
		
		EditProperty(L"Name", name, CIM_STRING);
		EditProperty(L"EventNamespace", wmiNamespace, CIM_STRING);
		EditProperty(L"Query", query, CIM_STRING);
		EditProperty(L"QueryLanguage", queryLang, CIM_STRING);
	}
};

class CommandLineEventConsumer : public BaseIWbemInstance {
public:
	CommandLineEventConsumer(OLECHAR* name, OLECHAR* command, bool runInteractively,
		IWbemComWrapper& iWbem) : BaseIWbemInstance(L"CommandLineEventConsumer", iWbem) {

		OLECHAR* rInteractively = (OLECHAR*)(runInteractively ? L"TRUE" : L"FALSE");

		EditProperty(L"Name", name, CIM_STRING);
		EditProperty(L"CommandLineTemplate", command, CIM_STRING);
		EditProperty(L"RunInteractively", rInteractively, CIM_BOOLEAN);
	}
};

class __FilterToConsumerBinding : public BaseIWbemInstance {
public:
	__FilterToConsumerBinding(OLECHAR* consumer, OLECHAR* filter,
		IWbemComWrapper& iWbem) : BaseIWbemInstance(L"__FilterToConsumerBinding", iWbem) {

		EditProperty(L"Consumer", consumer, CIM_REFERENCE);
		EditProperty(L"Filter", filter, CIM_REFERENCE);
	}
};
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "IWbemComWrapper.h"
#include "Instances.h"

static std::stringstream eMessage; 

int main() {
	int retval = 0;
	HRESULT hres;

	try
	{
		/* Initialize COM env.*/
		hres = InitComLibrary();
		if (FAILED(hres)) {
			eMessage << "Failed to initialize COM Library. Error code = 0x"
				<< std::hex << hres;
			throw std::runtime_error(eMessage.str().c_str());
		}

		/* Initialize WMI COM object.*/
		IWbemComWrapper iWbem;
		hres = iWbem.ConnectServer(_bstr_t(L"ROOT\\SUBSCRIPTION"));
		if (FAILED(hres)) {
			eMessage << "Could not connect. Error code = 0x"
				<< std::hex << hres;
			throw std::runtime_error(eMessage.str().c_str());
		}

		hres = iWbem.SetSecurity();
		if (FAILED(hres)) {
			eMessage << "Could not set proxy blanket. Error code = 0x"
				<< std::hex << hres;
			throw std::runtime_error(eMessage.str().c_str());
		}

		/* Create new instace of __EventFilter and write it to WMI Repository.*/
		OLECHAR* name = const_cast<OLECHAR*>(L"BugSecFilter");
		OLECHAR* nspace = const_cast<OLECHAR*>(L"root\\cimv2");
		OLECHAR* q = const_cast<OLECHAR*>(L"SELECT * FROM __InstanceCreationEvent WITHIN 5 WHERE TargetInstance ISA \"Win32_Process\" AND TargetInstance.Caption = \"chrome.exe\"");
		OLECHAR* qlang = const_cast<OLECHAR*>(L"WQL");
		__EventFilter e(name, nspace, q, qlang, iWbem);

		hres = e.Save(iWbem);
		if (FAILED(hres)) {
			eMessage << "Save EventFilter Failed. Error code = 0x"
				<< std::hex << hres;
			throw std::runtime_error(eMessage.str().c_str());
		}

		/* Create new instace of CommandLineEventConsumer and write it to WMI Repository.*/
		name = const_cast<OLECHAR*>(L"BugSecConsumer");
		OLECHAR* command = const_cast<OLECHAR*>(L"cmd /c echo test > C:\\WMI-CPP.txt");
		CommandLineEventConsumer c(name, command, false, iWbem);
		
		hres = c.Save(iWbem);
		if (FAILED(hres)) {
			eMessage << "Save CommadLineEventConsumer Failed. Error code = 0x"
				<< std::hex << hres;
			throw std::runtime_error(eMessage.str().c_str());
		}

		/* Create new instace of __FilterToConsumerBinding and write it to WMI Repository.*/
		OLECHAR* consumer = const_cast<OLECHAR*>(L"CommandLineEventConsumer.Name=\"BugSecConsumer\"");
		OLECHAR* filter = const_cast<OLECHAR*>(L"__EventFilter.Name=\"BugSecFilter\"");
		__FilterToConsumerBinding fc(consumer, filter, iWbem);

		hres = fc.Save(iWbem);
		if (FAILED(hres)) {
			eMessage << "Save FilterToConsumer Failed. Error code = 0x"
				<< std::hex << hres;
			throw std::runtime_error(eMessage.str().c_str());
		}

	}

	catch (std::exception &e)
	{
		std::cout << e.what();
		retval = 1;
	}

	FreeComLibrary();

	return retval;
}
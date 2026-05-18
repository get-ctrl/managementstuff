// divManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ManagementScope.h"
#include "ManagementSearcher.h"

void on_new_instance(IWbemClassObject* object)
{
	VARIANT property;
	VariantInit(&property);

	HRESULT result = object->Get(L"Name", 0, &property, 0, 0);

	std::wcout << property.bstrVal << std::endl;

	VariantClear(&property);
}

int main()
{
	HRESULT result;
	ManagementScope scope;
	ManagementSearcher searcher;
	result = scope.connect(".", "root\\cimv2");
	result = searcher.get_instances(&scope, "SELECT Name FROM Win32_ComputerSystem", on_new_instance);

	std::cout << std::hex << result << std::endl;
}
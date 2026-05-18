

#pragma once

#include "ManagementScope.h"

class ManagementSearcher
{
public:
	ComPtr<IEnumWbemClassObject> enumerator = nullptr;
	ComPtr<IWbemClassObject> object = nullptr;
	HRESULT get_instances(ManagementScope* scope, std::string query, std::function<void(ComPtr<IWbemClassObject>)> callback);
	HRESULT close();
};
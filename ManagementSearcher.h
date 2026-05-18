

#pragma once

#include "ManagementScope.h"

class ManagementSearcher
{
public:
	IEnumWbemClassObject* enumerator = nullptr;
	IWbemClassObject* object = nullptr;
	HRESULT get_instances(ManagementScope* scope, std::string query, std::function<void(IWbemClassObject*)> callback);
	HRESULT close();
};
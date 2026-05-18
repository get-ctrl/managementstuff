

#pragma once

#include "ManagementCommon.h"

class ManagementScope
{
public:
	BOOL initialized = FALSE;
	BOOL connected = FALSE;
	ComPtr<IWbemLocator> locator = nullptr;
	ComPtr<IWbemServices> service = nullptr;
	HRESULT connect(std::string node, std::string path);
	HRESULT close();
};
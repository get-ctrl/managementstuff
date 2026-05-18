

#pragma once

#include "ManagementCommon.h"

class ManagementScope
{
public:
	BOOL connected = FALSE;
	IWbemLocator* locator = nullptr;
	IWbemServices* service = nullptr;
	HRESULT connect(std::string node, std::string path);
	HRESULT close();
};


#include "ManagementScope.h"

HRESULT ManagementScope::connect(std::string node, std::string path)
{
	if (connected == TRUE)
	{
		ManagementScope::close();
	}

	HRESULT result;

	result = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (result == S_OK)
	{
		initialized = TRUE;
	}
	else if (result == RPC_E_CHANGED_MODE)
	{
		initialized = FALSE;
	}
	else if (FAILED(result))
	{
		return result;
	}

	result = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

	if (FAILED(result) && result != RPC_E_TOO_LATE)
	{
		CoUninitialize();
		return result;
	}

	result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator);

	if (FAILED(result))
	{
		CoUninitialize();
		return result;
	}

	std::string full_path = "\\\\" + node + "\\" + path;
	_bstr_t com_path(full_path.c_str());

	result = locator->ConnectServer(com_path, NULL, NULL, 0, NULL, 0, 0, &service);

	if (FAILED(result))
	{
		locator.Reset();
		CoUninitialize();
		return result;
	}

	result = CoSetProxyBlanket(service.Get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	if (FAILED(result))
	{
		service.Reset();
		locator.Reset();
		CoUninitialize();
		return result;
	}

	connected = TRUE;

	return S_OK;
}

HRESULT ManagementScope::close()
{
	service.Reset();
	locator.Reset();

	if (initialized == TRUE)
	{
		CoUninitialize();
		initialized = FALSE;
	}

	connected = FALSE;
	return S_OK;
}

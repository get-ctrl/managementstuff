

#include "ManagementScope.h"

HRESULT ManagementScope::connect(std::string node, std::string path)
{
	if (connected == TRUE)
	{
		ManagementScope::close();
	}

	HRESULT result;

	result = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(result))
	{
		return result;
	}

	result = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(result))
	{
		CoUninitialize();
		return result;
	}

	result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator);
	if (FAILED(result) || locator == nullptr)
	{
		CoUninitialize();
		return result;
	}

	std::string full_path = "\\\\" + node + "\\" + path;
	_bstr_t com_path(full_path.c_str());

	result = locator->ConnectServer(com_path, NULL, NULL, 0, NULL, 0, 0, &service);
	if (FAILED(result) || service == nullptr)
	{
		locator->Release();
		CoUninitialize();
		return result;
	}

	result = CoSetProxyBlanket(service, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(result))
	{
		service->Release();
		locator->Release();
		CoUninitialize();
		return result;
	}

	connected = TRUE;

	return S_OK;
}

HRESULT ManagementScope::close()
{
	if (service == nullptr)
	{
		service->Release();
	}

	if (locator == nullptr)
	{
		locator->Release();
	}

	CoUninitialize();

	connected = FALSE;

	return S_OK;
}

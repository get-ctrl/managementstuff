
#include "ManagementSearcher.h"

HRESULT ManagementSearcher::get_instances(ManagementScope* scope, std::string query, std::function<void(IWbemClassObject*)> callback)
{
	if (scope->connected == FALSE)
	{
		return E_NOT_VALID_STATE;
	}

	HRESULT result;

	_bstr_t query_lang("WQL");
	_bstr_t query_string(query.c_str());

	result = scope->service->ExecQuery(query_lang, query_string, WBEM_FLAG_FORWARD_ONLY, NULL, &enumerator);
	if (FAILED(result))
	{
		return result;
	}

	ULONG object_result;
	long timeout = 10 * 1000;

	while (enumerator)
	{
		result = enumerator->Next(timeout, 1, &object, &object_result);

		if (object_result == 0)
		{
			break;
		}

		callback(object);

		object->Release();
	}

	enumerator->Release();

	return S_OK;
}
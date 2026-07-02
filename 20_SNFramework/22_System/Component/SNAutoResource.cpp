#include "SNAutoResource.h"
#include "SNWindowsAPI.h"

// コンストラクタ
SNAutoResource::SNAutoResource(SNResourceIF* resource)
{
	Resource = resource;

	if (Resource != nullptr)
	{
		Resource->Acquire();
	}

	return;
}

// デストラクタ
SNAutoResource::~SNAutoResource()
{
	if (Resource != nullptr)
	{
		Resource->Release();
	}

	return;
}

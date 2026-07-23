#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUIPicture.h"


class SNUserApp : public SNLayerController
{
public:
	SNUserApp();
	virtual ~SNUserApp();

	virtual Void OnPreDraw();


	SNGUIPicture Picture;
};


#pragma once
#include "SNFrameworkInternal.h"
#include "SNWorldAppBase.h"
#include "SNWNearbySpace.h"

class SNWorldAppCreation : public SNWorldAppBase
{
public:
	static constexpr Int32 MapchipListView = 4;
	static constexpr Int32 FocusAcc = 3;
public:
	SNWorldAppCreation();
	virtual ~SNWorldAppCreation();

	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnGamePad1();

	virtual Boolean OnInternalEvent();

	virtual Void OnPreDraw();

	virtual Void OnDraw();

protected:
	virtual Void WriteBlock();

private:
	SNGUIWindow Win;
	SNGUITextEx	txtX;
	SNGUITextEx txtY;
	SNGUITextEx txtZ;
	SNGUIWindow WinBlock;

	UInt16 SelectBlock;


	Boolean RangeMode;
	Boolean CopiedFlag;
	UInt16 CopySpace[SNSystemConfig::WorldNearbySpaceSizeV][SNSystemConfig::WorldNearbySpaceSizeH][SNSystemConfig::WorldNearbySpaceSizeH];
	UInt16 WorkSpace[SNSystemConfig::WorldNearbySpaceSizeV][SNSystemConfig::WorldNearbySpaceSizeH][SNSystemConfig::WorldNearbySpaceSizeH];
};


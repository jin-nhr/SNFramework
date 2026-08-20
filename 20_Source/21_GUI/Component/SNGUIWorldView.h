#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNBitmap.h"

// ワールドビュー
class SNGUIWorldView : public SNScene
{
public:
	// コンストラクタ
	SNGUIWorldView();

	// デストラクタ
	virtual ~SNGUIWorldView();

	virtual Void SetViewPos(SNWorldPos* pos);

	virtual Void MoveViewPos(SNWorldPos* pos);

	virtual Void SetViewScale(Float32 scale);

	virtual Void UpViewScale();

	virtual Void DownViewScale();

	virtual Void GetViewPos(SNWorldPos* pos);

	virtual Void SetViewDir(SNWorldDir dir);

	virtual Void RotateRViewDir();

	virtual Void RotateLViewDir();

	virtual SNWorldDir GetViewDir();

protected:
	virtual Void OnInitialize();

	virtual Void OnPreDraw();

	virtual Void OnDraw(SNGraphicsContext* grc);

private:
	SNWorldPos TargetPos;
	Float32 ViewScale;
	SNBitmap WorkSurface;
	
	SNWorldDir ViewDir;
};

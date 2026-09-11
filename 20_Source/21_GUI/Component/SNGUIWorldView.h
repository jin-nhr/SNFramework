#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNBitmap.h"
#include "SNWNearbySpace.h"

// ワールドビュー
class SNGUIWorldView : public SNScene
{
public:
	static constexpr SNGraphicsResID BGRes[SNWTimeZoneNum] =
	{
		SNGraphicsResBG1,
		SNGraphicsResBG2,
		SNGraphicsResBG3,
		SNGraphicsResBG4,
	};

	static const UInt8 TimeZoneAlpha = 40;

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

	virtual SNWorldDir UpToAngle();
	virtual SNWorldDir UpLeftToAngle();
	virtual SNWorldDir UpRightToAngle();
	virtual SNWorldDir DownToAngle();
	virtual SNWorldDir DownLeftToAngle();
	virtual SNWorldDir DownRightToAngle();
	virtual SNWorldDir LeftToAngle();
	virtual SNWorldDir RightToAngle();

	virtual Void UpToWorldDirPos(Float32* x, Float32* y);
	virtual Void DownToWorldDirPos(Float32* x, Float32* y);
	virtual Void LeftToWorldDirPos(Float32* x, Float32* y);
	virtual Void RightToWorldDirPos(Float32* x, Float32* y);

	virtual Void SetFocusVisible(Boolean visible);

protected:
	virtual Void OnInitialize();
	virtual Void OnTerminate();

	virtual Void OnPreDraw();
	virtual Void SortObject();
	virtual Void DrawWrokSurface();

	virtual Void OnDraw();

	// 周辺オブジェクト描画(D3D)
	virtual Void DrawNearbyObject(SNWNearbyObject* obj, SNPoint* draw_base);
	virtual Void DrawNearbyObjectGround(SNWNearbyObject* obj, SNPoint* draw_base);
	virtual Void DrawNearbyObjectEffectGround(SNWNearbyObject* obj, SNPoint* draw_base);
	virtual Void DrawNearbyObjectActiveObject(SNWNearbyObject* obj, SNPoint* draw_base);
	virtual Void DrawNearbyObjectFocus(SNWNearbyObject* obj, SNPoint* draw_base);

	virtual Void DrawGround(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base);
	virtual Void DrawGroundBorder(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base);
	virtual Void DrawGroundShadow(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base);
	virtual Void DrawActiveObject(SNWNearbyObject* obj, UInt16 code, SNWorldDir obj_dir, SNWObjectchip::SNWActState obj_state, SNPoint* draw_base);

	// ソート処理用オブジェクト比較
	static Boolean CompareDrawObjectN(Void* a, Void* b);
	static Boolean CompareDrawObjectNE(Void* a, Void* b);
	static Boolean CompareDrawObjectE(Void* a, Void* b);
	static Boolean CompareDrawObjectSE(Void* a, Void* b);
	static Boolean CompareDrawObjectS(Void* a, Void* b);
	static Boolean CompareDrawObjectSW(Void* a, Void* b);
	static Boolean CompareDrawObjectW(Void* a, Void* b);
	static Boolean CompareDrawObjectNW(Void* a, Void* b);


private:
	SNWorldPos TargetPos;
	Float32 ViewScale;
	Boolean FocusVisible;
	
	SNWorldDir ViewDir;

	SNBitmap WorkSurface;
};

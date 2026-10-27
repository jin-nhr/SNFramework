#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNBitmap.h"
#include "SNWNearbySpace.h"

// ワールドビュー
class SNGUIWorldView : public SNScene
{
public:
	enum SNGUIWorldViewFrontTransparentType
	{
		SNGUIWorldViewFrontTransparentTypeOff,
		SNGUIWorldViewFrontTransparentTypeNum = 7,
	};

	static constexpr UInt8 TimeZoneAlpha = 40;

	static constexpr Float32 TransGainOff = 1.00f;
	static constexpr Float32 TransGainLow = 0.20f;
	static constexpr Float32 TransGainHigh = 0.00f;

	static constexpr UInt8 TransRangeMinZ = 1;
	static constexpr Int32 TransViewRange = 128;

	static constexpr Float32 CursorSpeed = 0.2f;

	static constexpr Float32 SegmentHeight = 8;

	static constexpr Float32 FrontTransAlpha[SNGUIWorldViewFrontTransparentTypeNum] =
	{
		1.00f,
		0.20f,
		0.20f,
		0.20f,
		0.00f,
		0.00f,
		0.00f,
	};

	static constexpr Int32 FrontTransZ[SNGUIWorldViewFrontTransparentTypeNum] =
	{
		4,
		4,
		2,
		0,
		4,
		2,
		0,
	};

	static constexpr SNSize MiniMapSize = { 128, 80 };
	static constexpr SNPoint MiniMapPos = { 816, 16 };
	static constexpr Float32 MiniMapScale = 0.125f;

	static constexpr SNPoint MiniMapFramePos = { 96, 48 };

	static constexpr SNPoint TimeIconPos[SNWTimeZoneNum] =
	{
		{112, 128},
		{128, 128},
		{144, 128},
		{ 96, 128 }
	};

	static constexpr SNSize TimeIconSize = { 16, 16 };

	static constexpr SNPoint TimeIconDrawPos =
	{
		MiniMapSize.Width,
		MiniMapSize.Height
	};

	static constexpr SNPoint TimeIconDrawPosCenter =
	{
		MiniMapPos.X + MiniMapSize.Width / 2,
		MiniMapPos.Y + MiniMapSize.Height / 2
	};

public:
	// コンストラクタ
	SNGUIWorldView();

	// デストラクタ
	virtual ~SNGUIWorldView();

	virtual Void SetViewPos(SNWorldPos* pos);

	virtual Void MoveViewPos(SNWorldPos* pos);

	virtual Void SaveFocusRangeStart();

	virtual Void SetViewScale(Float32 scale);
	virtual Float32 GetViewScale();

	virtual Void UpViewScale();

	virtual Void DownViewScale();

	virtual Void GetViewPos(SNWorldPos* pos);

	virtual Void GetRangeStartPos(SNWorldPos* pos);

	virtual Void SetVisibleMiniMap(Boolean visible);

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

	virtual Void ChangeTransparentFrontGround();

	virtual Void CalcLeftTop(SNWorldPos* pos1, SNWorldPos* pos2, SNWorldPos* lt_pos);
	virtual Void CalcoRightBottom(SNWorldPos* lt_pos, SNWorldPos* size, SNWorldPos* rb_pos);

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
	virtual Void DrawNearbyObjectShadow(SNWNearbyObject* obj, SNPoint* draw_base);

	virtual Void DrawGround(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base);
	virtual Void DrawActiveObject(SNWNearbyObject* obj, UInt16 code, SNWorldDir obj_dir, UInt8 act_state, UInt8 anm_step, SNPoint* draw_base);

	// マップチップ描画座標計算
	virtual Void CalcMapchipDrawPos(SNWNearbyObject* obj, SNPoint* draw_base, SNPoint* out);
	virtual Void CalcObjectchipDrawPos(SNWNearbyObject* obj, SNPoint* draw_base, SNPoint* out);

	// 手前描画物の透過率判定
	virtual Float32 JudgeFrontTransparent(SNWNearbyObject* obj, SNRect* dst_rect);

	// フォーカス登録処理
	virtual Void RegisterFocus();

	// ソート処理用オブジェクト比較
	static Boolean CompareDrawObject(Void* a, Void* b);

private:
	SNWorldPos TargetPos;
	SNWorldPos RangeStart;
	Float32 ViewScale;
	Boolean FocusVisible;
	
	SNBitmap WorkSurface;
	SNSize WorkSurfaceSize;

	Boolean VisibleMiniMap;

	SNGUIWorldViewFrontTransparentType TransparentFrontGround;
};

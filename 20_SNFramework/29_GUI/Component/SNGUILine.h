#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// GUIラインクラス
class SNGUILine : public virtual SNScene
{
public:
	static constexpr UInt8 PointMax = 8;

public:
	// コンストラクタ
	SNGUILine();

	// デストラクタ
	virtual ~SNGUILine();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 色設定
	Void SetColor(SNColorCode Color);

	// 座標設定
	Void SetPoint(SNPoint* point, UInt8 point_num);

protected:
	// 描画処理
	virtual Void OnDraw(SNSurface* surface);


	SNColorCode Color;				// 色
	SNPoint     Point[PointMax];	// 座標
	UInt8		PointNum;			// 座標数
};


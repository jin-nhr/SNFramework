#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

class SNSurface;

// フェーダークラス
class SNGUIFader : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUIFader();

	// デストラクタ
	virtual ~SNGUIFader();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// フェード種別設定
	Void SetFadeType(SNGUIFadeType fadetype);

	// 色設定
	Void SetColor(SNColorCode color);

	// サーフェス設定
	Void SetSurface(SNSurface* surface);

	// フェードイン
	// パラメータ：待ち時間をミリ秒指定(最大約65秒/ 但し255フレームが上限)
	Void FadeIn(UInt16 msec);

	// フェードアウト
	// パラメータ：待ち時間をミリ秒指定(最大約65秒/ 但し255フレームが上限)
	Void FadeOut(UInt16 msec);

	// アルファ値設定
	// フェード指示前の状態を指定したい場合に設定
	Void SetAlpha(UInt8 alpha);

	// 処理状態取得
	Boolean IsProcessing();

	// 完了状態取得
	Boolean IsComplete();

protected:
	// Step実行
	virtual SNTransitionCode OnStep(SNEvent* event);

	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	// フェード種別毎の描画処理
	virtual Void DrawFadeEffect(SNSurface* surface);

	// 左描画
	virtual Void DrawFadeEffectLeft(SNSurface* surface, UInt8 step, SNRect* dst_rect);

	// 右描画
	virtual Void DrawFadeEffectRight(SNSurface* surface, UInt8 step, SNRect* dst_rect);

	// 横方向中央描画
	virtual Void DrawFadeEffectCenterH(SNSurface* surface, UInt8 step, SNRect* dst_rect);

	// 上描画
	virtual Void DrawFadeEffectTop(SNSurface* surface, UInt8 step, SNRect* dst_rect);

	// 下描画
	virtual Void DrawFadeEffectBottom(SNSurface* surface, UInt8 step, SNRect* dst_rect);

	// 縦方向中央描画
	virtual Void DrawFadeEffectCenterV(SNSurface* surface, UInt8 step, SNRect* dst_rect);


	// Alpha = フェードの現在の進行度
	// FadeStep = 1フレームあたりの進行度

	UInt8 Alpha;			// アルファ値
	SNGUIFadeType FadeType;	// フェード種別
	UInt8 FadeStep;			// フェードステップ
	Boolean TriggerIn;		// フェードイントリガ
	Boolean TriggerOut;		// フェードアウトトリガ
	SNGUIFadeState FadeState;	// 状態
	SNSurface* Surface;		// サーフェス	
};

#pragma once
#include "../Include/SNFramework.h"
#include "SNDeviceContext.h"
#include "SNDDB.h"
#include "SNDIBSection.h"

// サーフェスクラス
class SNSurface
{
public:
	// コンストラクタ
	SNSurface();

	// デストラクタ
	virtual ~SNSurface();

	// サーフェス生成
	virtual Void CreateSurface(Int32 width, Int32 height) = 0;

	// サーフェス破棄
	virtual Void DeleteSurface() = 0;

	// デバイスコンテキスト取得
	SNDeviceContext* GetDC();

	// 幅取得
	virtual Int32 GetWidth() = 0;

	// 高さ取得
	virtual Int32 GetHeight() = 0;

protected:
	SNDeviceContext DeviceContext;	// デバイスコンテキスト
};

// サーフェスクラス(DDB)
class SNSurfaceDDB : public SNSurface
{
public:
	// コンストラクタ
	SNSurfaceDDB();

	// デストラクタ
	virtual ~SNSurfaceDDB();

	// サーフェス生成
	Void CreateSurface(Int32 width, Int32 height) override;

	// サーフェス破棄
	Void DeleteSurface() override;

	// 幅取得
	Int32 GetWidth() override;

	// 高さ取得
	Int32 GetHeight() override;

private:
	SNDDB DDB;	// DDB
};


// サーフェスクラス(DIB)
class SNSurfaceDIB : public SNSurface
{
public:
	// コンストラクタ
	SNSurfaceDIB();

	// デストラクタ
	virtual ~SNSurfaceDIB();

	// サーフェス生成
	Void CreateSurface(Int32 width, Int32 height) override;

	// サーフェス破棄
	Void DeleteSurface() override;

	// 幅取得
	Int32 GetWidth() override;

	// 高さ取得
	Int32 GetHeight() override;

private:
	SNDIBSection DIBSection;	// DIBSection
};

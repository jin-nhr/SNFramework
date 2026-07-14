#pragma once
#include "SNFrameworkInternal.h"
#include "SNImageCodecAccessor.h"
#include "SNThread.h"
#include "SNList.h"
#include "SNCriticalSection.h"
#include "SNMemory.h"
#include "SNDIB.h"

class SNDIB;

class SNImageCodec
{
public:
	// 初期化
	static Void Initialize();
	static Void CreateThread();

	// 終了
	static Void Terminate();
	static Void ReleaseThread();

	// スレッドメイン処理
	static Void UserMain();

	// リクエスト
	static Void Request(SNImageCodecAccessor* request);

	// リクエスト取得
	static SNImageCodecAccessor* GetRequest();

	// 画像デコード
	static Void Decode(SNMemory* in, SNDIB* out);

	// 画像エンコード
	static Void Encode(SNDIB* in, SNMemory* out);

	static Handle WICFactory;

private:
	static SNThread* CodecThread;
	static volatile Boolean ThreadEndRequest;		// スレッド終了要求

	static SNList RequestList;				// リクエストリスト
	static SNCriticalSection ListAccessCS;	// リストアクセス用クリティカルセクション

};

#include "SNUserConfig.h"
#include "SNSystemConfig.h"

// ユーザーコンフィグデータ
SNUserConfigData SNUserConfig::Data;

// コンフィグファイル
SNFile SNUserConfig::ConfigFile;

// 初期化
Void SNUserConfig::Initialize()
{
	// 初期値セット
	Data = InitValue;

	// コンフィグ読み込み
	LoadUserConfig();

	return;

}

// 終了処理
Void SNUserConfig::Terminate()
{
	// コンフィグ保存
	SaveUserConfig();

	return;
}

// コンフィグファイル読み込み
Void SNUserConfig::LoadUserConfig()
{
	SNUserConfigData* read_config;

	// コンフィグのパス、ファイル名設定
	ConfigFile.SetFolderFileName(SNSystemConfig::ConfigFileName);

	// 同期モード
	// 本クラスではStorageのスレッド起動前、終了後のファイルアクセスが必要となるため
	// 同期モードで動作させる
	ConfigFile.SetAsyncMode(false);

	// コンフィグRead
	if (ConfigFile.Read())
	{
		// Read成功時
		read_config = (SNUserConfigData*)ConfigFile.GetDataAddress();

		// 読み込みサイズNG
		if (ConfigFile.GetSize() != sizeof(SNUserConfigData))
		{
			// 初期値を採用のため何もしない
		}
		// 識別子NG
		else if ((read_config->Identifier[0] != SNSystemConfig::UserConfigIdentifier[0]) ||
				 (read_config->Identifier[1] != SNSystemConfig::UserConfigIdentifier[1]) ||
				 (read_config->Identifier[2] != SNSystemConfig::UserConfigIdentifier[2]) ||
				 (read_config->Identifier[3] != SNSystemConfig::UserConfigIdentifier[3]))
		{
			// 初期値を採用のため何もしない
		}

		// バージョン不一致
		else if (read_config->Version != SNSystemConfig::UserConfigVersion)
		{
			// 初期値を採用のため何もしない
		}

		// チェックサム確認
		else if (CalcCheckSum((Void*)read_config, ConfigFile.GetSize() - 4) != read_config->CheckSum)
		{
			// 初期値を採用のため何もしない
		}

		// 異常チェックをすべてパス
		else
		{
			// 読み込んだコンフィグを上書き
			Data = *read_config;
		}
	}

	// Read失敗
	else
	{
		// 初期値を採用のため何もしない
	}

	// いったんメモリを解放
	ConfigFile.ReleaseMemory();

	return;
}

// コンフィグファイル保存
Void SNUserConfig::SaveUserConfig()
{
	UInt32 data_size = sizeof(SNUserConfigData);

	// ユーザーコンフィグのチェックサム計算
	Data.CheckSum = CalcCheckSum((Void*)&Data, data_size - 4);

	// ファイル書き込み準備
	ConfigFile.GetMemoryObject()->Allocate(data_size);

	// データコピー
	ConfigFile.GetMemoryObject()->Copy((Void*)&Data, data_size);

	// 書き込み処理
	ConfigFile.Write();

	// メモリを解放
	ConfigFile.ReleaseMemory();

	// 書き込み結果は見ない
	// 仮にNGなら次回起動～終了時に初期化される

	return;
}

// チェックサム計算 (sizeは4バイト単位で指定)
UInt32 SNUserConfig::CalcCheckSum(Void* start, UInt32 size)
{
	UInt32* cur_address = (UInt32*)start;
	UInt32 ret = 0;
	UInt32 uint32_num = size / 4;
	UInt32 cnt;

	// startからsize分の値を足し込む
	for (cnt = 0; cnt < uint32_num; cnt++)
	{
		ret = (UInt32)(ret + *cur_address);
		cur_address++;
	}

	return ret;
}

#include "SNImageFile.h"

// コンストラクタ
SNImageFile::SNImageFile()
{
    return;
}

// デストラクタ
SNImageFile::~SNImageFile()
{
    return;
}

// デコード(非同期実行)
// SNFile::Read() で FileData に読み込まれている前提
Void SNImageFile::Decode()
{
    // SNFile が保持しているメモリオブジェクトを取得
    SNMemory* mem = GetMemoryObject();

    if (mem != nullptr)
    {
        // SNDIB の Decode を呼ぶ（非同期）
        SNDIB::Decode(mem);
    }

    return;
}

// エンコード(非同期実行)
// SNDIB の Encode 結果を SNFile::FileData に書き込む
Void SNImageFile::Encode()
{
    // SNFile が保持しているメモリオブジェクトを取得
    SNMemory* mem = GetMemoryObject();

    if (mem != nullptr)
    {
        // SNDIB の Encode を呼ぶ（非同期）
        SNDIB::Encode(mem);
    }

    return;
}

// 処理完了？
Boolean SNImageFile::IsOperationComplete()
{
    Boolean ret = true;
    ret &= SNFile::IsOperationComplete();
    ret &= SNDIB::IsOperationComplete();

    return ret;
}

// 処理完了待ち
Void SNImageFile::WaitForOperationComplete()
{
    // SNFile 側の非同期処理待ち
    SNFile::WaitForOperationComplete();

    // SNDIB 側の非同期処理待ち
    SNDIB::WaitForOperationComplete();

    return;
}

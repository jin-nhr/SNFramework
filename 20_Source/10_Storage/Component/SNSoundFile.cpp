#include "SNSoundFile.h"

// コンストラクタ
SNSoundFile::SNSoundFile()
{
    return;
}

// デストラクタ
SNSoundFile::~SNSoundFile()
{
    return;
}

// デコード(非同期実行)
// SNFile::Read() で FileData に読み込まれている前提
Void SNSoundFile::Decode()
{
    // SNFile が保持しているメモリオブジェクトを取得
    SNMemory* mem = GetMemoryObject();

    if (mem != nullptr)
    {
        // SNPCM の Decode を呼ぶ（非同期）
        SNPCM::Decode(mem);
    }

    return;
}



// 処理完了？
Boolean SNSoundFile::IsOperationComplete()
{
    Boolean ret = true;
    ret &= SNFile::IsOperationComplete();
    ret &= SNPCM::IsOperationComplete();

    return ret;
}

// 処理完了待ち
Void SNSoundFile::WaitForOperationComplete()
{
    // SNFile 側の非同期処理待ち
    SNFile::WaitForOperationComplete();

    // SNDIB 側の非同期処理待ち
    SNPCM::WaitForOperationComplete();

    return;
}

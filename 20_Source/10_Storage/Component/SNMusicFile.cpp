#include "SNMusicFile.h"

// コンストラクタ
SNMusicFile::SNMusicFile()
{
    return;
}

// デストラクタ
SNMusicFile::~SNMusicFile()
{
    return;
}

// ストリームオープン(非同期実行)
// SNFile::Read() で FileData に読み込まれている前提
Void SNMusicFile::OpenPCMStream()
{
    // SNFile が保持しているメモリオブジェクトを取得
    SNMemory* mem = GetMemoryObject();

    if (mem != nullptr)
    {
        // SNPCMStream の OpenPCMStream を呼ぶ（非同期）
        SNPCMStream::OpenPCMStream(mem);
    }

    return;
}



// 処理完了？
Boolean SNMusicFile::IsOperationComplete()
{
    Boolean ret = true;
    ret &= SNFile::IsOperationComplete();
    ret &= SNPCMStream::IsOperationComplete();

    return ret;
}

// 処理完了待ち
Void SNMusicFile::WaitForOperationComplete()
{
    // SNFile 側の非同期処理待ち
    SNFile::WaitForOperationComplete();

    // SNDIB 側の非同期処理待ち
    SNPCMStream::WaitForOperationComplete();

    return;
}

#include "SNImageCodec.h"
#include "SNWindowsAPI.h"
#include "SNImageCodecThread.h"
#include "SNAutoResource.h"
#include "SNSystemConfig.h"
#include "SNGraphicsDevice.h"

Handle SNImageCodec::WICFactory = nullptr;
SNThread* SNImageCodec::CodecThread;
volatile Boolean SNImageCodec::ThreadEndRequest = false;		// スレッド終了要求
SNList SNImageCodec::RequestList;				// リクエストリスト
SNCriticalSection SNImageCodec::ListAccessCS;	// リストアクセス用クリティカルセクション



Void SNImageCodec::Initialize()
{
	ListAccessCS.Initialize();
	RequestList.Allocate(SNSystemConfig::CodecRequestMax);

    CreateWIC();
    CreateThread();

    return;
}

Void SNImageCodec::CreateWIC()
{
    IWICImagingFactory* wic = nullptr;
    CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wic)
    );
    WICFactory = wic;

    return;
}

Void SNImageCodec::CreateThread()
{
    ReleaseThread();
    CodecThread = new SNImageCodecThread();
	CodecThread->Run();
    return;
}


Void SNImageCodec::Terminate()
{
    ReleaseThread();
    ReleaseWIC();

    return;
}

Void SNImageCodec::ReleaseWIC()
{
    if (WICFactory)
    {
        ((IWICImagingFactory*)WICFactory)->Release();
        WICFactory = nullptr;
    }

    return;
}

Void SNImageCodec::ReleaseThread()
{
    if (CodecThread != nullptr)
    {
		// 終了要求セット
		ThreadEndRequest = true;
		
		// 終了待ち(MAX1秒)
		CodecThread->WaitForThreadEnd();

        delete CodecThread;
        CodecThread = nullptr;
    }

    return;
}

// ストレージスレッドメイン
Void SNImageCodec::UserMain()
{
	SNImageCodecAccessor* request;

	// ストレージスレッドメインループ
	while (true)
	{
		// 終了要求がセットされたら抜ける
		if (ThreadEndRequest)
		{
			break;
		}

		// リクエスト取得
		request = GetRequest();
		if (request != nullptr)
		{
			request->Callback();
		}

		// 処理後、いったんCPU解放
		::Sleep(1);
	}

	return;
}

// リクエスト
Void SNImageCodec::Request(SNImageCodecAccessor* request)
{
	SNListContainer* list;

	{
		SNAutoResource cs(&ListAccessCS);

		// リストの最後にリクエストを追加
		list = RequestList.InsertLast();
		if (list != nullptr)
		{
			list->UserData = (Void*)request;
		}
	}

	return;
}

// リクエスト取得
SNImageCodecAccessor* SNImageCodec::GetRequest()
{
	SNListContainer* list;
	SNImageCodecAccessor* ret = nullptr;

	{
		SNAutoResource cs(&ListAccessCS);

		// リスト先頭からリクエストを取得しリスト削除
		list = RequestList.GetTop();

		if (list != nullptr)
		{
			ret = (SNImageCodecAccessor*)list->UserData;
			RequestList.RemoveTop();
		}
	}

	return ret;
}

Void SNImageCodec::Decode(SNMemory* in, SNDIB* out)
{
    IWICImagingFactory* factory;
    IWICStream* stream;
    IWICBitmapDecoder* decoder;
    IWICBitmapFrameDecode* frame;
    IWICFormatConverter* converter;
    SNSize snsize;
    SNDIBPixel* pix;
    UINT w, h;

    factory = (IWICImagingFactory*)(WICFactory);

    // ストリーム生成
    factory->CreateStream(&stream);

    // ストリームをinで初期化
    stream->InitializeFromMemory(
        (WICInProcPointer)in->GetAddress(),
        in->GetSize());

    // デコーダー生成
    factory->CreateDecoderFromStream(
        stream,
        nullptr,
        WICDecodeMetadataCacheOnDemand,
        &decoder);

    // フレーム取得
    decoder->GetFrame(0, &frame);
    frame->GetSize(&w, &h);

    // DIB生成
    snsize.Width = w;
    snsize.Height = h;
    out->CreateDIB(&snsize);

    // コンバーター生成
    factory->CreateFormatConverter(&converter);
    converter->Initialize(
        frame,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeCustom);

    // ピクセルロックしピクセルデータをコピーする
    pix = out->GetPixel();
    converter->CopyPixels(nullptr, pix->GetStride(), pix->GetDataSize(), (BYTE*)pix->Ref(0, 0));
    out->ReleasePixel(pix);

    converter->Release();
    frame->Release();
    decoder->Release();
    stream->Release();

    return;
}

Void SNImageCodec::Encode(SNDIB* in, SNMemory* out)
{
    IWICImagingFactory* factory;
    IWICBitmap* wicbmp;
    IWICBitmapEncoder* encoder;
    IWICBitmapFrameEncode* frame;
    IWICFormatConverter* conv;
    IStream* stream;

    STATSTG stat;
    SNSize size;
    ULONG png_size;
    ULONG read_size;
    LARGE_INTEGER pos;

    factory = (IWICImagingFactory*)(WICFactory);

    // SNDIB が保持している WICBitmap を取得
    wicbmp = (IWICBitmap*)in->GetBitmap();
    in->GetSize(&size);

    // 書き込み可能な IStream を作成（必須）
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, 0);
    CreateStreamOnHGlobal(hMem, TRUE, &stream);

    // PBGRA → BGRA の変換コンバータ
    factory->CreateFormatConverter(&conv);
    conv->Initialize(
        wicbmp,
        GUID_WICPixelFormat32bppBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeCustom);

    // PNG エンコーダ生成
    factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);

    encoder->Initialize(stream, WICBitmapEncoderNoCache);

    // フレーム生成
    encoder->CreateNewFrame(&frame, nullptr);

    frame->Initialize(nullptr);

    // サイズ設定
    frame->SetSize(size.Width, size.Height);

    // conv を WriteSource に渡す
    frame->WriteSource(conv, nullptr);

    // コミット
    frame->Commit();
    encoder->Commit();

    // ストリームから PNG データを取り出す
    stream->Stat(&stat, STATFLAG_NONAME);

    png_size = (ULONG)stat.cbSize.QuadPart;
    out->Allocate(png_size);

    pos.QuadPart = 0;
    stream->Seek(pos, STREAM_SEEK_SET, nullptr);

    stream->Read(out->GetAddress(), png_size, &read_size);

    // 解放
    conv->Release();
    frame->Release();
    encoder->Release();
    stream->Release();

    return;
}

#pragma once
#include "SNFrameworkInternal.h"

class SNFixedString
{
public:
	static constexpr String Startup1 = (String)L"SNFramework System";
	static constexpr String Startup2 = (String)L"Version.%08x";
	static constexpr String Startup3_1 = (String)L"Starting";
	static constexpr String Startup3_2 = (String)L"Starting.";
	static constexpr String Startup3_3 = (String)L"Starting..";
	static constexpr String Startup3_4 = (String)L"Starting...";
	static constexpr String Startup4_1 = (String)L"Starting... Success!";
	static constexpr String Startup4_2 = (String)L"Starting... Error! [Duplicate Launch]";
	static constexpr String Startup4_3 = (String)L"Starting... Error! [Initialization Failure]";
	static constexpr String Startup5 = (String)L"Startup failed. Shutting down.";

	static constexpr String PreCloseMsg = (String)L"しゅうりょうしますか";
	static constexpr String PreCloseYes = (String)L"はい";
	static constexpr String PreCloseNo = (String)L"いいえ";

	static constexpr String TitleMenuNew = (String)L"はじめから";
	static constexpr String TitleMenuCont = (String)L"つづきから";
	static constexpr String TitleMenuConfig = (String)L"システムせってい";
	static constexpr String TitleMenuExit = (String)L"しゅうりょう";

	static constexpr String ConfigTitle = (String)L"[システムせってい]";
	static constexpr String ConfigScrMode = (String)L"がめんモード";
	static constexpr String ConfigSYNC = (String)L"VSYNC";
	static constexpr String ConfigFilter = (String)L"びょうがフィルタ";
	static constexpr String ConfigMstVol = (String)L"マスターおんりょう";
	static constexpr String ConfigBGMVol = (String)L"BGMおんりょう";
	static constexpr String ConfigSEVol = (String)L"SEおんりょう";
	static constexpr String ConfigP1 = (String)L"プレイヤー1にゅうりょく";
	static constexpr String ConfigP2 = (String)L"プレイヤー2にゅうりょく";

	static constexpr String ConfigScrModeWin = (String)L"ウインドウ";
	static constexpr String ConfigScrModeFull = (String)L"フルスクリーン";
	static constexpr String ConfigOFF = (String)L"OFF";
	static constexpr String ConfigON = (String)L"ON";
	static constexpr String ConfigVolDown10 = (String)L"-10";
	static constexpr String ConfigVolDown1 = (String)L"-1";
	static constexpr String ConfigVolUp10 = (String)L"+10";
	static constexpr String ConfigVolUp1 = (String)L"+1";
	static constexpr String ConfigPadMap = (String)L"ゲームパッドわりあて";
	static constexpr String ConfigKeyboardMap = (String)L"キーボードわりあて";
	static constexpr String ConfigInit = (String)L"デフォルトせっていにもどす";
	static constexpr String ConfigOK = (String)L"OK";
	static constexpr String ConfigCancel = (String)L"キャンセル";


};

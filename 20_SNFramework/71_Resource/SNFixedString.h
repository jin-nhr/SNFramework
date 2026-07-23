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

	static constexpr String PreClose1 = (String)L"‚µ‚ã‚¤‚è‚å‚¤‚µ‚Ü‚·‚©";
	static constexpr String PreClose2 = (String)L"‚Í‚¢";
	static constexpr String PreClose3 = (String)L"‚¢‚¢‚¦";



};

#pragma once
#include "SNFrameworkInternal.h"

class SNScaling
{
public:
	static Void LetterBox(SNRect* src_rect, SNRect* dst_rect);

	static Void Fit(SNRect* src_rect, SNRect* dst_rect);

	static Void FitV(SNRect* src_rect, SNRect* dst_rect);

	static Void FitH(SNRect* src_rect, SNRect* dst_rect);

};

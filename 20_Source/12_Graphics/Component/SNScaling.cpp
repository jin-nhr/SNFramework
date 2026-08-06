#include "SNScaling.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

Void SNScaling::LetterBox(SNRect* src_rect, SNRect* dst_rect)
{
	UInt8 draw_align = SNSystemConfig::DrawAlign;

	// アスペクト比を維持したスケール計算
	Float32 scale_x = (Float32)dst_rect->Width / (Float32)src_rect->Width;
	Float32 scale_y = (Float32)dst_rect->Height / (Float32)src_rect->Height;
	Float32 scale = SNMath::SelectMinF(scale_x, scale_y);  // アスペクト比を維持

	Int32 new_width = (Int32)(src_rect->Width * scale);
	Int32 new_height = (Int32)(src_rect->Height * scale);

	// 4の倍数に補正
	new_width = (new_width / draw_align) * draw_align;
	new_height = (new_height / draw_align) * draw_align;

	// 描画領域のオフセット計算（中央配置）
	Int32 offset_x = (dst_rect->Width - new_width) / 2;
	Int32 offset_y = (dst_rect->Height - new_height) / 2;

	// 描画範囲を更新
	dst_rect->PointX = dst_rect->PointX + offset_x;
	dst_rect->PointY = dst_rect->PointY + offset_y;
	dst_rect->Width = new_width;
	dst_rect->Height = new_height;

	return;
}

Void SNScaling::Fit(SNRect* src_rect, SNRect* dst_rect)
{
	UInt8 draw_align = SNSystemConfig::DrawAlign;

	// アスペクト比を維持したスケール計算
	Float32 scale_x = (Float32)dst_rect->Width / (Float32)src_rect->Width;
	Float32 scale_y = (Float32)dst_rect->Height / (Float32)src_rect->Height;
	Float32 scale = SNMath::SelectMaxF(scale_x, scale_y);  // アスペクト比を維持

	Int32 new_width = (Int32)(src_rect->Width * scale);
	Int32 new_height = (Int32)(src_rect->Height * scale);

	// 4の倍数に補正
	new_width = (new_width / draw_align) * draw_align;
	new_height = (new_height / draw_align) * draw_align;

	// 描画領域のオフセット計算（中央配置）
	Int32 offset_x = (new_width - dst_rect->Width) / 2;
	Int32 offset_y = (new_height - dst_rect->Height) / 2;

	// 描画範囲を更新
	src_rect->PointX = src_rect->PointX + offset_x;
	src_rect->PointY = src_rect->PointY + offset_y;
	src_rect->Width = new_width - (offset_x * 2);
	src_rect->Height = new_height - (offset_y * 2);

	return;
}

Void SNScaling::FitV(SNRect* src_rect, SNRect* dst_rect)
{
	Float32 scale_x = (Float32)dst_rect->Width / (Float32)src_rect->Width;
	Float32 scale_y = (Float32)dst_rect->Height / (Float32)src_rect->Height;

	// 縦が大きい
	if (scale_x < scale_y)
	{
		LetterBox(src_rect, dst_rect);
	}

	else
	{
		Fit(src_rect, dst_rect);
	}

	return;
}

Void SNScaling::FitH(SNRect* src_rect, SNRect* dst_rect)
{
	Float32 scale_x = (Float32)dst_rect->Width / (Float32)src_rect->Width;
	Float32 scale_y = (Float32)dst_rect->Height / (Float32)src_rect->Height;

	// 横が大きい
	if (scale_x > scale_y)
	{
		LetterBox(src_rect, dst_rect);
	}

	else
	{
		Fit(src_rect, dst_rect);
	}

	return;
}

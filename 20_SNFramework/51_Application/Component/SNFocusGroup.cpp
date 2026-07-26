#include "SNFocusGroup.h"
#include "SNFocus.h"
#include "SNMath.h"

SNFocusGroup::SNFocusGroup()
{
	FocusLayer = SNFocusLayerSysApp;

	ActiveButton = nullptr;
	LastActive = nullptr;
	
	LoopV = false;
	LoopH = false;

	DecideSts = false;
	CancelSts = false;

	return;
}

SNFocusGroup::~SNFocusGroup()
{
	return;
}

Void SNFocusGroup::StartRegister(SNFocusLayer layer, Int32 num, Boolean loop_v, Boolean loop_h)
{
	ClearButton();

	FocusLayer = layer;
	LoopV = loop_v;
	LoopH = loop_h;

	TargetList.Allocate(num);

	return;
}
Void SNFocusGroup::RegisterButton(SNGUIButton* button)
{
	SNListContainer* it;

	it = TargetList.InsertLast();

	if (it != nullptr)
	{
		it->UserData = (Void*)button;
	}

	return;
}

Void SNFocusGroup::EndRegister()
{
	TargetList.Restore();

	return;
}

Void SNFocusGroup::ClearButton()
{
	TargetList.Free();
	ActiveButton = nullptr;

	FocusLayer = SNFocusLayerSysApp;

	LoopV = false;
	LoopH = false;

	ClearButtonSts();

	return;
}

Void SNFocusGroup::Entry()
{
	SNPoint point;
	SNRect rect;

	if (FocusLayer == SNFocusLayerSysApp)
	{
		SNFocus::RegisterSysApp(this);
	}
	else
	{
		SNFocus::RegisterUserApp(this);
	}

	if (LastActive == nullptr)
	{
		point.X = 0;
		point.Y = 0;
	}

	else
	{
		rect = LastActive->CalcGlobalRect();
		point.X = rect.PointX + rect.Width / 2;
		point.Y = rect.PointY + rect.Height / 2;
	}

	SelectNearest(&point);

	return;
}


Void SNFocusGroup::Exit()
{
	if (FocusLayer == SNFocusLayerSysApp)
	{
		SNFocus::ReleaseSysApp(this);
	}
	else
	{
		SNFocus::ReleaseUserApp(this);
	}

	// アクティブを保存
	LastActive = ActiveButton;

	FocusChange(nullptr);

	return;
}

Void SNFocusGroup::MovePoint(SNPoint* pnt)
{
	// アクティブ外に移動
	if (!ActiveButton->CheckHover(pnt))
	{
		ActiveButton->Push = false;
		DecideSts = false;
		CancelSts = false;
	}

	SelectHover(pnt);
	return;
}

Void SNFocusGroup::DirUp()
{
	Int32 offset_x = 0;
	Int32 offset_y = 0;

	if (LoopV)
	{
		offset_y = (Int32)SNSystemConfig::ScreenHeight * -1;
	}

	SelectDir(96, 32, offset_x, offset_y);
	return;
}

Void SNFocusGroup::DirDown()
{
	Int32 offset_x = 0;
	Int32 offset_y = 0;

	if (LoopV)
	{
		offset_y = SNSystemConfig::ScreenHeight;
	}

	SelectDir(-32, -96, offset_x, offset_y);
	return;
}

Void SNFocusGroup::DirLeft()
{
	Int32 offset_x = 0;
	Int32 offset_y = 0;

	if (LoopH)
	{
		offset_x = (Int32)SNSystemConfig::ScreenWidth * -1;
	}

	SelectDir(127, 96, offset_x, offset_y);
	SelectDir(-96, -128, offset_x, offset_y);
	return;
}

Void SNFocusGroup::DirRight()
{
	Int32 offset_x = 0;
	Int32 offset_y = 0;

	if (LoopH)
	{
		offset_x = SNSystemConfig::ScreenWidth;
	}

	SelectDir(32, -32, offset_x, offset_y);
	return;
}


Void SNFocusGroup::SelectNearest(SNPoint* pnt)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();
	Int cnt;
	Int64 dist = 0x7FFFFFFFFFFFFFFF;
	Int64 tmp_dist;
	SNPoint pnt2;
	SNRect rect;

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			rect = it_btn->CalcGlobalRect();

			pnt2.X = rect.PointX + rect.Width / 2;
			pnt2.Y = rect.PointY + rect.Height / 2;

			tmp_dist = SNMath::CalcDist2(pnt, &pnt2);

			// より近いボタンを発見
			if (tmp_dist < dist)
			{
				dist = tmp_dist;
				tmp_btn = it_btn;
			}
		}
	}

	// 候補発見時
	if (tmp_btn != nullptr)
	{
		FocusChange(tmp_btn);
	}

	return;
}

Void SNFocusGroup::SelectHover(SNPoint* pnt)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();
	Int cnt;

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			if (it_btn->CheckHover(pnt))
			{
				tmp_btn = it_btn;
				break;
			}
		}
	}

	// 候補発見時
	if (tmp_btn != nullptr)
	{
		FocusChange(tmp_btn);
	}

	return;
}

Void SNFocusGroup::SelectDir(Int8 angle_top, Int8 angle_bottom, Int32 offset_x, Int32 offset_y)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();
	Int cnt;
	Int64 dist = 0x7FFFFFFFFFFFFFFF;
	Int64 tmp_dist;
	Int8 angle = 0;
	SNPoint pnt = {0, 0}, pnt2;
	SNRect rect;

	if (ActiveButton != nullptr)
	{
		rect = ActiveButton->CalcGlobalRect();

		pnt.X = rect.PointX + rect.Width / 2;
		pnt.Y = rect.PointY + rect.Height / 2;
	}

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			rect = it_btn->CalcGlobalRect();

			pnt2.X = rect.PointX + rect.Width / 2;
			pnt2.Y = rect.PointY + rect.Height / 2;

			tmp_dist = SNMath::CalcDist2(&pnt, &pnt2);
			angle = SNMath::CalcAngle(&pnt, &pnt2);

			// より近いボタンを発見
			if ((tmp_dist < dist) && 
				(angle_bottom <= angle) && (angle <= angle_top))
			{
				dist = tmp_dist;
				tmp_btn = it_btn;
			}

			// オフセット指定ありのときはループ分の追加処理
			if ((offset_x != 0) || (offset_y != 0))
			{
				pnt2.X += offset_x;
				pnt2.Y += offset_y;

				tmp_dist = SNMath::CalcDist2(&pnt, &pnt2);
				angle = SNMath::CalcAngle(&pnt, &pnt2);

				// より近いボタンを発見
				if ((tmp_dist < dist) &&
					(angle_bottom <= angle) && (angle <= angle_top))
				{
					dist = tmp_dist;
					tmp_btn = it_btn;
				}
			}
		}
	}

	// 候補発見時
	if (tmp_btn != nullptr)
	{
		FocusChange(tmp_btn);
	}

	return;
}

Void SNFocusGroup::FocusChange(SNGUIButton* next)
{
	// アクティブ変更時のみ処理
	if (next != ActiveButton)
	{
		if (ActiveButton != nullptr)
		{
			ActiveButton->Focus = false;
			ActiveButton->Push = false;
			ActiveButton = nullptr;
		}

		if (next != nullptr)
		{
			next->Focus = true;
			ActiveButton = next;
		}

		ClearButtonSts();
	}

	return;
}

Boolean SNFocusGroup::JudgeActDecide(SNGUIButton* btn)
{
	Boolean ret = false;

	if (ActiveButton == btn)
	{
		ret = DecideSts;
	}

	return ret;
}

Boolean SNFocusGroup::JudgeActCancel()
{
	return CancelSts;
}

Void SNFocusGroup::PushDecide()
{
	if (ActiveButton != nullptr)
	{
		ActiveButton->Push = true;
	}

	return;
}

Void SNFocusGroup::PushCancel()
{
	CancelSts = true;

	return;
}

Void SNFocusGroup::PushPointingDecide(SNPoint* pnt)
{
	if (ActiveButton != nullptr)
	{
		// アクティブ上
		if (ActiveButton->CheckHover(pnt))
		{
			PushDecide();
		}
	}

	return;
}

Void SNFocusGroup::ReleaseDecide()
{
	if (ActiveButton != nullptr)
	{
		if (ActiveButton->Push)
		{
			ActiveButton->Push = false;
			DecideSts = true;
		}
	}

	return;
}

Void SNFocusGroup::ClearButtonSts()
{
	DecideSts = false;
	CancelSts = false;

	return;
}
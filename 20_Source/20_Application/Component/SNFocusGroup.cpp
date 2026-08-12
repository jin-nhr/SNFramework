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
		point.X = rect.PointX;
		point.Y = rect.PointY;
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
	SelectHover(pnt);
	return;
}

Void SNFocusGroup::DirUp()
{
	SelectUp(LoopV);
	return;
}

Void SNFocusGroup::DirDown()
{
	SelectDown(LoopV);
	return;
}

Void SNFocusGroup::DirLeft()
{
	SelectLeft(LoopH);
	return;
}

Void SNFocusGroup::DirRight()
{
	SelectRight(LoopH);
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

			pnt2.X = rect.PointX;
			pnt2.Y = rect.PointY;

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

Void SNFocusGroup::SelectUp(Boolean loop)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();

	SNRect active_rect = { 0 };
	SNRect rect;
	SNRect keep_rect = { 0 };
	Int32 cnt;
	Boolean cond_dir = false;
	Boolean cond_col = false;
	Boolean cond_pos = false;

	if (ActiveButton != nullptr)
	{
		active_rect = ActiveButton->CalcGlobalRect();
	}

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			rect = it_btn->CalcGlobalRect();

			// 上方かどうか (ループ指定時は常に上方と判断)
			cond_dir = ((active_rect.PointY > rect.PointY) || loop);

			// Y軸方向に重なっているか
			cond_col = ((active_rect.PointX < rect.PointX + rect.Width) &&
				        (rect.PointX < active_rect.PointX + active_rect.Width));

			// より近いもの、左のものを選択する
			cond_pos = ((tmp_btn == nullptr) ||
						((keep_rect.PointY < rect.PointY) ||
						 ((keep_rect.PointY == rect.PointY) && (keep_rect.PointX > rect.PointX))));

			if (cond_dir && cond_col && cond_pos)
			{
				keep_rect = rect;
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


Void SNFocusGroup::SelectDown(Boolean loop)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();

	SNRect active_rect = { 0 };
	SNRect rect;
	SNRect keep_rect = { 0 };
	Int32 cnt;
	Boolean cond_dir = false;
	Boolean cond_col = false;
	Boolean cond_pos = false;

	if (ActiveButton != nullptr)
	{
		active_rect = ActiveButton->CalcGlobalRect();
	}

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			rect = it_btn->CalcGlobalRect();

			// 下方かどうか (ループ指定時は常に上方と判断)
			cond_dir = ((active_rect.PointY < rect.PointY) || loop);

			// Y軸方向に重なっているか
			cond_col = ((active_rect.PointX < rect.PointX + rect.Width) &&
				(rect.PointX < active_rect.PointX + active_rect.Width));

			// より近いもの、左のものを選択する
			cond_pos = ((tmp_btn == nullptr) ||
						((keep_rect.PointY > rect.PointY) ||
						 ((keep_rect.PointY == rect.PointY) && (keep_rect.PointX > rect.PointX))));

			if (cond_dir && cond_col && cond_pos)
			{
				keep_rect = rect;
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


Void SNFocusGroup::SelectLeft(Boolean loop)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();

	SNRect active_rect = { 0 };
	SNRect rect;
	SNRect keep_rect = { 0 };
	Int32 cnt;
	Boolean cond_dir = false;
	Boolean cond_col = false;
	Boolean cond_pos = false;

	if (ActiveButton != nullptr)
	{
		active_rect = ActiveButton->CalcGlobalRect();
	}

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			rect = it_btn->CalcGlobalRect();

			// 左方かどうか (ループ指定時は常に上方と判断)
			cond_dir = ((active_rect.PointX > rect.PointX) || loop);

			// Y軸方向に重なっているか
			cond_col = ((active_rect.PointY < rect.PointY + rect.Height) &&
				(rect.PointY < active_rect.PointY + active_rect.Height));

			// より近いもの、左のものを選択する
			cond_pos = ((tmp_btn == nullptr) ||
						((keep_rect.PointX < rect.PointX) ||
						 ((keep_rect.PointX == rect.PointX) && (keep_rect.PointY > rect.PointY))));

			if (cond_dir && cond_col && cond_pos)
			{
				keep_rect = rect;
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


Void SNFocusGroup::SelectRight(Boolean loop)
{
	SNListContainer* it;
	SNGUIButton* it_btn;
	SNGUIButton* tmp_btn = nullptr;
	Int list_num = TargetList.GetNum();

	SNRect active_rect = { 0 };
	SNRect rect;
	SNRect keep_rect = { 0 };
	Int32 cnt;
	Boolean cond_dir = false;
	Boolean cond_col = false;
	Boolean cond_pos = false;

	if (ActiveButton != nullptr)
	{
		active_rect = ActiveButton->CalcGlobalRect();
	}

	for (cnt = 0; cnt < list_num; cnt++)
	{
		it = TargetList.DirectAccess(cnt);
		it_btn = (SNGUIButton*)it->UserData;

		// アクティブではないボタンを対象に処理する
		if (ActiveButton != it_btn)
		{
			rect = it_btn->CalcGlobalRect();

			// 右方かどうか (ループ指定時は常に上方と判断)
			cond_dir = ((active_rect.PointX < rect.PointX) || loop);

			// Y軸方向に重なっているか
			cond_col = ((active_rect.PointY < rect.PointY + rect.Height) &&
				(rect.PointY < active_rect.PointY + active_rect.Height));

			// より近いもの、左のものを選択する
			cond_pos = ((tmp_btn == nullptr) ||
				((keep_rect.PointX > rect.PointX) ||
					((keep_rect.PointX == rect.PointX) && (keep_rect.PointY > rect.PointY))));

			if (cond_dir && cond_col && cond_pos)
			{
				keep_rect = rect;
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

SNGUIButton* SNFocusGroup::GetActiveButton()
{
	return ActiveButton;
}

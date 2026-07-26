#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"
#include "SNGUIButton.h"

class SNGUIButton;

class SNFocusGroup
{
public:
	SNFocusGroup();
	virtual ~SNFocusGroup();

	virtual Void StartRegister(SNFocusLayer layer, Int32 num, Boolean loop_v, Boolean loop_h);
	virtual Void RegisterButton(SNGUIButton* button);
	virtual Void EndRegister();
	virtual Void ClearButton();

	virtual Void Entry();
	virtual Void Exit();

	virtual Void MovePoint(SNPoint* pnt);
	virtual Void DirUp();
	virtual Void DirDown();
	virtual Void DirLeft();
	virtual Void DirRight();

	virtual Void SelectNearest(SNPoint* pnt);
	virtual Void SelectHover(SNPoint* pnt);
	virtual Void SelectDir(Int8 angle_top, Int8 angle_bottom, Int32 offset_x, Int32 offset_y);

	virtual Void FocusChange(SNGUIButton* next);

	virtual Boolean JudgeActDecide(SNGUIButton* btn);
	virtual Boolean JudgeActCancel();

	virtual Void PushDecide();
	virtual Void PushCancel();

	virtual Void PushPointingDecide(SNPoint* pnt);
	virtual Void ReleaseDecide();	// ボタン反転の解除を行う ボタンの離しとは同期しない点に注意
	virtual Void ClearButtonSts();

protected:
	SNFocusLayer FocusLayer;
	Boolean LoopV;	// 縦方向ループ
	Boolean LoopH;	// 横方向ループ

	SNList TargetList;
	SNGUIButton* ActiveButton;
	SNGUIButton* LastActive;
	Boolean DecideSts;
	Boolean CancelSts;
};

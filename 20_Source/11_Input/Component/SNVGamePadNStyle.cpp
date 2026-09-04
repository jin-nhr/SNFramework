#include "SNVGamePadNStyle.h"
#include "SNVirtualGamePad.h"

SNVGamePadNStyle::SNVGamePadNStyle()
{
	ID = SNVirtualGamePadID1;
	return;
}
SNVGamePadNStyle::~SNVGamePadNStyle()
{
	return;
}

Void SNVGamePadNStyle::SetID(SNVirtualGamePadID id)
{
	ID = id;
	return;
}

// Up
Boolean SNVGamePadNStyle::DPadUpPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadUp][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadUpPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadUp][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadUpRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadUpLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadUpRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadUp][SNVirtualGamePadEventRelease];
}

// Down
Boolean SNVGamePadNStyle::DPadDownPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDown][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadDownPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDown][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadDownRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadDownLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadDownRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDown][SNVirtualGamePadEventRelease];
}

// Left
Boolean SNVGamePadNStyle::DPadLeftPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLeft][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadLeftPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLeft][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadLeftRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadLeftLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadLeftRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLeft][SNVirtualGamePadEventRelease];
}

// Right
Boolean SNVGamePadNStyle::DPadRightPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRight][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadRightPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRight][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadRightRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadRightLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadRightRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRight][SNVirtualGamePadEventRelease];
}

// A
Boolean SNVGamePadNStyle::APush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDecide][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::APress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDecide][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::ARepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDecide][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::APadALong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDecide][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::APadARelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadDecide][SNVirtualGamePadEventRelease];
}

// B
Boolean SNVGamePadNStyle::BPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadCancel][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::BPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadCancel][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::BRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadCancel][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::BLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadCancel][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::BRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadCancel][SNVirtualGamePadEventRelease];
}

// X
Boolean SNVGamePadNStyle::XPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadMenu][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::XPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadMenu][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::XRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadMenu][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::XLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadMenu][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::XRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadMenu][SNVirtualGamePadEventRelease];
}

// Y
Boolean SNVGamePadNStyle::YPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadAction][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::YPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadAction][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::YRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadAction][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::YLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadAction][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::YRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadAction][SNVirtualGamePadEventRelease];
}

// Select
Boolean SNVGamePadNStyle::SelectPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadSelect][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::SelectPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadSelect][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::SelectRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadSelect][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::SelectLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadSelect][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::SelectRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadSelect][SNVirtualGamePadEventRelease];
}

// Start
Boolean SNVGamePadNStyle::StartPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadStart][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::StartPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadStart][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::StartRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadStart][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::StartLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadStart][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::StartRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadStart][SNVirtualGamePadEventRelease];
}

// L
Boolean SNVGamePadNStyle::L1Push()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPagePrev][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::L1Press()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPagePrev][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::L1Repeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPagePrev][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::L1Long()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPagePrev][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::L1Release()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPagePrev][SNVirtualGamePadEventRelease];
}

// R
Boolean SNVGamePadNStyle::R1Push()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPageNext][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::R1Press()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPageNext][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::R1Repeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPageNext][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::R1Long()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPageNext][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::R1Release()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadPageNext][SNVirtualGamePadEventRelease];
}

// L2
Boolean SNVGamePadNStyle::L2Push()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerL][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::L2Press()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerL][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::L2Repeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerL][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::L2Long()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerL][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::L2Release()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerL][SNVirtualGamePadEventRelease];
}

// R2
Boolean SNVGamePadNStyle::R2Push()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerR][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::R2Press()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerR][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::R2Repeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerR][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::R2Long()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerR][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::R2Release()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadTriggerR][SNVirtualGamePadEventRelease];
}

// L3
Boolean SNVGamePadNStyle::L3Push()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadL3][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::L3Press()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadL3][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::L3Repeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadL3][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::L3Long()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadL3][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::L3Release()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadL3][SNVirtualGamePadEventRelease];
}

// R3
Boolean SNVGamePadNStyle::R3Push()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadR3][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::R3Press()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadR3][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::R3Repeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadR3][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::R3Long()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadR3][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::R3Release()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadR3][SNVirtualGamePadEventRelease];
}

// L Stick Up
Boolean SNVGamePadNStyle::LStkUpPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListUp][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkUpPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListUp][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkUpRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkUpLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkUpRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListUp][SNVirtualGamePadEventRelease];
}

// L Stick Down
Boolean SNVGamePadNStyle::LStkDownpPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListDown][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkDownpPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListDown][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkDownpRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkDownpLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkDownpRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadListDown][SNVirtualGamePadEventRelease];
}

// L Stick Left
Boolean SNVGamePadNStyle::LStkLeftPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkLeft][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkLeftPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkLeft][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkLeftRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkLeftLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkLeftRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkLeft][SNVirtualGamePadEventRelease];
}

// L Stick Right
Boolean SNVGamePadNStyle::LStkRightPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkRight][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkRightPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkRight][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkRightRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkRightLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkRightRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadLStkRight][SNVirtualGamePadEventRelease];
}

// R Stick Up
Boolean SNVGamePadNStyle::RStkUpPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkUp][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkUpPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkUp][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkUpRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkUpLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkUpRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkUp][SNVirtualGamePadEventRelease];
}

// R Stick Down
Boolean SNVGamePadNStyle::RStkDownpPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkDown][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkDownpPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkDown][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkDownpRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkDownpLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkDownpRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkDown][SNVirtualGamePadEventRelease];
}

// R Stick Left
Boolean SNVGamePadNStyle::RStkLeftPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkLeft][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkLeftPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkLeft][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkLeftRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkLeftLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkLeftRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkLeft][SNVirtualGamePadEventRelease];
}

// R Stick Right
Boolean SNVGamePadNStyle::RStkRightPush()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkRight][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkRightPress()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkRight][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkRightRepeat()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkRightLong()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkRightRelease()
{
	return SNVirtualGamePad::Event[ID][SNVirtualGamePadRStkRight][SNVirtualGamePadEventRelease];
}


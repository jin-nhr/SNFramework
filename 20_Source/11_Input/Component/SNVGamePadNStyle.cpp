#include "SNVGamePadNStyle.h"
#include "SNVirtualGamePad.h"

SNVGamePadNStyle::SNVGamePadNStyle()
{
	return;
}
SNVGamePadNStyle::~SNVGamePadNStyle()
{
	return;
}

// Up
Boolean SNVGamePadNStyle::DPadUpPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadUp][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadUpPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadUp][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadUpRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadUpLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadUpRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadUp][SNVirtualGamePadEventRelease];
}

// Down
Boolean SNVGamePadNStyle::DPadDownPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDown][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadDownPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDown][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadDownRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadDownLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadDownRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDown][SNVirtualGamePadEventRelease];
}

// Left
Boolean SNVGamePadNStyle::DPadLeftPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLeft][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadLeftPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLeft][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadLeftRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadLeftLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadLeftRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLeft][SNVirtualGamePadEventRelease];
}

// Right
Boolean SNVGamePadNStyle::DPadRightPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRight][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::DPadRightPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRight][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::DPadRightRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::DPadRightLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::DPadRightRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRight][SNVirtualGamePadEventRelease];
}

// A
Boolean SNVGamePadNStyle::APush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDecide][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::APress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDecide][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::ARepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDecide][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::APadALong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDecide][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::APadARelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadDecide][SNVirtualGamePadEventRelease];
}

// B
Boolean SNVGamePadNStyle::BPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadCancel][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::BPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadCancel][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::BRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadCancel][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::BLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadCancel][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::BRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadCancel][SNVirtualGamePadEventRelease];
}

// X
Boolean SNVGamePadNStyle::XPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadMenu][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::XPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadMenu][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::XRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadMenu][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::XLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadMenu][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::XRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadMenu][SNVirtualGamePadEventRelease];
}

// Y
Boolean SNVGamePadNStyle::YPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadAction][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::YPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadAction][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::YRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadAction][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::YLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadAction][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::YRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadAction][SNVirtualGamePadEventRelease];
}

// Select
Boolean SNVGamePadNStyle::SelectPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadSelect][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::SelectPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadSelect][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::SelectRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadSelect][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::SelectLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadSelect][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::SelectRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadSelect][SNVirtualGamePadEventRelease];
}

// Start
Boolean SNVGamePadNStyle::StartPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadStart][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::StartPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadStart][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::StartRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadStart][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::StartLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadStart][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::StartRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadStart][SNVirtualGamePadEventRelease];
}

// L
Boolean SNVGamePadNStyle::L1Push()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::L1Press()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::L1Repeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::L1Long()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::L1Release()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventRelease];
}

// R
Boolean SNVGamePadNStyle::R1Push()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::R1Press()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::R1Repeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::R1Long()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::R1Release()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventRelease];
}

// L2
Boolean SNVGamePadNStyle::L2Push()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::L2Press()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::L2Repeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::L2Long()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::L2Release()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventRelease];
}

// R2
Boolean SNVGamePadNStyle::R2Push()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::R2Press()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::R2Repeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::R2Long()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::R2Release()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventRelease];
}

// L3
Boolean SNVGamePadNStyle::L3Push()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadL3][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::L3Press()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadL3][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::L3Repeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadL3][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::L3Long()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadL3][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::L3Release()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadL3][SNVirtualGamePadEventRelease];
}

// R3
Boolean SNVGamePadNStyle::R3Push()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadR3][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::R3Press()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadR3][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::R3Repeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadR3][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::R3Long()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadR3][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::R3Release()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadR3][SNVirtualGamePadEventRelease];
}

// L Stick Up
Boolean SNVGamePadNStyle::LStkUpPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListUp][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkUpPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListUp][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkUpRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkUpLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkUpRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListUp][SNVirtualGamePadEventRelease];
}

// L Stick Down
Boolean SNVGamePadNStyle::LStkDownPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListDown][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkDownPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListDown][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkDownRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkDownLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkDownRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadListDown][SNVirtualGamePadEventRelease];
}

// L Stick Left
Boolean SNVGamePadNStyle::LStkLeftPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkLeftPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkLeftRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkLeftLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkLeftRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventRelease];
}

// L Stick Right
Boolean SNVGamePadNStyle::LStkRightPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::LStkRightPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::LStkRightRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::LStkRightLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::LStkRightRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventRelease];
}

// R Stick Up
Boolean SNVGamePadNStyle::RStkUpPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkUpPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkUpRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkUpLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkUpRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventRelease];
}

// R Stick Down
Boolean SNVGamePadNStyle::RStkDownPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkDownPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkDownRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkDownLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkDownRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventRelease];
}

// R Stick Left
Boolean SNVGamePadNStyle::RStkLeftPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkLeftPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkLeftRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkLeftLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkLeftRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventRelease];
}

// R Stick Right
Boolean SNVGamePadNStyle::RStkRightPush()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventPush];
}
Boolean SNVGamePadNStyle::RStkRightPress()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventPress];
}
Boolean SNVGamePadNStyle::RStkRightRepeat()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVGamePadNStyle::RStkRightLong()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVGamePadNStyle::RStkRightRelease()
{
	return SNVirtualGamePad::Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventRelease];
}


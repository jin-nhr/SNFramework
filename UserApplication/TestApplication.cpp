#include "TestApplication.h"
#include "Windows.h"


TestApplication UserApp;

SNSurfaceDIB* dib;
SNSurfaceDDB* ddb;

TestApplication::TestApplication()
{
	dib = nullptr;
	ddb = nullptr;
}


TestApplication::~TestApplication()
{
	delete dib;
	delete ddb;
}


Void TestApplication::Initialize()
{
	Int32 cnt_y;
	Int32 cnt_x;

	SNColor* line_buf;

	ddb = new SNSurfaceDDB;
	dib = new SNSurfaceDIB;

#
	dib->CreateSurface(960, 540);
	ddb->CreateSurface(960, 540);

	for (cnt_y = 0; cnt_y < 540; cnt_y++)
	{
		line_buf = &dib->GetDIBSection()->GetPixelData()[cnt_y * 960];

		for (cnt_x = 0; cnt_x < 960; cnt_x++)
		{
			line_buf[cnt_x].Blue = (UInt8)(255 - ((cnt_y * 255) / 540));
			line_buf[cnt_x].Green = 0;
			line_buf[cnt_x].Red = 0;
			line_buf[cnt_x].Alpha = 0;
		}
	}

	ddb->GetDC()->BitBlt(0, 0, dib->GetDC()->GetDeviceContext(), 0, 0, 960, 540);

	return;
}

Void TestApplication::Terminate()
{

	return;
}


Void TestApplication::Entry()
{

	return;
}

Void TestApplication::Exit()
{

	return;
}


SNTransitionCode TestApplication::Step(SNEvent* event)
{

	return SNTransitionCodeNo;
}



Void TestApplication::Draw(SNSurface* surface)
{
	//surface->GetDC()->StretchBlt(0, 0, surface->GetWidth(), surface->GetHeight(), ddb->GetDC()->GetDeviceContext(), 0, 0, 256, 256);
	surface->GetDC()->BitBlt(0, 0, ddb->GetDC()->GetDeviceContext(), 0, 0, 960, 540);
	//surface->GetDC()->Clear();

	return;
}


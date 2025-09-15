#include "TestApplication.h"
#include "Windows.h"


//TestApplication UserApp;

SNSurfaceDIB GUIWindow;

TestApplication::TestApplication()
{
	Width = 0;
	Height = 0;
	return;
}


TestApplication::~TestApplication()
{
	return;
}


Void TestApplication::Initialize()
{
	Int32 cnt_y;
	Int32 cnt_x;

	Width = SNConfiguration::SystemConfiguration.ScreenWidth;
	Height = SNConfiguration::SystemConfiguration.ScreenHeight;

	SNColor* line_buf;

	DIB.CreateSurface(Width, Height);
	DDB.CreateSurface(Width, Height);

#if 0
	for (cnt_y = 0; cnt_y < Height; cnt_y++)
	{
		line_buf = &DIB.GetPixelData()[cnt_y * Width];

		for (cnt_x = 0; cnt_x < 960; cnt_x++)
		{
			line_buf[cnt_x].Blue = (UInt8)(255 - ((cnt_y * 255) / Height));
			line_buf[cnt_x].Green = 0;
			line_buf[cnt_x].Red = 0;
		}
	}
#endif

	DDB.BitBlt(0, 0, DIB.GetDC(), 0, 0, Width, Height);

	int w = 300;
	int h = 300;
	SNColor white;
	SNColor gray;

	white.Blue = 255;
	white.Green = 255;
	white.Red = 255;
	gray.Blue = 100;
	gray.Green = 100;
	gray.Red = 100;

	// GUIウインドウ
	GUIWindow.CreateSurface(w, h);
	
	for (cnt_y = 0; cnt_y < h; cnt_y++)
	{
		line_buf = &GUIWindow.GetPixelData()[cnt_y * w];
		for (cnt_x = 0; cnt_x < w; cnt_x++)
		{
#if 0
			if (cnt_y == 299 || cnt_x == 299)
			{
				line_buf[cnt_x] = gray;
			}
			else if (cnt_y == 0 || cnt_x == 0)
			{
				line_buf[cnt_x] = white;
			}
			else if (cnt_y == 298 || cnt_x == 298)
			{
				line_buf[cnt_x] = white;
			}

			else if (cnt_y == 1 || cnt_x == 1)
			{
				line_buf[cnt_x] = gray;
			}
#else
			if (cnt_y == 299 || cnt_x == 299)
			{
				line_buf[cnt_x] = white;
			}
			else if (cnt_y == 0 || cnt_x == 0)
			{
				line_buf[cnt_x] = white;
			}
#endif
			else
			{
				line_buf[cnt_x].Blue = 0;
				line_buf[cnt_x].Green = (UInt8)(128 - ((cnt_y * 128) / h));
				line_buf[cnt_x].Red = 0;
			}
		}
	}

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
	return SNTransitionCodeStay;
}



Void TestApplication::Draw(SNSurface* surface)
{
	surface->BitBlt(0, 0, DDB.GetDC(), 0, 0, Width, Height);

	surface->BitBlt(200, 200, GUIWindow.GetDC(), 0, 0, 300, 300);

	return;
}


#include "DDBitmap.h"
#include "Graphics.h"
#include "framework.h"


DDBitmap::DDBitmap()
{
	m_hBitmap = NULL;
	m_Width = 0;
	m_Height = 0;

	return;
}

DDBitmap::~DDBitmap()
{
	Delete();

	return;
}

void DDBitmap::Create(int w, int h)
{
	Delete();

	m_Width = w;
	m_Height = h;

	m_hBitmap = (void*)CreateCompatibleBitmap((HDC)Graphics::GetScreenDC() , w, h);

	return;
}

void DDBitmap::Delete()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject((HGDIOBJ)m_hBitmap);
		m_hBitmap = NULL;
	}

	return;
}

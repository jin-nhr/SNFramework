#pragma once
class DDBitmap
{
public:
	DDBitmap();
	virtual ~DDBitmap();

	void Create(int w, int h);
	void Delete();

	void* GetHandle() { return m_hBitmap; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

private:
	void* m_hBitmap;
	int m_Width;
	int m_Height;
};

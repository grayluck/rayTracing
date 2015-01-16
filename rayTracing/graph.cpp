
#include "stdafx.h"
#include "graph.h"
#include "FileUtils.h"
#include "octtree.h"

#include <algorithm>


CDialogEx* dlg;
HWND hwnd;

Point* pbitmap;
HBITMAP bitmap; 
HDC hdc, bitdc;

bool hold = false;
class Texture
{
public:
	// –¥µ√∫‹¥¿
	Point* clr;
	int width, height;
	HBITMAP bit;
	Texture(int width, int height, HBITMAP bit)
		:width(width),height(height),bit(bit)
	{
		clr = new Point[width * height];
	}
};

Texture** textures;

void initGraph(void* _dlg)
{
	deb = fopen("deb.txt", "w");
	loadStage();
	dlg = (CDialogEx*) _dlg;
	pbitmap = new Point[scrwidth * scrheight]();
	hwnd = dlg->m_hWnd;
	hdc = GetDC(hwnd);
	bitmap = CreateCompatibleBitmap(hdc, scrwidth, scrheight);
	bitdc = CreateCompatibleDC(hdc);
	SelectObject(bitdc, bitmap);
	::SetWindowPos(hwnd,HWND_TOPMOST,0,0,scrwidth + 50,scrheight + 50,SWP_NOZORDER);
	xratio = (double)width / scrwidth;
	yratio = (double)height / scrheight;

	textures = new Texture*[textureNumber];
	// º”‘ÿÃ˘Õº
	for(int i = 0; i < textureNumber; ++i)
	{
		HBITMAP tmp = (HBITMAP)LoadImageA(
			NULL, textureList[i], IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE | LR_VGACOLOR);
		BITMAP bm = {0};
		GetObject( tmp, sizeof(bm), &bm );
		textures[i] = new Texture(bm.bmWidth, bm.bmHeight, tmp);
		SelectObject(bitdc, textures[i]->bit);
		for(int x = 0; x < textures[i]->width; ++x)
		{
			for(int y = 0; y < textures[i]->height; ++y)
			{
				COLORREF tmp = GetPixel(bitdc, x, y);
				textures[i]->clr[x * textures[i]->height + y] = Point(
					GetRValue(tmp) / 255.0, GetGValue(tmp) / 255.0, GetBValue(tmp) / 255.0);
			}
		}
	}
	SelectObject(bitdc, bitmap);
	buildOctTree();
}

Point getTextureColor(int id, double x, double y, int repeatx, int repeaty)
{
	Texture& t = *textures[id];
	Point tmp, res;
	double onex = 1.0 / repeatx;
	double oney = 1.0 / repeaty;
	x = (x - (int)(x / onex) * onex) / onex;
	y = (y - (int)(y / oney) * oney) / oney;
	x *= textures[id]->width;
	y *= textures[id]->height;
	//fprintf(deb, "%lf %lf\n", x, y);
	double vx = x - (int)x;
	double vy = y - (int)y;
	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 2; ++j)
		{
			int xx = ((int)(x + i)) % t.width, yy = ((int)(y + j)) % t.height;
			tmp = textures[id]->clr[xx * textures[id]->height + yy];
			res = res + tmp * (i?vx:1-vx) * (j?vy:1-vy);
		}
	return res;
}

void drawPixel(int x, int y, Point clr, int tim)
{
	setPixel(x, y, clr, tim);
	if(!hold)
		refresh();
}

void refresh()
{
	BitBlt(hdc, 0, 0, scrwidth, scrheight, bitdc, 0, 0, SRCCOPY);
	fclose(deb);
}

void holdOn()
{
	hold = true;
}

void holdOff()
{
	hold = false;
}

void clearMem()
{
	for(int i = 0; i < scrwidth*scrheight; ++i)
		pbitmap[i].x = pbitmap[i].y = pbitmap[i].z = 0;
}

void setPixel(int x, int y, Point clr, int tim)
{
	Point& p = pbitmap[x * scrheight + y];
	p = p + clr;
	Point tmp(p.x / tim, p.y / tim, p.z / tim);
	tmp.rect();
	SetPixel(bitdc, x, y, RGB(tmp.x * 255, tmp.y * 255, tmp.z * 255));
}

bool SaveToFile(HBITMAP hBitmap3, char* lpszFileName)
{   
	HDC hDC;
	int iBits;
	WORD wBitCount;
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
	BITMAP Bitmap0;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal,hOldPal2=NULL;
	hDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24; 
	GetObject(hBitmap3, sizeof(Bitmap0), (LPSTR)&Bitmap0);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap0.bmWidth;
	bi.biHeight =-Bitmap0.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 256;
	dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount +31) & ~31) /8
												* Bitmap0.bmHeight; 
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	GetDIBits(hDC, hBitmap3, 0, (UINT) Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
	+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPal2)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFileA(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 

	if (fh == INVALID_HANDLE_VALUE)
	return FALSE; 

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
} 

int cnt = 0;

void saveToFile(int typ)
{
	char tmps[100];
	sprintf(tmps, "img/test%04d.bmp\0", cnt);
	SaveToFile(bitmap, tmps);
	if(typ)
	{
		sprintf(tmps, "img/output.bmp\0");
		SaveToFile(bitmap, tmps);
	}
	cnt ++;
}

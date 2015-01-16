
#pragma once

#include "common.h"

/** 这个函数必须在开始绘图前被调用，初始化图形功能 **/
void initGraph(void*);

/** 向屏幕上画一个像素点 **/
void drawPixel(int x, int y, Point clr, int tim = -1);

void setPixel(int x, int y, Point clr, int tim = -1);

/** 刷新整个屏幕（好担心撞名啊……） **/
void refresh();

void holdOn();

void holdOff();

void saveToFile(int typ = 0);
void clearMem();

Point getTextureColor(int id, double x, double y,int, int);

extern HWND hwnd;

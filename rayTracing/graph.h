
#pragma once

#include "common.h"

/** ������������ڿ�ʼ��ͼǰ�����ã���ʼ��ͼ�ι��� **/
void initGraph(void*);

/** ����Ļ�ϻ�һ�����ص� **/
void drawPixel(int x, int y, Point clr, int tim = -1);

void setPixel(int x, int y, Point clr, int tim = -1);

/** ˢ��������Ļ���õ���ײ���������� **/
void refresh();

void holdOn();

void holdOff();

void saveToFile(int typ = 0);
void clearMem();

Point getTextureColor(int id, double x, double y,int, int);

extern HWND hwnd;


#pragma once

class Camera;

#include "common.h"

class Camera
{
public:
	// �����������������
	Point pos;
	// ͷ���ķ���������������
	Point headup;
	Point lookat;
	Point dir;
	// ������λ��������lookat���������
	Point xaxis, yaxis, zaxis;
	Camera(Point&, Point&, Point&);
	// ��ȡ������һ��λ�õõ�����
	virtual Line getSightLight(double x, double y);
	void unitise();
	void setPos(Point*, Point*, Point*);
};

class CameraProspective : public Camera
{
public:
	// ��Ȧ�����ڹ��캯�������
	Point apetureCenter;
	// ��Ȧ����
	double dist;
	// ��ƽ����루��Ȧ->��ƽ�棩
	double focusDist, focusOffset;
	// ��ƽ�����ģ���ʵ�ǽ�ƽ��������һ�㣩
	Point focusCenter;
	// depth in field �Ƿ���
	int DIFEnabled;
	Line getSightLight(double x, double y);
	CameraProspective(Point&, Point&, Point&,
		double dist = 200, int DIFEnabled = 0, double focusDist = 0, double focusOffset = 0);
	static Camera* readCamera(FILE* fi);
};

/*
class CameraDepth : public Camera
{
	// ��Ȧ��С
	double apetureSize;
	// ��Ȧ����ȡ�������
	double apetureDist;
	Line getSightLight(int x, int y);
};
*/
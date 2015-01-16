
#pragma once

class Camera;

#include "common.h"

class Camera
{
public:
	// 相机所处的世界坐标
	Point pos;
	// 头顶的方向，用来求正交基
	Point headup;
	Point lookat;
	Point dir;
	// 两个单位向量，与lookat组成正交基
	Point xaxis, yaxis, zaxis;
	Camera(Point&, Point&, Point&);
	// 从取景器的一个位置得到视线
	virtual Line getSightLight(double x, double y);
	void unitise();
	void setPos(Point*, Point*, Point*);
};

class CameraProspective : public Camera
{
public:
	// 光圈中心在构造函数后计算
	Point apetureCenter;
	// 光圈距离
	double dist;
	// 焦平面距离（光圈->焦平面）
	double focusDist, focusOffset;
	// 焦平面中心（其实是焦平面上任意一点）
	Point focusCenter;
	// depth in field 是否开启
	int DIFEnabled;
	Line getSightLight(double x, double y);
	CameraProspective(Point&, Point&, Point&,
		double dist = 200, int DIFEnabled = 0, double focusDist = 0, double focusOffset = 0);
	static Camera* readCamera(FILE* fi);
};

/*
class CameraDepth : public Camera
{
	// 光圈大小
	double apetureSize;
	// 光圈距离取景面距离
	double apetureDist;
	Line getSightLight(int x, int y);
};
*/
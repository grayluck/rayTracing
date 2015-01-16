
#include "stdafx.h"
#include "camera.h"

Camera::Camera(Point& pos, Point& headup, Point& lookat)
	:pos(pos),headup(headup),lookat(lookat)
{
	unitise();
}

Line Camera::getSightLight(double x, double y)
{
	Point mov = xaxis * x + yaxis * y;
	return Line(pos + mov, lookat + mov);
}

void Camera::unitise()
{
	dir = (lookat - pos).norm();
	lookat = pos + dir;
	xaxis = dir.cross(headup);
	xaxis = xaxis / xaxis.len();
	yaxis = xaxis.cross(dir);
	yaxis = yaxis / yaxis.len();
	zaxis = xaxis.cross(yaxis);
	zaxis = zaxis / zaxis.len();
}

Camera* CameraProspective::readCamera(FILE* fi)
{
	Point a, b, c;
	a.readPoint(fi);
	b.readPoint(fi);
	c.readPoint(fi);
	double dist;
	fscanf(fi, "%lf", &dist);
	int DIFEnabled;
	double focusDist, focusOffset;
	fscanf(fi, "%d%lf%lf", &DIFEnabled, &focusDist, &focusOffset);
	return new CameraProspective(a, b, c, dist, DIFEnabled, focusDist, focusOffset);
}

void Camera::setPos(Point* _pos, Point* _headup, Point* _lookat)
{
	if(_pos != 0)
		pos = *_pos;
	
	if(_headup != 0)
		headup = *_headup;

	if(_lookat != 0)
		lookat = *_lookat;

	unitise();
}

CameraProspective::CameraProspective(
	Point& pos, Point& headup, Point& lookat,
	double dist, int DIFEnabled, double focusDist, double focusOffset)
		:Camera(pos, headup, lookat),
		dist(dist),DIFEnabled(DIFEnabled), focusDist(focusDist), focusOffset(focusOffset)
{
	apetureCenter = pos + dir * dist; 
#define fabs(a)		((a)>0?(a):-(a))
	focusCenter = apetureCenter + dir * focusDist;
}

Line CameraProspective::getSightLight(double x, double y)
{
	Point p = pos + xaxis * x * xratio + yaxis * y * yratio;
	Point d = (apetureCenter - p).norm();
	double cosa = fabs(d.dot(dir));
	Point focusPoint = apetureCenter + d * (focusDist / cosa);
	Point mov = 
		(	DIFEnabled == 1?
			Point(randf() * focusOffset, randf() * focusOffset, randf() * focusOffset):
			Point());
	Point pp = apetureCenter + mov; // 在光圈上随机扰动
	Point dd = focusPoint - pp; // 新的方向
	dd.norm();
	Line l(pp, pp + dd);
	return l;
	/*
	double movx = x / halfw * dist;
	double movy = y / halfh * dist;
	Point mov = xaxis * x + yaxis * y;
	Point mov2 = xaxis * (x + movx) + yaxis * (y + movy);
	return Line(pos + mov, lookat + mov2);
	*/
}


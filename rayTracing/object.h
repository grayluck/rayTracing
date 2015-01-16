
#pragma once

class Object;

#include "common.h"

// 判断一根射线和谁相交了
Object* getCollideObject(Line&);

class Object
{
public:
	// 半径
	double r;
	// 漫反射系数（有三个颜色哦）
	Point wd;
	// 发射的光亮度
	double brightness;
	// 折射概率 refrection
	double wr;
	// 反射概率 reflection
	double wl;
	// 折射系数
	double nr;

	int texture;
	Point vn, ve;
	int repeatx, repeaty;
	Point coord;	// coordination
	virtual void readObject(FILE*);
	Object();
	// 一条线与该Object交在什么位置
	virtual Point collide(Line&);
	// 对于一根射来的光线，返回其颜色
	// 第二个参数表示能量
	// 会递归哦递归哦
	virtual Point getPointFromLine(Line&, double);
	void readMaterial(FILE* fi);
	Point getColorFromLine(Point lin, Point col, Point norm, int energy);
	void copyMaterial(Object&);
	virtual int getType();
};

class Sphere : public Object
{
public:
	void readObject(FILE*);
	// 线与球相交的位置
	Point collide(Line&);
	Point getPointFromLine(Line&, double);
	Sphere();
	int getType();
};

class Triangle : public Object
{
public:
	Point a, b, c;
	// 包围盒
	Point p, q;
	Point norm;
	void readObject(FILE*);
	// 三角形和光线交
	Point collide(Line&);
	Point getPointFromLine(Line&, double);
	Triangle();
	Triangle(
		Point a, Point b, Point c);
	int getType();
	void getBox(Point& p, Point& q);
};

/*
class Obj : public Object
{
public:
	int gn;
	Triangle* g;
	Triangle* last;
	void readObject(FILE*);
	Point collide(Line&);
	// getPointFromLine 一定会在collide之后被调用
	// 此时如果有相撞那么last中一定有值
	Point getPointFromLine(Line&, double);
	Obj();
};

*/

void readCube(FILE*, int& i);

void readObj(FILE*, int&);

extern int debobj;

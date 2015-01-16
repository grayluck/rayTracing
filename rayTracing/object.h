
#pragma once

class Object;

#include "common.h"

// �ж�һ�����ߺ�˭�ཻ��
Object* getCollideObject(Line&);

class Object
{
public:
	// �뾶
	double r;
	// ������ϵ������������ɫŶ��
	Point wd;
	// ����Ĺ�����
	double brightness;
	// ������� refrection
	double wr;
	// ������� reflection
	double wl;
	// ����ϵ��
	double nr;

	int texture;
	Point vn, ve;
	int repeatx, repeaty;
	Point coord;	// coordination
	virtual void readObject(FILE*);
	Object();
	// һ�������Object����ʲôλ��
	virtual Point collide(Line&);
	// ����һ�������Ĺ��ߣ���������ɫ
	// �ڶ���������ʾ����
	// ��ݹ�Ŷ�ݹ�Ŷ
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
	// �������ཻ��λ��
	Point collide(Line&);
	Point getPointFromLine(Line&, double);
	Sphere();
	int getType();
};

class Triangle : public Object
{
public:
	Point a, b, c;
	// ��Χ��
	Point p, q;
	Point norm;
	void readObject(FILE*);
	// �����κ͹��߽�
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
	// getPointFromLine һ������collide֮�󱻵���
	// ��ʱ�������ײ��ôlast��һ����ֵ
	Point getPointFromLine(Line&, double);
	Obj();
};

*/

void readCube(FILE*, int& i);

void readObj(FILE*, int&);

extern int debobj;


#pragma once

#define sqr(a)		((a)*(a))

#include "parser\Vec3f.h"

using namespace SimpleOBJ;

extern const char* stageName;
extern int width;
extern int height;
extern int scrwidth;
extern int scrheight;
extern double halfw;
extern double halfh;
extern const double pi;
extern double xratio, yratio;
extern double energyLim;

// ����ʵ���Ǹ�������
// �����뿪������vector�̳�pointһ��
// ��Ȼ����̫����
class Point
{
public:
	double x, y, z;
	int typ;
	Point(double x, double y, double z, int typ = 0);
	Point();
	Point(Vec3f&);
	double dot(Point&);
	Point cross(Point&);
	double len2();
	double len();
	Point& norm();
	Point operator * (double b);
	Point operator * (Point& b);
	Point operator / (double b);
	Point operator + (Point& b);
	Point operator - (Point& b);
	Point mirror(Point& b);
	void readPoint(FILE*);
	// ���ڴ���1��Ԫ�غ�С��0��Ԫ�ؽ��й淶
	void rect();
};

class Line
{
public:
	// ����������ȷ����һ��ֱ�ߡ� 
	Point a, b;
	Line(Point a, Point b);
	Point getVector();
};

#include "object.h"
#include "camera.h"

extern int n;
extern Object* g[1000000];
extern Camera* camera;

// ���������ƽ��
double dist2(Point &a, Point &b);
double dist(Point &a, Point &b);
double dot(Point& a, Point& b);
Point dip(Point&, Point&);

double randf();
void readConfigure(FILE*);

extern char* textureList[];
extern const int textureNumber;

void log(char*);
extern FILE* deb;
extern int useOctTree;
extern int useNormalOffset;

extern int globalCounter;

extern int repeatTime;

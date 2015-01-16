
#include "stdafx.h"
#include "common.h"
#include <cmath>

int useOctTree = 1;

const char* stageName = "test.txt";
int scrwidth = 600;
int scrheight = 400;
int width = 600;
int height = 400;
double halfw = width / 2.0;
double halfh = height / 2.0;
const double pi = 3.141592653589;
double xratio;
double yratio;
int repeatTime = 200;
int useNormalOffset = 0;

double energyLim = 10;

const int textureNumber = 5;

int globalCounter = 0;

char* textureList[textureNumber] = 
	{"texture/0.bmp", "texture/1.bmp", "texture/2.bmp", "texture/3.bmp", "texture/4.bmp"};

Camera* camera;

int n = 0;
Object* g[1000000];

FILE* deb;

void Point::readPoint(FILE* fi)
{
	fscanf(fi, "%lf%lf%lf", &x, &y, &z);
}

Point::Point(double x, double y, double z, int typ)
	:x(x), y(y), z(z), typ(typ){}

Point::Point()
	:x(0), y(0), z(0), typ(0){}

Point::Point(Vec3f& obj)
	:x(obj.x),y(obj.y),z(obj.z){}

void Point::rect()
{
	x = max(0, min(1, x));
	y = max(0, min(1, y));
	z = max(0, min(1, z));
}

Point Point::operator* (double b)
{
	Point tmp(x * b, y * b, z * b);
	return tmp;
}

Point Point::operator* (Point& b)
{
	Point ret(x * b.x, y * b.y, z * b.z);
	return ret;
}

Point Point::operator/ (double b)
{
	Point tmp(x / b, y / b, z / b);
	return tmp;
}

Point Point::operator+ (Point& b)
{
	Point tmp(x + b.x, y + b.y, z + b.z);
	return tmp;
}

Point Point::operator- (Point& b)
{
	Point tmp(x - b.x, y - b.y, z - b.z);
	return tmp;
}

Point Point::mirror(Point& b)
{
	/*
	Point tmp = b / b.len() * len();
	return Point(2 * tmp.x - x, 2 * tmp.y - y, 2 * tmp.z - z);
	*/
	return *this-b*2*b.dot(*this);
}

Line::Line(Point a, Point b):a(a), b(b){}

Point Line::getVector()
{
	return Point(b.x - a.x, b.y - a.y, b.z - a.z);
}

double dist2(Point &a, Point &b)
{
	return sqr(a.x - b.x) + sqr(a.y - b.y) + sqr(a.z - b.z);
}

double dist(Point &a, Point &b)
{
	return sqrt(dist2(a, b));
}

double Point::dot(Point& p)
{
	return x * p.x  + y * p.y + z * p.z;
}

double Point::len2()
{
	return x * x + y * y + z * z;
}

double Point::len()
{
	return sqrt(len2());
}

double dot(Point& a, Point& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point dip(Point& a, Point& b)
{
	return Point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Point Point::cross(Point& p)
{
	return dip(*this, p);
}

double randf()
{
	return rand() / (double)RAND_MAX;
}

Point& Point::norm()
{
	double l = len();
	x /= l, y /= l, z /= l;
	return *this;
}

void readConfigure(FILE* fi)
{
	fscanf(fi, "%d", &repeatTime);
	fscanf(fi, "%d%d%d%d",&scrwidth, &scrheight, &width, &height);
	fscanf(fi, "%lf", &energyLim);
	halfw = scrwidth / 2.0;
	halfh = scrheight / 2.0;
}


void log(char* s)
{
	fprintf(deb, s);
	fflush(deb);
}
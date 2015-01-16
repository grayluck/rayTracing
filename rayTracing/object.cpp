
#include "stdafx.h"
#include "object.h"
#include <cmath>
#include "graph.h"
#include "parser\SimpleObject.h"
#include "octtree.h"

using namespace SimpleOBJ;

int debobj;

Object* getCollideObject(Line& l)
{
	double mind = 1e100;
	debobj = -1;
	Object* minobj = NULL;
	for(int i = 0; i < n; ++i)
	{
		if(useOctTree && g[i]->getType() == 2)
			continue;
		Point ret = g[i]->collide(l);
		if(ret.typ == -1)
			continue;
		double d = dist2(ret, l.a);
		if( d < mind)
			mind = d, minobj = g[i], debobj = i;
	}
	if(useOctTree)
		octTreeSearch(0, l, minobj, mind);
	return minobj;
}

Object::Object():coord(0, 0, 0)
{}

void Object::readObject(FILE* fi)
{
	fscanf(fi, "%lf%lf%lf", &coord.x, &coord.y, &coord.z);
}

Point Object::collide(Line& l)
{
	return Point(0, 0, 0, -1);
}

Point Object::getPointFromLine(Line& l, double energy)
{
	return Point();
}

void Object::readMaterial(FILE* fi)
{
	wd.readPoint(fi);
	fscanf(fi, "%lf", &brightness);
	fscanf(fi, "%lf%lf", &wr, &wl);
	fscanf(fi, "%lf", &nr);
	fscanf(fi, "%d", &texture);
	if(texture != -1)
	{
		vn.readPoint(fi);
		vn = vn.norm();
		ve.readPoint(fi);
		ve = ve.norm();
		fscanf(fi, "%d%d", &repeatx, &repeaty);
	}
}

void Sphere::readObject(FILE* fi)
{
	Object::readObject(fi);
	fscanf(fi, "%lf", &r);
	readMaterial(fi);
}

Sphere::Sphere()
{
}

Point Sphere::collide(Line& l)
{
	Point fail(0,0,0,-1);
	Point a = l.getVector();
	Point b = Point(coord.x - l.a.x, coord.y - l.a.y, coord.z - l.a.z);
	double al = a.len();
	double dot = a.dot(b) / al;
	double bl = b.len();
	double dis = sqr(bl) - sqr(dot); // 射线到圆心的距离 ^ 2
	if(dot < 0 || dis > r * r)
		return fail;
	double dl = sqrt(r * r - dis);
	double ratio = (dot - dl) / al;
	if(ratio < 1e-7)
		ratio = (dot + dl) / al;
	Point aa(l.a.x + a.x * ratio, l.a.y + a.y * ratio, l.a.z + a.z * ratio);
	return aa;
}

Point Object::getColorFromLine(Point lin, Point col, Point norm, int energy)
{	
	Line nline = Line(lin, lin);
	double rnd = randf();
	Point textureCol;
	if(texture >= 0)
	{
		// 如果有贴图的话就修改颜色
		Point vp = (col - coord).norm();
		double phi = acos(max(-1, min(1, -vn.dot(vp))));
		double v = phi / pi, u;
		double theta = (acos(max(-1, min(1, vp.dot(ve)/sin(phi))))) / (2.0 * pi);
		double deb = vn.cross(ve).dot(vp);
		u = (deb>0?theta:1-theta);
		textureCol = getTextureColor(texture, u, v, repeatx, repeaty);
		if(!useNormalOffset)
			wd = textureCol;
	}
	
	Point ret(wd * brightness);
	
	if(energy >= energyLim)
		return ret;

	if(rnd < wr)
	{
		Point nl = (norm.dot(lin) < 0 ? norm : norm * -1);

		Line reflRay(col, col + lin - norm*2*norm.dot(lin));
		bool into = norm.dot(nl)>0;
		double nc=1, nt=nr, nnt=into?nc/nt:nt/nc, ddn=lin.dot(nl);
		double cos2t=1-nnt*nnt*(1-ddn*ddn);
		if (cos2t<0)
			goto reflection;
		Point tdir = (lin*nnt - norm*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
		double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(norm));
		double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
		Point refl, refr;
		Object* obj = getCollideObject(reflRay);
		nline = Line(col, col+tdir);
		obj = getCollideObject(nline);
		if(energy>2)
			if(randf()<P)
			{
				if(obj != 0)	refl = obj->getPointFromLine(reflRay, energy + 1) * wd;
				ret = ret + refl*RP;
			}
			else
			{
				if(obj != 0)	refr = obj->getPointFromLine(nline, energy + 1) * wd;
				ret = ret + refr * TP;
			}
			else
			{
				if(obj != 0)	refl = obj->getPointFromLine(reflRay, energy + 1) * wd;
				if(obj != 0)	refr = obj->getPointFromLine(nline, energy + 1) * wd;
				ret = ret + refl*Re + refr*Tr;
			}
		return ret;
	}else if(rnd < wr + wl)
	{
		reflection:
		// 理想镜面
		Point lout =  lin.mirror(norm);
		nline = Line(col, col + lout);
	} else
	{
		// 漫反射
		norm.norm();
		double r1 = 2 * pi * randf();
		double r2 = randf(), r2s = sqrt(r2);
		Point w = norm;
		w.norm();
		Point u = (fabs(w.x)>.1?Point(0,1,0):Point(1,0,0)).cross(w);
		u.norm();
		Point v = w.cross(u);
		v.norm();
		if(useNormalOffset && texture >=0)
		{
			norm = norm + u * textureCol.x + v * textureCol.y;
			norm.norm();
			w = norm;
			u = (fabs(w.x)>.1?Point(0,1,0):Point(1,0,0)).cross(w);
			u.norm();
			v = w.cross(u);
			v.norm();
		}
		Point lout = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1-r2));
		lout.norm();
		nline = Line(col, col+lout);
	}
	Object* obj = getCollideObject(nline);
	if(obj != 0)
	{
		Point tmp = obj->getPointFromLine(nline, energy + 1) * wd;
		ret = ret + tmp;
	}
	return ret;
}

Point Sphere::getPointFromLine(Line& l, double energy)
{
	Point lin = (l.b - l.a).norm();
	Point col = collide(l);

	Point norm = (col - coord).norm();
			
	return Object::getColorFromLine(lin, col, norm, energy);
}

void Triangle::readObject(FILE* fi)
{
	a.readPoint(fi);
	b.readPoint(fi);
	c.readPoint(fi);
	
	readMaterial(fi);
	norm = (b - a).cross(c - a).norm();
}

Triangle::Triangle(){}

Point Triangle::collide(Line& l)
{
	globalCounter ++;
	Point ba = (b - a).norm();
    Point ca = (c - a).norm();
    Point N = norm;
	Point lin = (l.b - l.a).norm();
	Point fail(0,0,0,-1);
    if (dot(N, lin) == 0)
		return fail; // 线与三角形平行
    double t = (a.dot(N) - l.a.dot(N)) / lin.dot(N); // 解三元一次不等式
    // inside-out test
    Point p = l.a + lin * t;
    if (dot(N, (a - c).cross(p - c)) < 0 || 
		dot(N, (c - b).cross(p - b)) < 0 ||
		dot(N, ba.cross(p - a)) < 0 )
		return fail; // p落在三角形外
	Point lout = p - l.a;
	double tmp = lin.dot(lout);
	if(tmp <= 1e-7)
		return fail;
	return p;
}

Point Triangle::getPointFromLine(Line& l, double energy)
{
	Point lin = (l.b - l.a).norm();
	Point col = collide(l);
	Point ret =  Object::getColorFromLine(lin, col, norm, energy);
	return ret;
}


Triangle::Triangle(
		Point a, Point b, Point c):a(a),b(b),c(c)
{
	norm = (b - a).cross(c - a).norm();
}

void readCube(FILE* fi, int& i)
{
	double pos[3], siz[3];
	for(int i = 0; i < 3; ++i)
		fscanf(fi, "%lf", &pos[i]);
	for(int i = 0; i < 3; ++i)
		fscanf(fi, "%lf", &siz[i]);
	Point p[8];
	for(int z = 0; z < 2; ++z)
		for(int y = 0; y < 2; ++y)
			for(int x = 0; x < 2; ++x)
				p[z * 4 + y * 2 + x] = 
					Point(	pos[0] + x * siz[0],
							pos[1] + y * siz[1],
							pos[2] + z * siz[2]);
	Object tmp;
	tmp.readMaterial(fi);
	int ord[12][3] = 
	{
		{0,1,2},{1,3,2},
		{4,0,6},{0,2,6},
		{1,5,3},{5,7,3},
		{2,3,6},{3,7,6},
		{4,5,0},{5,1,0},
		{6,7,4},{7,5,4}
	};
	for(int t = 0; t < 12; ++t, ++i)
	{
		g[i] = new Triangle(p[ord[t][0]], p[ord[t][2]], p[ord[t][1]]);
		g[i]->copyMaterial(tmp);
	}
	n += 11;
	i --;
}

void Object::copyMaterial(Object& obj)
{	
	wd = obj.wd;
	brightness = obj.brightness;
	wr = obj.wr;
	wl = obj.wl;
	nr = obj.nr;
	texture = obj.texture;
	vn = obj.vn;
	ve = obj.ve;
	repeatx = obj.repeatx;
	repeaty = obj.repeaty;
}

/*
Obj::Obj(){}

void Obj::readObject(FILE* fi)
{
	// 需要的参数：文件名，坐标，缩放比例和材质
	char nam[100];
	fscanf(fi, "%s", nam);
	coord.readPoint(fi);
	double zoom;
	fscanf(fi, "%lf", &zoom);
	Object tmpMaterial;
	tmpMaterial.readMaterial(fi);

	// 开始将parser得到的三角形转换为我的格式
	CSimpleObject obj;
	obj.LoadFromObj(nam);
	gn = obj.m_nTriangles;
	Vec3f*& vec = obj.m_pVertexList;
	g = new Triangle[gn];
	for(int i = 0; i < gn; ++i)
	{
		Array<int, 3>& tmp = obj.m_pTriangleList[i];
		g[i].a = Point(vec[tmp[0]]) * zoom + coord;
		g[i].b = Point(vec[tmp[1]]) * zoom + coord;
		g[i].c = Point(vec[tmp[2]]) * zoom + coord;
		g[i].copyMaterial(tmpMaterial);
	}
}

Point Obj::collide(Line& l)
{
	return Point();
}

Point Obj::getPointFromLine(Line& l, double energy)
{
	return Point();
}
*/

void readObj(FILE* fi, int& i)
{
	// 需要的参数：文件名，坐标，缩放比例和材质
	char nam[100];
	fscanf(fi, "%s", nam);
	Point coord;
	coord.readPoint(fi);
	double zoom;
	fscanf(fi, "%lf", &zoom);
	Object tmpMaterial;
	tmpMaterial.readMaterial(fi);

	// 开始将parser得到的三角形转换为我的格式
	CSimpleObject obj;
	obj.LoadFromObj(nam);
	int gn = obj.m_nTriangles;
	Vec3f*& vec = obj.m_pVertexList;
	for(int t = 0; t < gn; ++t, ++i)
	{
		Array<int, 3>& tmp = obj.m_pTriangleList[t];
		g[i] = new Triangle(Point(vec[tmp[0]]) * zoom + coord,
					Point(vec[tmp[1]]) * zoom + coord,
					Point(vec[tmp[2]]) * zoom + coord);
		g[i]->copyMaterial(tmpMaterial);
	}
	n += gn-1;
	i--;
}

int Object::getType()	{	return 0;	}
int Sphere::getType()	{	return 1;	}
int Triangle::getType()	{	return 2;	}

void Triangle::getBox(Point& p, Point& q)
{
	p = Point(	min(a.x, min(b.x, c.x)),
				min(a.y, min(b.y, c.y)),
				min(a.z, min(b.z, c.z)));
	q = Point(	max(a.x, max(b.x, c.x)),
				max(a.y, max(b.y, c.y)),
				max(a.z, max(b.z, c.z)));
	this->p = p;
	this->q = q;
}

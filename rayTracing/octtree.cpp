
#include "stdafx.h"
#include "octtree.h"
#include <algorithm>

using namespace std;

const int nodelim = 30;
const int maxOctN = 10000;
int octn;
OctNode* oct;

// 两个局部变量用于公用缓存
int tmpn;
int tmplist[100000];

int cmpx(const int a, const int b)
{
	return ((Triangle*)g[a])->p.x < ((Triangle*)g[b])->p.x;
}

int cmpy(const int a, const int b)
{
	return ((Triangle*)g[a])->p.y < ((Triangle*)g[b])->p.y;
}

int cmpz(const int a, const int b)
{
	return ((Triangle*)g[a])->p.z < ((Triangle*)g[b])->p.z;
}

int createSon(int st, int en)
{
	int* tmpp = new int[en - st];
	for(int i = 0; i < en - st; ++i)
		tmpp[i] = tmplist[st + i];
	OctNode& p = oct[octn];
	p.lst = tmpp;
	p.lstn = en - st;
	p.typ = 1;
	memset(p.son, 0, sizeof(int[8]));
	return octn++;
}

void dfs(int pno)
{
	OctNode& p = oct[pno];
	p.getBox();
	// 配额已经完成
	if(p.lstn < nodelim)
	{
		p.typ = 0;
		return;
	}
	tmpn = p.lstn;
	for(int i = 0; i < p.lstn; ++i)
		tmplist[i] = p.lst[i];
	sort(tmplist, tmplist+tmpn, cmpx);
	int xn = tmpn/2;
	int yn = xn / 2;
	int zn = yn / 2;
	sort(tmplist, tmplist+xn, cmpy);
	sort(tmplist + xn, tmplist+tmpn, cmpy);

	sort(tmplist, tmplist + yn, cmpz);
	sort(tmplist+ yn, tmplist + xn, cmpz);
	sort(tmplist+ xn, tmplist + xn + yn, cmpz);
	sort(tmplist+ xn + yn, tmplist + tmpn, cmpz);

	p.son[0] = createSon(0, zn);
	p.son[1] = createSon(zn, yn);
	p.son[2] = createSon(yn, yn+zn);
	p.son[3] = createSon(yn+zn, xn);
	p.son[4] = createSon(xn, xn+zn);
	p.son[5] = createSon(xn+zn, xn+yn);
	p.son[6] = createSon(xn + yn, xn + yn + zn);
	p.son[7] = createSon(xn + yn + zn, tmpn);

	for(int i =0; i < 8; ++i)
		dfs(p.son[i]);
}

void buildOctTree()
{
	oct = new OctNode[maxOctN];
	Point a = Point(1e100, 1e100, 1e100);
	Point b = Point(-1e100, -1e100, -1e100);
	tmpn = 0;
	for(int i = 0; i < n; ++i)
		if(g[i]->getType() == 2)	// 如果是三角面片
		{
			tmplist[tmpn++] = i;
			Triangle* t = (Triangle*)g[i];
			Point p, q;
			t->getBox(p, q);
		}
	createSon(0, tmpn);
	dfs(0);
}

void octTreeSearch(int pno, Line& l, Object*& minobj, double& mind)
{
	OctNode& p = oct[pno];
	// 得到和包围盒的两个z平面的交点
	Point lin = (l.b - l.a).norm();
	double t1 = (p.a.z - l.a.z) / lin.z;
	Point aa = l.a + lin * t1;
	double t2 = (p.b.z - l.a.z) / lin.z;
	Point bb = l.a + lin * t2;
	if(t1 <=1e-7 && t2 <= 1e-7)
		return;
	if(	aa.x < p.a.x && bb.x < p.a.x ||
		aa.y < p.a.y && bb.y < p.a.y ||
		aa.z < p.a.z && bb.z < p.a.z ||
		aa.x > p.b.x && bb.x > p.b.x ||
		aa.y > p.b.y && bb.y > p.b.y ||
		aa.z > p.b.z && bb.z > p.b.z)
		return;
	if(p.typ == 0)
	{
		for(int i = 0; i < p.lstn; ++i)
		{
			Point ret = g[p.lst[i]]->collide(l);
			if(ret.typ == -1)
				continue;
			double d = dist2(ret, l.a);
			if( d < mind)
				mind = d, minobj = g[p.lst[i]], debobj = p.lst[i];
		}
		return;
	}
	for(int i = 0; i < 8; ++i)
		octTreeSearch(p.son[i], l, minobj, mind);
}

void OctNode::getBox()
{
	a = Point(1e100, 1e100, 1e100);
	b = Point(-1e100, -1e100, -1e100);
	for(int i = 0; i < lstn; ++i)
	{
		Point pp = ((Triangle*)g[lst[i]])->p;
		Point qq = ((Triangle*)g[lst[i]])->q;
		a = Point(min(a.x, pp.x), min(a.y, pp.y), min(a.z, pp.z));
		b = Point(max(b.x, qq.x), max(b.y, qq.y), max(b.z, qq.z));
	}
}

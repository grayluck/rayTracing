#include <iostream>

using namespace std;

int repeatTime = 1000;
int scrwidth = 600;
int scrheight = 400;
int width = 600;
int height = 400;
int rebound = 5;

double randf()
{
	return rand() / (double)RAND_MAX;
}

class Printable
{
public:
	virtual void outp(){}
};

class Point : public Printable
{
public:
	double x, y, z;
	Point(){}
	Point(double x, double y, double z):x(x),y(y),z(z){}
	void outp()
	{
		printf("%lf %lf %lf\n", x, y, z);
	}
};

class Camera : public Printable
{
public:
	Point pos, headup, lookat;
	double aDir;
	int useDIF;
	double DIFdist, DIFoffset;
	void outp()
	{
		pos.outp();
		headup.outp();
		lookat.outp();
		printf("%lf\n%d %lf %lf\n\n", aDir, useDIF, DIFdist, DIFoffset);
	}
};

class Material : public Printable
{
public:
	Point wd;
	double brightness;
	double wr, wl;
	double nr;
	int texture;
	Point tn, tr;
	int repeatx, repeaty;
	void outp()
	{
		wd.outp();
		printf("%lf\n", brightness);
		printf("%lf %lf\n", wr, wl);
		printf("%lf\n", nr);
		printf("%d\n", texture);
		if(texture != -1)
		{
			tn.outp();
			tr.outp();
			printf("%d %d\n", repeatx, repeaty);
		}
		printf("\n");
	}
};


class Sphere : public Printable
{
public:
	Point pos;
	double r;
	Material m;
	void outp()
	{
		printf("0\n");
		pos.outp();
		printf("%lf\n", r);
		m.outp();
		printf("\n");
	}
};

class Cube : public Printable
{
public:
	Point pos;
	Point siz;
	Material m;
	void outp()
	{
		printf("2\n");
		pos.outp();
		siz.outp();
		m.outp();
		printf("\n");
	}
};

/**
脚本：
Scene 1
	方形积木 - 转相机
Scene 2
	求坠落被大球看着（反射）
	3秒后灭灯 动态光影
	10秒后 坠落彩虹球、木球
Scene 3
	波状球  反射 -> 漫反射 -> 投射 -> 反射  景深
**/

Camera camera;

const int maxn = 1001;

int cnt = 0;
int n = 0;
Printable* g[maxn];

Material emptMaterial;

void outpAll(int typ = 0)
{
	char tmps[100];
	if(typ == 0)
		sprintf(tmps, "script/%03d.txt", cnt);
	else
		sprintf(tmps, "test.txt");
	freopen(tmps, "w", stdout);
	printf("%d\n", repeatTime);
	printf("%d %d %d %d\n", scrwidth, scrheight, width, height);
	printf("%d\n\n", rebound);
	camera.outp();
	printf("%d\n\n", n);
	for(int i = 0; i < n; ++i)
		g[i]->outp();
	cnt++;
}

void clear(int st = 3)
{
	for(int i = st; i < n; ++i)
		delete g[i];
	n = st;
}

double pi = 3.141592653589;

void setGlobalBrightness(double v)
{
	((Sphere*)g[1])->m.brightness = v;
}

void setGlobalPointBrightness(double v)
{
	((Sphere*)g[2])->m.brightness = v;
}

double getGlobalBrightness()
{
	return ((Sphere*)g[1])->m.brightness;
}

double getGlobalPointBrightness()
{
	return ((Sphere*)g[2])->m.brightness;
}

void stage1(int typ = 1)
{
	double resolution = 200;
	double r = camera.pos.z;
	double y = camera.pos.y;
	double myBrightness[5] = {0, 0, 0, 0, 0};
	double tmpCol = 1.5;
	Point myCol[3] = {
		Point(0, 0, tmpCol),
		Point(tmpCol, 0, 0), 
		Point(0, tmpCol, 0)};
	double threshold = 0.4;
	double step = threshold/(resolution/2/5);
	double mywr = 0;
	double mywrSpd = 1.0 / 20;
	int nowtim = 0;
	for(double a = 0; a < 2 * pi; a += 2 * pi / resolution)
	{
		nowtim++;
		int siz = 30;
		camera.pos.x = sin(a) * r;
		camera.pos.y = y - y * 0.5 * sin(a / 2);
		camera.pos.z = cos(a) * r;
		{
			Sphere* p = new Sphere();
			p->pos = Point(2 * siz, siz, -2 * siz);
			p->r = siz;
			p->m = emptMaterial;
			p->m.brightness = 0;
			//p->m.wr = min(1.0, nowtim / 40.0);
			g[n++] = p;
		}
		Point st(-2 * siz, 0, 2 * siz);
		{
			double sigleBright = 0;
			int offset[18][3] = 
			{
				{0, 0, 0},	//0
				{0, 0, -2},
				{0, 0, -4},
				{2, 0, 0},	
				{2, 0, -2},
				{4, 0, 0},

				{1, 1, 0},	//6
				{0, 1, -1},
				{0, 1, -3},
				{1, 1, -2},
				{2, 1, -1},
				{3, 1, 0},
				
				{0, 2, 0},	//12
				{0, 2, -2},
				{2, 2, 0},
			
				{0, 3, -1},	//15
				{1, 3, 0},	

				{0, 4, 0}	//17
			};
			int nn = n;
			for(int i = 0; i < 18; ++i)
			{
				Cube* p = new Cube();
				p->pos = Point(
					offset[i][0] * siz + st.x,
					offset[i][1] * siz + st.y,
					offset[i][2] * siz + st.z);
				p->siz = Point(siz, siz, siz);
				p->m = emptMaterial;
				g[n++] = p;
			}
			if(typ)
			{
				double coi = 0.5;
				if(a < pi)
				{
					for(int i = 4; i >=0; --i)
					if(myBrightness[i]<threshold)
					{
						myBrightness[i] += step;
						break;
					}
				
					for(int i =0 ; i < 18; ++i)
					{
						Cube* tmp = (Cube*)g[nn+i];
						int typ = (int)(offset[i][0]+0.1);
						double myb = myBrightness[typ];
						double v = abs(offset[i][0]) + abs(offset[i][1]) + abs(offset[i][2]);
						double coif = myb /threshold * coi / 4.0* v/4.0;
						tmp->m.wd = Point(1 -  coif * 4.0,  1 - coif * (offset[i][0]), 1 - coif* (4 - offset[i][0]));
						tmp->m.brightness = myb;
						//tmp->m.wd = Point(1 - myb * myCol[typ%3].x,  1 - myb * myCol[typ%3].y, 1 - myb * myCol[typ%3].z);
					}
				}else
				{
					/*
					for(int i = 4; i >=0; --i)
					if(myBrightness[i]>0)
					{
						myBrightness[i] = max(0.0, myBrightness[i] - step);
						break;
					}
					*/
					mywr = min(1.0, mywr+mywrSpd);
					for(int i =0 ; i < 18; ++i)
					{
						Cube* tmp = (Cube*)g[nn+i];
						int typ = (int)(offset[i][0]+0.1);
						double myb = myBrightness[typ];
						double v = abs(offset[i][0]) + abs(offset[i][1]) + abs(offset[i][2]);
						double coif = myb /threshold * coi / 4.0* v/4.0;
						tmp->m.wd = Point(1 -  coif * 4.0,  1 - coif * (offset[i][0]), 1 - coif* (4 - offset[i][0]));
						tmp->m.wr = mywr;
						tmp->m.brightness = myb;
						//tmp->m.wd = Point(1 - myb * myCol[typ%3].x,  1 - myb * myCol[typ%3].y, 1 - myb * myCol[typ%3].z);
					}
				}
			}
		}
		
		outpAll();
		clear();
	}
	camera.pos = Point(0, y, r);
}

double sptr = 141.4213562;
double spty = 330;
double deltaa = 2 * pi / 200;
double alpha = 0.7853981634;

void changespt()
{
	alpha += deltaa;
	/*
	spta->pos = Point( -cos(alpha) * sptr, spty + cos(alpha) * sptofst, sin(alpha) * sptr);
	sptb->pos = Point( sin(alpha) * sptr, spty - cos(alpha) * sptofst, cos(alpha) * sptr);
	*/
	((Sphere*)g[2])->pos = Point(-sin(alpha) * sptr, spty, -cos(alpha) * sptr);
}

Point defCol[3] = { Point(1,0.8,1), Point(0.8,1,1), Point(1,1,0.8)};
int stn, enn;

void stage2()
{
	Point ori = camera.pos;
	{
		double cr = ori.z;
		double a = 0.6;
		camera.pos.z = cr * cos(a);
		camera.pos.x = cr * sin(a);
	}
	/*
	{
			Sphere* p = new Sphere();
			double r = 100;
			p->pos = Point(260, r, 80);
			p->r = r;
			p->m = emptMaterial;
			p->m.wl = 0.2;
			p->m.brightness = 0;
			g[n++] = p;
	}
	*/
	double v[1000];
	int bounceCnt[1000];
	memset(v, 0, sizeof(v));
	memset(bounceCnt, 0, sizeof(bounceCnt));
	int bounceLim = 6;
	int fin = 0;

	// 下落过程
	{
		double a = 1;
		double stz = 300;
		double zoffset = 300;
		double adec = 0.5;
		double adec2 = 0.3;
		
		for(int i = stn; i < enn; ++i)
		{
			Sphere* p = (Sphere*)g[i];
			p->pos.y = stz + randf() * zoffset;
		}

		while(fin < enn - stn)
		{
			for(int i = stn; i < enn; ++i)
			if(bounceCnt[i] != bounceLim)
			{
				v[i] += a;
				Sphere* p = (Sphere*)g[i];
				p->pos.y -= v[i];
				if(p->pos.y - p->r <=0)
				{
					bounceCnt[i] ++;
					v[i] = -v[i] * (bounceCnt[i] > 2 ? adec2 : adec);
					if(bounceCnt[i] == bounceLim)
					{
						p->pos.y = p->r;
						v[i] = 0;
						fin++;
					}
					p->pos.y = -(p->pos.y - p->r) + p->r;
				}
			}
			outpAll();
		}
	}

	// 关灯过程
	cerr<<"关灯"<<cnt<<endl;
	/*
	double targBrightness = 5;
	spta = new Sphere();
	spta->pos = Point(-200, 300, 0);
	spta->r = 100;
	spta->m = emptMaterial;
	spta->m.brightness = 0;
	spta->m.wd = Point(1,0.2,1);
	g[n++] = spta;
		
	sptb = new Sphere();
	sptb->pos = Point(0, 200, -200);
	sptb->r = 100;
	sptb->m = emptMaterial;
	sptb->m.brightness = 0;
	sptb->m.wd = Point(0.2,0.2,1);
	g[n++] = sptb;
	*/

	int tim = 40;
	double gb = getGlobalBrightness() / tim;
	double gp = getGlobalPointBrightness() / tim;
	int reso = 100;
	deltaa = 2 * pi / reso;
	for(int i = tim - 1; i >=0; --i)
	{
		setGlobalBrightness(gb * i);
		//setGlobalPointBrightness(gp * i);
		changespt();
		outpAll();
	}

	// 镜子球动态光照
	cerr<<"镜子球"<<cnt<<endl;
	for(int i = 0; i < reso* 3 / 4; ++i)
	{
		changespt();
		outpAll();
	}

	// 石膏球动态光照
	cerr<<"石膏球"<<cnt<<endl;
	double trans = reso / 4;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
			((Sphere*)g[j])->m.wl -= 1.0 / trans;
		changespt();
		outpAll();
	}

	for(int i = 0; i < reso* 1 / 4; ++i)
	{
		changespt();
		outpAll();
	}
	
	tim = 40;
	for(int i = tim - 1; i >=0; --i)
	{
		setGlobalPointBrightness(gp * i);
		changespt();
		outpAll();
	}

	double smalllight = 1.0;

	memset(bounceCnt, 0, sizeof(bounceCnt));
	for(int i = stn; i < enn; ++i)
		bounceCnt[i] = rand() %3;

	// 亮球
	cerr<<"亮球"<<cnt<<endl;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			((Sphere*)g[j])->m.brightness += smalllight / trans;
			if(bounceCnt[j] == 0)
				((Sphere*)g[j])->m.wd.x -= 0.2 / trans;
			else if (bounceCnt[j] == 1)
				((Sphere*)g[j])->m.wd.y -= 0.2 / trans;
			else
				((Sphere*)g[j])->m.wd.z -= 0.2 / trans;
		}
		changespt();
		outpAll();
	}

	// 玻璃球动态光照
	
	cerr<<"玻璃球"<<cnt<<endl;
	trans = tim;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			((Sphere*)g[j])->m.wr += 1.0 / trans;
			((Sphere*)g[j])->m.brightness -= smalllight / trans;
		}
		setGlobalPointBrightness(gp * i);
		changespt();
		outpAll();
	}
	
	for(int j = stn; j < enn; ++j)
		((Sphere*)g[j])->m.wd = Point(1,1,1);

	for(int i = 0; i < reso* 3 / 4; ++i)
	{
		changespt();
		outpAll();
	}

	// 贴图球动态光照
	// 灭灯
	cerr<<"贴图球灭"<<cnt<<endl;
	trans = tim;
	for(int i = tim-1; i >=0; --i)
	{
		setGlobalPointBrightness(gp * i);
		changespt();
		outpAll();
	}
	// 贴图
	for(int j = stn; j < enn; ++j)
	{
		((Sphere*)g[j])->m.texture = 0;
		v[j] = randf();
		((Sphere*)g[j])->m.tr = Point(0, cos(v[j]), sin(v[j]));
		((Sphere*)g[j])->m.tn = Point(1, 0, 0);
		((Sphere*)g[j])->m.wr = 0;
		((Sphere*)g[j])->m.wl = 0;
		((Sphere*)g[j])->m.wd = Point(1, 1, 1);
	}
	double vv = 0.1;
	//亮灯
	cerr<<"贴图球"<<cnt<<endl;
	trans = reso / 4;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			v[j] += vv;
			((Sphere*)g[j])->m.brightness += smalllight / trans;
			((Sphere*)g[j])->m.tr = Point(0, cos(v[j]), sin(v[j]));
			((Sphere*)g[j])->m.tn = Point(1, 0, 0);
		}
		changespt();
		outpAll();
	}
	
	for(int i = 0; i < reso / 2; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			v[j] += vv;
			((Sphere*)g[j])->m.tr = Point(0, cos(v[j]), sin(v[j]));
			((Sphere*)g[j])->m.tn = Point(1, 0, 0);
		}
		changespt();
		outpAll();
	}
	cerr<<"贴图球灭"<<cnt<<endl;
	trans = tim;
	for(int i = 0; i < tim; ++i)
	{
		setGlobalPointBrightness(gp * i);
		setGlobalBrightness(gb * i);
		for(int j = stn; j < enn; ++j)
		{
			((Sphere*)g[j])->m.brightness -= smalllight / trans;
			v[j] += vv;
			((Sphere*)g[j])->m.tr = Point(0, cos(v[j]), sin(v[j]));
			((Sphere*)g[j])->m.tn = Point(1, 0, 0);
		}
		changespt();
		outpAll();
	}

	cerr<<"转球"<<cnt<<endl;
	for(int i = 0; i < reso / 2; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			v[j] += vv;
			((Sphere*)g[j])->m.tr = Point(0, cos(v[j]), sin(v[j]));
			((Sphere*)g[j])->m.tn = Point(1, 0, 0);
		}
		changespt();
		outpAll();
	}

	cerr<<"变透明"<<cnt<<endl;
	trans = 40;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			v[j] += vv;
			((Sphere*)g[j])->m.tr = Point(0, cos(v[j]), sin(v[j]));
			((Sphere*)g[j])->m.tn = Point(1, 0, 0);
			((Sphere*)g[j])->m.wl += 1.0 / trans;
		}
		changespt();
		outpAll();
	}

	clear(enn);
}

int row = 10;
int col = 10;
double r0 = 25;
double k = 0.1;
double roffset = 20;
double gridwidth = r0 * 3.5;
double gridheight = r0 * 3.5;
double stx = -650;
double sty = -550;

int tim = 0;
int stLayout = 10;

double omega = 1.0 / 6.0 / 20.0 * ( 2.0 * pi );
double h = 100;
double wave[1000];

void setY(int j)
{
	int xx = (j - stn) / col;
	int yy = (j - stn) % col;
	if(tim <= stLayout  * ( xx + yy))
		return;
	wave[j] += omega;
	double y = -cos(wave[j]) * h;
	((Sphere*)g[j])->pos.y = ((Sphere*)g[j])->r + h + y;
}

void stage3()
{
	cerr<<"排列"<<cnt<<endl;
	camera.pos = Point(395.249731, 400, 577.734930);
	Point camTar(550, 400, 500);
	double trans = 40;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			int xx = (j - stn) / col;
			int yy = (j - stn) % col;
			double x = xx * gridwidth + stx;
			double y = yy * gridheight + sty;
			Sphere* p = ((Sphere*)g[j]);
			p->r += (r0 - p->r) * k;
			p->pos = Point((x - p->pos.x) * k + p->pos.x, p->r, (y - p->pos.z) * k + p->pos.z);
		}
		camera.pos = Point(	(camTar.x - camera.pos.x) * k + camera.pos.x,
							(camTar.y - camera.pos.y) * k + camera.pos.y,
							(camTar.z - camera.pos.z) * k + camera.pos.z);
		outpAll();
	}

	cerr<<"运动"<<cnt<<endl;
	trans = (col + row + 4) * stLayout;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
			setY(j);
		tim++;
		outpAll();
	}
	/*
	if(repeatTime < 10)
		repeatTime = 50;
	*/
	cerr<<"应用景深"<<cnt<<endl;
	camera.useDIF = 1;
	camera.DIFdist = 1000;
	camera.DIFoffset = 0;
	for(int i = 0; i < 20; ++i)
	{
		for(int j = stn; j < enn; ++j)
			setY(j);
		tim++;
		camera.useDIF = 1;
		camera.DIFdist = 1000;
		camera.DIFoffset = 0;
		outpAll();
	}
	
	cerr<<"调整深度"<<cnt<<endl;
	double dec = 10;
	trans = (camera.DIFdist - 300) / dec;
	double offsetInc = 20 / trans;
	for(int i = 0; i < trans; ++i)
	{
		camera.DIFoffset += offsetInc;
		camera.DIFdist -= dec;
		for(int j = stn; j < enn; ++j)
		{
			setY(j);
			((Sphere*)g[j])->m.wl -= 1.0 / trans;
		}
		tim++;
		outpAll();
	}
	

	cerr<<"变白"<<cnt<<endl;
	trans = 40;
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
		{
			setY(j);
			((Sphere*)g[j])->m.wl -= 1.0 / trans;
		}
		tim++;
		omega *= 0.9;
		outpAll();
	}
	
	for(int i = 0; i < trans; ++i)
	{
		for(int j = stn; j < enn; ++j)
			setY(j);
		tim++;
		omega *= 0.9;
		outpAll();
	}
	

	cerr<<"下落"<<cnt<<endl;
	double v[1000];
	int bounceCnt[1000];
	memset(v, 0, sizeof(v));
	memset(bounceCnt, 0, sizeof(bounceCnt));
	int bounceLim = 6;
	int fin = 0;

	// 下落过程
	double a = 5;
	double adec = 0.5;
	double adec2 = 0.3;
		
	while(fin < enn - stn)
	{
		for(int i = stn; i < enn; ++i)
		if(bounceCnt[i] != bounceLim)
		{
			v[i] += a;
			Sphere* p = (Sphere*)g[i];
			p->pos.y -= v[i];
			if(p->pos.y - p->r <=0)
			{
				bounceCnt[i] ++;
				v[i] = -v[i] * (bounceCnt[i] > 2 ? adec2 : adec);
				if(bounceCnt[i] == bounceLim)
				{
					p->pos.y = p->r;
					v[i] = 0;
					fin++;
				}
				p->pos.y = -(p->pos.y - p->r) + p->r;
			}
		}
		outpAll();
	}
	
	int col[1000];
	for(int i = stn; i < enn; ++i)
	{
		bounceCnt[i] = rand() % 60;
		col[i] = rand() % 3;
	}



	cerr<<"调整深度"<<cnt<<endl;
	double inc = 10;
	trans = (5000 - camera.DIFdist) / dec;
	for(int i = 0; i < trans; ++i)
	{
		camera.DIFdist += inc;
		camera.DIFoffset +=2;
		for(int j = stn; j < enn; ++j)
		if(i >= bounceCnt[j])
		{
			double d = (-cos((i - bounceCnt[j]) * 2 * pi / 40) + 1) / 2; //0~1
			d = sqrt(d);
			((Sphere*)g[j])->m.brightness =  d / 2.0;
			if(col[j] == 0)
				((Sphere*)g[j])->m.wd.x = 1 - 0.2 * d;
			else if(col[j] == 1)
				((Sphere*)g[j])->m.wd.y = 1 - 0.2 * d;
			else
				((Sphere*)g[j])->m.wd.z = 1 - 0.2 * d;
		}

		outpAll();
	}
	
}

int main()
{
	camera.pos = Point(0, 400, 700);
	camera.headup = Point(0, 1, 0);
	camera.lookat = Point(0,0,0);
	camera.aDir = 400;
	camera.useDIF = 0;
	camera.DIFdist = 300;
	camera.DIFoffset = 20;

	Sphere* p = new Sphere();
	p->pos = Point(0, -1e10, 0);
	p->r = 1e10;
	p->m = Material();
	p->m.brightness = 0;
	p->m.wd = Point(1,1,1);
	p->m.nr = 1.5;
	p->m.tn = Point();
	p->m.tr = Point();
	p->m.texture = -1;
	p->m.repeatx = 1;
	p->m.repeaty = 1;
	p->m.wl = p->m.wr = 0;

	emptMaterial = p->m;

	g[n++] = p;

	p = new Sphere();
	p->pos = Point(0, 1e10 + 1000, 0);
	p->r = 1e10;
	p->m = ((Sphere*)g[0])->m;
	p->m.brightness = 0.25;

	g[n++] = p;
	
	p = new Sphere();
	p->pos = Point(-100, 330, -100);
	p->r = 100;
	p->m = ((Sphere*)g[0])->m;
	p->m.brightness = 4;

	g[n++] = p;
	
	/*
	{
		// debug	
		scrwidth = 300;
		scrheight = 200;
		rebound = 5;
		repeatTime = 10;
	}
	*/
	stage1();
	
	stn = n;
	// 创建球阵
	for(int i = 0; i < row; ++i)
		for(int j = 0; j < col; ++j)
		{
			double r = r0 + randf() * roffset - roffset / 2.0;
			double x = i * gridwidth + randf() * (gridwidth - r) + stx;
			double y = j * gridheight + randf() * (gridheight - r) + sty;
			Sphere* p = new Sphere();
			p->pos = Point(x, r, y);
			p->r = r;
			p->m = emptMaterial;
			p->m.brightness = 0;
			p->m.wl = 1;
			g[n++] = p;
		}
	enn = n;
	//stage2();
	//stage3();

	return 0;
}

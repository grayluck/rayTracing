
#include "stdafx.h"

#include "work.h"
#include "graph.h"
#include "FileUtils.h"
#include <cmath>

DWORD WINAPI work( LPVOID lpParam ) 
{
	int tim = 0;
	double cnt = 0;
	int mytim =0 ;
	while(tim < repeatTime)
	{
		tim++;
//		getCollideObject(Line(Point(1,1,100), Point(1,1,0)));
//		int i  = halfw;
		/*
		Object* obj = getCollideObject(Line(Point(0,0,100),Point(0,0,0)));
		Point clr = obj->getPointFromLine(Line(Point(0,0,100),Point(0,0,0)), 0);
		*/
		globalCounter = 0;

		for(int i = 0;i < scrwidth; ++i)
			for(int j = 0; j < scrheight; ++j)
			{
				// ³¬²ÉÑù¿¹¾â³Ý
				double movx = randf(), movy = randf();
				// movx = movy = 0;
				Line l = camera->getSightLight(i + movx - halfw, j + movy - halfh);
				Object* obj = getCollideObject(l);
				Point clr;
				int tmpobj = debobj;
				if( obj == NULL)
					clr = Point();
				else
					clr = obj->getPointFromLine(l, 0);
				setPixel(scrwidth - i - 1, j, clr, tim);
			}

		fprintf(deb,"===============================\n%d\n", globalCounter), fflush(deb);
		mytim ++;
		/*
		if(tim % 10 == 0)
		{
			clearMem();
			tim = 0;
			camera = new CameraProspective(Point(-100 * cos(mytim*2.0*3.1415926535/300.0),100 * sin(mytim*2.0*3.1415926535/300.0) ,700),
				Point(0,1, 0), Point(0,0,0)
				,400, 0, 300, 20);
			saveToFile();
		}
		*/
		refresh();
		saveToFile(tim == repeatTime);
	}
	exit(0);
	return 0;
}

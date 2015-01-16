
#include "stdafx.h"
#include "FileUtils.h"
#include "common.h"

void loadStage()
{
	FILE* fi = fopen(stageName, "r");
	readConfigure(fi);
	camera = CameraProspective::readCamera(fi);
	fscanf(fi, "%d", &n);
	for(int i = 0; i < n; ++i)
	{
		int typ;
		fscanf(fi, "%d", &typ);
		switch(typ)
		{
		case 0:g[i] = new Sphere();g[i]->readObject(fi);break;
		case 1:g[i] = new Triangle();g[i]->readObject(fi);break;
		case 2:	readCube(fi, i);
				break;
//		case 3:	g[i] = new Obj();g[i]->readObject(fi);break;
		case 3: readObj(fi, i);break;
		}
	}
	fclose(fi);
}

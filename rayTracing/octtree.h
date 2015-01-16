
#pragma once

#include "common.h"

class OctNode
{
public:
	// 按照x*4 + y*2+z编号的儿子们
	int son[8];
	// 该节点含有的面片list
	int* lst;
	// 面片list内容多少
	int lstn;
	// 该包围盒的左下角和右上角（大概就这意思）
	Point a, b;
	// 是否有子孙
	int typ;
	void getBox();
};

// 构建八叉树。只会考虑三角形面片
void buildOctTree();

/**
 * 八叉树查找
 * \param p 八叉树节点index
 * \param l 欲求交的线
 **/
void octTreeSearch(int pno, Line& l, Object*& minobj, double& mind);

extern int octn;
extern OctNode* oct;

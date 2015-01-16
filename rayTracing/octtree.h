
#pragma once

#include "common.h"

class OctNode
{
public:
	// ����x*4 + y*2+z��ŵĶ�����
	int son[8];
	// �ýڵ㺬�е���Ƭlist
	int* lst;
	// ��Ƭlist���ݶ���
	int lstn;
	// �ð�Χ�е����½Ǻ����Ͻǣ���ž�����˼��
	Point a, b;
	// �Ƿ�������
	int typ;
	void getBox();
};

// �����˲�����ֻ�ῼ����������Ƭ
void buildOctTree();

/**
 * �˲�������
 * \param p �˲����ڵ�index
 * \param l ���󽻵���
 **/
void octTreeSearch(int pno, Line& l, Object*& minobj, double& mind);

extern int octn;
extern OctNode* oct;

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <iostream>
#include <time.h>
#include <stdlib.h>

typedef enum Color{ RED, BLACK } Color;
typedef struct Node{
	int key;
	int size;//�Ե�ǰ���Ϊ�������������������Ľ����
	Color color;
	Node* left;
	Node* parent;
	Node* right;
}Node;
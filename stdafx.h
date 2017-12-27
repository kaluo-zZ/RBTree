// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include <iostream>
#include <time.h>
#include <stdlib.h>

typedef enum Color{ RED, BLACK } Color;
typedef struct Node{
	int key;
	int size;//以当前结点为根的子树（包括本身）的结点数
	Color color;
	Node* left;
	Node* parent;
	Node* right;
}Node;
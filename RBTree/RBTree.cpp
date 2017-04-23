// RBTree.cpp : 定义控制台应用程序的入口点。
//
/*
	实验一：实现红黑树的基本算法，分别对整数n=20、40、60、80，随机生成n个互异的正整数(K1,K2,...,Kn且1<=Ki<=150)，
	用前n个正整数作为节点的关键字，向一棵初始空的红黑树中依次插入n个节点，统计算法运行所需时间，画出时间曲线。
	（红黑树采用三叉链表）
	实验二：对上述生成的红黑树，找出树中n/4小的节点和第n/2小的节点，并依次删除这两个节点，统计算法运行所需时间，画出时间曲线。
*/
#include "stdafx.h"

using namespace std;

class RBTree{
private:
	Node* nil = NULL;
	void LEFT_ROTATE(Node* x); //对x做左旋操作
	void RIGHT_ROTATE(Node* y); //对y做右旋操作
	void RB_INSERT_FIXUP(Node* z);//对可能违反红黑树性质的结点z 做调整
	void RB_TRANSPLANT(Node* u,Node* v);//将以v为根的子树替换以u为根的子树，结点u的双亲变为结点v的双亲，v成为u的双亲的相应孩子
	void RB_DELETE_FIXUP(Node*);
public:
	RBTree();
	Node* root = NULL;
	void RB_INSERT(int value);//将value插入到红黑树中
	void RB_DELETE(int value);//从红黑树中删除值为value的结点
	Node* RB_SEARCH(int value);//找值为value的结点，返回指向该结点的指针
	Node* RB_MINIMUM(Node* x);//找树x中最小值，返回指向最小值结点的指针
	Node* RB_MAXIMUN(Node* x);//找树x中最大值，返回指向最大值结点的指针
	Node* RB_SUCCESSOR(int value);//找值为value的结点的前驱，返回指向其前驱的指针
	void RB_TRAVERSAL(Node* x);//遍历红黑树
	Node* OS_SELECT(Node* x, int i);//在树x中查找第i小的结点
	int OS_RANK(Node* x);//返回红黑树中结点的秩
	~RBTree();
};
RBTree::RBTree(){
	nil = new Node();//哨兵nil
	nil->color = BLACK;
	nil->size = 0;
	root = nil;
}
void RBTree::RB_INSERT(int value){
	Node* y = nil;
	Node* x = root;
	Node* z = new Node();
	z->key = value;
	z->size = 1;
	while (x != nil){ //寻找适当位置插入z节点，循环结束后z所在位置为插入位置，y为z的父节点
		y = x;
		x->size = x->size + 1;
		if (z->key < x->key){
			x = x->left;
		}
		else{
			x = x->right;
		}
	}
	z->parent = y;
	if (y == nil){
		root = z;
	}
	else if (z->key < y->key){  //判断z是y的左孩子还是右孩子
		y->left = z;
	}
	else{
		y->right = z;
	}
	z->left = nil;
	z->right = nil;
	z->color = RED;
	RB_INSERT_FIXUP(z);
}
Node* RBTree::RB_SEARCH(int value){
	Node* x = root;
	while (x != nil && value != x->key){
		if (value < x->key)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}
void RBTree::RB_INSERT_FIXUP(Node* z){
	while (z->parent->color == RED){
		if (z->parent == z->parent->parent->left){
			Node* y = z->parent->parent->right;
			if (y->color == RED){
				//情况一，z的叔结点是红色的
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else{
				if (z == z->parent->right){
					//情况二，z的叔结点是黑色的，且z是其父结点的右孩子
					z = z->parent;
					LEFT_ROTATE(z);
				}
				//情况三，z的叔结点是黑色的，且z是其父结点的左孩子
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RIGHT_ROTATE(z->parent->parent);
			}
		}
		else{
			Node* y = z->parent->parent->left;
			if (y->color == RED){
				//情况一
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else{
				if (z == z->parent->left){
					//情况二
					z = z->parent;
					RIGHT_ROTATE(z);
				}
				//情况三
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				LEFT_ROTATE(z->parent->parent);
			}
			
		}
	}
	root->color = BLACK;
}
void RBTree::RB_DELETE(int value){
	Node* z = RB_SEARCH(value); //找到指向值为value的结点的指针
	Node* y = z,*x;
	Color y_original_color = y->color;
	if (z->left == nil){
		x = z->right;
		RB_TRANSPLANT(z,z->right);
	}
	else if (z->right == nil){
		x = z->left;
		RB_TRANSPLANT(z,z->left);
	}
	else{
		y = RB_MINIMUM(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else{
			RB_TRANSPLANT(y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		RB_TRANSPLANT(z,y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	y = z->parent;
	while (y != nil){
		y->size = y->size - 1;
		y = y->parent;
	}
	delete z; //回收z结点占用空间
	if (y_original_color == BLACK)
		RB_DELETE_FIXUP(x);

}
void RBTree::RB_DELETE_FIXUP(Node* x){
	Node* w;
	while (x != root && x->color == BLACK){
		if (x == x->parent->left){
			w = x->parent->right;
			if (w->color == RED){
				//情况一
				w->color = BLACK;
				x->parent->color = RED;
				LEFT_ROTATE(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK){\
				//情况二
				w->color = RED;
				x = x->parent;
			}
			else{
				if (w->right->color == BLACK){
					//情况三
					w->left->color = BLACK;
					w->color = RED;
					RIGHT_ROTATE(w);
					w = x->parent->right;
				}
				
				//情况四
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				LEFT_ROTATE(x->parent);
				x = root; //将x设置为根，while循环测试时，终止循环
			}
		}
		else{
			w = x->parent->left;
			if (w->color == RED){
				w->color = BLACK;
				x->parent->color = RED;
				RIGHT_ROTATE(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK){
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK){
					w->right->color = BLACK;
					w->color = RED;
					LEFT_ROTATE(w);
					w = x->parent->left;
				}
				
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				RIGHT_ROTATE(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}
void RBTree::RB_TRANSPLANT(Node* u, Node* v){
	if (u->parent == nil){ //如果u为根节点root
		root = v;
	}
	else if (u == u->parent->left){
		u->parent->left = v;
	}
	else{
		u->parent->right = v;
	}
	v->parent = u->parent;
}
void RBTree::RIGHT_ROTATE(Node* y){
	if (y == nil){
		return;
	}
	Node* x = y->left;
	y->left = x->right;
	if (x->right != nil){
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == nil){
		root = x;
	}
	else if(y== y->parent->left){
		y->parent->left = x;
	}
	else{
		y->parent->right = x;
	}
	x->right = y;
	y->parent = x;

	x->size = y->size;
	y->size = y->left->size + y->right->size + 1;
}
void RBTree::LEFT_ROTATE(Node* x){
	if (x == nil){
		return;
	}
	Node* y = x->right;
	x->right = y->left;
	if (y->left != nil){
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == nil){
		root = y;
	}
	else if (x == x->parent->left){
		x->parent->left = y;
	}
	else{
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;

	y->size = x->size;
	x->size = x->left->size + x->right->size + 1;
}
Node* RBTree::RB_MAXIMUN(Node* x){
	Node* y = x;
	while (y->right != nil){
		y = y->right;
	}
	return y;
}
Node* RBTree::RB_MINIMUM(Node* x){
	Node* y = x;
	while (y->left != nil){
		y = y->left;
	}
	return y;
}
Node* RBTree::RB_SUCCESSOR(int value){
	Node * x = RB_SEARCH(value);
	if (x->right != nil){
		return RB_MINIMUM(x->right);
	}
	Node* y = x->parent;
	while (y != nil && x == y->right){
		x = y;
		y = y->parent;
	}
	return y;
}
void RBTree::RB_TRAVERSAL(Node* x){
	if (x != nil){
		RB_TRAVERSAL(x->left);
		cout << x->key <<":"<<OS_RANK(x)<< "\t";
		RB_TRAVERSAL(x->right);
	}

}
Node* RBTree::OS_SELECT(Node* x,int i){
	int r = x->left->size + 1;
	if (i == r){
		return x;
	}
	else if (i < r){
		return OS_SELECT(x->left,i);
	}
	else{
		return OS_SELECT(x->right, i - r);
	}
}
int RBTree::OS_RANK(Node* x){
	int r = x->left->size + 1;
	Node* y = x;
	while (y != root){
		if (y == y->parent->right){
			r = r + y->parent->left->size + 1;
		}
		y = y->parent;
	}
	return r;
}
RBTree::~RBTree(){
	//回收红黑树内存空间
	Node* x = root;
	while (x != nil){
		while (x->left != nil){
			x = x->left;
		}
		if (x->right != nil){
			x = x->right;
		}
		else{//清除结点
			Node* y = x;
			x = x->parent; //回退一层
			if (x == nil){
				delete y;
			}
			else if (y == y->parent->left){
				y->parent->left = nil;
				delete y;
			}
			else{
				y->parent->right = nil;
				delete y;
			}

		}
	}
	delete nil;
}


class Shuffle{
private:
	void exchange(int& a, int& b){
		int temp = a;
		a = b;
		b = temp;
	}
public:
	void shuffle(int* data, int n){
		//洗牌算法
		srand((int)time(NULL));
		for (int i = 0; i < 150; i++)
			data[i] = i + 1;
		for (int i = 0; i < n; i++){
			int t = rand() % (150 - i) + i;
			exchange(data[i], data[t]);
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	RBTree tree;
	Shuffle shuf;
	int n;
	cout << "请输入n=(0<n<=150)";
	cin >> n;
	int data[150];
	
	
	shuf.shuffle(data, n);
	cout << "插入顺序如下：" << endl;
	for (int i = 0; i < n; i++){
		tree.RB_INSERT(data[i]);
		cout << data[i] << "\t";
	}
	cout << endl;
	cout << "中序遍历结果：" << endl;
	tree.RB_TRAVERSAL(tree.root);
	cout << endl;
	Node * node = tree.RB_MINIMUM(tree.root);
	cout <<"最小值："<< node->key << endl;
	node = tree.RB_MAXIMUN(tree.root);
	cout << "最大值：" << node->key << endl;
	
	cout << "请输入要删除的整数:";
	int k;
	cin >> k;
	tree.RB_DELETE(k);
	cout << "删除"<<k<<"后：" << endl;
	tree.RB_TRAVERSAL(tree.root);
	cout << endl;

	cout << "查找某个数的后继：";
	cin >> k;
	node = tree.RB_SUCCESSOR(k);
	cout << node->key << endl;

	node = tree.OS_SELECT(tree.root, n / 4);
	cout << "第" << n / 4 << "小的数：" << node->key << endl;
	node = tree.OS_SELECT(tree.root,n/2);
	cout << "第" << n / 2 << "小的数：" << node->key << endl;
	cout << endl;
	return 0;
}


/*犯过的错误：直接用用户给的n值确定data数组大小，结果在shuffle函数中时给数组赋值时是按长度150来的*/
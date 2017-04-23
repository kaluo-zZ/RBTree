// RBTree.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*
	ʵ��һ��ʵ�ֺ�����Ļ����㷨���ֱ������n=20��40��60��80���������n�������������(K1,K2,...,Kn��1<=Ki<=150)��
	��ǰn����������Ϊ�ڵ�Ĺؼ��֣���һ�ó�ʼ�յĺ���������β���n���ڵ㣬ͳ���㷨��������ʱ�䣬����ʱ�����ߡ�
	�������������������
	ʵ��������������ɵĺ�������ҳ�����n/4С�Ľڵ�͵�n/2С�Ľڵ㣬������ɾ���������ڵ㣬ͳ���㷨��������ʱ�䣬����ʱ�����ߡ�
*/
#include "stdafx.h"

using namespace std;

class RBTree{
private:
	Node* nil = NULL;
	void LEFT_ROTATE(Node* x); //��x����������
	void RIGHT_ROTATE(Node* y); //��y����������
	void RB_INSERT_FIXUP(Node* z);//�Կ���Υ����������ʵĽ��z ������
	void RB_TRANSPLANT(Node* u,Node* v);//����vΪ���������滻��uΪ�������������u��˫�ױ�Ϊ���v��˫�ף�v��Ϊu��˫�׵���Ӧ����
	void RB_DELETE_FIXUP(Node*);
public:
	RBTree();
	Node* root = NULL;
	void RB_INSERT(int value);//��value���뵽�������
	void RB_DELETE(int value);//�Ӻ������ɾ��ֵΪvalue�Ľ��
	Node* RB_SEARCH(int value);//��ֵΪvalue�Ľ�㣬����ָ��ý���ָ��
	Node* RB_MINIMUM(Node* x);//����x����Сֵ������ָ����Сֵ����ָ��
	Node* RB_MAXIMUN(Node* x);//����x�����ֵ������ָ�����ֵ����ָ��
	Node* RB_SUCCESSOR(int value);//��ֵΪvalue�Ľ���ǰ��������ָ����ǰ����ָ��
	void RB_TRAVERSAL(Node* x);//���������
	Node* OS_SELECT(Node* x, int i);//����x�в��ҵ�iС�Ľ��
	int OS_RANK(Node* x);//���غ�����н�����
	~RBTree();
};
RBTree::RBTree(){
	nil = new Node();//�ڱ�nil
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
	while (x != nil){ //Ѱ���ʵ�λ�ò���z�ڵ㣬ѭ��������z����λ��Ϊ����λ�ã�yΪz�ĸ��ڵ�
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
	else if (z->key < y->key){  //�ж�z��y�����ӻ����Һ���
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
				//���һ��z�������Ǻ�ɫ��
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else{
				if (z == z->parent->right){
					//�������z�������Ǻ�ɫ�ģ���z���丸�����Һ���
					z = z->parent;
					LEFT_ROTATE(z);
				}
				//�������z�������Ǻ�ɫ�ģ���z���丸��������
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RIGHT_ROTATE(z->parent->parent);
			}
		}
		else{
			Node* y = z->parent->parent->left;
			if (y->color == RED){
				//���һ
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else{
				if (z == z->parent->left){
					//�����
					z = z->parent;
					RIGHT_ROTATE(z);
				}
				//�����
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				LEFT_ROTATE(z->parent->parent);
			}
			
		}
	}
	root->color = BLACK;
}
void RBTree::RB_DELETE(int value){
	Node* z = RB_SEARCH(value); //�ҵ�ָ��ֵΪvalue�Ľ���ָ��
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
	delete z; //����z���ռ�ÿռ�
	if (y_original_color == BLACK)
		RB_DELETE_FIXUP(x);

}
void RBTree::RB_DELETE_FIXUP(Node* x){
	Node* w;
	while (x != root && x->color == BLACK){
		if (x == x->parent->left){
			w = x->parent->right;
			if (w->color == RED){
				//���һ
				w->color = BLACK;
				x->parent->color = RED;
				LEFT_ROTATE(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK){\
				//�����
				w->color = RED;
				x = x->parent;
			}
			else{
				if (w->right->color == BLACK){
					//�����
					w->left->color = BLACK;
					w->color = RED;
					RIGHT_ROTATE(w);
					w = x->parent->right;
				}
				
				//�����
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				LEFT_ROTATE(x->parent);
				x = root; //��x����Ϊ����whileѭ������ʱ����ֹѭ��
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
	if (u->parent == nil){ //���uΪ���ڵ�root
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
	//���պ�����ڴ�ռ�
	Node* x = root;
	while (x != nil){
		while (x->left != nil){
			x = x->left;
		}
		if (x->right != nil){
			x = x->right;
		}
		else{//������
			Node* y = x;
			x = x->parent; //����һ��
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
		//ϴ���㷨
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
	cout << "������n=(0<n<=150)";
	cin >> n;
	int data[150];
	
	
	shuf.shuffle(data, n);
	cout << "����˳�����£�" << endl;
	for (int i = 0; i < n; i++){
		tree.RB_INSERT(data[i]);
		cout << data[i] << "\t";
	}
	cout << endl;
	cout << "������������" << endl;
	tree.RB_TRAVERSAL(tree.root);
	cout << endl;
	Node * node = tree.RB_MINIMUM(tree.root);
	cout <<"��Сֵ��"<< node->key << endl;
	node = tree.RB_MAXIMUN(tree.root);
	cout << "���ֵ��" << node->key << endl;
	
	cout << "������Ҫɾ��������:";
	int k;
	cin >> k;
	tree.RB_DELETE(k);
	cout << "ɾ��"<<k<<"��" << endl;
	tree.RB_TRAVERSAL(tree.root);
	cout << endl;

	cout << "����ĳ�����ĺ�̣�";
	cin >> k;
	node = tree.RB_SUCCESSOR(k);
	cout << node->key << endl;

	node = tree.OS_SELECT(tree.root, n / 4);
	cout << "��" << n / 4 << "С������" << node->key << endl;
	node = tree.OS_SELECT(tree.root,n/2);
	cout << "��" << n / 2 << "С������" << node->key << endl;
	cout << endl;
	return 0;
}


/*�����Ĵ���ֱ�����û�����nֵȷ��data�����С�������shuffle������ʱ�����鸳ֵʱ�ǰ�����150����*/
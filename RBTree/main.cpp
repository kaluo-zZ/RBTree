


void exchange(int& a, int& b){
	int temp = a;
	a = b;
	b = temp;
}

void shuffle(int* data, int n){
	//œ¥≈∆À„∑®
	srand((int)time(NULL));
	for (int i = 0; i < 150; i++)
		data[i] = i + 1;
	for (int i = 0; i < n; i++){
		int t = rand() % (n - i) + i;
		exchange(data[i], data[t]);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	RBTree tree;
	int n;
	cout << "please input the n=(0<n<=150)";
	cin >> n;
	int* data = new int[n];
	shuffle(data, n);
	for (int i = 0; i < n; i++){
		tree.RB_INSERT(23);
	}

	for (int i = 0; i < n; i++){
		Node * node = tree.RB_MINIMUM(tree.root);
		cout << node->key << "\t";
		tree.RB_DELETE(node->key);
	}
	cout << endl;
	return 0;
}
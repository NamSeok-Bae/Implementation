#include<iostream>
#include<string>

using namespace std;

class Data {
public:
	int id, cap, price;
	string name;

	Data() {
		id = 0;
		cap = 0;
		price = 0;
		name = "";
	}

	Data(int id, string name, int capacity, int price)
	{
		this->id = id;
		this->name = name;
		this->cap = capacity;
		this->price = price;
	}
};

class Node {
public:
	Data* data;
	Node* parent;
	Node* left;
	Node* right;
	bool isBlacked;
	Node() {
		data = new Data();
		parent = left = right = NULL;
		isBlacked = false;
	}
};

class RBT {
public:
	Node* root;
	RBT() {
		root = NULL;
	}

	// ��� Ž�� , ���� ���� ����
	pair<Node*,int> searchNode(int x) {

		// cur: ���� Ž������ ��带 ����Ű�� ������
		// root���� ����
		Node* curr = root;
		int cnt = 0;

		//���� Ž�������� ��尡 ������ ����
		while (curr != NULL) {
			//ã���� �ϴ� Ű���� ã���� return
			if (curr->data->id == x)
			{
				return { curr,cnt };
			}
			//Ž������ ����� Ű���� ã�����ϴ� Ű������ ũ��
			else if (curr->data->id > x)
			{
				curr = curr->left;
				cnt++;
			}
			//Ž������ ����� Ű���� ã�����ϴ� Ű������ ������
			else
			{
				curr = curr->right;
				cnt++;
			}
		}
		return { NULL,-1 };
	}

	// ����� ���� ����
	void update(Node* node, string name, int capacity, int price)
	{
		node->data->name = name;
		node->data->cap = capacity;
		node->data->price = price;
	}

	//��� ��� Ž���ؼ� �ʿ��� ��带 Ž�� �� ���� ����
	void preOrder_change(Node* node, int x, int y, int p) {
		if (node == NULL) return;
		if (node->data->id >= x && node->data->id <= y)
			node->data->price = (node->data->price * (100 - p)) / 100;
		if (node->left != NULL)
			preOrder_change(node->left,x,y,p);
		if (node->right != NULL)
			preOrder_change(node->right,x,y,p);
	}

	int insert(int id, string name, int capacity, int price) {
		//���� �߰��� ��� ����
		Node* new_Node = new Node();
		Data* data = new Data(id, name, capacity, price);
		new_Node->data = data;

		//���ο� ��尡 �� �ڸ��� Ž��
		Node* curr = root;
		Node* parent = NULL;
		while (curr != NULL) {
			//par�� curr �����ϸ鼭 �� �ڸ� Ž��
			parent = curr;
			if (curr->data->id > new_Node->data->id)
			{
				curr = curr->left;
			}
			else if (curr->data->id < new_Node->data->id)
			{
				curr = curr->right;
			}
			else
				return searchNode(id).second;
		}
		//���࿡ rbt�� ���������
		if (parent == NULL)
		{
			new_Node->isBlacked = true;
			root = new_Node;
			return 0;
		}
		//Ž���� �ڸ��� �θ� �������ڽ����� ���� �����ڽ����� ���� ����
		else if (parent->data->id > new_Node->data->id)
		{
			parent->left = new_Node;
		}
		else
		{
			parent->right = new_Node;
		}

		//���� ������ ����� �θ� ����
		new_Node->parent = parent;
		fixTree(new_Node);
		return searchNode(new_Node->data->id).second;
	}

	void fixTree(Node* node)
	{
		// Ʈ�� ���� -> ���÷��� or ����Ʈ����

		Node* uncle; // ���� ���
		while (node != this->root && node->parent->isBlacked == false)
		{
			if (node->parent == node->parent->parent->right) // �θ� ������
			{
				uncle = node->parent->parent->left;
				if (uncle != NULL && uncle->isBlacked == false) //���� ������ -> ���÷���
				{
					node->parent->isBlacked = true;
					uncle->isBlacked = true;
					node->parent->parent->isBlacked = false;
					node = node->parent->parent;
				}
				else
				{
					restructuring(node);
					this->root->isBlacked = true;
					return;
				}
			}
			else if (node->parent == node->parent->parent->left) // �θ� ����
			{
				uncle = node->parent->parent->right;
				if (uncle != NULL && uncle->isBlacked == false) //���� ������ -> ���÷���
				{
					node->parent->isBlacked = true;
					uncle->isBlacked = true;
					node->parent->parent->isBlacked = false;
					node = node->parent->parent;
				}
				else
				{
					restructuring(node);
					this->root->isBlacked = true;
					return;
				}
			}
		}
		this->root->isBlacked = true;
	}

	void restructuring(Node* node)
	{
		Node* parent = node->parent; 
		Node* grandParent = parent->parent;

		if (parent == parent->parent->left) { // ���ʺθ� restructure
			if (node == parent->left) { // ���ʺθ� ���� �ڽ�
				// node�� �θ� ���� �ö󰡰�, node�� ���� �ڽ�, ������� ������ �ڽ��� �ȴ�.
				grandParent->left = parent->right;
				if (parent->right != NULL)
					parent->right->parent = grandParent;

				parent->parent = grandParent->parent;
				if (parent->parent == NULL)
					this->root = parent;
				else { 
					// ������ ��������� �Ǻ�
					if (grandParent == grandParent->parent->left)
						grandParent->parent->left = parent;
					else if (grandParent == grandParent->parent->right)
						grandParent->parent->right = parent;
				}
				parent->right = grandParent;
				grandParent->parent = parent;

				// coloring
				parent->isBlacked = true;
				grandParent->isBlacked = false;
				node->isBlacked = false;
			}
			else if (node == parent->right) { //���ʺθ� ������ �ڽ�
				// ��尡 ���� �ö󰡰�, �θ� ��尡 ����, ���� ��尡 ������
				grandParent->left = node->right;
				if (node->right != NULL)
					node->right->parent = grandParent;

				parent->right = node->left;
				if (node->left != NULL)
					node->left->parent = parent;

				node->parent = grandParent->parent;
				if (node->parent == NULL)
					this->root = node;
				else {
					// ������ ��������� �Ǻ�
					if (grandParent == grandParent->parent->left)
						grandParent->parent->left = node;
					else
						grandParent->parent->right = node;
				}
				node->left = parent;
				node->right = grandParent;
				parent->parent = node;
				grandParent->parent = node;

				//coloring
				node->isBlacked = true;
				grandParent->isBlacked = false;
				parent->isBlacked = false;
			}
		}
		else if (parent == parent->parent->right) { // ������ �θ� restructing
			if (node == parent->left) { //������ �θ� ���� �ڽ�
				// ��尡 ���� �ö�, ������ �����ڽ�, �θ� ������ �ڽ�
				parent->left = node->right;
				if (node->right != NULL)
					node->right->parent = parent;

				grandParent->right = node->left;
				if (node->left != NULL)
					node->left->parent = grandParent;

				node->parent = grandParent->parent;
				if (node->parent == NULL)
					this->root = node;
				else {
					if (grandParent == grandParent->parent->left)
						grandParent->parent->left = node;
					else
						grandParent->parent->right = node;
				}
				node->right = parent;
				node->left = grandParent;
				parent->parent = node;
				grandParent->parent = node;

				// coloring
				node->isBlacked = true;
				grandParent->isBlacked = false;
				parent->isBlacked = false;
			}
			else if (node == parent->right) { //������ �θ� ������ �ڽ�
				// �θ� ��尡 ���� �ö�, ������ �����ڽ�, ��尡 ������ �ڽ�
				grandParent->right = parent->left;
				if (parent->left != NULL)
					parent->left->parent = grandParent;

				parent->parent = grandParent->parent;
				if (parent->parent == NULL)
					this->root = parent;
				else {
					if (grandParent == grandParent->parent->left)
						grandParent->parent->left = parent;
					else
						grandParent->parent->right = parent;
				}
				parent->left = grandParent;
				grandParent->parent = parent;

				// coloring
				parent->isBlacked = true;
				grandParent->isBlacked = false;
				node->isBlacked = false;
			}
		}
	}
};

int main() {
	char sign; // ��ɾ�
	int t,id, cap, price; // ���ɼ�, ID, �뷮, ����
	string name; // �̸�
	int x, y, p; // �ּ� ID, �ִ�ID, ���η�
	RBT rbt; // �����Ʈ�� ��ü

	cin >> t;
	while (t--)
	{
		cin >> sign;
		if (sign == 'I') // Insert
		{
			cin >> id >> name >> cap >> price;
			int node_depth = rbt.insert(id, name, cap, price);
			cout << node_depth << "\n";
		}
		else if (sign == 'F') // Find
		{
			cin >> id;
			pair<Node*, int> result = rbt.searchNode(id);
			if (result.second == -1)
			{
				cout << "NULL\n";
			}
			else
			{
				cout << result.second << " " << result.first->data->name << " "
					<< result.first->data->cap << " " << result.first->data->price << "\n";
			}
		}
		else if (sign == 'R') // Update_Info
		{
			cin >> id >> name >> cap >> price;
			pair<Node*, int> result = rbt.searchNode(id);
			if (result.second == -1)
			{
				cout << "NULL\n";
			}
			else
			{
				rbt.update(result.first,name,cap,price);
				cout << result.second <<"\n";
			}
		}
		else if (sign == 'D') // Change_Price
		{
			cin >> x >> y >> p;
			rbt.preOrder_change(rbt.root, x, y, p);
		}
	}
	return 0;
}
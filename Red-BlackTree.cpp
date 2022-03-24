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

	// 노드 탐색 , 노드와 깊이 리턴
	pair<Node*,int> searchNode(int x) {

		// cur: 현재 탐색중인 노드를 가리키는 포인터
		// root부터 시작
		Node* curr = root;
		int cnt = 0;

		//현재 탐색가능한 노드가 있을때 까지
		while (curr != NULL) {
			//찾고자 하는 키값을 찾으면 return
			if (curr->data->id == x)
			{
				return { curr,cnt };
			}
			//탐색중인 노드의 키값이 찾고자하는 키값보다 크면
			else if (curr->data->id > x)
			{
				curr = curr->left;
				cnt++;
			}
			//탐색중인 노드의 키값이 찾고자하는 키값보다 작으면
			else
			{
				curr = curr->right;
				cnt++;
			}
		}
		return { NULL,-1 };
	}

	// 노드의 정보 갱신
	void update(Node* node, string name, int capacity, int price)
	{
		node->data->name = name;
		node->data->cap = capacity;
		node->data->price = price;
	}

	//모든 노드 탐색해서 필요한 노드를 탐색 후 가격 변경
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
		//새로 추가할 노드 생성
		Node* new_Node = new Node();
		Data* data = new Data(id, name, capacity, price);
		new_Node->data = data;

		//새로운 노드가 들어갈 자리를 탐색
		Node* curr = root;
		Node* parent = NULL;
		while (curr != NULL) {
			//par과 curr 갱신하면서 들어갈 자리 탐색
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
		//만약에 rbt가 비어있으면
		if (parent == NULL)
		{
			new_Node->isBlacked = true;
			root = new_Node;
			return 0;
		}
		//탐색한 자리의 부모에 오른쪽자식으로 갈지 왼쪽자식으로 갈지 결정
		else if (parent->data->id > new_Node->data->id)
		{
			parent->left = new_Node;
		}
		else
		{
			parent->right = new_Node;
		}

		//새로 생성한 노드의 부모 설정
		new_Node->parent = parent;
		fixTree(new_Node);
		return searchNode(new_Node->data->id).second;
	}

	void fixTree(Node* node)
	{
		// 트리 갱신 -> 리컬러링 or 리스트럭팅

		Node* uncle; // 삼촌 노드
		while (node != this->root && node->parent->isBlacked == false)
		{
			if (node->parent == node->parent->parent->right) // 부모 오른쪽
			{
				uncle = node->parent->parent->left;
				if (uncle != NULL && uncle->isBlacked == false) //삼촌 빨간색 -> 리컬러링
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
			else if (node->parent == node->parent->parent->left) // 부모 왼쪽
			{
				uncle = node->parent->parent->right;
				if (uncle != NULL && uncle->isBlacked == false) //삼촌 빨간색 -> 리컬러링
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

		if (parent == parent->parent->left) { // 왼쪽부모 restructure
			if (node == parent->left) { // 왼쪽부모 왼쪽 자식
				// node의 부모가 위로 올라가고, node는 왼쪽 자식, 조상노드는 오른쪽 자식이 된다.
				grandParent->left = parent->right;
				if (parent->right != NULL)
					parent->right->parent = grandParent;

				parent->parent = grandParent->parent;
				if (parent->parent == NULL)
					this->root = parent;
				else { 
					// 조상이 어느쪽인지 판별
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
			else if (node == parent->right) { //왼쪽부모 오른쪽 자식
				// 노드가 위로 올라가고, 부모 노드가 왼쪽, 조상 노드가 오른쪽
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
					// 조상이 어느쪽인지 판별
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
		else if (parent == parent->parent->right) { // 오른쪽 부모 restructing
			if (node == parent->left) { //오른쪽 부모 왼쪽 자식
				// 노드가 위로 올라감, 조상이 왼쪽자식, 부모가 오른쪽 자식
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
			else if (node == parent->right) { //오른쪽 부모 오른쪽 자식
				// 부모 노드가 위로 올라감, 조상이 왼쪽자식, 노드가 오른쪽 자식
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
	char sign; // 명령어
	int t,id, cap, price; // 테케수, ID, 용량, 가격
	string name; // 이름
	int x, y, p; // 최소 ID, 최대ID, 할인률
	RBT rbt; // 레드블랙트리 객체

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
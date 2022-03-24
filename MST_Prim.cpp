#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum direction { MIN = 1, MAX = -1 };

// MIN Heap ���� ������ Prim �˰���

int n, m, q; //������ ������ �� n, ��å�� ������ �� m, ������ �� q
vector<pair<int,int>> v[10001]; //��������Ʈ��� �׷���

// MIN Heap
class heap {
public:
	vector<pair<int,int>> list; //�������� ��å�� ��� ����
	vector<int> save_list; // �湮�� ������ ������� ����.
	int save_cnt; // �湮�� �������� ��
	int save_cost; // ������ ��å���� ��� ����
	int root_index; // root �� ��Ÿ�������� index
	int direction; // MIN, MAX �����ϱ� ���� ����
	int heap_size; // Heap�� ũ��

	heap() {
		list.push_back({ -1,0 });
		save_cnt = 0;
		save_cost = 0;
		root_index = 1;
		direction = MIN;
		heap_size = 0;
	}

	void swap(int a, int b) { // upheap, downheap �Ͼ �� �ٲ��ֱ� ����.
		pair<int,int> temp = list[a];
		list[a] = list[b];
		list[b] = temp;
	}

	void insert(int index, int cost)
	{
		list.push_back({ cost,index }); // fisrt�� ���, second�� ������ ����
		heap_size++;
		upheap(heap_size);
	}

	// heap�� ���Ұ� insert ���� �� ���ο� ���Ұ� parent���� ������ swap() ȣ��
	// �̶� first�� ���ϰ� ���� ������ second�� �� �� second�� ������ swap() ȣ��
	// �־��� ���� ������忡�� ��Ʈ������ �ö�� �� O(logN) �ð��� �ɸ��ϴ�.
	void upheap(int index) { 
		if (index == 1) return;
		int parent = index / 2;
		if (list[parent].first * direction > list[index].first * direction)
		{
			swap(parent, index);
			upheap(parent);
		}
		else if (list[parent].first * direction == list[index].first * direction)
		{
			if (list[parent].second * direction > list[index].second * direction)
			{
				swap(parent, index);
				upheap(parent);
			}
		}
	}

	// heap�� ��������� ������ȯ
	void deletion()
	{
		if (heap_size == 0) { cout << "-1" << endl; return; }
		list[root_index] = list[heap_size];
		list.pop_back();
		heap_size--;
		downheap(root_index);
		return;
	}

	// heap ���� root�� delete ���� �� ȣ��
	// root�������� �����Ͽ� ���ʰ� ������ �� �� ���� �ڽİ� swap �ϰ� �ڽ����� downheap
	// �־��� ���� ��Ʈ��忡�� ���������� �������� �� O(logN) �ð��� �ɸ��ϴ�.
	void downheap(int index) {
		int left = index * 2;
		int right = index * 2 + 1;

		// ������ �ڽ��� ���� ��
		if (right <= heap_size) 
		{
			// left �ڽİ� right �ڽ��� first �� �� left �� �� ���� ��
			if (list[left].first * direction < list[right].first * direction)
			{
				// left �� index�� first �� �� left �� �� ������ swap
				if (list[left].first * direction < list[index].first * direction)
				{
					swap(left, index);
					downheap(left);
				}
				// left �� index �� ������ second ��
				else if (list[left].first * direction == list[index].first * direction)
				{
					if (list[left].second * direction < list[index].second * direction)
					{
						swap(left, index);
						downheap(index);
					}
					else return;
				}
				else return;
			}
			// left �� right �� first�� ���� ��
			else if (list[left].first * direction == list[right].first * direction)
			{
				// left �� right �� second �� --> left�� ���� ��
				if (list[left].second * direction <= list[right].second * direction)
				{
					// left�� index�� first �� �� index���� ������ swap
					if (list[left].first * direction < list[index].first * direction)
					{
						swap(left, index);
						downheap(left);
					}
					// left �� index�� first�� ������ second ��
					else if (list[left].first * direction == list[index].first * direction)
					{
						if (list[left].second * direction < list[index].second * direction)
						{
							swap(left, index);
							downheap(index);
						}
						else return;
					}
					else return;
				}
				// right�� first�� ���� ��
				else
				{
					// right �� index�� first ��
					if (list[right].first * direction < list[index].first * direction)
					{
						swap(right, index);
						downheap(right);
					}
					// ������ second ��
					else if (list[right].first * direction == list[index].first * direction)
					{
						if (list[right].second * direction < list[index].second * direction)
						{
							swap(right, index);
							downheap(right);
						}
						else return;
					}
					else return;
				}
			}
			// right�� left ���� ���� ��
			else {
				// right �� index�� first ��
				if (list[right].first * direction < list[index].first * direction)
				{
					swap(right, index);
					downheap(right);
				}
				// ������ second ��
				else if (list[right].first * direction == list[index].first * direction)
				{
					if (list[right].second * direction < list[index].second * direction)
					{
						swap(right, index);
						downheap(right);
					}
					else return;
				}
			}
		}
		// ���� �ڽĸ� ���� ��
		else if (left <= heap_size)
		{
			// left �� index�� first ��
			if (list[left].first * direction < list[index].first * direction)
			{
				swap(left, index);
				downheap(left);
			}
			// ������ second ��
			else if (list[left].first * direction == list[index].first * direction)
			{
				if (list[left].second * direction < list[index].second * direction)
				{
					swap(left, index);
					downheap(left);
				}
				else return;
			}
			else return;
		}
		// �ڽ��� ���� ��
		else
			return;
	}

	// ���� ��Ʈ ������ ��ȯ
	// list �迭���� ������ �������� ������ O(1) �ð�
	pair<int,int> top() {
		if (isEmpty()) return { -1,0 };
		else
			return list[root_index];
	}

	// ���� ��������� true, �ƴϸ� false
	bool isEmpty() {
		if (heap_size == 0) return true;
		else return false;
	}

	// ���� ���� ���¸� list�� ���
	// ��� ����� ���¸� ���⋚���� O(N) �ð�
	void print() {
		if (isEmpty() == false) {
			for (int i = root_index; i <= heap_size; i++) {
				cout << list[i].first << " " << list[i].second << "\n";
			}
			cout << endl;
		}
		else cout << -1 << endl;
	}

	// �湮�� �������� ������ ����
	void save(int index, int cost) {
		save_cnt++;
		save_list.push_back( index );
		save_cost += cost;
	}

	// �湮 ������� �������� ���
	// ��� ��带 �湮���� �� N�� �̹Ƿ� O(N) �ð�
	void print_save() {
		cout << save_cost << " ";
		for (int i = 0; i < save_cnt; i++)
		{
			cout << save_list[i] << " ";
		}
		cout << "\n";
	}
};

int main()
{
	cin >> n >> m >> q;

	//��������Ʈ��� �׷��� �Է�
	for (int i = 0; i < m; i++)
	{
		int first, second, cost;
		cin >> first >> second >> cost;
		v[first].push_back({ second,cost });
		v[second].push_back({ first,cost });
	}

	// ���� �Է�
	for (int j = 0; j < q; j++)
	{
		string s;
		cin >> s;

		// ���Ǵ� O(1) �ð��̰� �����˰����� �����ϸ鼭 �ɸ��� �ð���
		// heap�� insert�ϴ� ���� N * logN --> NlogN
		// deletion �ϴ� ���� N * logN --> NlogN
		// �� NlogN + NlogN �̹Ƿ� O(NlogN)�� �ɸ��ϴ�.
		if (s == "P") 
		{
			// ��� ����
			int start;
			cin >> start;

			// heap ����
			heap h;
			h.save(start, 0);

			// �湮�� ���� ó��
			bool visit[10001] = { 0 };
			visit[start] = true;

			// ����������� �� �� �ִ� ���� �Է�
			for (int i = 0; i < v[start].size(); i++)
			{
				h.insert( v[start][i].first, v[start][i].second );
			}

			while (!h.isEmpty())
			{
				int cur = h.top().second; // ���� �湮�� ����
				int cost = h.top().first; // ������ ��å���� ���
				h.deletion();

				// �湮���� ���� ���� �湮
				if (!visit[cur])
				{
					visit[cur] = true;

					h.save(cur,cost);

					// ���� �湮�� �������� �� �� �ִ� ���� �Է�
					for (int i = 0; i < v[cur].size(); i++)
					{
						h.insert(v[cur][i].first, v[cur][i].second);
					}
				}
			}

			//���
			h.print_save();
		}
	}
}
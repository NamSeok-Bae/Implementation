#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum direction { MIN = 1, MAX = -1 };

// MIN Heap 으로 구현한 Prim 알고리즘

int n, m, q; //조형물 정보의 수 n, 산책로 정보의 수 m, 질의의 수 q
vector<pair<int,int>> v[10001]; //인접리스트기반 그래프

// MIN Heap
class heap {
public:
	vector<pair<int,int>> list; //조형물과 산책로 비용 저장
	vector<int> save_list; // 방문한 조형물 순서대로 저장.
	int save_cnt; // 방문한 조형물의 수
	int save_cost; // 지나간 산책로의 비용 저장
	int root_index; // root 를 나타내기위한 index
	int direction; // MIN, MAX 구분하기 위한 변수
	int heap_size; // Heap의 크기

	heap() {
		list.push_back({ -1,0 });
		save_cnt = 0;
		save_cost = 0;
		root_index = 1;
		direction = MIN;
		heap_size = 0;
	}

	void swap(int a, int b) { // upheap, downheap 일어날 때 바꿔주기 위함.
		pair<int,int> temp = list[a];
		list[a] = list[b];
		list[b] = temp;
	}

	void insert(int index, int cost)
	{
		list.push_back({ cost,index }); // fisrt로 비용, second로 정점을 저장
		heap_size++;
		upheap(heap_size);
	}

	// heap에 원소가 insert 됐을 때 새로운 원소가 parent보다 작으면 swap() 호출
	// 이때 first를 비교하고 만약 같으면 second를 비교 후 second가 작으면 swap() 호출
	// 최악의 순간 리프노드에서 루트노드까지 올라올 때 O(logN) 시간이 걸립니다.
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

	// heap이 비어있으면 에러반환
	void deletion()
	{
		if (heap_size == 0) { cout << "-1" << endl; return; }
		list[root_index] = list[heap_size];
		list.pop_back();
		heap_size--;
		downheap(root_index);
		return;
	}

	// heap 에서 root가 delete 됐을 때 호출
	// root에서부터 시작하여 왼쪽과 오른쪽 중 더 작은 자식과 swap 하고 자식으로 downheap
	// 최악의 순간 루트노드에서 리프노드까지 내려갔을 떄 O(logN) 시간이 걸립니다.
	void downheap(int index) {
		int left = index * 2;
		int right = index * 2 + 1;

		// 오른쪽 자식이 존재 시
		if (right <= heap_size) 
		{
			// left 자식과 right 자식의 first 비교 후 left 가 더 작을 때
			if (list[left].first * direction < list[right].first * direction)
			{
				// left 와 index의 first 비교 후 left 가 더 작으면 swap
				if (list[left].first * direction < list[index].first * direction)
				{
					swap(left, index);
					downheap(left);
				}
				// left 와 index 가 같으면 second 비교
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
			// left 와 right 의 first가 같을 때
			else if (list[left].first * direction == list[right].first * direction)
			{
				// left 와 right 의 second 비교 --> left가 작을 때
				if (list[left].second * direction <= list[right].second * direction)
				{
					// left와 index의 first 비교 후 index보다 작으면 swap
					if (list[left].first * direction < list[index].first * direction)
					{
						swap(left, index);
						downheap(left);
					}
					// left 와 index의 first가 같으면 second 비교
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
				// right의 first가 작을 때
				else
				{
					// right 와 index의 first 비교
					if (list[right].first * direction < list[index].first * direction)
					{
						swap(right, index);
						downheap(right);
					}
					// 같으면 second 비교
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
			// right가 left 보다 작을 때
			else {
				// right 와 index의 first 비교
				if (list[right].first * direction < list[index].first * direction)
				{
					swap(right, index);
					downheap(right);
				}
				// 같으면 second 비교
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
		// 왼쪽 자식만 있을 시
		else if (left <= heap_size)
		{
			// left 와 index의 first 비교
			if (list[left].first * direction < list[index].first * direction)
			{
				swap(left, index);
				downheap(left);
			}
			// 같으면 second 비교
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
		// 자식이 없을 시
		else
			return;
	}

	// 힙의 루트 정보를 반환
	// list 배열에서 정보를 가져오기 때문에 O(1) 시간
	pair<int,int> top() {
		if (isEmpty()) return { -1,0 };
		else
			return list[root_index];
	}

	// 힙이 비어있으면 true, 아니면 false
	bool isEmpty() {
		if (heap_size == 0) return true;
		else return false;
	}

	// 현재 힙의 상태를 list로 출력
	// 모든 노드의 상태를 보기떄문에 O(N) 시간
	void print() {
		if (isEmpty() == false) {
			for (int i = root_index; i <= heap_size; i++) {
				cout << list[i].first << " " << list[i].second << "\n";
			}
			cout << endl;
		}
		else cout << -1 << endl;
	}

	// 방문한 조형물의 순서를 저장
	void save(int index, int cost) {
		save_cnt++;
		save_list.push_back( index );
		save_cost += cost;
	}

	// 방문 순서대로 조형물을 출력
	// 모든 노드를 방문했을 떄 N개 이므로 O(N) 시간
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

	//인접리스트기반 그래프 입력
	for (int i = 0; i < m; i++)
	{
		int first, second, cost;
		cin >> first >> second >> cost;
		v[first].push_back({ second,cost });
		v[second].push_back({ first,cost });
	}

	// 질의 입력
	for (int j = 0; j < q; j++)
	{
		string s;
		cin >> s;

		// 질의는 O(1) 시간이고 프림알고리즘을 실행하면서 걸리는 시간은
		// heap에 insert하는 과정 N * logN --> NlogN
		// deletion 하는 과정 N * logN --> NlogN
		// 총 NlogN + NlogN 이므로 O(NlogN)이 걸립니다.
		if (s == "P") 
		{
			// 출발 정점
			int start;
			cin >> start;

			// heap 생성
			heap h;
			h.save(start, 0);

			// 방문한 정점 처리
			bool visit[10001] = { 0 };
			visit[start] = true;

			// 출발지점에서 갈 수 있는 정점 입력
			for (int i = 0; i < v[start].size(); i++)
			{
				h.insert( v[start][i].first, v[start][i].second );
			}

			while (!h.isEmpty())
			{
				int cur = h.top().second; // 현재 방문한 정점
				int cost = h.top().first; // 지나온 산책로의 비용
				h.deletion();

				// 방문하지 않은 곳만 방문
				if (!visit[cur])
				{
					visit[cur] = true;

					h.save(cur,cost);

					// 현재 방문한 정점에서 갈 수 있는 정점 입력
					for (int i = 0; i < v[cur].size(); i++)
					{
						h.insert(v[cur][i].first, v[cur][i].second);
					}
				}
			}

			//출력
			h.print_save();
		}
	}
}
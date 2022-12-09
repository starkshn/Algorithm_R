#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

#include "RedBlackTree.h"
#include "Bst.h"
#include <thread>
#include <list>


# pragma region ArrayQueue
// [front][][][][][][][][][][][][back]
template <typename T>
class ArrayQueue
{
public:
    void push(const T& v)
    {
        // 다 찼는지 체크
        if (_size == _container.size())
        {
            // 증설
            int newSize = max(1, _size * 2);
            vector<T> newData;
            newData.resize(newSize);

            for (int i = 0; i < _size; ++i)
            {
                int index = (_front + i) % _container.size();
                newData[i] = _container[index];
            }

            _container.swap(newData);
            _front = 0;
            _back = _size;
        }

        _container[_back] = v;
        _back = (_back + 1) % _container.size();
        ++_size;
    }

    void pop()
    {
        _front = (_front + 1) % _container.size();
        --_size;
    }

    T& front()
    {
        return _container[_front];
    }

    bool empty() { return _size == 0; }
    int size() { return _size; }

public:
    vector<T>   _container;

    int         _front = 0;
    int         _back = 0;
    int         _size = 0;

};
#pragma endregion

vector<bool>        v;
vector<vector<int>> ad;
vector<bool>        discovered;

std::vector<std::vector<int>>	adjacent;

void CreateGraphList()
{
    v.resize(6);

    // 인접 리스트 방법
    ad = vector<vector<int>>(6);
    ad[0].push_back(1);
    ad[0].push_back(3);
    ad[1].push_back(0);
    ad[1].push_back(2);
    ad[1].push_back(3);
    ad[3].push_back(4);
    ad[5].push_back(4);

    // 인접 행렬
   /* ad = vector<vector<int>>
    {
        {0, 1, 0, 1, 0, 0},
        {1, 0, 1, 1 , 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
    };*/
}

void CreateGraphVec()
{
    adjacent = std::vector<std::vector<int>>(6, std::vector<int>(6, -1));

    adjacent[0][1] = 15;
    adjacent[0][3] = 35;

    adjacent[1][0] = 15;
    adjacent[1][2] = 5;
    adjacent[1][3] = 10;

    adjacent[3][4] = 5;
    adjacent[5][4] = 5;
}


#pragma region Binary Search
template <typename T>
concept Container = requires(T t)
{
    t.begin();
    t.end();
};

template <typename T>
requires Container<T>
void BinarySearch(int v, std::vector<T> vec)
{
    int left = 0;
    int right = vec.size() - 1;

    int count = 0;

    while (left <= right)
    {
        int mid = (left + right) / 2;

        cout << "수행 횟수 : " << count << endl;
        cout << "범위 : " << left << "~" << right << endl;

        if (v < vec[mid])
        {
            right = mid - 1;
        }
        else if (v > vec[mid])
        {
            left = mid + 1;
        }
        else
        {
            cout << "find : " << v << endl;
            break;
        }

        ++count;
    }
}
#pragma endregion

#pragma region DFS
// DFS
void DFS(int here)
{
    // 방문 
    // 인접 리스트 버전
    v[here] = true;
    cout << "Visitied : " << here << endl;

    /* for (int i = 0; i < ad[here].size(); ++i)
     {
         int there = ad[here][i];
         if (v[there] == false)
             DFS(there);
     }*/

     // 인접 행렬 버전
    for (int there = 0; there < 6; ++there)
    {
        if (ad[here][there] == 0)
            continue;

        if (v[there] == false)
            DFS(there);

    }
}
#pragma endregion

#pragma region BFS
void BFS(int here)
{
    // 누구에게 발견되었는지?
    vector<int> parent(6, -1);
    // 시작점에서 얼만큼 떨여져 있는지?
    vector<int> distance(6, -1);

    queue<int> q;
    q.push(here);
    discovered[here] = true;

    parent[here] = here;
    distance[here] = 0;

    while (q.empty() == false) // 0
    {
        here = q.front();
        q.pop();

        cout << "Vistied : " << here << endl;

        for (int there : ad[here])
        {
            /*if (discovered[here][there] == 0)
                continue;*/

            if (discovered[there])
                continue;

            q.push(there);
            discovered[there] = true;

            parent[there] = here;
            distance[there] = distance[here] + 1;
        }
    }
}

void BFS_ALL()
{
    for (int i = 0; i < 6; ++i)
        if (discovered[i] == false)
            BFS(i);
}
#pragma endregion

#pragma region Dijkstra
void Dijkstra(int here)
{
    std::pair<int, int>				vertexCost;
    std::list<std::pair<int, int>>	discovered;			// 발견 목록
    std::vector<int>				best(6, INT32_MAX);	// 각 정점별로 지금까지 발견한 최소거리
    std::vector<int>				parent(6, -1);

    // discovered.push_back(std::make_pair(here, 0));
    discovered.emplace_back(here, 0);

    best[here] = 0;
    parent[here] = here;

    while (discovered.empty() == false)
    {
        // 제일 좋은 후보를 찾는다.
        std::list<std::pair<int, int>>::iterator bestIter;
        int bestCost = INT32_MAX;

        for (auto iter = discovered.begin(); iter != discovered.end(); ++iter)
        {
            const int vertex = iter->first;
            const int cost = iter->second;

            if (cost < bestCost)
            {
                bestCost = cost;
                bestIter = iter;
            }
        }

        int cost = bestIter->second;
        here = bestIter->first;
        discovered.erase(bestIter);

        // 방문? 더 짧은 경로를 뒤늦게 찾았다면 스킵.
        if (best[here] < cost)
            continue;

        // 방문! (인접한 애들 다 등록을 해주어여한다)
        for (int there = 0; there < 6; ++there)
        {
            // 연결되지 않았다면 스킵
            if (adjacent[here][there] == -1)
                continue;

            // 더 좋은 경로를 과거에 찾았다면 스킵.
            int nextCost = best[here] + adjacent[here][there];
            if (nextCost >= best[there])
                continue;

            best[there] = nextCost;
            parent[there] = here;

            discovered.emplace_back(there, nextCost);
        }
    }
}


#pragma endregion

#pragma region Sorting

// 1) 버블 정렬 (Bubble Sort)
void BubbleSort(vector<int>& v)
{
    const int n = (int)v.size();

    // (N-1) + (N-2) + ... + 2 + 1
    // 등차수열의 합 = N * (N-1) / 2
    // O(N^2)
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < (n - 1 - i); j++)
        {
            if (v[j] > v[j + 1])
            {
                std::swap(v[j+1], v[j]);
            }
        }
    }
}

// [3][5][9][J][K]

// 2) 선택 정렬 (Selection Sort)
void SelectionSort(vector<int>& v)
{
    const int n = static_cast<int>(v.size());

    for (int i = 0; i < n - 1; ++i)
    {
        int bestIdx = i;

        for (int j = i + 1; j < n; ++j)
        {
            if (v[j] < v[bestIdx])
                bestIdx = j;
        }

        std::swap(v[bestIdx], v[i]);
    }
}

void SelectionSort2(std::vector<int>& vec)
{
    const int size = static_cast<int>(vec.size());
    
    for (int i = 0; i < size - 1; ++i)
    {
        int bestIdx = i;                        // 0
        
        for (int j = i + 1; j < size; ++j)      // j = 1
        {
            if (vec[j] < vec[bestIdx])          // vec[j] == 20 < vec[bestIdx] == 30
                bestIdx = j;
        }
        
        std::swap(vec[bestIdx], vec[i]);
    }
}

// 3) 삽입 정렬 (Insertion Sort)
// 
// i = 2
// insertData = [5]
// [5][4][9][K][3]
// 
void InsertionSort(vector<int>& v)
{
    const int n = static_cast<int>(v.size());

    // O(N^2)
    for (int i = 1; i < n; ++i)
    {
        int insertData = v[i]; // [5]

        int j;
        for (j = i - 1; j >= 0; --j)
        {
            if (v[j] > insertData)
                std::swap(v[j + 1], v[j]);    // [5][4] => [5][5]
            else
                break;
        }

        // v[j + 1] = insertData;
    }
}
// 힙 정렬
void HeapSort(vector<int>& v)
{
    priority_queue<int, vector<int>, greater<int>> pq;

    // O(NlogN)
    for (int num : v)
        pq.push(num);

    v.clear();

    // O(NlogN)
    while (pq.empty() == false)
    {
        v.push_back(pq.top());
        pq.pop();
    }
}


// 병합 정렬
// 분할 정복 (Divide and Conquer)
// - 분할 (Divide)		문제를 더 단순하게 분할한다
// - 정복 (Conquer)		분할된 문제를 해결
// - 결합 (Combine)		결과를 취합하여 마무리

// [3][K][7][2][J][4][8][9]			8개 * 1
// [3][K][7][2] [J][4][8][9]		4개 * 2
// [3][K] [7][2] [J][4] [8][9]		2개 * 4
// [3] [K] [7] [2] [J] [4] [8] [9]	1개 * 8
// [3][K] [2][7]  [4][J]  [8][9]	2개 * 4


// 테스트 문제 (병합 정렬)
// 이미 정렬이 되어있는 vec1, vec2를 하나의 벡터에 정렬된 상태로 만들어라.
vector<int> MergeFunc(vector<int>& vec1, vector<int>& vec2)
{
    vector<int> temp;

    int leftVecIdx = 0;
    int rightVecIdx = 0;

    int leftVecMaxIdx = vec1.size() - 1;
    int rightVecMaxIdx = leftVecMaxIdx + vec2.size();

    temp.reserve(vec1.size() + vec2.size());
    
    while (leftVecIdx <= leftVecMaxIdx && rightVecIdx <= rightVecMaxIdx)
    {
        if (vec1[leftVecIdx] <= vec2[rightVecIdx])
        {
            temp.push_back(vec1[leftVecIdx++]);
        }
        else
        {
            temp.push_back(vec2[rightVecIdx++]);
        }
    }
    
    // 먼저 빠져 나온 경우
    if (leftVecIdx > leftVecMaxIdx)
    {
        while (rightVecIdx > rightVecMaxIdx)
        {
            temp.push_back(vec2[rightVecIdx++]);
        }
    }
    
    if (rightVecIdx > rightVecMaxIdx)
    {
        while (leftVecIdx > leftVecMaxIdx)
        {
            temp.push_back(vec1[leftVecIdx++]);
        }
    }

    return temp;
}

// O(NlogN)
void MergeResult(vector<int>& v, int left, int mid, int right)
{
    int leftIdx = left;
    int rightIdx = mid + 1;

    vector<int> temp;

    cout << "Left : " << left << " " << "Mid : " << mid << " " << "Right : " << right << " " << "에 대해서 Merge 중!" << endl;

    while (leftIdx <= mid && rightIdx <= right)
    {
        if (v[leftIdx] <= v[rightIdx])
        {
            temp.push_back(v[leftIdx]);
            leftIdx++;
        }
        else
        {
            temp.push_back(v[rightIdx]);
            rightIdx++;
        }
    }

    // 왼쪽이 먼저 끝났으면, 오른쪽 나머지 데이터 복사
    if (leftIdx > mid)
    {
        while (rightIdx <= right)
        {
            temp.push_back(v[rightIdx]);
            rightIdx++;
        }
    }
    // 오른쪽이 먼저 끝났으면, 왼쪽 나머지 데이터 복사
    else
    {
        while (leftIdx <= mid)
        {
            temp.push_back(v[leftIdx]);
            leftIdx++;
        }
    }

    for (int i = 0; i < temp.size(); i++)
        v[left + i] = temp[i];
}

void MergeSort(vector<int>& v, int left, int right)
{
    if (left >= right)
        return;

    int mid = (left + right) / 2;

    cout << "Left : " << left << " " << "Mid : " << mid << " " << "Right : " << right << " " << "에 대해서 진행!" << endl;

    MergeSort(v, left, mid);
    MergeSort(v, mid + 1, right);
    MergeResult(v, left, mid, right);
}

#include <time.h>


// Quick Sort
int Partition(std::vector<int>& v, int left, int right)
{
    int pivotData = v[left];
    int pLeft = left + 1;
    int pRight = right;

    while (pLeft <= pRight)
    {
        while (pLeft <= right && pivotData >= v[pLeft])
            ++pLeft;
        
        while (pRight >= left + 1 && pivotData <= v[pRight])
            --pRight;

        if (pLeft < pRight)
            std::swap(v[pLeft], v[pRight]);        
    }

    std::swap(v[left], v[pRight]);
    return pRight;
}

void QuickSort(std::vector<int>& v, int left, int right)
{
    if (left > right)
        return;

    int pivotIdx = Partition(v, left, right);
    QuickSort(v, left, pivotIdx - 1);
    QuickSort(v, pivotIdx + 1, right);
}
#pragma endregion

#pragma region Disjoint  Set

// 그래프/트리 응용
// 오늘의 주제 : 최소 스패닝 트리 (Minimum Spanning Tree)

// 상호 배타적 집합 (Disjoint Set)
// -> 유니온-파인드 Union-Find (합치기-찾기)

// Lineage Battleground (혼종!)
// 혈맹전 + 서바이벌
// 1인 팀 1000명 (팀id 0~999)
// 동맹 (1번팀 + 2번팀 = 1번팀)

void LineageBattleground()
{
    struct User
    {
        int teamId;
        // TODO
    };
    
    // TODO : UserManager
    std::vector<User> users;
    for (int i = 0; i < 1000; ++i)
    {
        users.push_back(User{ i });
    }

    // 팀 동맹
    // users[1] <-> users[5]
    users[5].teamId = users[1].teamId;

    // [0][1][2][3][4]....[999]
    // [1][1][1][1][1]...[2][2][2][2]...[999]
    
    // teamId = 1인 팀과 teamId = 2인 팀이 통함
    for (User& user : users)
    {
        if (user.teamId == 1)
            user.teamId = 2;
    }

    // 찾기 연산 O(1)
    // 합치기 연산 O(N)
}

// 조직 폭력배 구조?
// [1]		[3]
// [2]	 [4][5]
//			[0]

// 시간 복잡도 : 트리의 높이에 비례한 시간이 걸림

class DisjointSet
{
public:
    DisjointSet(int n) : _parent(n), _rank(n, 1) 
    // std::vector's size => n
    {
        for (int i = 0; i < n; ++i)
        {
            _parent[i] = i;
        }
    }

    // 조직 폭력배 구조?
    // [1]		[3]
    // [2]	 [4][5][0]
    // 	

    // 니 대장이 누구니?
    int Find(int u)
    {
        if (u == _parent[u])
            return u;

        // _parent[u] = Find(_parent[u]);
        // return _parent[u];

        return _parent[u] = Find(_parent[u]);
    }

    // u와 v를 합친다 (일단 u가 v 밑으로)
    void Merge(int u, int v)
    {
        u = Find(u);
        v = Find(v);

        if (u == v)
            return;
        
        if (_rank[u] > _rank[v])
            std::swap(u, v);

        // rank[u] <= rank[v] 보장됨
        _parent[u] = v;
        
        if (_rank[u] == _rank[v])
            ++_rank[v];
    }

private:
    std::vector<int> _parent;
    std::vector<int> _rank;
};

#pragma endregion




int main()
{
    DisjointSet teams(1000);

    teams.Merge(10, 1);
    int teamId = teams.Find(1);
    int teamId2 = teams.Find(10);
    // teamId = 1, teamId2 = 1

    teams.Merge(3, 2);
    int teamId3 = teams.Find(3);
    int teamId4 = teams.Find(2);
    // teamId3 = 2, teamId4 = 2

    teams.Merge(1, 3);
    int teamId6 = teams.Find(1);
    int teamId7 = teams.Find(3);
    // teamId6 = 2, teamId7 = 2

    return 0;
}


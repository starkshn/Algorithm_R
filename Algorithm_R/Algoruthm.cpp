#include <iostream>
#include <istream>
#include <vector>
#include <stack>
#include <queue>
#include <list>

#include "RedBlackTree.h"
#include "Bst.h"
#include <thread>
#include <Windows.h>
#include <string>
using namespace std;

#pragma region DFS

// 그냥 무조건 파고들어감.

std::vector<bool> v_DFS;
std::vector<std::vector<int>> a_DFS;

void CreateGraph_DFS()
{
    v_DFS.resize(6);
    
    // DFS 트리 인접 리스트 방법
    a_DFS = std::vector<vector<int>>(6);
    a_DFS[0].push_back(1);
    a_DFS[0].push_back(3);
    a_DFS[1].push_back(0);
    a_DFS[1].push_back(2);
    a_DFS[1].push_back(3);
    a_DFS[3].push_back(4);
    a_DFS[5].push_back(4);

    // DFS 트리 인접 행렬방법
    /*a_DFS = vector<vector<int>>
    {
        {0, 1, 0, 1, 0, 0},
        {1, 0, 1, 1 , 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
    };*/
}

//  깊이우선 탐색
void DFS(int here)
{
    v_DFS[here] = true;
    cout << "방문 : " << here << endl;

    // DFS가 인접 리스트 방법일 경우
    for (int i = 0; i < a_DFS[here].size(); ++i)
    {
        int there = a_DFS[here][i];
        if (v_DFS[there] == false)
            DFS(there);
    }

    // DFS가 인접 행렬일 경우
   /* for (int there = 0; there < 6; ++there)
    {
        if (a_DFS[here][there] == 0)
            continue;
        
        if (v_DFS[there] == false)
            DFS(there);
    }*/
}
#pragma endregion

#pragma region BFS

// 출발점으로부터 가까운 것들부터

vector<bool>        v_BFS;
vector<vector<int>> a_BFS;
std::vector<bool>   discovered_B;

void CreateGraph_BFS()
{
    v_BFS.resize(6);
    discovered_B.resize(6);

    // BFS 트리 인접 리스트 방법
    a_BFS = std::vector<vector<int>>(6);
    a_BFS[0].push_back(1);
    a_BFS[0].push_back(3);
    a_BFS[1].push_back(0);
    a_BFS[1].push_back(2);
    a_BFS[1].push_back(3);
    a_BFS[3].push_back(4);
    a_BFS[5].push_back(4);
}

void BFS(int here)
{
    std::vector<int> parent(6, -1);
    std::vector<int> distance(6, -1);
    
    std::queue<int> q;
    q.push(here);
    discovered_B[here] = true;

    parent[here] = here;
    distance[here] = 0;

    while (q.empty() == false)
    {
        here = q.front();
        q.pop();

        cout << "방문함 : " << here << endl;

        for (int there : a_BFS[here])
        {
            if (discovered_B[there])
                continue;
            
            q.push(there);
            discovered_B[there] = true;
            
            parent[there] = here;
            distance[there] = distance[here] + 1;
        }
    }
}
#pragma endregion

#pragma region Dijkstra

struct Vertex
{
    int vertex;
    int cost;

public:
    constexpr bool operator < (const Vertex& other) const
    {
        return cost < other.cost;
    }
};

std::vector<Vertex>             vertices;
std::vector<std::vector<int>>   a_D;

void CreateGraph_Dijikstra()
{
    vertices.resize(6);
    a_D = vector<vector<int>>(6, vector<int>(6, -1));

    a_D[0][1] = 15;
    a_D[0][3] = 35;
    a_D[1][0] = 15;
    a_D[1][2] = 5;
    a_D[1][3] = 10;
    a_D[3][4] = 5;
    a_D[5][4] = 5;
}

// BFS + 가중치있을경우
void Dijikstra(int here)
{
    std::priority_queue<Vertex>     discovered;
    std::vector<int>                best(6, INT32_MAX);
    std::vector<int>                parent(6, -1);

    discovered.push(Vertex{here, 0});
    best[here] = 0;
    parent[here] = here;

    while (discovered.empty() == false)
    {
        int bestCost = INT32_MAX;

        // 가장 좋은 후보 뽑는다.
        Vertex info = discovered.top();
        discovered.pop();

        int cost = info.cost;
        here = info.vertex;

        if (best[here] < cost) continue;

        // 방문
        for (int there = 0; there < 6; ++there)
        {
            if (a_D[here][there] == -1) continue;
            
            int nextCost = best[here] + a_D[here][there];

            if (nextCost >= best[there])
            {
                std::cout << "후보 : " << here << " => " << there << " " << "점수 : " << a_D[here][there] << " " << "후보 이지만 탈락!" << endl;
                continue;
            }

            best[there] = nextCost;
            parent[there] = here;
            discovered.push(Vertex{ there, nextCost });

            std::cout << "발견한 부분에 넣은거 : " << a_D[here][there] << " " << "점수 : " << nextCost << endl;
        }
        cout << endl;
    }
}

#pragma endregion

template <typename T>
concept Container = requires(T t)
{
    t.begin();
    t.end();
};

#pragma region Binary Search
template <typename T>
requires Container<T>
void BinarySearch(int find, T c)
{
    int left = 0;
    int right = c.size() - 1;
    int count = 0;

    while (left <= right)
    {
        int mid = (left + right) / 2;

        cout << "수행 횟수 : " << count << endl;
        cout << "범위 : " << left << "~" << right << endl;

        if (find < c[mid])
        {
            right = mid - 1;
        }
        else if (find > c[mid])
        {
            left = mid + 1;
        }
        else
        {
            cout << "Find : " << find << endl;
            break;
        }

        ++count;
    }
}

#pragma endregion

#pragma region BubbleSort

template <typename T>
void BubbleSort(T& c) requires Container<T>
{
    const int size = static_cast<int>(c.size());

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size - 1 - i; ++j)
        {
            if (c[j] > c[j + 1])
                std::swap(c[j], c[j + 1]);
        }
    }
}
#pragma endregion

#pragma region SelsectionSort

template <typename T>
void SelectionSort(T& c) requires Container<T>
{
    const int n = static_cast<int>(c.size());

    for (int i = 0; i < n - 1; ++i)
    {
        int bestIdx = i;

        for (int j = i + 1; j < n; ++j)
        {
            if (c[j] < c[bestIdx])
                bestIdx = j;
        }

        std::swap(c[bestIdx], c[i]);
    }
}

#pragma endregion

#pragma region InsertionSort

template <typename T1, typename T2>
void InsertionSort(T1& c) requires Container<T1>
{
    const int size = static_cast<int>(c.size());

    // [1][4] [5][3][7][8][2]
    // [5]가 [4]보다 작나요?

    // [1][3][4] [5]...
    for (int i = 1; i < size; ++i)
    {
        T2 insertData = c[i];

        int j;
        for (j = i - 1; j >= 0; --j)
        {
            if (c[j] > insertData)
                std::swap(c[j + 1], c[j]);
            else
                break;
        }
    }
}
#pragma endregion

#pragma region HeapSort

template <typename T1, typename T2>
void HeapSort(T1& c) requires Container<T1>
{
    std::priority_queue<T2, std::vector<T2>, greater<T2>> pq;
    
    for (T2 num : c)
        pq.push(num);

    c.clear();

    while (pq.empty() == false)
    {
        c.push_back(pq.top());
        pq.pop();
    }
}
#pragma endregion

#pragma region MergeSort

template <typename T>
void Merge(T& c, int left, int mid, int right) requires Container<T>
{
    int leftIdx = left;
    int rightIdx = mid + 1;

    T temp;
    
    while (leftIdx <= mid && rightIdx <= right)
    {
        if (c[leftIdx] <= c[rightIdx])
        {
            temp.emplace_back(c[leftIdx]);
            ++leftIdx;
        }
        else
        {
            temp.emplace_back(c[rightIdx]);
            ++rightIdx;
        }
    }

    if (leftIdx > mid)
    {
        while (rightIdx <= right)
        {
            temp.emplace_back(c[rightIdx]);
            ++rightIdx;
        }
    }
    else
    {
        while (leftIdx <= mid)
        {
            temp.emplace_back(c[leftIdx]);
            ++leftIdx;
        }
    }

    for (int i = 0; i < temp.size(); ++i)
        c[left + i] = temp[i];
}

template <typename T>
void MergeSort(T& c, int left, int right) requires Container<T>
{
    if (left >= right)
        return;

    int mid = (left + right) / 2;
    
    MergeSort(c, left, mid);
    MergeSort(c, mid + 1, right);
    Merge(c, left, mid, right);
}

#pragma endregion

#pragma region QuickSort

template <typename T>
int Partition(T& c, int left, int right) requires Container<T>
{
    int pivotData = c[left];
    int pLeft = left + 1;
    int pRight = right;

    while (pLeft <= pRight)
    {
        while (pLeft <= right && c[pLeft] <= pivotData)
            ++pLeft;

        while (pRight >= left + 1 && c[pRight] >= pivotData)
            --pRight;

        if (pLeft < pRight)
            std::swap(c[pLeft], c[pRight]);
    }

    std::swap(c[left], c[pRight]);
    return pRight;
}

template <typename T>
void QuickSort(T& c, int left, int right) requires Container<T>
{
    if (left > right)
        return;

    int pivot = Partition(c, left, right);
    QuickSort(c, left, pivot - 1);
    QuickSort(c, pivot + 1, right);
}

#pragma endregion

#pragma region Disjoint Set
class DisjointSet
{
public:
    DisjointSet(int n) : _parent(n), _rank(n, 1)
    {
        for (int i = 0; i < n; ++i)
            _parent[i] = i;
    }

public:
    int Find(int u)
    {
        if (u == _parent[u])
            return u;

        return _parent[u] = Find(_parent[u]);
    }
    
    void Merge(int u, int v)
    {
        u = Find(u);
        v = Find(v);
        // u => 1, v => 2

        if (u == v)
            return;

        if (_rank[u] > _rank[v])
            std::swap(u, v);

        _parent[u] = v;

        if (_rank[u] == _rank[v])
            ++_rank[v];
    }
    
private:
    std::vector<int> _parent;
    std::vector<int> _rank;
};

#pragma endregion

#pragma region Kruskal

std::vector<Vertex>             vertices_Kruskal;
std::vector<std::vector<int>>   adjacent_Kruskal;

void CreateGraph_Kruskal()
{
    vertices_Kruskal.resize(6);
    adjacent_Kruskal = std::vector<std::vector<int>>(6, std::vector<int>(6, -1));

    adjacent_Kruskal[0][1] = adjacent_Kruskal[1][0] = 15;
    adjacent_Kruskal[0][3] = adjacent_Kruskal[3][0] = 35;
    adjacent_Kruskal[1][2] = adjacent_Kruskal[2][1] = 5;
    adjacent_Kruskal[1][3] = adjacent_Kruskal[3][1] = 10;
    adjacent_Kruskal[3][4] = adjacent_Kruskal[4][3] = 5;
    adjacent_Kruskal[3][5] = adjacent_Kruskal[5][3] = 10;
    adjacent_Kruskal[5][4] = adjacent_Kruskal[4][5] = 5;
}

struct CostEdge 
{
    int cost;
    int u;
    int v;

    bool operator < (const CostEdge& other) const
    {
        return cost < other.cost;
    }
};

template <typename T>
int Kruskal(T& selected) requires Container<T>
{
    int result = 0;
    
    selected.clear();

    std::vector<CostEdge> edges;

    for (int u = 0; u < adjacent_Kruskal.size(); ++u)
    {
        for (int v = 0; v < adjacent_Kruskal.size(); ++v)
        {
            if (u > v) continue; // 중복 스킵
            int cost = adjacent_Kruskal[u][v];
            if (cost == -1) continue;

            edges.emplace_back(CostEdge{ cost, u, v});
        }
    }

    std::sort(edges.begin(), edges.end());

    DisjointSet sets (vertices_Kruskal.size());

    for (CostEdge& edge : edges)
    {
        if (sets.Find(edge.u) == sets.Find(edge.v)) continue;

        sets.Merge(edge.u, edge.v);
        selected.emplace_back(edge);
        result += edge.cost;
    }

    return result;
}

#pragma endregion

#pragma region DP

// 메모이제이션 (memoization)
int cache[50][50];

int Combination(int n, int r)
{
    // 기저 사례
    if (r == 0 || n == r) return 1;

    // 이미 답을 구한 적이 있으면 바로 반환
    int& ret = cache[n][r];
    if (ret != -1) return ret;

    return Combination(n - 1, r - 1) + Combination(n - 1, r);
}

#pragma endregion

#pragma region LIS

int cache_LIS[50];
vector<int> seq;

int LIS(int pos)
{
    // 기저사항

    // 캐시 확인
    int& ret = cache_LIS[pos];
    if (ret != -1)
        return ret;

    // 최소 seq[pos]는 있으니 1부터 시작
    ret = 1;

    // seq = { 1, 9, 2, 5, 7 };
    // 1 9
    // 1 2
    // 1 5
    // 1 7

    // 구하기
    for (int next = pos + 1; next < seq.size(); ++next)
        if (seq[pos] < seq[next])
            ret = max(ret, 1 + LIS(next));

    return ret;
}

#pragma endregion

#pragma region Triangle Path
// 오늘의 주제 : 동적 계획법 (DP)
// TRIANGLE_PATH
// - (0,0)부터 시작해서 아래 or 아래우측으로 이동 가능
// - 만나는 숫자를 모두 더함
// - 더한 숫자가 최대가 되는 경로? 합?

// 6
// 1 2
// 3 7 4
// 9 4 1 7
// 2 7 5 9 4

int N;
vector<vector<int>> board_TP;
vector<vector<int>> cache_TP;
vector<vector<int>> nextX_TP;

int path(int y, int x)
{
    if (y == N) return 0;
    
    // 캐시확인
    int& ret = cache_TP[y][x];
    if (ret != -1) return ret;

    // 경로 기록
    {
        int nextBottom = path(y + 1, x);
        int nextBottomRight = path(y + 1, x + 1);
        if (nextBottom > nextBottomRight)
            nextX_TP[y][x] = x;
        else
            nextX_TP[y][x] = x + 1;
    }

    // 적용
    return ret = board_TP[y][x] + max(path(y + 1, x), path(y + 1, x + 1));
}
#pragma endregion


#pragma region DP
int sum = 0;
int input;

vector<vector<int>> board(12, vector<int>(12, 0));
int cache[12][12];

int SumPath(int y, int x)
{
    // 기저
    if (y == board.size() || x == board.size()) return 0;

    // 캐시
    int& ret = cache[y][x];
    if (ret != -1) return ret;

    // 구하기
    return SumPath(y, x + 1) + SumPath(y + 1, x + 1) + SumPath(y + 1, x);
}

#pragma endregion



int main()
{
    // CreateGraph_DFS();
    // DFS(0);

    // CreateGraph_BFS();
    // BFS(0);

    // CreateGraph_Dijikstra();
    // Dijikstra(0);

    // std::vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // BinarySearch(1, vec);

    std::vector<int> vec{1, 5, 6, 2, 5, 10, 7, 9};

    // BubbleSort(vec);
    // SelectionSort(vec);
    // InsertionSort(vec);
    // HeapSort(vec);
    // MergeSort(vec, 0, vec.size() - 1);
    // QuickSort(vec, 0, vec.size() - 1);

    //DisjointSet teams(1000);

    //teams.Merge(10, 1);
    //int teamId = teams.Find(1);
    //int teamId2 = teams.Find(10);
    //
    //teams.Merge(3, 2);
    //int teamId3 = teams.Find(3);
    //int teamId4 = teams.Find(2);

    //teams.Merge(1, 3);
    //int teamId6 = teams.Find(1);
    //int teamId7 = teams.Find(3);

    /*CreateGraph_Kruskal();
    std::vector<CostEdge> s;
    int cost = Kruskal(s);*/

    // DP
    /*::memset(cache, -1, sizeof(cache));

    __int64 start = GetTickCount64();

    int lotto = Combination(45, 6);

    __int64 end = GetTickCount64();

    std::cout << end - start << " ms" << std::endl;*/


    // DP, LIS
    /*::memset(cache_LIS, -1, sizeof(cache_LIS));

    seq = { 1, 9, 2, 5, 7 };

    int ret = 0;
    for (int pos = 0; pos < seq.size(); ++pos)
        ret = max(ret, LIS(pos));*/

    // TP
    board_TP = vector<vector<int>>
    {
        {6},
        {1, 2},
        {3, 7, 4},
        {9, 4, 1, 7},
        {2, 7, 5, 9, 4}
    };

    N = board_TP.size();
    cache_TP = vector<vector<int>>(N, vector<int>(N, -1));
    nextX_TP = vector<vector<int>>(N, vector<int>(N));

    int ret = path(0, 0);
    cout << ret << endl;


    // 경로 만들기
    int y = 0;
    int x = 0;

    while (y < N)
    {
        cout << board_TP[y][x] << " -> ";

        x = nextX_TP[y][x];
        ++y;
    }

    return 0;
}
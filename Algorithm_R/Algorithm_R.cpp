﻿#include <iostream>
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

vector<int> Merge(vector<int> a, vector<int> b)
{
    vector<int> temp;

    return temp;
}

// O(NlogN)
void MergeResult(vector<int>& v, int left, int mid, int right)
{
    // [2][3][7][K][4][8][9][J]
    //          [l]            [r]
    int leftIdx = left;
    int rightIdx = mid + 1;

    // [2]
    vector<int> temp;

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
    MergeSort(v, left, mid);
    MergeSort(v, mid + 1, right);

    MergeResult(v, left, mid, right);
}


#pragma endregion


int main()
{
    // ###################################
    // BST, Red-Black Tree
    // 
    // Bst bst;
    //RedBlackTree bst;

    //bst.Insert(30);
    //bst.Print();
    //this_thread::sleep_for(1s);

    //bst.Insert(10);
    //bst.Print();
    //this_thread::sleep_for(1s);

    //bst.Insert(20);
    //bst.Print();
    //this_thread::sleep_for(1s);

    //bst.Insert(25);
    //bst.Print();
    //this_thread::sleep_for(1s);



    //bst.Delete(20);
    //bst.Print();
    //this_thread::sleep_for(1s);

    //bst.Delete(10);
    //bst.Print();
    //this_thread::sleep_for(1s);

    std::vector<int> vec;
    vec.push_back(30);
    vec.push_back(20);
    vec.push_back(10);
    vec.push_back(40);
    vec.push_back(70);
    vec.push_back(50);

    // BubbleSort(vec);
    SelectionSort2(vec);
    // InsertionSort(vec);
   
    int a = 10;
    //bst.Insert(30);
    //bst.Insert(10);
    //bst.Insert(20);
    //bst.Insert(25);
    //bst.Insert(40);
    //bst.Insert(50);
    //bst.Print();
    // ###################################
}


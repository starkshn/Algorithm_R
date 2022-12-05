#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

#include "RedBlackTree.h"
#include "Bst.h"
#include <thread>

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

void CreateGraph()
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

int main()
{
    // Bst bst;


    RedBlackTree bst;

    bst.Insert(30);
    bst.Print();
    this_thread::sleep_for(1s);

    bst.Insert(10);
    bst.Print();
    this_thread::sleep_for(1s);

    bst.Insert(20);
    bst.Print();
    this_thread::sleep_for(1s);

    bst.Insert(25);
    bst.Print();
    this_thread::sleep_for(1s);

    bst.Insert(40);
    bst.Print();
    this_thread::sleep_for(1s);

    bst.Insert(50);
    bst.Print();
    this_thread::sleep_for(1s);



    //bst.Insert(30);
    //bst.Insert(10);
    //bst.Insert(20);
    //bst.Insert(25);
    //bst.Insert(40);
    //bst.Insert(50);
    //bst.Print();

}


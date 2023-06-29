#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <memory.h>
#include <vector>
#include <queue>
using namespace std;

vector<vector<int>> adjacent;
vector<bool> discovered;


void CreateGraph()
{
    adjacent = vector<vector<int>>(6);
    adjacent[0].push_back(1);
    adjacent[0].push_back(3);
    adjacent[1].push_back(0);
    adjacent[1].push_back(2);
    adjacent[1].push_back(3);
    adjacent[3].push_back(4);
    adjacent[5].push_back(4);
}

void Bfs(int here)
{
    queue<int> q;
    discovered[here] = true;
    q.push(here);
    
    while (!q.empty())
    {
        here = q.front();
        q.pop();
        cout << "Visited : " << here << endl;
        for (int there : adjacent[here])
        {
            // 만약 전에 발견된것이 아니었다면 발견을 외치고 큐에 푸시한다
            if (discovered[there] == false)
            {
                discovered[there] = true;
                q.push(there);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    discovered = vector<bool>(6, false);
    
    CreateGraph();
    Bfs(0);
}

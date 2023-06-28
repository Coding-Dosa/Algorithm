#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <memory.h>
#include <vector>
using namespace std;



int board[50][50] = {0};
bool visited[50][50] = {0};
int M, N, K;
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, -1, 0, 1};




void Dfs(int y, int x)
{
    visited[y][x] = true;
    for (int i = 0; i < 4; i++)
    {
        int newY = y + dy[i];
        int newX = x + dx[i];
        
        if(newY < 0 || newY >= N || newX < 0 || newX >= M)
            continue;
        
        if (board[newY][newX] && !visited[newY][newX])
        {
            Dfs(newY, newX);
        }
            
    }
}

int main(int argc, char* argv[])
{
    int T;
    int x, y;
    scanf("%d", &T);

    for (int testCase = 0; testCase < T; testCase++)
    {
        scanf("%d %d %d", &M,&N,&K);
        
        memset(board, 0, sizeof(board));
        memset(visited, 0, sizeof(visited));
        
        int ans = 0;

        // 보드 세팅
        for (int i = 0; i < K; i++)
        {
            scanf("%d %d", &x, &y);
            board[y][x] = 1;
        }

        // 탐색
        for (int y = 0; y < N; y++)
        {
            for (int x = 0; x < M; x++)
            {
                if (board[y][x] && !visited[y][x])
                {
                    ans++;
                    Dfs(y, x);
                }
            }
        }
        printf("%d\n", ans);
    }
}

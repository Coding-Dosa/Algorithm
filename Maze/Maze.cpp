#include "pch.h"
#include "Board.h"

Board board;

int main(int argc, char* argv[])
{
    board.Init(25);
    
    uint64 lastTick = 0;
    while (true)
    {
        const uint64 currentTick = GetTickCount64();
        const uint64 deltaTick = currentTick - lastTick;
        lastTick = currentTick;
        // 입력
        
        // 로직

        // 렌더링
        board.Render();
    }
}

#pragma once
#include "ConsoleHelper.h"
#include "types.h"

enum { BOARD_MAX_SIZE = 100 };

enum class TileType
{
    NONE = 0,
    EMPTY,
    WALL,
};

class Board
{
public:
    Board();
    ~Board();

    void Init(int32 size);
    void Render();

    void GenerateMap();

    TileType GetTileType(Pos pos);
    ConsoleColor GetTileColor(Pos pos);
    
private:
    TileType _tiles[BOARD_MAX_SIZE][BOARD_MAX_SIZE] = {};
    int32 _size = 0;
};

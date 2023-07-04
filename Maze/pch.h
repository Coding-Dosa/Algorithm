#pragma once

#include "Types.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;


struct Pos
{
    bool operator == (Pos& other)
    {
        return y == other.y && x == other.x;
    }
    
    bool operator != (Pos& other)
    {
        return (y != other.y) || (x != other.x);
    }

    bool operator < (const Pos& other) const
    {
        if (y != other.y)
            return y < other.y;
        return x < other.x;
    }
    
    Pos operator + (Pos& other)
    {
        Pos ret;
        ret.y = this->y + other.y;
        ret.x = this->x + other.x;
        return ret;
    }

    Pos& operator += (Pos& other)
    {
        y += other.y;
        x += other.x;
        return *this;
    }
    
    int32 y = 0;
    int32 x = 0;
};

enum Dir
{
    DIR_UP = 0,
    DIR_LEFT = 1,
    DIR_DOWN = 2,
    DOR_RIGHT = 3,
    DIR_COUNT = 4
};
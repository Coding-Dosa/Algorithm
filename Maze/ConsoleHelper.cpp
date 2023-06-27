#include "pch.h"
#include "ConsoleHelper.h"

// 마우스 깜빡거리는 커서 위치 설정해주는 함수
void ConsoleHelper::SetCursorPosition(int32 x, int32 y)
{
    HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<short>(x), static_cast<short>(y) };
    ::SetConsoleCursorPosition(output, pos);
}

// 커서의 색상을 정해주는 함수
void ConsoleHelper::SetCursorColor(ConsoleColor color)
{
    HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ::SetConsoleTextAttribute(output, static_cast<int16>(color));
}

// 커서를 보여줄지 안보여줄지 지정하는 함수
void ConsoleHelper::ShowConsoleCursor(bool flag)
{
    HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    ::GetConsoleCursorInfo(output, &cursorInfo);
    cursorInfo.bVisible = flag;
    ::SetConsoleCursorInfo(output, &cursorInfo);
}

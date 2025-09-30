##include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

void gotoxy(int x, int y)
{
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon,dwPos);
}
void space()
{
    for(int i = 4 ; i < 70; i++)
    {
        gotoxy(i,5);
        printf("%c",205);
        gotoxy(i,25);
        printf("%c",205); //marco horizontal
    }
    for(int i = 6 ; i < 25; i++)
    {
        gotoxy(4,i);
        printf("%c",186);
        gotoxy(69,i);
        printf("%c",186);  //marco VERTICL
    }
}

int main()
{
    int x=8;
    int y=10;
    string snake = "--->";
    space();
    while(true)
    {
        gotoxy(x,y);
        cout<<snake;
        Sleep(80);
        gotoxy(x,y);
        cout<<"  ";
        x++;

        if(x >= 68)
        {
            gotoxy(30,15);
            cout<<"PERDISTE, BUEN INTENTO";
            break;
        }
    }
    system("pause");
    return 0;
}

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
    int dx = 1, dy = 0; 
    bool paused = false;
    
    string snake = "--->";
    space();
    
    while(true)
    {
        if(_kbhit()) {
            char tecla = _getch();

            if(tecla == 'w' || tecla == 'W' || tecla == 72) { dx = 0; dy = -1; snake = "^"; }
            if(tecla == 's' || tecla == 'S' || tecla == 80) { dx = 0; dy =  1; snake = "v"; }
            if(tecla == 'a' || tecla == 'A' || tecla == 75) { dx = -1; dy = 0; snake = "<"; }
            if(tecla == 'd' || tecla == 'D' || tecla == 77) { dx =  1; dy = 0; snake = ">"; }

            if(tecla == 'p' || tecla == 'P') {
                paused = true;
                gotoxy(30, 3);
                cout << "[Juego en PAUSA - presiona R]";
            }

            if(tecla == 'r' || tecla == 'R') {
                paused = false;
                gotoxy(30, 3);
                cout << "                              ";
            }
        }

        if(!paused) {
            gotoxy(x,y);
            cout<<snake;
            Sleep(80);
            gotoxy(x,y);
            cout<<"   ";
            x += dx;
            y += dy;
        }

        if(!paused && (x <= 4 || x >= 68 || y <= 5 || y >= 25))
        {
            gotoxy(30,15);
            cout<<"PERDISTE, BUEN INTENTO";
            break;
        }
    }
    system("pause");
    return 0;
}


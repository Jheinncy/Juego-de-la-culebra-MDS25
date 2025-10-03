#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include "points_system.hpp"  // usa tu versión mejorada

using namespace std;

PointsSystem points;  // sistema de puntos
float snakeSpeed = 4.0f;  // factor de velocidad

// mover cursor en consola
void gotoxy(int x, int y) {
    HANDLE hCon;
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

// dibujar borde
void space() {
    for (int i = 4; i < 70; i++) {
        gotoxy(i, 5);  printf("%c", 205);
        gotoxy(i, 25); printf("%c", 205);
    }
    for (int i = 6; i < 25; i++) {
        gotoxy(4, i);  printf("%c", 186);
        gotoxy(69, i); printf("%c", 186);
    }
}

// generar comida
void generarComida(int &comidaX, int &comidaY) {
    comidaX = 5 + rand() % (68 - 5);
    comidaY = 6 + rand() % (24 - 6);
    gotoxy(comidaX, comidaY);
    cout << "*";
}

// actualizar HUD con puntaje y combo
void actualizarHUD() {
    gotoxy(10, 3);
    cout << "Puntos: " << points.getScore() 
         << "  Highscore: " << points.getHighScore()
         << "  Combo: " << points.getComboCount()
         << "  x" << points.getMultiplier() << "   ";
}

// evento al comer
void onSnakeAteFood() {
    points.onFoodEaten(snakeSpeed);
    actualizarHUD();
}

// evento de game over
void onGameOver() {
    points.saveHighScore();
    gotoxy(25, 17);
    cout << "Score: " << points.getScore() 
         << "  Highscore: " << points.getHighScore();
}

// reinicio de partida
void startNewRun() {
    points.resetRun();
    actualizarHUD();
}

// loop principal del juego
void jugar() {
    int x = 8, y = 10;
    int dx = 1, dy = 0;
    bool paused = false;
    string snake = ">";
    int comidaX, comidaY;

    system("cls");
    space();
    startNewRun();
    generarComida(comidaX, comidaY);

    while (true) {
        if (_kbhit()) {
            char tecla = _getch();

            if (tecla == 'w' || tecla == 'W' || tecla == 72) { dx = 0; dy = -1; snake = "^"; }
            if (tecla == 's' || tecla == 'S' || tecla == 80) { dx = 0; dy =  1; snake = "v"; }
            if (tecla == 'a' || tecla == 'A' || tecla == 75) { dx = -1; dy = 0; snake = "<"; }
            if (tecla == 'd' || tecla == 'D' || tecla == 77) { dx =  1; dy = 0; snake = ">"; }

            if (tecla == 'p' || tecla == 'P') {
                paused = true;
                gotoxy(30, 3);
                cout << "[Juego en PAUSA - presiona R]";
            }

            if (tecla == 'r' || tecla == 'R') {
                paused = false;
                gotoxy(30, 3);
                cout << "                              ";
            }
        }

        if (!paused) {
            gotoxy(x, y);
            cout << snake;

            Sleep(80);

            gotoxy(x, y);
            cout << " "; 

            x += dx;
            y += dy;
        }

        // perder si toca pared
        if (!paused && (x <= 4 || x >= 68 || y <= 5 || y >= 25)) {
            gotoxy(30, 15);
            cout << "GAME OVER - TOCASTE LA PARED";
            onGameOver();
            break;
        }

        // comer comida
        if (x == comidaX && y == comidaY) {
            generarComida(comidaX, comidaY);
            onSnakeAteFood();
        }
    }
}

int main() {
    srand(time(NULL));
    char opcion;

    do {
        jugar();
        gotoxy(25, 19);
        cout << "Deseas jugar otra vez? (s/n): ";
        opcion = getch();
    } while (opcion == 's' || opcion == 'S');

    gotoxy(25, 21);
    cout << "Gracias por jugar!" << endl;

    return 0;
}

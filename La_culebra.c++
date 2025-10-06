#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "PointsSystem.hpp"

using namespace std;

PointsSystem points;
float snakeSpeed = 4.0f;
int gameMode = 0; // 0 = clásico, 1 = con obstáculos
int snakeSkin = 0; // 0..2 (Clásico, Elegante, Retro)

struct Segment { int x, y; };
struct Obstacle { int x, y; };
vector<Obstacle> obstacles;

// mover cursor en consola
void gotoxy(int x, int y) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = (SHORT)x;
    dwPos.Y = (SHORT)y;
    SetConsoleCursorPosition(hCon, dwPos);
}

// dibujar bordes
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

// obtener carácter para la cabeza según skin y dirección
string getSnakeHeadChar(char direction) {
    switch (snakeSkin) {
        case 1: return "@"; // Elegante
        case 2: return "#"; // Retro
        default:
            if (direction == 'U') return "^";
            if (direction == 'D') return "v";
            if (direction == 'L') return "<";
            if (direction == 'R') return ">";
            return ">";
    }
}

// obtener carácter para el cuerpo según skin
string getSnakeBodyChar() {
    switch (snakeSkin) {
        case 1: return "o"; // Elegante
        case 2: return "+"; // Retro
        default: return "o"; // Clásico
    }
}

// HUD
void actualizarHUD() {
    gotoxy(10, 3);
    cout << "Puntos: " << points.getScore()
         << "  Highscore: " << points.getHighScore()
         << "  Combo: " << points.getComboCount()
         << "  x" << points.getMultiplier()
         << "   ";
}

void onSnakeAteFood() {
    points.onFoodEaten(snakeSpeed);
    snakeSpeed += 0.5f;
    actualizarHUD();
}

void onGameOver() {
    points.saveHighScore();
    gotoxy(25, 17);
    cout << "Score: " << points.getScore()
         << "  Highscore: " << points.getHighScore();
}

// generar obstáculos evitando la serpiente
void generarObstaculos(int cantidad, const vector<Segment>& snake) {
    obstacles.clear();
    while ((int)obstacles.size() < cantidad) {
        Obstacle ob;
        ob.x = 6 + rand() % (67 - 6);
        ob.y = 7 + rand() % (24 - 7);

        bool clash = false;
        for (auto &o : obstacles)
            if (o.x == ob.x && o.y == ob.y) { clash = true; break; }

        for (auto &s : snake)
            if (s.x == ob.x && s.y == ob.y) { clash = true; break; }

        if (!clash) {
            obstacles.push_back(ob);
            gotoxy(ob.x, ob.y);
            cout << "#";
        }
    }
}

// generar comida
void generarComida(int &comidaX, int &comidaY, const vector<Segment>& snake) {
    bool ok = false;
    while (!ok) {
        comidaX = 5 + rand() % (68 - 5);
        comidaY = 6 + rand() % (24 - 6);
        ok = true;
        for (auto &s : snake)
            if (s.x == comidaX && s.y == comidaY) { ok = false; break; }
        for (auto &o : obstacles)
            if (o.x == comidaX && o.y == comidaY) { ok = false; break; }
    }
    gotoxy(comidaX, comidaY);
    cout << "*";
}

void startNewRun() {
    points.resetRun();
    snakeSpeed = 4.0f;
    actualizarHUD();
}

void jugar() {
    int startX = 8, startY = 10;
    int initialLength = 3;
    int dx = 1, dy = 0;
    char dir = 'R';
    bool paused = false;

    vector<Segment> snake;
    for (int i = 0; i < initialLength; ++i)
        snake.push_back({ startX - i, startY });

    int comidaX, comidaY;

    system("cls");
    space();
    startNewRun();

    for (size_t i = 0; i < snake.size(); ++i) {
        gotoxy(snake[i].x, snake[i].y);
        if (i == 0) cout << getSnakeHeadChar(dir);
        else cout << getSnakeBodyChar();
    }

    if (gameMode == 1)
        generarObstaculos(10, snake);

    generarComida(comidaX, comidaY, snake);

    while (true) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                ch = _getch();
                if (ch == 72 && !(dx == 0 && dy == 1)) { dx = 0; dy = -1; dir = 'U'; }
                else if (ch == 80 && !(dx == 0 && dy == -1)) { dx = 0; dy = 1; dir = 'D'; }
                else if (ch == 75 && !(dx == 1 && dy == 0)) { dx = -1; dy = 0; dir = 'L'; }
                else if (ch == 77 && !(dx == -1 && dy == 0)) { dx = 1; dy = 0; dir = 'R'; }
            } else {
                if (ch == 'w' || ch == 'W') { if (!(dx == 0 && dy == 1)) { dx = 0; dy = -1; dir = 'U'; } }
                if (ch == 's' || ch == 'S') { if (!(dx == 0 && dy == -1)) { dx = 0; dy = 1; dir = 'D'; } }
                if (ch == 'a' || ch == 'A') { if (!(dx == 1 && dy == 0)) { dx = -1; dy = 0; dir = 'L'; } }
                if (ch == 'd' || ch == 'D') { if (!(dx == -1 && dy == 0)) { dx = 1; dy = 0; dir = 'R'; } }
                if (ch == 'p' || ch == 'P') {
                    paused = true;
                    gotoxy(30, 3); cout << "[PAUSA - presiona R]";
                }
                if (ch == 'r' || ch == 'R') {
                    paused = false;
                    gotoxy(30, 3); cout << "                    ";
                }
            }
        }

        if (!paused) {
            int newHeadX = snake.front().x + dx;
            int newHeadY = snake.front().y + dy;

            if (newHeadX <= 4 || newHeadX >= 68 || newHeadY <= 5 || newHeadY >= 25) {
                gotoxy(30, 15); cout << "GAME OVER - TOCASTE LA PARED";
                onGameOver();
                break;
            }

            if (gameMode == 1) {
                for (auto &ob : obstacles)
                    if (newHeadX == ob.x && newHeadY == ob.y) {
                        gotoxy(30, 15);
                        cout << "GAME OVER - GOLPEASTE UN OBSTÁCULO";
                        onGameOver();
                        return;
                    }
            }

            bool willGrow = (newHeadX == comidaX && newHeadY == comidaY);

            size_t checkLimit = snake.size();
            if (!willGrow && checkLimit > 0) checkLimit -= 1;

            for (size_t i = 0; i < checkLimit; ++i)
                if (snake[i].x == newHeadX && snake[i].y == newHeadY) {
                    gotoxy(30, 15);
                    cout << "GAME OVER - TE CHOCASTE CONTIGO MISMO";
                    onGameOver();
                    return;
                }

            Segment newHead = { newHeadX, newHeadY };
            snake.insert(snake.begin(), newHead);

            gotoxy(newHeadX, newHeadY);
            cout << getSnakeHeadChar(dir);

            if (snake.size() > 1) {
                gotoxy(snake[1].x, snake[1].y);
                cout << getSnakeBodyChar();
            }

            if (willGrow) {
                onSnakeAteFood();
                generarComida(comidaX, comidaY, snake);
            } else {
                Segment tail = snake.back();
                gotoxy(tail.x, tail.y);
                cout << " ";
                snake.pop_back();
            }

            int delay = max(20, int(140 - snakeSpeed * 6));
            Sleep(delay);
        } else Sleep(50);
    }
}

int main() {
    srand((unsigned)time(NULL));
    char opcion;

    do {
        system("cls");
        cout << "=== MENU PRINCIPAL ===\n";
        cout << "Selecciona el modo de juego:\n";
        cout << "1. Clasico\n";
        cout << "2. Con Obstaculos\n";
        cout << "Opcion: ";
        char modo = _getch();
        gameMode = (modo == '2') ? 1 : 0;

        system("cls");
        cout << "=== SELECCIONA SKIN DE LA SERPIENTE ===\n";
        cout << "1. Clasico (>,<,^,v)\n";
        cout << "2. Elegante (@)\n";
        cout << "3. Retro (#)\n";
        cout << "Opcion: ";
        char skin = _getch();
        int chosen = (skin - '1');
        if (chosen < 0 || chosen > 2) chosen = 0;
        snakeSkin = chosen;

        jugar();

        gotoxy(25, 19);
        cout << "Deseas jugar otra vez? (s/n): ";
        opcion = _getch();
    } while (opcion == 's' || opcion == 'S');

    gotoxy(25, 21);
    cout << "Gracias por jugar!" << endl;
    return 0;
}

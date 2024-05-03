// Snake_v0.01a.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>

using std::vector;
using std::cout;
using std::endl;

COORD _POSITION = { 0, 0 };
HANDLE _HCONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO structCursorInfo;

static size_t BoardPositionX = 0;
static size_t BoardPositionY = 0;
static size_t BoardWidth = 50;
static size_t BoardHeight = 25;

void gotoxy(int _x, int _y) {
    _POSITION.X = _x;
    _POSITION.Y = _y;
    SetConsoleCursorPosition(_HCONSOLE, _POSITION);
}

class GameBoard {
public:
    GameBoard(int _x, int _y, size_t _width, size_t _height) 
        : pos_x(_x), pos_y(_y), width(_width), height(_height) {
        init();
        //set(5, 5, 1);
    }
    void show_debug() {        
        gotoxy(pos_x + 55, pos_y);
        size_t i = 0;
        size_t offset = 0;
        for (auto cell : iboard) {
            cout << cell;
            ++i;
            if (i == width) {
                ++offset;
                gotoxy(pos_x + 55, pos_y + offset);
                i = 0;
            }
        }
    }
    void show() {
        gotoxy(pos_x, pos_y);
        size_t i = 0;
        size_t offset = 0;
        for (auto cell : iboard) {
            switch (cell) {
            case 0: 
                cout << ' ';
                break;
            case 1:
                cout << (char)205;
                break;
            case 2:
                cout << (char)186;
                break;
            case 3:
                cout << (char)201;
                break;
            case 4:
                cout << (char)187;
                break;
            case 5:
                cout << (char)200;
                break;
            case 6:
                cout << (char)188;
                break;
            default:
                break;
            }

            ++i;
            if (i == width) {
                ++offset;
                gotoxy(pos_x, pos_y + offset);
                i = 0;
            }
        }
    }
private:
    void init() {
        for (size_t i = 0; i != (width * height); ++i) {
            iboard.push_back(0);
        }
        for (size_t i = 0; i != width; ++i) {
            iboard[i] = 1;
        }
        for (size_t i = width * (height - 1); i != width * height; ++i) {
            iboard[i] = 1;
        }
        for (size_t i = width; i < width * height; i += width) {
            iboard[i] = 2;
        }
        for (size_t i = width - 1; i < width * height; i += width) {
            iboard[i] = 2;
        }
        iboard[0] = 3;
        iboard[width - 1] = 4;
        iboard[width * (height - 1)] = 5;
        iboard[(width * height) - 1] = 6;

    }
    void set(size_t row, size_t col, int value) {
        iboard[(row - 1) * 10 + (col - 1)] = value;
    }
    int pos_x;
    int pos_y;
    vector<int> iboard;
    size_t width;
    size_t height;
};

class Snake {
public:
    Snake(size_t _x, size_t _y) 
        : pos_x(_x), pos_y(_y), length(1) {

    }
    void move_up() {
        if (check_top_board()) {
            hide();
            --pos_y;
            show();
        }
    }
    void move_down() {
        if (check_down_board()) {
            hide();
            ++pos_y;
            show();
        }
    }
    void move_right() {
        if (check_right_board()) {
            hide();
            ++pos_x;
            show();
        }
    }
    void move_left() {
        if (check_left_board()) {
            hide();
            --pos_x;
            show();
        }
    }
    void spawn() {
        show();
    }
private:
    void show() {
        gotoxy(pos_x, pos_y);
        cout << '@';
    }
    void hide() {
        gotoxy(pos_x, pos_y);
        cout << ' ';
    }
    bool check_left_board() {
        if (pos_x != (BoardPositionX + 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_right_board() {
        if (pos_x != (BoardPositionX + BoardWidth - 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_top_board() {
        if (pos_y != (BoardPositionY + 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down_board() {
        if (pos_y != (BoardPositionY + BoardHeight - 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    int pos_x;
    int pos_y;
    vector<int> ifig;
    size_t length;
};

int main()
{
    
    GetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // 
    structCursorInfo.bVisible = FALSE;                  // 
    SetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // Отключение курсора в консоли

    GameBoard snakeBoard(BoardPositionX, BoardPositionY, BoardWidth, BoardHeight);

    snakeBoard.show();
    snakeBoard.show_debug();

    Snake snake(5, 5);

    snake.spawn();

    char key = ' ';

    while (true) {
        while (!_kbhit()) {

        }
        key = _getch();
        switch (key) {
        case 72:
            snake.move_up();
            break;
        case 80:
            snake.move_down();
            break;
        case 75:
            snake.move_left();
            break;
        case 77:
            snake.move_right();
            break;
        case 27:
            break;
        default:
            break;
        }
    }

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

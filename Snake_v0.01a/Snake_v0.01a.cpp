// Snake_v0.01a.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define DEBUG // Убрать по готовности

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

static size_t BoardPositionX = 1;
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
        gotoxy(pos_x + BoardWidth + 15, pos_y);
        size_t i = 0;
        size_t offset = 0;
        for (auto cell : iboard) {
            cout << cell;
            ++i;
            if (i == width) {
                ++offset;
                gotoxy(pos_x + BoardWidth + 15, pos_y + offset);
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
            case 7:
                cout << '@';
                break;
            case 8:
                cout << '*';
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
    void set_value(size_t col, size_t row, int value) {
        set(row, col, value);
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
        iboard[(row - 1) * width + (col - 1)] = value;
    }
    size_t pos_x;
    size_t pos_y;
    vector<int> iboard;
    size_t width;
    size_t height;
};

class Snake {
public:
    Snake(size_t _x, size_t _y, GameBoard& _refBoard) 
        : pos_x(_x), pos_y(_y), length(1), refBoard(_refBoard) {

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
#ifndef DEBUG
        Beep(600, 100);
        Beep(500, 100);
#endif  
        show();
    }
    size_t get_posX() {
        return pos_x;
    }
    size_t get_posY() {
        return pos_y;
    }
private:
    void show() {
        refBoard.set_value(pos_x, pos_y, 7);
        refBoard.show();
#ifdef DEBUG
        refBoard.show_debug();
#endif
    }
    void hide() {
        refBoard.set_value(pos_x, pos_y, 0);
    }
    bool check_left_board() {
        if (pos_x != (BoardPositionX + 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_right_board() {
        if (pos_x != (BoardPositionX + BoardWidth - 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_top_board() {
        if (pos_y != (BoardPositionY + 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down_board() {
        if (pos_y != (BoardPositionY + BoardHeight - 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    size_t pos_x;
    size_t pos_y;
    vector<int> ifig;
    size_t length;
    GameBoard& refBoard;
};

struct Tail {
    size_t pos_x = 0;
    size_t pos_y = 0;
};

class TSnake {
public:
    TSnake(size_t _x, size_t _y, GameBoard& _refBoard)
        : pos_x(_x), pos_y(_y), length(4), refBoard(_refBoard) {
        for (size_t i = 1; i != length; ++i) {
            Tail tail{ pos_x + i, pos_y };
            tvec.push_back(tail);
        }
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
#ifndef DEBUG
        Beep(600, 100);
        Beep(500, 100);
#endif  
        show();
    }
    size_t get_posX() {
        return pos_x;
    }
    size_t get_posY() {
        return pos_y;
    }
private:
    void show() {
        //refBoard.set_value(pos_x, pos_y, 7);
        for(auto c : tvec) {
            refBoard.set_value(c.pos_x, c.pos_y, 7);
        }
        refBoard.show();
#ifdef DEBUG
        refBoard.show_debug();
#endif
    }
    void hide() {
        //auto it_bgn = tvec.begin();
        refBoard.set_value((tvec.end() - 1)[0].pos_x, pos_y, 0);
    }
    bool check_left_board() {
        if (pos_x != (BoardPositionX + 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_right_board() {
        if (pos_x != (BoardPositionX + BoardWidth - 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_top_board() {
        if (pos_y != (BoardPositionY + 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down_board() {
        if (pos_y != (BoardPositionY + BoardHeight - 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    size_t pos_x;
    size_t pos_y;
public:
    vector<Tail> tvec;
    size_t length;
    GameBoard& refBoard;
    
};

class Food {
public:
    Food(GameBoard& _refBoard, Snake& _refSnake) 
        : refBoard(_refBoard), refSnake(_refSnake), pos_x(0), pos_y(0) {
        start_X = BoardPositionX + 2;
        end_X = BoardPositionX + BoardWidth - 2;
        start_Y = BoardPositionY + 2;
        end_Y = BoardPositionY + BoardHeight - 2;
    }
    Food(size_t _x, size_t _y, GameBoard& _refBoard, Snake& _refSnake)
        : pos_x(_x), pos_y(_y), refBoard(_refBoard), refSnake(_refSnake) {
    }
    void spawn() {
        do {
            pos_x = rand() % (end_X - start_X) + start_X;
            pos_y = rand() % (end_Y - start_Y) + start_Y;
        } while (check_collision());
        show();
    }
    void eat() {
#ifndef DEBUG
        Beep(500, 50);
#endif
        ++counter;
        gotoxy(BoardPositionX + BoardWidth + 3, BoardPositionY + 3);
        cout << "Counter: " << counter;
    }
    bool check_collision() {
        if (refSnake.get_posX() == pos_x && refSnake.get_posY() == pos_y) {
            return true;
        }
        else {
            return false;
        }
    }
    int get_cnt() {
        return counter;
    }
private:
    void show() {
        refBoard.set_value(pos_x, pos_y, 8);
        refBoard.show();
#ifdef DEBUG
        refBoard.show_debug();
#endif
    }
    void hide() {
        refBoard.set_value(pos_x, pos_y, 0);
    }
    //char symbol = '*';
    size_t start_X;
    size_t end_X;
    size_t start_Y;
    size_t end_Y;
    size_t pos_x;
    size_t pos_y;
    GameBoard& refBoard;
    Snake& refSnake;
    static size_t counter;
};

size_t Food::counter = 0;

int main()
{

    srand(time(0));
    
    GetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // 
    structCursorInfo.bVisible = FALSE;                  // 
    SetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // Отключение курсора в консоли

    GameBoard snakeBoard(BoardPositionX, BoardPositionY, BoardWidth, BoardHeight);

    snakeBoard.show();
#ifdef DEBUG
    snakeBoard.show_debug();
#endif
    GameBoard& refBoard = snakeBoard;

    TSnake snake(5, 5, refBoard);

    TSnake& refSnake = snake;
    //Food apple(refBoard, snake);
    snake.spawn();    
    //apple.spawn();
    char key = ' ';
    
    gotoxy(BoardPositionX + BoardWidth + 3, BoardPositionY + 3);
    //cout << "Counter: " << apple.get_cnt();

    while (true) {
       /* if (apple.check_collision()) {
            apple.eat();
            apple.spawn();
        }*/
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

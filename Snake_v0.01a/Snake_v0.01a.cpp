// Snake_v0.01a.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

//#define DEBUG // Убрать по готовности
//#define MENU

// Сделать рейтинг Топ-3
// + Координаты появления яблока на поле не должны совпадать с координатами змеи
// + Сделать восстановление рамок игрового поля после проигрыша
// + Сделать snake параметр скорости Speed вместо прямой задержки Latency
// + Добавить заголовки в меню
// + Добавить описание настройки в меню Settings
// + Добавить возможность проигрыша в игре
// + Добавить возможность после проигрыша начать игру сначала
// + Добавить сохранение рекордов в файл
// Рефакторинг кода
// Дебаг

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <conio.h>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;

COORD _POSITION = { 0, 0 };
HANDLE _HCONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO structCursorInfo;

static size_t BoardPositionX = 1;
static size_t BoardPositionY = 0;
static size_t BoardWidth = 50;
static size_t BoardHeight = 25;

static size_t MenuWidth = 25;
static size_t MenuHeight = 10;
static size_t MenuPositionX = (BoardPositionX / 2) + (MenuWidth / 2);
static size_t MenuPositionY = (BoardPositionY / 2) + (MenuHeight / 2);

static size_t MenuWidth_1 = MenuWidth - 2;
static size_t MenuHeight_1 = MenuHeight - 2;
static size_t MenuPositionX_1 = MenuPositionX + 1;
static size_t MenuPositionY_1 = MenuPositionY + 1;

void gotoxy(int _x, int _y) {
    _POSITION.X = _x;
    _POSITION.Y = _y;
    SetConsoleCursorPosition(_HCONSOLE, _POSITION);
}
class Food;
class Board {
public:
    Board(size_t _x, size_t _y, size_t _width, size_t _height)
        : pos_x(_x), pos_y(_y), width(_width), height(_height) {
    }
    virtual Board init() {
        return *this;
    }
protected:  
    size_t pos_x;
    size_t pos_y;
    vector<int> iboard;
    size_t width;
    size_t height;
};

class Menu : public Board {
public:
    Menu(size_t _x, size_t _y, vector<string>& _list, string _header, string _description = " ")
        : Board(_x, _y, MenuWidth + max_str_size * 2, MenuHeight + list.size() * 2) {
        list = _list;
        header = _header;
        description = _description;
        init();
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
            case 9:
                cout << '#';
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
        size_t j = 0;
        set_selector();
        gotoxy((MenuPositionX + 5), (MenuPositionY + 1));
        cout << header;
        gotoxy((MenuPositionX + 3), (MenuPositionY + 2));
        cout << description;
        for (auto& c : list) {
            if (list.size() > 1) {
                gotoxy((MenuPositionX + (MenuWidth - max_str_size) / 2) - 1, (MenuPositionY + MenuHeight / list.size() + j));
            }
            else {
                gotoxy((MenuPositionX + (MenuWidth - max_str_size) / 3) - 1, (MenuPositionY + MenuHeight / 2 + j));
            }
            cout << c;
            j += 2;
        }
    }
    void set_selector() {
        if (list[selector][0] != '*') {
            list[selector] = "*" + list[selector];
        }
    }
    void move_up() {
        if (selector != 0) {
            list[selector].erase(0, 1);
            --selector;
            show();
        }
    }
    void move_down() {
        if ((selector + 1) != list.size()) {
            list[selector].erase(0, 1);
            ++selector;
            show();
        }
    }
    size_t get_selector() {
        return selector;
    }
private:
    Board init() {
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

        for (auto& c : list) {
            if (c.size() > max_str_size) {
                max_str_size = c.size();
            }
            c = " " + c;
        }
        return *this;
    }

    string header;
    string description;
    vector<string> list;
    size_t selector = 0;
    size_t max_str_size = 0;

};

class GameBoard : public Board {
public:
    GameBoard(size_t _x, size_t _y, size_t _width, size_t _height) 
        : Board(_x, _y, _width, _height) {
        init();
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
            case 9:
                cout << '#';
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
    void end_of_round(Food _apple, GameBoard& _refBoard);
    vector<int>& get_vec_board() {
        return iboard;
    }
    Board init() {
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
        return *this;
    }
private:
    GameBoard set(size_t row, size_t col, int value) {
        iboard[(row - 1) * width + (col - 1)] = value;
        return *this;
    }    
};

class Snake {
public:
    Snake(size_t _x, size_t _y, GameBoard& _refBoard) 
        : pos_x(_x), pos_y(_y), length(1), refBoard(_refBoard) {

    }
    void move_up() {
        if (check_top()) {
            hide();            
            --pos_y;
            show();
        }
    }
    void move_down() {
        if (check_down()) {
            hide();
            ++pos_y;
            show();
        }
    }
    void move_right() {
        if (check_right()) {
            hide();
            ++pos_x;
            show();
        }
    }
    void move_left() {
        if (check_left()) {
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
    bool check_left() {
        if (pos_x != (BoardPositionX + 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_right() {
        if (pos_x != (BoardPositionX + BoardWidth - 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_top() {
        if (pos_y != (BoardPositionY + 2)) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down() {
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

struct SnakeElement {
    char symbol = ' ';
    size_t pos_x = 0;
    size_t pos_y = 0;
};

class TSnake {
public:
    TSnake(size_t _x, size_t _y, GameBoard& _refBoard, size_t _length)
        : length(_length), length_start(_length), refBoard(_refBoard) {
        //snake.push_back(head);
        /*snake.push_back({ '#', head.pos_x + 1, head.pos_y });
        snake.push_back({ '#', head.pos_x + 2, head.pos_y });
        snake.push_back({ '#', head.pos_x + 3, head.pos_y });*/
        create();
    }
    TSnake grow() {
        ++length;
        auto _bgn = snake.begin();
        auto _end = snake.end();
        size_t ins_x = 0;
        size_t ins_y = 0;
        switch (direction)
        {
        case 1:
            /*ins_x = head.pos_x - 1;
            ins_y = head.pos_y;*/
            //head.pos_x - 1;
            break;
        case 2:
            /*ins_x = head.pos_x;
            ins_y = head.pos_y - 1;*/
            //head.pos_y - 1;
            break;
        case 3:
            /*ins_x = head.pos_x + 1;
            ins_y = head.pos_y;*/
            //head.pos_x + 1;
            break;
        case 4:
           /* ins_x = head.pos_x;
            ins_y = head.pos_y + 1;*/
            //head.pos_y + 1;
            break;
        default:
            break;
        }
        snake.insert(_bgn, { '#', head.pos_x, head.pos_y });
        return *this;
    }
    size_t get_posX() {
        return head.pos_x;
    }
    size_t get_posY() {
        return head.pos_y;
    }
    void create() {
        head = { '#', BoardPositionX + BoardWidth / 2, BoardPositionY + BoardHeight / 2 };
        snake.clear();
        for (size_t i = 1; i <= length; ++i) {
            snake.push_back({ '#', head.pos_x + i, head.pos_y });
        }
        direction = 1;
        alive = true;
        latency = 1000 / speed;
    }
    void move_up() {
        if (check_top()) {
            move();
            --head.pos_y;
            show();
            direction = 2;
            Sleep(latency);
        }
    }
    void move_down() {
        if (check_down()) {
            move();
            ++head.pos_y;
            show();
            direction = 4;
            Sleep(latency);
        }
    }
    void move_right() {
        if (check_right()) {
            move();
            ++head.pos_x;
            show();
            direction = 3;
            Sleep(latency);
        }
    }
    void move_left() {
        if (check_left()) {
            move();
            --head.pos_x;
            show();
            direction = 1;
            Sleep(latency);
        }
       /* else {
            head.pos_x;
        }*/
    }
    void spawn() {
        show();
    }
    void kill() {
        hide();
        //snake = {};
        /*for (auto& c : snake) {
            c = { ' ', 0, 0 };
        }*/
        /*for (size_t i = 1; i <= length; ++i) {
            snake.push_back({ '#', head.pos_x + i, head.pos_y });
        }*/
        /*head.pos_x = BoardPositionX + BoardWidth / 2;
        head.pos_y = BoardPositionY + BoardHeight / 2;*/
        /*head.pos_x = 0;
        head.pos_y = 0;*/
        length = length_start;
        alive = false;
    }
    void move() {
        auto _bgn = snake.begin();
        auto _end = snake.end();
        auto _rend = snake.rend();
        refBoard.set_value((*(_end - 1)).pos_x, (*(_end - 1)).pos_y, 0);
        for (auto _rbgn = snake.rbegin(); _rbgn != (_rend - 1); ++_rbgn) {
            (*_rbgn).pos_x = (*(_rbgn + 1)).pos_x;
            (*_rbgn).pos_y = (*(_rbgn + 1)).pos_y;
        }
        (*_bgn).pos_x = head.pos_x;
        (*_bgn).pos_y = head.pos_y;
    }
    void spawn_XY(size_t _x, size_t _y) {
        head.pos_x = _x;
        head.pos_y = _y;
        for (size_t i = 1; i <= length; ++i) {
            snake.push_back({ '#', head.pos_x + i, head.pos_y });
        }
    }
    void GO() {
        switch (direction)
        {
        case 1:
            move_left();
            Sleep(latency);
            break;
        case 2:
            move_up();
            Sleep(latency);
            break;
        case 3:
            move_right();
            Sleep(latency);
            break;
        case 4:
            move_down();
            Sleep(latency);
            break;
        default:
            break;
        }
        
    }
    bool check_all_collision() {
        if(check_left_board() && check_right_board() && check_top_board() && check_down_board() && check_tail_collision() && alive) {
            return true;
        }
        else {
            return false;
        }
    }
    vector<SnakeElement> snake;
    SnakeElement head;
    size_t length;
    size_t length_start;
    GameBoard& refBoard;
    bool alive;
    
    int get_speed() {
        return speed;
    }
    void set_speed(int _speed) {
        speed = _speed;
        set_latency(1000 / speed);
    }
private:
    void show() {
        refBoard.set_value(head.pos_x, head.pos_y, 9);
        for(auto& c : snake) {
            refBoard.set_value(c.pos_x, c.pos_y, 7);
        }
        refBoard.show();
        //alive = true;
#ifdef DEBUG
        refBoard.show_debug();
#endif
    }
    void hide() {
        refBoard.set_value(head.pos_x, head.pos_y, 0);
        for (auto& c : snake) {
            refBoard.set_value(c.pos_x, c.pos_y, 0);
        }
        refBoard.show();
        //auto it_bgn = tvec.begin();
        //refBoard.set_value((tvec.end() - 1)[0].pos_x, pos_y, 0);
    }
    bool check_tail_collision() {   // Check conflict with tail
        bool collision = true;
        bool thow_first_fl = true;  // Throw firs element of tail for non collision in grow
        for (auto& c : snake) {
            if (thow_first_fl) {
                thow_first_fl = false;
                continue;
            }
            if ((head.pos_x) == c.pos_x && head.pos_y == c.pos_y) {
                collision = false;
                break;
            }
            else {
                collision = true;
            }
        }
        return collision;
    }
    bool check_left() {
        bool collision = true;
        bool thow_first_fl = true;
        for (auto& c : snake) {
            if (thow_first_fl) {
                thow_first_fl = false;
                continue;
            }
            if ((head.pos_x - 1) == c.pos_x && head.pos_y == c.pos_y) {
                collision = false;
                break;
            }
            else {
                collision = true;
            }
        }
        if ((head.pos_x != (BoardPositionX)) && ((head.pos_x - 1) != snake[0].pos_x) && check_tail_collision()) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_right() {
        /*bool collision = true;
        for (auto c : snake) {
            if ((head.pos_x + 1) == c.pos_x && head.pos_y == c.pos_y) {
                collision = false;
                break;
            }
            else {
                collision = true;
            }
        }*/
        bool collision = true;
        bool thow_first_fl = true;
        for (auto& c : snake) {
            if (thow_first_fl) {
                thow_first_fl = false;
                continue;
            }
            if ((head.pos_x + 1) == c.pos_x && head.pos_y == c.pos_y) {
                collision = false;
                break;
            }
            else {
                collision = true;
            }
        }
        if ((head.pos_x != (BoardPositionX + BoardWidth - 1)) && ((head.pos_x + 1) != snake[0].pos_x) && check_tail_collision()) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_top() {
        /*bool collision = true;
        for (auto c : snake) {
            if ((head.pos_y - 1) == c.pos_y && head.pos_x == c.pos_x) {
                collision = false;
                break;
            }
            else {
                collision = true;
            }
        }*/
        if ((head.pos_y != (BoardPositionY + 1)) && ((head.pos_y - 1) != snake[0].pos_y) && check_tail_collision()) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down() {
        /*bool collision = true;
        for (auto c : snake) {
            if ((head.pos_y + 1) == c.pos_y && head.pos_x == c.pos_x) {
                collision = false;
                break;
            }
            else {
                collision = true;
            }
        }*/
        if (head.pos_y != (BoardPositionY + BoardHeight - 0) && ((head.pos_y + 1) != snake[0].pos_y) && check_tail_collision()) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_left_board() {
        bool collision = true;
        if (head.pos_x == (BoardPositionX)) {
            gotoxy(55, 25);
            cout << "Left board";
            return false;
        }
    }
    bool check_right_board() {
        bool collision = true;
        if (head.pos_x == (BoardPositionX + BoardWidth - 1)) {
            gotoxy(55, 25);
            cout << "Right board";
            return false;
        }
    }
    bool check_top_board() {
        bool collision = true;
        if (head.pos_y != (BoardPositionY + 1)) {
            gotoxy(55, 25);
            cout << "Top board";
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down_board() {
        bool collision = true;
        if (head.pos_y != (BoardPositionY + BoardHeight)) {
            gotoxy(55, 25);
            cout << "Down board";
            return true;
        }
        else {
            return false;
        }
    }

    int get_latency() {
        return latency;
    }
    void set_latency(int _latency) {
        latency = _latency;
    }
    
    int direction = 1;
    int latency = 500;
    int speed = 30;
    bool autoGo = true;

};

class Food {
public:
    Food(GameBoard& _refBoard, TSnake& _refSnake)
        : refBoard(_refBoard), refSnake(_refSnake), pos_x(0), pos_y(0) {
        start_X = BoardPositionX + 2;
        end_X = BoardPositionX + BoardWidth - 2;
        start_Y = BoardPositionY + 2;
        end_Y = BoardPositionY + BoardHeight - 2;
    }
    Food(size_t _x, size_t _y, GameBoard& _refBoard, TSnake& _refSnake)
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
        cout << "Score: " << counter << " " << exist_on_Board;
        exist_on_Board = false;
    }
    bool check_collision() {
        if (refSnake.get_posX() == pos_x && refSnake.get_posY() == pos_y &&
            refSnake.head.pos_x == pos_x && refSnake.head.pos_y == pos_y) {
            return true;
        }
        else {
            return false;
        }
    }
    int get_cnt() {
        return counter;
    }
    void reset_cnt() {
        counter = 0;
    }
    bool get_status() {
        return exist_on_Board;
    }
private:
    void show() {
        refBoard.set_value(pos_x, pos_y, 8);
        refBoard.show();
        exist_on_Board = true;
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
    TSnake& refSnake;
    bool exist_on_Board = false;
    static size_t counter;
};

void GameBoard::end_of_round(Food _apple, GameBoard& _refBoard) {
    gotoxy(BoardPositionX + (BoardWidth / 2) - 5, BoardPositionY + (BoardHeight / 2) - 1);
    cout << "GAME OVER";
    int i_record = 0;
    int cur_record = _apple.get_cnt();
    string rec_name;
    std::ifstream read_record_file("record");
    string str_record;
    if (read_record_file.is_open()) {
        //std::getline(read_record_file, str_record);
        read_record_file >> rec_name >> str_record;
        i_record = std::stoi(str_record);   // Max record of the game
    }
    gotoxy(BoardPositionX + (BoardWidth / 2) - 10, BoardPositionY + (BoardHeight / 2) - 1 + 3);
    cout << "Max record: " << rec_name << " - " << i_record;
    gotoxy(BoardPositionX + (BoardWidth / 2) - 10, BoardPositionY + (BoardHeight / 2) - 1 + 4);
    cout << "Your record: " << cur_record << endl;
    read_record_file.close();
    if (cur_record > i_record) {
        gotoxy(BoardPositionX + (BoardWidth / 2) - 10, BoardPositionY + (BoardHeight / 2) - 1 + 5);
        cout << "Enter your name: ";
        cin >> rec_name;
        std::ofstream write_record_file("record");
        write_record_file << rec_name << " " << cur_record;
        write_record_file.close();
    }
    
    _apple.reset_cnt();
    _refBoard.get_vec_board().clear();
    _refBoard.init();
}

size_t Food::counter = 0;

int main()
{

    system("cls");

    srand(time(0));
    
    GetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // 
    structCursorInfo.bVisible = FALSE;                  // 
    SetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // Отключение курсора в консоли

    GameBoard snakeBoard(BoardPositionX, BoardPositionY, BoardWidth, BoardHeight);

    vector<string> list_menu{ "Play", "Settings", "Exit" };
    vector<string> list_menu_Settings{ "Speed" };

    Menu menu(MenuPositionX, MenuPositionY, list_menu, "MAIN MENU");
    Menu menu_settings(MenuPositionX, MenuPositionY, list_menu_Settings, "SETTINGS MENU", "Esc for back to main");
    //GameBoard menu_1(MenuPositionX_1, MenuPositionY_1, MenuWidth_1, MenuHeight_1);
    bool menuFl = true;
    bool menuSettingsFl = false;
    bool gameStarted = false;
    bool gameEnded = true;
    int speed = 0;
    menu.show();

#ifndef MENU

    //snakeBoard.show();
#ifdef DEBUG
    snakeBoard.show_debug();
#endif
    GameBoard& refBoard = snakeBoard;

    TSnake snake(5, 5, refBoard, 3);
    //TSnake* snake = new TSnake(5, 5, refBoard);
    //    
    TSnake& refSnake = snake;

    Food apple(refBoard, refSnake);
    //Food* apple = new Food(refBoard, refSnake);
    
    char key = ' ';
    char key_1 = ' ';

    while (true) {
        if (gameEnded) {

            gameEnded = false;
            gameStarted = true;

            /*GameBoard snakeBoard(BoardPositionX, BoardPositionY, BoardWidth, BoardHeight);
            GameBoard& refBoard = snakeBoard;
            TSnake snake(5, 5, refBoard);
            TSnake& refSnake = snake;*/
            //Food apple(refBoard, refSnake);

            snake.create();

            

            //apple.spawn();

            //delete snake;
            //delete apple;
            /*TSnake* snake = new TSnake(5, 5, refBoard);
            TSnake& refSnake = *snake;
            Food* apple = new Food(refBoard, refSnake);*/
            //snake.spawn_XY(10, 10);


        }
        while (gameStarted) {
            while (menuFl) {
                if (_kbhit()) {
                    key = _getch();
                    switch (key) {
                    case 72:    // Up arrow
                        menu.move_up();
                        break;
                    case 80:    // Down arrow
                        menu.move_down();
                        break;
                    case 13:    // Enter
                        switch (menu.get_selector())
                        {
                        case 0: // Play
                            /*if (!gameStarted) {
                                apple.spawn();
                            }*/
                            gameStarted = true;
                            menuFl = false;
                            gotoxy(BoardPositionX + BoardWidth + 3, BoardPositionY + 3);
                            cout << "Score: " << apple.get_cnt();
                            gotoxy(BoardPositionX + BoardWidth + 3, BoardPositionY + 5);
                            cout << "Speed: " << snake.get_speed();
                            if (!apple.get_status()) {
                                apple.spawn();
                            }
                            break;
                        case 1: // Settings
                            system("cls");
                            menu_settings.show();
                            menuSettingsFl = true;
                            speed = 0;
                            gotoxy((MenuPositionX + (MenuWidth - list_menu_Settings[0].size()) / 3) + list_menu_Settings[0].size() + 3, (MenuPositionY + MenuHeight / 2));
                            cout << snake.get_speed();
                            while (menuSettingsFl) {
                                key_1 = _getch();
                                switch (key_1)
                                {
                                case 13:
                                    if (speed > 0 && speed <= 100) {
                                        snake.set_speed(speed);
                                    }
                                    system("cls");
                                    menu.show();
                                    menuSettingsFl = false;
                                    break;
                                case 27:
                                    system("cls");
                                    menu.show();
                                    menuSettingsFl = false;
                                    break;
                                default:
                                    gotoxy((MenuPositionX + (MenuWidth - list_menu_Settings[0].size()) / 3) + list_menu_Settings[0].size() + 3, (MenuPositionY + MenuHeight / 2));
                                    cin >> speed;
                                    break;
                                }
                            }
                            break;
                        case 2: // Exit
                            system("cls");
                            gotoxy(10, 5);
                            cout << "GAME OVER" << endl;
                            return 0;
                            break;
                        default:
                            break;
                        }
                        break;
                    case 27:    // Esc
                        if (gameStarted) {
                            system("cls");
                            snakeBoard.show();
                            gotoxy(BoardPositionX + BoardWidth + 3, BoardPositionY + 3);
                            cout << "Score: " << apple.get_cnt();
                            gotoxy(BoardPositionX + BoardWidth + 3, BoardPositionY + 5);
                            cout << "Speed: " << snake.get_speed();
                            menuFl = false;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            if (!snake.check_all_collision()) {
                //system("cls");
                /*gotoxy(BoardPositionX + BoardPositionX / 2, BoardPositionY + BoardPositionY / 2);
                cout << "GAME OVER";*/
                gameStarted = false;
                gameEnded = true;
                snake.kill();
                snakeBoard.end_of_round(apple, refBoard);
                _getch();
                apple.spawn();
                break;
                //return 0;
            }
            if (apple.check_collision()) {
                apple.eat();
                snake.grow();
                apple.spawn();
            }
            //while (!_kbhit() && !apple.check_collision() && !snake.check_all_collision()) {
            //    if (snake.check_all_collision()) {
            //        //system("cls");
            //        gotoxy(BoardPositionX + BoardPositionX / 2, BoardPositionY + BoardPositionY / 2);
            //        cout << "GAME OVER";
            //        gameStarted = false;
            //        return 0;
            //    }

            /*}*/
            if (snake.alive) {
                if (_kbhit()) {
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
                        system("cls");
                        menu.show();
                        menuFl = true;
                        break;
                    default:
                        break;
                    }
                }
                else {
                    snake.GO();
                }
            }
        }
    }
#endif
    _getch();
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

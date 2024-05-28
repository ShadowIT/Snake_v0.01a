// Snake_v0.01a.cpp : by Shadow

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <algorithm>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cin;
using std::cout;
using std::endl;
using std::stoi;
using std::getline;
using std::sort;
using std::to_string;

COORD _POSITION = { 0, 0 };                                 // For Console
HANDLE _HCONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);         //
CONSOLE_CURSOR_INFO structCursorInfo;                       //

const size_t BoardPositionX = 1;
const size_t BoardPositionY = 0;
const size_t BoardWidth = 50;
const size_t BoardHeight = 25;

const size_t MenuWidth = 25;
const size_t MenuHeight = 10;
const size_t MenuPositionX = (BoardPositionX / 2) + (MenuWidth / 2);
const size_t MenuPositionY = (BoardPositionY / 2) + (MenuHeight / 2);

void gotoxy(size_t _x, size_t _y) {                         // Move cursor X, Y in Console
    _POSITION.X = _x;
    _POSITION.Y = _y;
    SetConsoleCursorPosition(_HCONSOLE, _POSITION);
}
class Food;
struct Top_Item {                                           // For Top-3 Items 
    string name;                                            // Name
    int score = 0;                                          // Score
};
class Top {                                                 // Top-3 list
public:
    Top(string _file_name)                                  // Constructor
        : file_name(_file_name), length(3) {                // length - Top-3 length
        update();
    }                       
    vector<Top_Item>& update() {                            // Uptade Top-3 vector
        ifstream read_record_file(file_name);
        if (read_record_file.is_open()) {
            string line;
            while (getline(read_record_file, line)) {
                istringstream iss(line);
                string _name;
                string _score;
                iss >> _name >> _score;
                Top_Item Player = { _name, stoi(_score) };
                top_vec.push_back(Player);
                line = "";
            }
        }
        read_record_file.close();
        min_record = (*(top_vec.end() - 1)).score;
        max_record = (*top_vec.begin()).score;
        return top_vec;
    }
    vector<Top_Item>& add_row(string _name, int _score) {   // Add row to Top-3
        Top_Item Player = { _name, _score };
        top_vec.push_back(Player);
        sort_top();
        top_vec.resize(length);
        write_top();
        update();
        return top_vec;
    }
    static bool compare(const Top_Item& left, const Top_Item right) {   // Compare function for sort
        return left.score > right.score;
    }
    vector<Top_Item>& sort_top() {                          // Sort Top-3
        sort(begin(top_vec), end(top_vec), compare);
        return top_vec;
    }
    vector<Top_Item>& get_top() {
        return top_vec;
    }
    vector<Top_Item>& write_top() {                         // Write Top-3 to file
        ofstream write_record_file(file_name);
        for (auto c : top_vec) {
            write_record_file << c.name << " " << c.score << endl;
        }
        write_record_file.close();
        return top_vec;
    }
    Top_Item& get_min_record() {
        return *(top_vec.end() - 1);
    }
    Top_Item& get_max_record() {
        return *(top_vec.begin());
    }
private:
    string file_name;
    size_t length;
    int min_record = 0;
    int max_record = 0;
    vector<Top_Item> top_vec;
};
class Board {                                               // Board in Console
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
class Menu : public Board {                                 // Menu
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
    string header;                          // Menu header
    string description;                     // Menu description under header
    vector<string> list;                    // Menu list
    size_t selector = 0;
    size_t max_str_size = 0;
};
class GameBoard : public Board {                            // Game field
public:
    GameBoard(size_t _x, size_t _y, size_t _width, size_t _height) 
        : Board(_x, _y, _width, _height) {
        init();
    }
    void show_debug() {       // For debug 
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
    void end_of_round(Food _apple, GameBoard& _refBoard, Top _top);                 // Called when you lose
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
struct SnakeElement {                                       // Snake's tail
    char symbol = ' ';                                      // 
    size_t pos_x = 0;                                       // 
    size_t pos_y = 0;
};
class TSnake {                                              // The whole snake
public:
    TSnake(size_t _x, size_t _y, GameBoard& _refBoard, size_t _length)
        : length(_length), length_start(_length), refBoard(_refBoard) {  
        create();
    }
    TSnake grow() {
        ++length;
        auto _bgn = snake.begin();
        auto _end = snake.end();
        size_t ins_x = 0;
        size_t ins_y = 0;
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
    }
    void spawn() {
        show();
    }
    void kill() {
        hide();
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
    int get_speed() {
        return speed;
    }
    void set_speed(int _speed) {
        speed = _speed;
        set_latency(1000 / speed);
    }
    vector<SnakeElement> snake;
    SnakeElement head;
    size_t length;
    size_t length_start;
    GameBoard& refBoard;
    bool alive;
private:
    void show() {
        refBoard.set_value(head.pos_x, head.pos_y, 9);
        for(auto& c : snake) {
            refBoard.set_value(c.pos_x, c.pos_y, 7);
        }
        refBoard.show();
    }
    void hide() {
        refBoard.set_value(head.pos_x, head.pos_y, 0);
        for (auto& c : snake) {
            refBoard.set_value(c.pos_x, c.pos_y, 0);
        }
        refBoard.show();
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
        if ((head.pos_y != (BoardPositionY + 1)) && ((head.pos_y - 1) != snake[0].pos_y) && check_tail_collision()) {
            return true;
        }
        else {
            return false;
        }
    }
    bool check_down() {
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
        Beep(500, 50);
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
    }
    void hide() {
        refBoard.set_value(pos_x, pos_y, 0);
    }
    size_t start_X = 0;
    size_t end_X = 0;
    size_t start_Y = 0;
    size_t end_Y = 0;
    size_t pos_x = 0;
    size_t pos_y = 0;
    GameBoard& refBoard;
    TSnake& refSnake;
    bool exist_on_Board = false;
    static size_t counter;
};
void GameBoard::end_of_round(Food _apple, GameBoard& _refBoard, Top _top) {                 // Called when you lose
    gotoxy(BoardPositionX + (BoardWidth / 2) - 5, BoardPositionY + (BoardHeight / 2) - 1);
    cout << "GAME OVER";
    _top.update();
    int i_record = 0;
    int cur_record = _apple.get_cnt();
    string rec_name;
    gotoxy(BoardPositionX + (BoardWidth / 2) - 10, BoardPositionY + (BoardHeight / 2) - 1 + 3);
    cout << "Max record: " << _top.get_max_record().name << " - " << _top.get_max_record().score;
    gotoxy(BoardPositionX + (BoardWidth / 2) - 10, BoardPositionY + (BoardHeight / 2) - 1 + 4);
    cout << "Your record: " << cur_record << endl;
    if (cur_record > _top.get_min_record().score) {
        gotoxy(BoardPositionX + (BoardWidth / 2) - 10, BoardPositionY + (BoardHeight / 2) - 1 + 5);
        cout << "Enter your name: ";
        cin >> rec_name;
        _top.add_row(rec_name, cur_record);
    }
    _apple.reset_cnt();                                 // Reset score counter
    _refBoard.get_vec_board().clear();                  // Clear the game field
    _refBoard.init();                                   // Draw an entire field without damage
}
size_t Food::counter = 0;

int main()
{
    system("cls");
    srand(time(0)); 
    GetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // 
    structCursorInfo.bVisible = FALSE;                  // 
    SetConsoleCursorInfo(_HCONSOLE, &structCursorInfo); // Отключение курсора в консоли
    Top top("record");
    GameBoard snakeBoard(BoardPositionX, BoardPositionY, BoardWidth, BoardHeight);
    vector<string> list_menu{ "Play", "Settings", "Rating" , "Exit"};
    vector<string> list_menu_Settings{ "Speed" };
    vector<string> list_menu_Raiting;
    for (auto c : top.get_top()) {
        list_menu_Raiting.push_back(c.name + " " + to_string(c.score));
    }
    Menu menu(MenuPositionX, MenuPositionY, list_menu, "MAIN MENU");
    Menu menu_settings(MenuPositionX, MenuPositionY, list_menu_Settings, "SETTINGS MENU", "Esc for back to main");
    Menu menu_rating(MenuPositionX, MenuPositionY, list_menu_Raiting, "RAITING", "Esc for back to main");
    bool menuFl = true;
    bool menuSettingsFl = false;
    bool menuRatingFl = false;
    bool gameStarted = false;
    bool gameEnded = true;
    int speed = 0;
    menu.show();
    GameBoard& refBoard = snakeBoard;
    TSnake snake(5, 5, refBoard, 3);   
    TSnake& refSnake = snake;
    Food apple(refBoard, refSnake);
    char key = ' ';
    while (true) {
        if (gameEnded) {
            gameEnded = false;
            gameStarted = true;
            snake.create();
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
                                key = _getch();
                                switch (key)
                                {
                                case 13:                    // Enter
                                    if (speed > 0 && speed <= 100) {
                                        snake.set_speed(speed);
                                    }
                                    system("cls");
                                    menu.show();
                                    menuSettingsFl = false;
                                    break;
                                case 27:                    // Esc
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
                        case 2: // Rating
                            system("cls");
                            menu_rating.show();
                            menuRatingFl = true;
                            while (menuRatingFl) {  
                                if (_getch() == 27) {
                                    system("cls");
                                    menu.show();
                                    menuRatingFl = false;
                                }
                            }
                            break;
                        case 3: // Exit
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
            if (!snake.check_all_collision()) {             // Check collision with game field's border or tail
                gameStarted = false;
                gameEnded = true;
                snake.kill();
                snakeBoard.end_of_round(apple, refBoard, top);
                int p = _getch();
                apple.spawn();
                break;
            }
            if (apple.check_collision()) {                  // Check collision with food for eating
                apple.eat();
                snake.grow();
                apple.spawn();
            }
            if (snake.alive) {
                if (_kbhit()) {
                    key = _getch();
                    switch (key) {
                    case 72:                    // Up arrow
                        snake.move_up();
                        break;
                    case 80:                    // Down arrow
                        snake.move_down();
                        break;
                    case 75:
                        snake.move_left();      // Left arrow
                        break;
                    case 77:                    // Right arrow
                        snake.move_right();
                        break;
                    case 27:                    // Esc
                        system("cls");
                        menu.show();
                        menuFl = true;
                        break;
                    default:
                        break;
                    }
                }
                else {
                    snake.GO();                 // Auto go snake
                }
            }
        }
    }
}
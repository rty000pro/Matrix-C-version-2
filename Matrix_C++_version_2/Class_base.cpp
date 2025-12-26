#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "Class_base.h"
#include "Windows.h"
#include <conio.h>
//Звуковое оповещение
void Matrix::sound_system(char type_sound) {
    if (type_sound == 'I') { //init
        Beep(500, 300);
    }
    else if (type_sound == 'E') { //ERROR
        Beep(230, 1500);
    }
    else if (type_sound == 'S') { //start
        Beep(1000, 200);
        Beep(1000, 200);
    }
}
// Иницилизация и вывод матрици
void Matrix::init_matrix() {//Иницилизируем
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    sound_system('I');
    for (int i = 0; i != 30; i++) {
        for (int j = 0; j != 120; j++) {
            Matrix[i][j] = ' ';
        }
    }
}
void Matrix::print_matrix() {// Выводим матрицу
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coord);

    for (int i = 0; i != 30; i++) {
        for (int j = 0; j != 120; j++) {
            std::cout << Matrix[i][j];
        }
        std::cout << std::endl;
    }
}
//Создания макета карты для матрици
void Matrix::create_arena(int rows, int cols) {// Ограничение до 29x29, дальше идут артефакты (10.12.25)
    if (rows > 29 && cols > 29) { // Специально поставил ограничение хотя его будет легко обойти.
        sound_system('E');
        std::cout << "Please change the create_arena() configuration." << std::endl;
    }
    else {
        int x_centry = 30 / 2;
        int y_centry = 120 / 2;
        cols *= 2;
        for (int i = y_centry - (cols / 2), j = 0; j != cols; i++, j++) {
            Matrix[x_centry - (rows / 2)][i] = '#';
        }
        for (int i = y_centry - (cols / 2), j = 0; j != cols; i++, j++) {
            Matrix[x_centry + (rows / 2)][i] = '#';
        }
        Matrix[x_centry + (rows / 2)][(y_centry - (cols / 2)) + cols] = '#';
        for (int i = x_centry - (rows / 2), j = 0; j != rows; i++, j++) {
            Matrix[i][y_centry - (cols / 2)] = '#';
        }
        for (int i = x_centry - (rows / 2), j = 0; j != rows; i++, j++) {
            Matrix[i][y_centry + (cols / 2)] = '#';
        }
    }
}
//Быстрое иницилизация и создания макета
void Matrix::start_matrix_base() {// Заранне команда что бы не писать по 100 раз
    init_matrix();
    create_arena(19, 19);
}
//Иницилизация игрока и управления
void Matrix::init_base_control() {
    sound_system('I');
    x_player = x_centry; //Координаты игрока
    y_player = y_centry;
    old_x_player = x_player;
    old_y_player = y_player;
    model_player = '*';// Как выглядит игрок
    num_model_player = 0; // Номер модели для функции 'change_icon()'
    Matrix[x_player][y_player] = model_player;
}

void Matrix::control() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        ch = _getch();
    }
    switch (ch) {
    case 'w': case 'W':
        if (Matrix[x_player - 1][y_player] == '#') {
            break;
        }
        else {
            x_player -= 1;
            break;
        }
    case 's': case 'S':
        if (Matrix[x_player + 1][y_player] == '#') {
            break;
        }
        else {
            x_player += 1;
            break;
        }
    case 'a': case 'A':
        if (Matrix[x_player][y_player - 1] == '#') {
            break;
        }
        else {
            y_player -= 1;
            break;
        }
    case 'd': case 'D':
        if (Matrix[x_player][y_player + 1] == '#') {
            break;
        }
        else {
            y_player += 1;
            break;
        }
    }
}
void Matrix::update_visual_model() { // Обновление положение игрока
    Matrix[old_x_player][old_y_player] = ' ';
    Matrix[x_player][y_player] = model_player;
    old_x_player = x_player;
    old_y_player = y_player;
}
void Matrix::interaction() { //Взаимодействие, временно отрубил, пока-что не могу понять почему игрок не двигается (26.12.25)
    if (GetAsyncKeyState('E') & 0x8000) {
        for (int i = y_player - 1; i != y_player + 1; i++) {
            if (Matrix[x_player - 1][i] == '1') {
                change_icon();
            }
        }
        for (int i = y_player - 1; i != y_player + 1; i++) {
            if (Matrix[x_player + 1][i] == '1') {
                change_icon();
            }
        }
        /* Это штука не стабильна
        if (Matrix[x_player + 1][y_centry + 1] == '1') {
            change_icon();
        }
        */
        for (int i = x_player - 1; i !=  x_player + 1; i++) {
            if (Matrix[i][y_player - 1] == '1') {
                change_icon();
            }
        }
        for (int i = x_player - 1; i != x_player + 1; i++) {
            if (Matrix[i][y_player + 1] == '1') {
                change_icon();
            }
        }
    }
}
//Загрузка карты
void Matrix::load_map_from_file(int load_limiter) { //Наконец-то я это починил(26.12.25)
    std::ifstream maps("Maps.txt");
    if (!maps.is_open()) {
        sound_system('E');
        std::cout << "Error! file 'Maps.txt' is not open. Pleas check file for damage or whether it is open!" << std::endl;
    }
    std::string map_line;
    int stop = 0;
    int i = x_centry - (20 / 2);
    while (std::getline(maps, map_line)) {
        if (map_line == "stop-map-load" && stop == load_limiter) {
            break;
        }
        if (map_line == "stop-map-load") {
            stop++;
            i = x_centry - (20 / 2);
            continue;
        }
        for (int j = y_centry - (40 / 2), m = 0; m != map_line.size(); j++, m++) {
            Matrix[i][j] = map_line[m];
        }
        i++;
    }
}
//Команда для визуальной прогрузки игрока
void Matrix::CPM_control() {
    control();
    update_visual_model();
}
//Сохранение и загрузка
void Matrix::save(std::string name_save) {
    std::ofstream save_file(name_save);
    save_file << x_player << std::endl << y_player << std::endl << status_load_map << std::endl;
}
void Matrix::load(std::string name_save) {
    std::ifstream load_file(name_save);
    std::string line;
    int i = 0;
    while (std::getline(load_file,line), i != 2) {
        if (i == 0) {
            x_player = std::stoi(line);
        }
        else if (i == 1) {
            y_player = std::stoi(line);
        }
        else if (i == 2) {
            //load_map(line);
        }
        else {
            sound_system('E');
            std::cout << "Error with load function!" << std::endl;
        }
        i++;
    }
}
//Игровые команды(Вызов интерфейса, функция взаимодействующего предмета)
void Matrix::change_icon() {
    char list_icon[] = { '*','P','@','•' };
    num_model_player++;
    model_player = list_icon[num_model_player];
}
/* Временно отложенно
void music_test() {

}*/

#include <iostream>
#include <fstream>
#include <string>
#include "Class_base.h"
#include "Windows.h"
#include "Maps.h"

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
    sound_system('I');
    for (int i = 0; i != 30; i++) {
        for (int j = 0; j != 120; j++) {
            Matrix[i][j] = ' ';
        }
    }
}
void Matrix::print_matrix() {// Выводим матрицу
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
    create_arena(20, 20);
}
//Иницилизация игрока и управления
void Matrix::init_base_control() {
    sound_system('I');
    x_player = x_centry; //Координаты игрока
    y_player = y_centry;
    old_x_player = x_player;
    old_y_player = y_player;
    model_player = '*';// Как выглядит игрок
    Matrix[x_player][y_player] = model_player;
}

void Matrix::control() { //Управление
    if (GetAsyncKeyState(VK_UP)) {
        x_player -= 1;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        x_player += 1;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        y_player += 1;
    }
    if (GetAsyncKeyState(VK_RIGHT)) {
        y_player -= 1;
    }
}
void Matrix::update_visual_model() { // Обновление положение игрока
    Matrix[old_x_player][old_y_player] = ' ';
    Matrix[x_player][y_player] = model_player;
    old_x_player = x_player;
    old_y_player = y_player;
}
//Загрузка карты
void Matrix::load_map(std::string name_map) { //Не могу понять почему он не грузит карту
    if (name_map == "map0") {
        for (int i = x_centry - (20 / 2), copy_i = 0; copy_i <= 20; i++, copy_i++) {
            for (int j = y_centry - (40 / 2), copy_j = 0; copy_j <= 40; i++, copy_j++) {
                Matrix[i][j] = map0[copy_i][copy_j];
            }
        }
        status_load_map = "map0";
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
            load_map(line);
        }
        else {
            sound_system('E');
            std::cout << "Error with load function!" << std::endl;
        }
        i++;
    }
}
#pragma once //Решил прибраться в файлах и разбить на 2 файла .h и .cpp для простоты работы (23.12.2025)
#include <string>
class Matrix {
private:
    //Параметры игрока
    int x_player;
    int y_player;
    int old_x_player;
    int old_y_player;
    char model_player;
    //Центр карты
    int x_centry = 30 / 2;
    int y_centry = 120 / 2;
    //Размер матрицы
    char Matrix[30][120];
    //Загруженная карта
    std::string status_load_map;
public:
    //Звуковые оповещение
    void sound_system(char type_sound);
    //Инцилизация матрици и заполнение
    void init_matrix();
    void print_matrix();
    //Создание макета карты для матрицы
    void create_arena(int rows, int cols);
    //Быстрая загрузка матрицы и макета
    void start_matrix_base();
    //Иницилизация игрока и управления
    void init_base_control();
    void control();
    void update_visual_model();
    //Загрузка карты
    void load_map(std::string name_map);
    //Команда для визуальной прогрузки игрока
    void CPM_control();
    //Сохранение и загрузка
    void save(std::string name_save);
    void load(std::string name_save);
};
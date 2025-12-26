#include "Class_base.h"
#include <iostream>
#include <Windows.h>
int main() {
    Matrix m1;
    m1.start_matrix_base();
    m1.load_map_from_file(1);
    m1.init_base_control();
    m1.print_matrix();
    while (true) {
        m1.CPM_control();
        //m1.interaction();
        m1.print_matrix();
    }
}

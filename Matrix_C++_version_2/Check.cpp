#include "Class_base.h"
#include <iostream>
int main() {
    Matrix m1;
    m1.start_matrix_base();
    m1.load_map_from_file(1);
    m1.print_matrix();
}

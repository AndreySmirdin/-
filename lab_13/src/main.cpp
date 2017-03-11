#include <cstddef>
#include "my_array.h"


int main() {
    my_array<bool, 20> a;
    a[3] = 1;
    a[3] = 0;
    a[7] = 1;
    a[8] = 1;
    a[16] = 1;
    a[16] = 0;
    a[19] = 1;
    for (int i = 0; i < 20; i++)
        std::cout <<"i = " << i << " " <<  a[i] << std::endl;

    my_array<int, 20> b;
    b[3] = 3;
    b[3] = 6;
    b[16] = 8;
    b[19] = 1;

    b.fill(1488);
    b[3] = 3;
    b[3] = 6;
    b[16] = 8;
    b[19] = 1;
    b.at(19) = 0;
    for (int i = 0; i < 20; i++)
        std::cout <<"i = " << i << " " <<  b[i] << std::endl;
}

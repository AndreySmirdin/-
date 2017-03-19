#include <string>
#include <fstream>
#include <iostream>
#include "matrix.h"


int main() {
    Matrix *regs = new Matrix[10];
    std::string cmd;
    int reg1, reg2;
    char dollar;

    while (std::cin >> cmd) {
        try {
            if (cmd == "exit")
                break;

            else if (cmd == "load") {
                std::string fname;
                std::cin >> dollar >> reg1 >> fname;
                std::ifstream fin(fname.c_str());
                fin >> regs[reg1];
                fin.close();
            }

            else if (cmd == "print") {
                std::cin >> dollar >> reg1;
                std::cout << regs[reg1];
            }

            else if (cmd == "add") {
                std::cin >> dollar >> reg1 >> dollar >> reg2;
                regs[reg1] += regs[reg2];
            }

            else if (cmd == "mul") {
                std::cin >> dollar >> reg1 >> dollar >> reg2;
                regs[reg1] *= regs[reg2];
            }

            else if (cmd == "elem") {
                int i, j;
                std::cin >> dollar >> reg1 >> i >> j;
                std::cout << regs[reg1].get(i, j);

            }
        }

        catch (MatrixException e) {
            std::cout << e.what();
        }
    }

    delete[] regs;
    return 0;
}

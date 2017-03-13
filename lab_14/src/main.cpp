#include <string>
#include <fstream>
#include <iostream>
#include "../include/matrix.h"

using namespace std;


int main() {
    Matrix regs[10];
    string cmd;
    int reg1, reg2;
    char dollar;

    while (cin >> cmd) {
        if (cmd == "exit")
            break;

        else if (cmd == "load") {
            string fname;
            cin >> dollar >> reg1 >> fname;
            ifstream fin(fname.c_str());
            fin >> regs[reg1];
            fin.close();
        }

        else if (cmd == "print"){
            cin >> dollar >> reg1;
            cout << regs[reg1];
        }

        else if (cmd == "add"){
            cin >> dollar >> reg1 >> dollar >> reg2;
            try {
                regs[reg1] += regs[reg2];
            }
            catch (MatrixException e) {
                cout << e;
            }
        }

        else if (cmd == "mul"){
            cin >> dollar >> reg1 >> dollar >> reg2;
            try {
                regs[reg1] *= regs[reg2];
            }
            catch (MatrixException e) {
                cout << e;
            }
        }

        else if (cmd == "elem"){
            int i, j;
            cin >> dollar >> reg1 >> i >> j;
            try {
                cout << regs[reg1].get(i, j);
            }
            catch (MatrixException e) {
                cout << e;
            }
        }
    }
}
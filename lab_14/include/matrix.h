#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H
#include <cstddef>
#include <ostream>
#include <fstream>
#include "string"

class Matrix {
public:
    Matrix();
    Matrix(std::size_t r, std::size_t c);
    Matrix(const Matrix& m);
    ~Matrix();

    std::size_t get_rows() const;
    std::size_t get_cols() const;

    void set(std::size_t i, std::size_t j, int val);
    int  get (std::size_t i, std::size_t j) const;

    Matrix operator=(const Matrix& m);

    Matrix& operator+=(const Matrix& m);
    Matrix& operator*=(const Matrix& m);

private:
    std::size_t _rows;
    std::size_t _cols;
    int **_data;
};


std::ostream &operator<<(std::ostream &os, const Matrix &m);
std::ifstream& operator>>(std::ifstream& fis, Matrix& m);



class MatrixException : public std::logic_error{
public:
    MatrixException(std::string msg) : logic_error(msg){};
};

#endif //MATRIX_MATRIX_H
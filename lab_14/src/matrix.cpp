#include <iostream>
#include "matrix.h"


Matrix::Matrix() : _rows(0), _cols(0) {
    _data = NULL;
}


Matrix::Matrix(std::size_t r, std::size_t c) {
    _rows = r;
    _cols = c;
    _data = new int*[_rows];
    
    for (size_t i = 0; i < _rows; i++)
        _data[i] = new int[_cols];

    for (size_t i = 0; i < _rows; i++)
        for (size_t j = 0; j < _cols; j++)
            _data[i][j] = 0;
}


Matrix::Matrix(const Matrix& m) {
    _rows = m._rows;
    _cols = m._cols;
    _data = new int*[_rows];
    
    for (size_t i = 0; i < _rows; i++)
        _data[i] = new int[_cols];

    for (size_t i = 0; i < _rows; i++)
        for (size_t j = 0; j < _cols; j++)
            _data[i][j] = m.get(i, j);
}


Matrix::~Matrix() {
    for (size_t i = 0; i < _rows; i++)
        delete[] _data[i];

    delete[] _data;
}


std::size_t Matrix::get_rows() const { return _rows; }
std::size_t Matrix::get_cols() const { return _cols; }


void Matrix::set(std::size_t i, std::size_t j, int val) {
    _data[i][j] = val;
}


int Matrix::get(std::size_t i, std::size_t j) const{
    if (i >= _rows || j >= _cols)
        throw MatrixException("ACCESS: bad index.");

    return _data[i][j];
}


Matrix Matrix::operator=(const Matrix& m) {
    Matrix tmp(m);
    std::swap(_cols, tmp._cols);
    std::swap(_rows, tmp._rows);
    std::swap(tmp._data, _data);
    
    return *this;
}


Matrix& Matrix::operator+=(const Matrix& m) {
    if (_rows != m._rows || _cols != m._cols)
        throw MatrixException("ADD: dimensions do not match.");

    for (size_t i = 0; i < _rows; i++)
        for (size_t j = 0; j < _cols; j++)
            this->set(i, j, this->get(i, j) + m.get(i, j));

    return *this;
}


Matrix& Matrix::operator*=(const Matrix& m) {
    if (_cols != m._rows)
        throw MatrixException("MUL: #arg1.columns != #arg2.rows.");

    Matrix ans(_rows, m.get_cols());
    for (size_t i = 0; i < _rows; i++)
        for (size_t k = 0; k < ans._cols; k++)
            for (size_t j = 0;j < _cols; j++)
                ans._data[i][k] += (_data[i][j])*(m._data[j][k]);

    *this = ans;
    return *this;
}


std::ostream &operator<<(std::ostream &os, const Matrix &m) {
    for (size_t i = 0; i < m.get_rows(); i++) {
        for (size_t j = 0; j < m.get_cols(); j++)
            os << m.get(i, j) << " ";
        os << "\n";
    }
    
    return os;
}


std::ifstream &operator>>(std::ifstream &fis, Matrix &m) {
    size_t rows, cols;
    int val;
    fis >> rows >> cols;
    m = Matrix(rows, cols);

    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++) {
            fis >> val;
            m.set(i, j, val);
        }

    return fis;
}

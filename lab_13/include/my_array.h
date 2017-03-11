#ifndef LAB_13_MY_ARRAY_H
#define LAB_13_MY_ARRAY_H


#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdint.h>

template<typename T, size_t N>
class my_array {
public:
    const T& at(size_t index) const;
    T& at(size_t index);
    const T& operator[](size_t index) const;
    T& operator[](size_t index);

    bool empty() const;
    size_t size() const;

    void fill(T val);

private:
    T _array[N];
};


template<size_t N>
class my_array<bool, N> {
public:

    class boolProxy{
    private:
        uint8_t *_byte;
        uint8_t _index;

    public:
        boolProxy(uint8_t *byte, uint8_t index) : _byte(byte), _index(index) {};
        bool operator=(bool val);
        operator bool() const;

    };
    my_array();
    const boolProxy  at(size_t index)const;
    boolProxy  at(size_t index);
    const boolProxy operator[](size_t index) const;
    boolProxy operator[](size_t index);

    bool empty() const;
    size_t size() const;

    void fill(bool val);

private:
    const static size_t _size = (N + 7) / 8;
    uint8_t _array[_size];
};




//--------------------------------------------------------------------------------
// Array for all types excepting bool.

template<typename T, size_t N>
const T& my_array<T,N>::at(size_t index) const{
    if (index >= N) {
        std::cout << "Out of range";
        abort();
    }
    return _array[index];
}

template<typename T, size_t N>
T& my_array<T,N>::at(size_t index) {
    if (index >= N) {
        std::cout << "Out of range";
        abort();
    }
    return _array[index];
}

template<typename T, size_t N>
const T& my_array<T,N>::operator[](size_t index) const {
    return _array[index];
}

template<typename T, size_t N>
T& my_array<T,N>::operator[](size_t index) {
    return _array[index];
}

template<typename T, size_t N>
void my_array<T,N>::fill(T val) {
    for (size_t i = 0; i < N; i++)
        _array[i] = val;
}

template<typename T, size_t N>
size_t my_array<T,N>::size() const {
    return N;
}

template<typename T, size_t N>
bool my_array<T,N>::empty() const {
    return N == 0;
}


//--------------------------------------------------------------------------------
// Array for bool.

template<size_t N>
my_array<bool, N>::my_array() {
    for (size_t i = 0; i < _size; i++)
        _array[i] = 0;
}

template<size_t N>
const typename my_array<bool, N>::boolProxy my_array<bool, N>::at(size_t index) const{
    if (index >= N) {
        std::cout << "Out of range";
        abort();
    }
    return boolProxy((uint8_t *) &_array[index / 8], index % 8);
}

template<size_t N>
typename my_array<bool, N>::boolProxy my_array<bool,N>::at(size_t index) {
    if (index >= N) {
        std::cout << "Out of range";
        abort();
    }
    return boolProxy((uint8_t *) &_array[index / 8], index % 8);
}

template<size_t N>
const typename my_array<bool, N>::boolProxy my_array<bool,N>::operator[](size_t index) const {
    return boolProxy((uint8_t *) &_array[index / 8], index % 8);
}

template<size_t N>
typename my_array<bool, N>::boolProxy my_array<bool,N>::operator[](size_t index) {
    return boolProxy((uint8_t *) &_array[index / 8], index % 8);
}

template<size_t N>
void my_array<bool,N>::fill(bool val) {
    if (val == 0)
        for (size_t i = 0; i < _size; i++)
            _array[i] = 0;
    else
        for (size_t i = 0; i < _size; i++)
            _array[i] = (1 << 8) - 1;
}

template<size_t N>
size_t my_array<bool,N>::size() const {
    return N;
}

template<size_t N>
bool my_array<bool,N>::empty() const {
    return N == 0;
}



template<size_t N>
bool my_array<bool, N>::boolProxy::operator=(bool val) {
    *_byte = ((*_byte) & ~(1 << _index)) | (val << _index);
    return (bool) *this;
}

template<size_t N>
my_array<bool, N>::boolProxy::operator bool() const{
    return (bool) (*_byte & (1 << _index));
}
#endif //LAB_13_MY_ARRAY_H

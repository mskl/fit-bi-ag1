#pragma once

#include <iostream>

template <typename T> class Vector {
private:
    size_t _size_used;              // number of elements used
    size_t _capacity;               // size of the array allocated
    T * _array;                     // The array holding the objects

    void realloc() {
        if (_capacity == 0) {
            _capacity = 1;
            _array = new T[1];
        } else {
            size_t new_capacity = _capacity * 2;

            auto * new_array = new T[new_capacity];
            std::copy(_array, _array + _capacity, new_array);
            delete[] _array;

            _capacity = new_capacity;
            _array = new_array;
        }
    }

public:
    Vector() :_size_used(0), _capacity(0), _array(nullptr) {
    }

    explicit Vector(const size_t size) {
        _size_used = size;
        _capacity = size;
        _array = new T[size];
    }

    size_t size() const {
        return _size_used;
    }

    void push_back (const T _a) {
        if (_size_used == _capacity || !_array) {
            realloc();
        }
        _array[_size_used++] = _a;
    }

    T& operator[]( size_t pos ) {
        return _array[pos];
    }
};
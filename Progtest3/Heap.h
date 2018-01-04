#pragma once

#include <iostream>

template <typename T> class Heap {
private:
    size_t _size;
    size_t _capacity;
    T * _array;

    void realloc() {
        if (_capacity == 0) {
            _array = new T[++_capacity];
        } else {
            size_t new_capacity = _capacity * 2;

            auto * new_array = new T[new_capacity];
            std::copy(_array, _array + _capacity, new_array);
            delete[] _array;

            _capacity = new_capacity;
            _array = new_array;
        }
    }

    size_t parent(size_t i) {
        return (i-1)/2;
    }

    size_t left(size_t i) {
        return (2*i + 1);
    }

    size_t right(size_t i) {
        return (2*i + 2);
    }

    void BubbleDown(size_t i, bool (*greater)(const T& left, const T& right)) {
        size_t l = left(i);
        size_t r = right(i);

        size_t smallest = i;

        if (l < size() && greater(_array[i], _array[l]))
            smallest = l;
        if (r < size() && greater(_array[smallest], _array[r]))
            smallest = r;

        if(smallest != i) {
            std::swap(_array[i], _array[smallest]);
            BubbleDown(smallest, greater);
        }

    }

    void BubbleUp(size_t i, bool (*greater)(const T& left, const T& right)) {
        while (i != 0 && greater(_array[parent(i)], _array[i])) {
            std::swap(_array[i], _array[parent(i)]);
            i = parent(i);
        }
    }

public:
    Heap() :_size(0), _capacity(0), _array(nullptr) { }

    size_t size() const {
        return _size;
    }

    void Insert (const T _a, bool (*greater)(const T& left, const T& right)) {
        // Insert the new element on the end
        if (_size >= _capacity || !_array) {
            realloc();
        }
        _array[_size++] = _a;

        // BubbleUp
        BubbleUp(size() - 1, greater);
    }

    T ExtractMin(bool (*greater)(const T& left, const T& right)) {
        if (size() == 1) {
            _size--;
            return _array[0];
        }

        std::swap(_array[_size-1], _array[0]);
        T min = _array[--_size];
        BubbleDown(0, greater);

        return min;
    }


    T& operator[]( size_t pos ) {
        return _array[pos];
    }
};
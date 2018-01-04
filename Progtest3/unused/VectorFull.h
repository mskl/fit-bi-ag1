#pragma once
#include <iostream>

#define OUT_OF_BOUNDS_EXCEPTION 0

template <typename T> class VectorFull {
    size_t _size_used{};              // number of elements used
    size_t _capacity{};               // size of the array allocated
    T * _array;                     // The array holding the objects

    void realloc() {
        // If this is an first alloc
        if (_capacity == 0) {
            _capacity = 1;
            _array = new T[1];
            return;
        }

        size_t new_capacity = _capacity * 2;
        T * new_array = new T[new_capacity];

        // Copy the old one
        std::copy(_array, _array + _capacity, new_array);

        // Delete the original
        delete[] _array;

        // Pass the new size
        _capacity = new_capacity;
        _array = new_array;
    }

    bool contains (const T& target, bool (*equal)(const T&, const T&)) const {
        for(size_t i = 0; i < size(); i++) {
            if(equal(target, _array[i])) {
                return true;
            }
        }
        return false;
    }

    bool array_contains(const T& target, T* aux, size_t lenght, bool (*equal)(const T&, const T&)) const {
        for (size_t i = 0; i < lenght; i++) {
            if(equal(target, aux[i])) {
                return true;
            }
        }
        return false;
    }

    void merge(T* array, T* aux, size_t lo, size_t mid, size_t hi, bool (*greater)(const T&, const T&)) {
        // copy to aux[]
        for (size_t k = lo; k <= hi; k++) {
            aux[k] = array[k];
        }

        // merge back to a[]
        size_t i = lo, j = mid+1;
        for (size_t k = lo; k <= hi; k++) {
            if (i > mid)
                array[k] = aux[j++];
            else if (j > hi)
                array[k] = aux[i++];
            else if (greater(aux[i], aux[j]))
                array[k] = aux[j++];
            else
                array[k] = aux[i++];
        }
    }

public:
    // Init the Vector empty with no elements
    VectorFull() :_size_used(0), _capacity(0), _array(nullptr) {

    }

    // Init the Vector with fixed size
    explicit VectorFull(const size_t size) {
        _size_used = size;
        _capacity = size;
        _array = new T[size];
    }

    //~Vector() {
    //    free();
    //}

    //void free() {
    //    if (_array && _capacity > 0)
    //        delete[] _array;
    //
    //    _capacity = 0;
    //    _size_used = 0;
    //}

    size_t size() const {
        return _size_used;
    }

    void push_back (const T _a) {
        if (_size_used == _capacity || !_array) {
            realloc();
        }

        _array[_size_used++] = _a;
    }

    void remove_multivalues(bool (*equal)(const T&, const T&)) {
        T* aux = new T[size()];
        size_t aux_used = 0;

        for(size_t i = 0; i < size(); i++) {
            if(!array_contains(_array[i], aux, size(), equal)) {
                aux[aux_used++] = _array[i];
            }
        }

        delete[] _array;

        _array = aux;
        _size_used = aux_used;
        _capacity = aux_used;
    }

    void push_back_unique (const T _a, bool (*equal)(const T&, const T&)) {
        if(!contains(_a, equal)) {
            push_back(_a);
        }
    }

    T pop_back () {
        if (_size_used > 0) {
            _size_used--;
            return _array[_size_used];
        }
        else return NULL;
    }

    bool remove (const T & t) {
        uint found_index = 0;
        bool found = false;

        // TODO: Make this use the pointer to a function
        for(uint i = 0; i < _size_used; i++) {
            if(t == _array[i]) {
                found_index = i;
                found = true;
                break;
            }
        }

        if (found) {
            for (uint i = found_index; i < _size_used - 1; i++) {
                _array[i] = _array[i+1];
            }
            _size_used--;
            return true;
        }
        else return false;
    }

    void print() const {
        std::__1::cout << "[" << size() << "/" << _capacity << "] ";

        for (size_t i = 0; i < _size_used; i++) {
            std::__1::cout << _array[i] << " ";
        }

        std::__1::cout << std::__1::endl;
    }


    // Edit operator
    T& operator[]( size_t pos ) {
        if (pos > _size_used) {
            throw OUT_OF_BOUNDS_EXCEPTION;
        }
        return _array[pos];
    }

    void bubble_sort(bool (*greater)(const T&, const T&)) {
        size_t end_index = _size_used;

        while (true){
            bool did_swap = false;

            for (size_t i = 0; i < end_index-1; i++) {
                if (greater(_array[i], _array[i+1])) {
                    std::__1::swap(_array[i], _array[i + 1]) ;
                    did_swap = true;
                }
            }

            end_index--;

            if (!did_swap)
                break;
        }
    }

    void merge_sort(bool (*greater)(const T&, const T&)) {
        size_t n = size();
        T* aux = new T[n];

        for (size_t len = 1; len < n; len *= 2) {
            for (size_t lo = 0; lo < n - len; lo += 2*len) {
                size_t mid  = lo+len-1;
                size_t hi = std::__1::min(lo + len + len - 1, n - 1);
                //size_t hi = min(lo+len+len-1, n-1);
                merge(_array, aux, lo, mid, hi, greater);
            }
        }

        delete[] aux;
    }
};
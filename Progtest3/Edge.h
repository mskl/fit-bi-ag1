#pragma once

class Edge {
private:
    int _id = -1;
    int _weight = -1;
    int _vertexA = -1;
    int _vertexB = -1;
public:

    Edge() = default;

    Edge(int vA, int vB, int weight, int id) {
        Init(vA, vB, weight, id);
    }

    void Init(int vA, int vB, int weight, int id) {
        _vertexA = vA;
        _vertexB = vB;
        _weight = weight;
        _id = id;
    }

    int weight() const {
        return _weight;
    }

    int A() const {
        return _vertexA;
    }

    int B() const {
        return _vertexB;
    }

    int other (int id1) const {
        return id1 == _vertexA ? _vertexB : _vertexA;
    }

    int id() const {
        return _id;
    }

    void print() const {
        std::cout << "[" << _id << "] " << _vertexA << " - " << _vertexB << " | " << weight() << std::endl;
    }
};
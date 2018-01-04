#include <iostream>

class Vertex;
class EdgePair;
class Graph;

enum VertexState {unknown, open, closed};

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

class EdgePair {
public:
    int edge_value = -1;
    int edge_id = -1;
    Vertex* edge_to_vertex = nullptr;
    Vertex* edge_from_vertex = nullptr;

    EdgePair (int _edge_value, int _edge_id, Vertex* _edge_target, Vertex* _from_vertex = nullptr) {
        edge_to_vertex = _edge_target;
        edge_from_vertex = _from_vertex;
        edge_value = _edge_value;
        edge_id = _edge_id;
    }

    EdgePair() {
        edge_value = -1;
        edge_id = -1;
        edge_to_vertex = nullptr;
        edge_from_vertex = nullptr;
    }
};

class Vertex {
    int _id = -1;
    bool _taken = false;
public:
    Vector<EdgePair> adjacent;
    Vector<Vertex*> adjacent_MVC;
    VertexState state = unknown;

    explicit Vertex(int id) {
        _id = id;
        adjacent = Vector<EdgePair>();
        adjacent_MVC = Vector<Vertex*>();
    }

    int id() const {
        return _id;
    }

    bool IsTaken() const {
        return _taken;
    }

    void Take() {
        _taken = true;
    }

};

using namespace std;

class Graph {
private:
    int vertex_count = -1;
    int edge_count = -1;

    bool* vertex_in_mst = nullptr;
    Vector<pair<int, int>> edges_output;

    Vector<Vertex*> vertices;
    Heap<EdgePair> edge_heap;

    bool* vertex_in_mvc = nullptr;

    static bool EdgePairGreater(const EdgePair &e1, const EdgePair &e2) {
        return e1.edge_value > e2.edge_value;
    }

    void GreedyMVC(Vertex* v, Vertex* parent = nullptr) {
        v->state = open;

        for (size_t s = 0; s < v->adjacent_MVC.size(); s++) {
            if (v->adjacent_MVC[s]->state != open) {
                GreedyMVC(v->adjacent_MVC[s], v);

                if(!v->adjacent_MVC[s]->IsTaken())
                {
                    v->Take();
                    vertex_in_mvc[v->id()] = true;
                }
            }
        }

        v->state = closed;
    }

public:
    pair<unsigned long, string> prepared_mst() {
        unsigned long sum = 0;
        string id_string;
        for (size_t i = 0; i < edges_output.size(); i++) {
            if (!id_string.empty())
                id_string += " ";
            id_string += to_string(edges_output[i].first);
            sum += edges_output[i].second;
        }
        return make_pair(sum, id_string);
    }

    pair<size_t, string> prepared_mvc() {
        size_t num = 0;
        string id_string;
        for (size_t i = 0; i < (size_t)vertex_count; i++) {
            if (vertex_in_mvc[i]) {
                if (!id_string.empty()) id_string += " ";
                id_string += to_string(i);
                num++;
            }
        }

        return make_pair(num, id_string);
    }

    void ScanInput() {
        cin >> vertex_count;
        cin >> edge_count;

        vertices = Vector<Vertex*>((size_t)vertex_count);
        edges_output = Vector<pair<int,int>>();
        edge_heap = Heap<EdgePair>();
        for (int i = 0; i < vertex_count; i++) {
            vertices[i] = new Vertex(i);
        }

        vertex_in_mst = new bool[vertex_count];
        vertex_in_mvc = new bool[vertex_count];
        for(int i = 0; i < vertex_count; i++) {
            vertex_in_mst[i] = false;
            vertex_in_mvc[i] = false;
        }

        for (int i = 0; i < edge_count; i++) {
            int A, B, weight;
            cin >> A;
            cin >> B;
            cin >> weight;

            vertices[A]->adjacent.push_back(EdgePair(weight, i, vertices[B], vertices[A]));
            vertices[B]->adjacent.push_back(EdgePair(weight, i, vertices[A], vertices[B]));
        }
    }

    void Jarnik() {
        // Select a random starting Vertex
        Vertex * start = vertices[0];
        vertex_in_mst[0] = true;
        int mst_counter = 1;

        // Add all its edges into the heap
        for (size_t i = 0; i < start->adjacent.size(); i++) {
            edge_heap.Insert(start->adjacent[i], EdgePairGreater);
        }

        // Do the Jarnik
        while (mst_counter < vertex_count) {
            // Pop the smallest edge
            EdgePair smallest = edge_heap.ExtractMin(EdgePairGreater);
            // Check if the vertex is already covered
            int smallest_vert_id = smallest.edge_to_vertex->id();
            int smallest_edge_id = smallest.edge_id;
            int smallest_edge_weight = smallest.edge_value;
            if(!vertex_in_mst[smallest_vert_id]) {
                // Cover the vertex
                vertex_in_mst[smallest_vert_id] = true;
                // Add the vertex to the output array
                auto out = std::make_pair(smallest_edge_id, smallest_edge_weight);
                edges_output.push_back(out);
                // Add edge to the MVC array of both vertices
                Vertex* A = smallest.edge_from_vertex;
                Vertex* B = smallest.edge_to_vertex;
                A->adjacent_MVC.push_back(B);
                B->adjacent_MVC.push_back(A);
                // Update the counter
                mst_counter++;
                // Add the edges to the heap
                for (size_t i = 0; i < smallest.edge_to_vertex->adjacent.size(); i++) {
                    edge_heap.Insert(smallest.edge_to_vertex->adjacent[i], EdgePairGreater);
                }
            }
        }
    }

    void GreedyMVC() {
        GreedyMVC(vertices[0]);
    }
};

int main() {
    //const clock_t t0 = clock();

    Graph graph = Graph();
    graph.ScanInput();
    graph.Jarnik();

    auto res1 = graph.prepared_mst();

    graph.GreedyMVC();
    auto res2 = graph.prepared_mvc();

    cout << res1.first << " " << res2.first << endl;
    cout << res1.second << endl;
    cout << res2.second << endl;

    //const clock_t t1 = clock();
    //cout << "execution took " << (t1 - t0) / (double) CLOCKS_PER_SEC << endl;
}
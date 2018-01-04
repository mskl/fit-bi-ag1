#include <iostream>

using namespace std;

static const char* ERROR_MSG = "No solution.";
uint64_t time_max = 0;

/*!
 * The Vector implements an array that grows and frees automatically
 * @tparam T type of the elements stored
 */
template <typename T> class Vector {
    uint64_t size_real       = 0;    // number of elements used
    uint64_t size_allocated  = 0;    // size of the array allocated
    T * array = nullptr;        // The array holding the objects

    /*!
     * Resizes the Vector based on the previous values
     */
    void realloc() {
        uint64_t new_size = 0;

        if (size_allocated == 0) {
            new_size = 1;
            array = new T[1];
            size_allocated = new_size;
            return;
        }

        new_size = size_allocated * 2;

        T* newbuf = new T[new_size];
        copy(array, array + size_allocated, newbuf);
        delete[] array;

        size_allocated = new_size;
        array = newbuf;
    }

public:
    /*!
     * The destructor frees the allocated memory of the vector
     */
     void Free() {
         if (array && size_allocated > 0) {
             delete[] array;
         }
     }

    /*!
     * Get the real number of objects stored in the Vector
     * @return size of the vector
     */
    uint64_t size() const {
        return size_real;
    }

    /*!
     * Adds an item into the Vector
     * @param _a item to be added
     */
    void push_back (const T _a) {
        if (size_real == size_allocated || !array) {
            realloc();
        }

        if (array) {
            array[size_real++] = _a;
        }
        else {
            throw "shit";
        }
    }

    /*!
     * Prints the contents of the Vector
     */
    void print() const {
        cout << "[" << size() << "/" << size_allocated << "] ";
        for (uint64_t i = 0; i < size_real; i++) {
            cout << array[i] << " ";
        }
        cout << endl;
    }

    /*!
     * Return an object from the Vector based on the index
     * @param i index of the target element
     * @return the object on the index. throws an exception if out of bounds
     */
    T& operator[] (const uint64_t i) {
        if(i > size_real || !array) {
            throw "Index out of bounds";
        } else {
            return array[i];
        }
    }
};

/*!
 * Vertex class holds the vetex id and adjascent vertices
 */
struct Vertex {
    uint64_t id = 0;                    // ID of the Vertex
    uint64_t time = 0;                  // The time it takes to make this part

    uint64_t max_time_previous = 0;     // The times of the parent vertices

    bool visited = false;
    bool solved = false;

    Vector<Vertex*> edges_in_vertex_vector;  // Vector of ptrs

    /*!
     * Default constructor that does nothing
     */
    Vertex() { }

    /*!
     * Initialise the Vertex calling the Init function
     * @param _id id of the vertex
     * @param _time time it takes to create the item
     * @param _edges_in_cnt number of edges going in
     */
    Vertex (const uint64_t _id, const uint64_t _time) {
        Init(_id, _time);
    }

    /*!
     * Initialise the Vertex
     * @param _id id of the vertex
     * @param _time time it takes to create the item
     * @param _edges_in_cnt number of edges going in
     */
    void Init (const uint64_t _id, const uint64_t _time) {
        id = _id;
        time = _time;
    }

    /*!
     * Prints the info about Vertex
     */
    void Print() {
        cout << "id = " << id << " time = " << time << " max_t_prev = " << max_time_previous << endl;
        cout << "\t edges going in [" << edges_in_vertex_vector.size() << "] = ";
        for (uint64_t i = 0; i < edges_in_vertex_vector.size(); i++) {
            Vertex* vert = edges_in_vertex_vector[i];
            cout << "\"" << vert->id << "\" ";
        }
        cout << endl;
    }
};

/*!
 * Prints the error message if there is a problem
 */
void PrintError() {
    cout << ERROR_MSG << endl;
}

/*!
 * Compares two values and return the bigger one
 * @param val1 value 1
 * @param val2 value 2
 * @return the bigger one
 */
uint64_t Max (const uint64_t val1, const uint64_t val2) {
   return val1 > val2 ? val1 : val2;
}

/*!
 * Goes deeper, returns max time of the previous vertices
 */
uint64_t DFS (Vertex * cur) {
    // uzel pri vstupu do funkce navstiveny a zaroven nevyreseny, je to cyklus
    if (cur->visited && !cur->solved) {
        throw "Loop";
    }

    cur->visited = true;

    for (uint64_t i = 0; i < cur->edges_in_vertex_vector.size(); i++) {
        Vertex* next = cur->edges_in_vertex_vector[i];

        uint64_t returned = next->solved ? next->time + next->max_time_previous : DFS(next);

        if (cur->edges_in_vertex_vector.size() != 0) {
            cur->max_time_previous = Max(returned, cur->max_time_previous);
        }
    }

    cur->solved = true;

    return cur->time + cur->max_time_previous;
}

/*!
 * The main program cycle
 * @return 0 if success
 */
int main() {
    try {
        uint64_t vertex_count = 0;  // Number of the vectices in the input
        cin >> vertex_count;        // Read the number of vertices

        // Array of the times of all vertices
        uint64_t *input_vertex_time = new uint64_t[vertex_count];

        // ***********************************************
        // *** SCAN ALL THE NUMBERS ON THE SECOND LINE ***
        for (uint64_t id = 0; id < vertex_count; id++) {
            cin >> input_vertex_time[id];
        }

        // Create an array of arrays that will hold the dependencies
        uint64_t **input_vertex_dependency = new uint64_t *[vertex_count];

        // *************************************
        // *** SCAN ALL THE REMAINING LINES ***
        for (uint64_t id = 0; id < vertex_count; id++) {
            uint64_t edges_in;
            cin >> edges_in;

            input_vertex_dependency[id] = new uint64_t[edges_in + 1];   // Create an array that is +1 size
            input_vertex_dependency[id][0] = edges_in;                  // Store the size into the first ([0]) slot

            // Read all the remaining numbers on the line
            for (uint64_t i = 1; i < edges_in + 1; i++) {
                uint64_t other_id;
                cin >> other_id;

                input_vertex_dependency[id][i] = other_id;
            }
        }


        // *********************************************
        // *** PARSE THE INPUT INTO THE VERTEX CLASS ***
        // The array of pointers to vertices
        Vertex** vertex_array = new Vertex*[vertex_count];

        // Create the vertices and save them to the array
        for (uint64_t id = 0; id < vertex_count; id++) {
            Vertex* vert = new Vertex(id, input_vertex_time[id]);   // Create new vertex

            vertex_array[id] = vert;                                // Assign to the array
        }

        // Add the incoming edges into the vertices
        for (uint64_t id = 0; id < vertex_count; id++) {
            uint64_t dependency_count = input_vertex_dependency[id][0]; // How many vertices is this one dependent on

            for (uint64_t i = 0; i < dependency_count; i++) {
                uint64_t dependent_on_id = input_vertex_dependency[id][i+1];  // The vertex id depends on vertex of id=i
                Vertex* dependent_on_ptr = vertex_array[dependent_on_id];

                vertex_array[id]->edges_in_vertex_vector.push_back(dependent_on_ptr);
            }

        }


        // Print all of the vertices after loading them in
        //cout << "ALL VERTICES: " << endl;
        //for (uint64_t id = 0; id < vertex_count; id++) {
        //    vertex_array[id]->Print();
        //}

        uint64_t max_time = 0;

        // For all Vertices call DFS
        for (uint64_t i = 0; i < vertex_count; i++) {
            uint64_t max = DFS(vertex_array[i]);
            max_time = Max(max_time, max);
        }

        // *************************
        // *** PRINT THE OUTPUT ***

        // Print the maximum time
        cout << max_time << endl;

        // For every vertex print the max induced time
        for (uint64_t i = 0; i < vertex_count; i++) {
            cout << vertex_array[i]->max_time_previous  << " \n"[i == vertex_count];
        }

    } catch(...) {
        PrintError();
        return 1;
    }

    return 0;
}
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
    uint64_t id = 0;

    uint64_t time = 0;          // The time it takes to make this part
    uint64_t edges_in_cnt = 0;  // How many edges go into this node

    uint64_t visited_cnt = 0;   // The number of times this vertex was visited
    uint64_t time_induced = 0;  // The times of the parent vertices
    bool ping = false;               // Ping it if you touch it

    Vector<Vertex*> vertex_out_vector;  // Vector of ptrs

    /*!
     * Default constructor that does nothing
     */
    Vertex() {

    }

    /*!
     * Initialise the Vertex calling the Init function
     * @param _id id of the vertex
     * @param _time time it takes to create the item
     * @param _edges_in_cnt number of edges going in
     */
    Vertex(const uint64_t _id, const uint64_t _time, const uint64_t _edges_in_cnt = 0) {
        Init(_id, _time, _edges_in_cnt);
    }

    /*!
     * Initialise the Vertex
     * @param _id id of the vertex
     * @param _time time it takes to create the item
     * @param _edges_in_cnt number of edges going in
     */
    void Init (const uint64_t _id, const uint64_t _time, const uint64_t _edges_in_cnt = 0) {
        id = _id;
        time = _time;
        edges_in_cnt = _edges_in_cnt;
    }

    /*!
     * Prints the info about Vertex
     */
    void Print() {
        cout << "id = " << id << ", time = " << time << ", edges_in_cnt = " << edges_in_cnt << endl;
        cout << "\t visited_cnt = " << visited_cnt << ", time_induced = " << time_induced << endl;
        cout << "\t outgoing edges to ids = ";
        for (uint64_t i = 0; i < vertex_out_vector.size(); i++) {
            Vertex* vert = vertex_out_vector[i];
            cout << vert->id << " ";
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
uint64_t Max(const uint64_t val1, const uint64_t val2) {
   return val1 > val2 ? val1 : val2;
}

/*!
 * Does DFS and shit you know
 * @param _this the current pointer to the current Vertex
 * @param _time_induced induced time from previous vertices
 */
void DFS (Vertex * _this, uint64_t _time_induced = 0) {
    // Check if this is the path with the uint64_test time
    if (_this->time + _time_induced > time_max) {
        time_max = _this->time + _time_induced;
    }

    // Do the work on Vertices
    // Do not do this if it is a root (_time_induced == 0)
    if(_time_induced != 0) {
        _this->visited_cnt++;
    }

    // Update the values on the current Vertex
    _this->time_induced = Max(_time_induced, _this->time_induced);  // If the current induced time is greater, update it
    _this->ping = true;                                             // Ping the vertex just to make sure. It should not be necesarry

    if (_this->visited_cnt > _this->edges_in_cnt) {                 // If there are more visits than edges in, there must be a loop
        throw "F LOOP";
    }
    else if (_this->visited_cnt == _this->edges_in_cnt) {           // If all routes in were explored go deeper
        for (uint64_t i = 0; i < _this->vertex_out_vector.size(); i++) {    // Visit all children
            DFS(_this->vertex_out_vector[i], _this->time_induced + _this->time);    // Do the DFS on the children with time + induced time
        }
    }
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

        // Print the parsed values
        /*for (int id = 0; id < vertex_count; id++) {
            int edges_in = input_vertex_dependency[id][0];
            cout << "id (" << id << ") value = " << input_vertex_time[id] << " edges_in_cnt = " << edges_in << endl;
            for (int i = 0; i < edges_in; i++) {
                cout << "\t in from id: " << input_vertex_dependency[id][i+1] << endl;
            }
        }*/

        // *********************************************
        // *** PARSE THE INPUT INTO THE VERTEX CLASS ***
        // The array of pointers to vertices
        Vertex** vertex_array = new Vertex*[vertex_count];

        // Create the vertices and save them to the array
        for (uint64_t id = 0; id < vertex_count; id++) {
            Vertex* vert = new Vertex(id, input_vertex_time[id], input_vertex_dependency[id][0]);   // Create new vertex
            vertex_array[id] = vert;                                                                // Assign to the array
        }

        Vector<Vertex*> roots;  // Save all the roots to the Vector for later use

        // Create the outgoing edges from all vertices
        for (uint64_t id = 0; id < vertex_count; id++) {
            uint64_t in_count = input_vertex_dependency[id][0];

            if (in_count == 0) {
                roots.push_back(vertex_array[id]);  // This must be a root since there are no edges in, save it in the Vector
            }
            else {
                for (uint64_t i = 1; i < in_count+1; i++) { // If it is not a root, then there are edges in. Add them to the Vertex where they origin
                    // the vertex of id ID is dependent on the vertex dependent_on
                    uint64_t dependent_on_id = input_vertex_dependency[id][i];

                    // Store it in the Vector inside the Vertex
                    vertex_array[dependent_on_id]->vertex_out_vector.push_back(vertex_array[id]);
                }
            }
        }

        // From each root do the DFS
        for (uint64_t i = 0; i < roots.size(); i++) {
            DFS(roots[i]);
        }

        // This is redundant and should not be needed
        // Try if I have (pinged) visited all vertices
        for (uint64_t i = 0; i < vertex_count; i++) {
            if (!vertex_array[i]->ping) {
                throw "PING ERROR";
            }
        }

        // Print all of the vertices
        cout << "ALL VERTICES: " << endl;
        for (uint64_t id = 0; id < vertex_count; id++) {
            vertex_array[id]->Print();
        }


        // *************************
        // *** PRINT THE OUTPUT ***
        // Print the maximum time
        cout << time_max << endl;

        // Print only the induced times
        for (uint64_t id = 0; id < vertex_count; id++) {
            cout << vertex_array[id]->time_induced;
            if (id != vertex_count - 1) {
                cout << " ";    // Print the space only if it is not on the end of the line
            }
        }
        cout << endl;           // Print the endline on the end

    } catch(...) {
        PrintError();
        return 1;
    }

    return 0;
}
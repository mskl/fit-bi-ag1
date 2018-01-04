
/*
void Kruskal(bool debug = false) {
    if(debug) {
        cout << "--- Edges before sorting: ---" << endl;
        PrintAllEdges();
        cout << "--- Edges sorted: ---" << endl;
    }

    SortEdges();

    if(debug) PrintAllEdges();

    int vertices_in_mst = 0;

    // Add the first edge to the MST
    vertex_in_mst[edges[0].A()] = true;
    vertex_in_mst[edges[0].B()] = true;

    edge_in_mst[0] = true;
    vertices_in_mst = 2;

    for (int i = 1; i < edges.size(); i++) {
        int a = edges[i].A();
        int b = edges[i].B();

        if(true) {
            // TODO
        }
        else if(vertex_in_mst[a] && vertex_in_mst[b]) {
            edge_in_mst[i] = true;
        }
        else if(!vertex_in_mst[a]) {
            vertex_in_mst[a] = true;
            edge_in_mst[i] = true;
            vertices_in_mst++;
        }
        else if(!vertex_in_mst[b]) {
            vertex_in_mst[b] = true;
            edge_in_mst[i] = true;
            vertices_in_mst++;
        }

        if (vertices_in_mst >= vertex_count) {
            break;
        }
    }

    // --- Kruscal ended. ---
    if(debug) cout << "--- Printing found vertices. ---" << endl;
    if(debug) PrintVertexIDsInMST();
    if(debug) cout << "--- Printing found edge ids. ---" << endl;
    if(debug) PrintEdgeIDsInMST();
}

static bool greater_edge(const Edge & e1, const Edge & e2) {
        return e1.weight() > e2.weight();
    }

        void PrintEdgeIDsInMST() {
        for(int i = 0; i < edge_count; i++) {
            if(edge_in_mst[i]) {
                cout << edges[i].id() << " ";
            }
        }
        cout << endl;
    }

    void PrintVertexIDsInMST() {
        for(int i = 0; i < vertex_count; i++) {
            if(vertex_in_mst[i]) {
                cout << i << " ";
            }
        }
        cout << endl;
    }


class MVC {
private:
    int vertex_count = -1;
    int edge_count = -1;

    Vertex** vertices;
    Vector<int> MVC_Ids;

    static bool equal_vertex(Vertex * const & v1, Vertex * const & v2) {
        return v1->id() == v2->id();
    }

    static bool equal_int(const int& i1, const int& i2) {
        return i1 == i2;
    }

    void DFSPrint(Vertex* v) {
        v->state = open;

        for(size_t s = 0; s < v->adjacent.size(); s++) {
            if (v->adjacent[s]->state != open) {
                DFSPrint(v->adjacent[s]);
            }
        }

        cout << "Vertex id " << v->id() << " |";

        for(size_t i = 0; i < v->adjacent.size(); i++) {
            cout << " " << v->adjacent[i]->id();
        }
        cout << endl;
    }

    void GreedyMVC(Vertex* v, bool debug = false, Vertex* parent = nullptr) {
        v->state = open;
        for(size_t s = 0; s < v->adjacent.size(); s++) {
            if (v->adjacent[s]->state != open) {
                GreedyMVC(v->adjacent[s], debug, v);
            }
        }
        v->state = closed;

        if(debug) {
            cout << "Vertex " << v->id()
                 << " is leaf " << v->IsLeaf()
                 << " NeighbourWithTaken " << v->NeigborWithTaken()
                 << endl;
            cout << "Neigbours: " << endl;
            cout << "|";
            for (size_t i = 0; i < v->adjacent.size(); i++) {
                cout << " " << v->adjacent[i]->id();
            }
            cout << endl;
        }


        if(v->IsLeaf()) {
            if(parent != nullptr) {
                parent->Take();
                MVC_Ids.push_back_unique(parent->id(), equal_int);
                if(debug) cout << " Taking the parent id " << parent->id() << endl;
            }
        }
        else {
            if(!v->NeigborWithTaken() && !v->IsTaken()){
                v->Take();
                MVC_Ids.push_back_unique(v->id(), equal_int);
                if(debug) cout << " Taking the id " << v->id() << endl;
            }
        }
    }

public:
    MVC (int _vertex_count, int _edge_count) {
        vertex_count = _vertex_count;
        edge_count = _edge_count;

        vertices = new Vertex*[_vertex_count];
        for(int i = 0; i < _vertex_count; i++) {
            vertices[i] = new Vertex(i);
        }

        MVC_Ids = Vector<int>();
    }

    void ConstructGraph(Vector<Edge>&  _edges, const bool* _edge_in_mst) {
        // Go through all edges in the graph and add them to appropriate vertices
        for(size_t i = 0; i < _edges.size(); i++) {
            if (_edge_in_mst[i]) {
                // Edge connect vertices of index A and B
                int vertex_id_a = _edges[i].A();
                int vertex_id_b = _edges[i].B();
                // This is really ineffective, should be upgraded
                vertices[vertex_id_a]->adjacent.push_back_unique(vertices[vertex_id_b]);
                vertices[vertex_id_b]->adjacent.push_back_unique(vertices[vertex_id_a]);
            }
        }
    }

    void GreedyMVC(bool debug = false) {
        Vertex* starter = nullptr;

        for (size_t i = 0; i < vertex_count; i++) {
            if(vertices[i]->adjacent.size() >= 2) {
                starter = vertices[i];
                break;
            }
        }

        if(starter == nullptr) {
            starter = vertices[0];
        }

        if(debug) cout << "Staring MVC at the node id " << starter->id() << endl;
        GreedyMVC(starter, debug);
    }

    void DFSPrint() {
        vertices[0]->state = open;
        DFSPrint(vertices[0]);
    }

    pair<size_t, string> prepared_mvc() {
        size_t num = MVC_Ids.size();
        string id_string;
        for (size_t i = 0; i < MVC_Ids.size(); i++) {
            if (!id_string.empty()) id_string += " ";
            id_string += to_string(MVC_Ids[i]);
        }

        return make_pair(num, id_string);
    }
};
 */
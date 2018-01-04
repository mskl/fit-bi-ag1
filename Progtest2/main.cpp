#include <iostream>

#ifndef __PROGTEST__
#include <assert.h>
#endif /* __PROGTEST__ */


using namespace std;

class NotFoundException {};

struct Node {
    int key     = 0;    // The value of the node
    int height  = 1;    // Height of the node
    int size    = 1;    // Size of the subtree

    Node* left  = nullptr;  // Left sibling
    Node* right = nullptr;  // Right sibling

    Node (int _key) {
        key = _key;
    }
};

struct Tuple {
    Node* previous      = nullptr;  // previous node
    int previous_sum    = 0;        // sum of the previous sizes
    Node* found         = nullptr;  // pointer to the leaf node that was found

    Tuple (Node* _previous = nullptr, int _previous_sum = 0, Node* _found = nullptr) {
        previous = _previous;
        found = _found;
        previous_sum = _previous_sum;
    }
};

class Graph {
public:
    // Inserts an element to the graph
    void Insert (int _key) {
        elements_count += 1;
        root = Insert(root, _key);
    }

    // Get the median or raise an exception if it does not exists
    int Median (int _lower, int _upper, bool debug = false) {
        // Throw an error if there are no elements
        if (elements_count < 1) {
            if(debug) cout << "less than 1 element" << endl;
            throw NotFoundException();
        }

        // First, get the lower one
        pair<int, Node*> pair_lower = Rank(_lower);
        int index_lower = pair_lower.first;
        int key_from_lower_index = pair_lower.second->key;

        // Then the upper one
        pair<int, Node*> pair_upper = Rank(_upper);
        int index_upper = pair_upper.first;
        int key_from_upper_index = pair_upper.second->key;

        // If the upper is too big, then search for the index - 1
        if (pair_upper.second->key > _upper) {
            index_upper = pair_upper.first - 1;
            key_from_upper_index = Select(index_upper)->key;
        }

        // If the lower is too small, then search for the index + 1
        if (pair_lower.second->key < _lower) {
            index_lower = pair_lower.first + 1;
            key_from_lower_index = Select(index_lower)->key;
        }

        if(debug) {
            cout << "range: <" << _lower << ", " << _upper << ">" <<endl;
            cout << "lower_index: " << index_lower << " (key=" << key_from_lower_index << ")"
                 <<" upper_index: " << index_upper << " (key=" << key_from_upper_index << ")" << endl;
        }

        // Check if both values are really in range
        if (key_from_lower_index > key_from_upper_index
            || index_lower > index_upper
            || key_from_lower_index < _lower
            || key_from_upper_index > _upper) {
            throw NotFoundException();
        }

        // je-li ve stávajícím seznamu příjmů k příjmů s hodnotou v rozsahu mezi X a Y (včetně),
        // je medián ⌈k⁄2⌉-tý nejmenší z těchto příjmů.
        int delta = index_upper - index_lower;
        int median_index = 0;

        if(debug) cout << "delta: " << delta << endl;

        median_index = index_lower + delta / 2;

        if(debug) cout << "index of median element: " << median_index << endl;

        return Select(median_index)->key;
    }

    // Get the elements count in the graph
    int GetSize() {
        return elements_count;
    }

    // Public interface for PreorderPrint
    void PreorderPrint() {
        cout << "=== preorder print ===" << endl;
        PreorderPrint(root, 0, 0);
    }

    // Public interface for InorderPrint
    void InorderPrint() {
        cout << "=== inorder print ===" << endl;
        InorderPrint(root, 0, 0);
    }

    // Public interface for IndexPrint
    void IndexPrint() {
        cout << "=== index print (" << elements_count << ") ===" << endl;
        IndexPrintShow();
    }

    // Public interface for LinePrint
    void LinePrint() {
        cout << "=== line print ===" << endl;
        LinePrint(root);
    }

private:
    Node* root = nullptr;
    int elements_count = 0;

    // Gets the element from the graph by index
    Node* Select (int index) {
        if(index > elements_count - 1|| index < 0) {
            throw NotFoundException();
        }
        return Select(root, index);
    }

    // Returns the i'th element (zero-indexed) of the elements in t
    Node* Select(Node* current, int ith_element) {
        int left_size  = Size(current->left);

        if (ith_element == left_size) {
            return current;
        }
        else if (ith_element < left_size) {
            return Select(current->left, ith_element);
        }
        else {
            return Select(current->right, ith_element - (left_size + 1));
        }
    }

    // Return the index of the node found and also the node found
    pair<int, Node*> Rank (int key) {
        if (root == nullptr) {
            return make_pair(0, nullptr);
        }

        // cout << "looking for rank of: " << key << " ";
        Tuple tuple = FindRank(root, key);
        // cout << "found the node with key: " << tuple.found->key << " of index: " << tuple.previous_sum - 1 << endl;

        // Return the one that's stored in the root
        return make_pair(tuple.previous_sum - 1, tuple.found);
    }

    // Get the size of the node
    int Size (Node* current) {
        if (current == nullptr) {
            return 0;
        }
        return current->size;
    }

    // Get the height of the node
    int Height (Node *current) {
        if (current == nullptr) {
            return 0;
        }
        return current->height;
    }

    // Return the height of the children (+1) for the current node
    int HeightFromChildren (Node* current) {
        return max(Height(current->left), Height(current->right)) + 1;
    }

    // Return the size of the chuldren (+1) for the current node
    int SizeFromChildren (Node* current) {
        return Size(current->left) + Size(current->right) + 1;
    }

    // Get the balance sign of a node
    int BalanceSIGN (Node *current) {
        if (current == nullptr) {
            return 0;
        }
        return (Height(current->left) - Height(current->right));
    }

    // Insert an element into the graph
    Node* Insert (Node* current, int key) {
        // Create a new leaf and return it
        if (current == nullptr) {
            return new Node(key);
        }

        // Insert an element in BST style
        if (key < current->key){
            current->left = Insert(current->left, key);
        } else {
            current->right = Insert(current->right, key);
        }

        current->height = HeightFromChildren(current);  // Set the proper height of this element
        current->size   = SizeFromChildren(current);    // Set the proper size of this element
        int balance_sign = BalanceSIGN(current);        // Get the balance sign (difference between the balance of the children)

        // Do the rotations based on the current height and the sign of its siblings
        if (balance_sign > 1 && key < current->left->key) {         // RIGHT
            current = RotateRight(current);
        }
        else if (balance_sign < -1 && key > current->right->key) {  // LEFT
            current = RotateLeft(current);
        }
        else if (balance_sign > 1 && key > current->left->key) {    // LEFT -> RIGHT
            current->left =  RotateLeft(current->left);
            current = RotateRight(current);
        }
        else if (balance_sign < -1 && key < current->right->key) {  // RIGHT -> LEFT
            current->right = RotateRight(current->right);
            current = RotateLeft(current);
        }

        return current;
    }

    // Rotate 2 nodes Right
    Node* RotateRight (Node* old_root){
        // new root will be on the left of the old one
        Node* new_root = old_root->left;
        // right node will be preserved and later connected
        Node* right_temp = new_root->right;
        // connect the old root to the new root
        new_root->right = old_root;
        // reconnect the temp to the left of the old root
        old_root->left = right_temp;

        // update the heights
        old_root->height = HeightFromChildren(old_root);
        new_root->height = HeightFromChildren(new_root);

        // update the sizes
        old_root->size = SizeFromChildren(old_root);
        new_root->size = SizeFromChildren(new_root);

        // return the new root
        return new_root;
    }

    // Rotate 2 nodes Left
    Node* RotateLeft (Node* old_root){
        // new root will be on the right side of the old one
        Node * new_root = old_root->right;
        // left node will be preserved and later connected
        Node* left_temp = new_root->left;
        // connect the old root to the new root
        new_root->left = old_root;
        // reconnect the temp to the right of the old root
        old_root->right = left_temp;

        // update the heights
        old_root->height = HeightFromChildren(old_root);
        new_root->height = HeightFromChildren(new_root);

        // update the sizes
        old_root->size = SizeFromChildren(old_root);
        new_root->size = SizeFromChildren(new_root);

        // return the new root
        return new_root;
    }

    // Go down, find the element (one-indexed) and while unwinding update the rank value
    Tuple FindRank (Node* current, int target) {
        Tuple returnTuple;

        // Find the element in the BTS
        if (target < current->key) {
            // exact key not found, but I can't go more to the left
            if (current->left == nullptr) {
                returnTuple = Tuple(current, Size(current->left) + 1, current);
            } else {
                returnTuple = FindRank(current->left, target);
            }
        }
        else if (target > current->key) {
            // exact key not found, but I can't go more to the right
            if (current->right == nullptr) {
                returnTuple = Tuple(current, Size(current->right) + 1, current);
            } else {
                returnTuple = FindRank(current->right, target);
            }
        }
        else if (target == current->key) {
            // Found the exact key, alloha
            returnTuple = Tuple(current, Size(current->left) + 1, current);
        }

        // if the current is the father of the previous
        if (current->right == returnTuple.previous) {
            returnTuple.previous_sum += Size(current->left) + 1;
        }

        return Tuple(current, returnTuple.previous_sum, returnTuple.found);
    }

    // Print the graph in Preorder
    void PreorderPrint(Node *current, int l, int r) {
        PrintKey(current, l, r, root == current);

        if (current->left != nullptr) {
            PreorderPrint(current->left, ++l, r);
        }

        if (current->right != nullptr) {
            PreorderPrint(current->right, l, ++r);
        }
    }

    // Print the graph in Postorder
    void InorderPrint(Node *current, int l, int r) {
        if (current->left != nullptr) {
            PreorderPrint(current->left, ++l, r);
        }

        PrintKey(current, l, r, root == current);

        if (current->right != nullptr) {
            PreorderPrint(current->right, l, ++r);
        }
    }

    // Print the graph by Indexes
    void IndexPrintShow() {
        for (int i = 0; i < elements_count; i++) {
            cout << "index " << i << ": " << Select(i)->key << endl;
        }
    }

    // Print the graph with lines from nodes
    void LinePrint(Node* current) {
        string leftkey = "N";
        string rightkey = "N";

        if(current->left != nullptr) {
            leftkey = to_string(current->left->key);
        }
        if(current->right != nullptr) {
            rightkey = to_string(current->right->key);
        }

        cout << "["<< current->key <<"] \t" << "L=(" << leftkey << ")\tR=(" << rightkey << ")" << endl;

        if(current->left != nullptr) {
            LinePrint(current->left);
        }

        if(current->right != nullptr) {
            LinePrint(current->right);
        }
    }

    // Print the key
    void PrintKey(Node *current, int l, int ri, bool r) {
        cout << current->key << "\t| " << l << "\t" << ri << "\t|\t" << Size(current) ;
        if (r) {
            cout << "\t(Root)";
        }
        cout << endl;
    }
};

#ifndef __PROGTEST__
void test1() {
    Graph graph = Graph();
    graph.Insert(13);
    graph.Insert(4);
    graph.Insert(17);
    graph.Insert(6);
    graph.Insert(9);
    graph.Insert(14);
    assert(graph.Median(4, 9) == 6);
    assert(graph.Median(3, 10) == 6);
    assert(graph.Median(14, 14) == 14);
    assert(graph.Median(1, 20) == 9);

    cout << "test 1 passed" << endl;
}
void test2() {
    Graph graph = Graph();
    graph.Insert(2);
    graph.Insert(3);
    graph.Insert(1);

    bool p1 = false;
    try {
        graph.Median(15, 20);
    }
    catch (NotFoundException) {
        p1 = true;
    }
    assert(p1);

    assert(graph.Median(2, 2) == 2);

    bool p2 = false;
    try {
        graph.Median(4, 4);
    }
    catch (NotFoundException) {
        p2 = true;
    }
    assert(p2);


    cout << "test 2 passed" << endl;
}
void test3() {
    Graph graph = Graph();
    graph.Insert(4);
    graph.Insert(8);
    graph.Insert(6);
    graph.Insert(12);
    graph.Insert(7);

    assert(graph.Median(10, 13) == 12);
    assert(graph.Median(8, 15) == 8);
    assert(graph.Median(1, 100) == 7);
    assert(graph.Median(2, 7) == 6);

    cout << "test 3 passed" << endl;
}
void test6() {
    Graph graph = Graph();
    graph.Insert(2);
    graph.Insert(4);
    graph.Insert(6);
    graph.Insert(8);
    graph.Insert(10);
    graph.Insert(12);
    graph.Insert(14);
    graph.Insert(16);

    //graph.PreorderPrint();
    //graph.LinePrint();
    //graph.IndexPrint();

    assert(graph.Median(3, 9) == 6);
    assert(graph.Median(1, 7) == 4);
    assert(graph.Median(9, 15) == 12);
    assert(graph.Median(11, 17) == 14);
    assert(graph.Median(5, 15) == 10);

    cout << "test 6 passed" << endl;
}
void test7() {
    Graph graph = Graph ();
    graph.Insert(2);
    graph.Insert(4);
    graph.Insert(6);
    graph.Insert(8);
    graph.Insert(10);
    graph.Insert(12);
    graph.Insert(14);
    graph.Insert(16);
    assert(graph.Median(3, 11) == 6);
    assert(graph.Median(9, 20) == 12);
    assert(graph.Median(5, 13) == 8);

    cout << "test 7 passed" << endl;
}
void test8() {
    Graph graph = Graph();

    graph.Insert(2);
    graph.Insert(4);
    graph.Insert(6);
    graph.Insert(8);
    graph.Insert(10);
    graph.Insert(12);
    graph.Insert(14);
    assert(graph.Median(1, 15) == 8);

    cout << "test 8 passed" << endl;
}
void test9() {
    Graph graph = Graph();
    graph.Insert(2);
    graph.Insert(4);
    graph.Insert(6);
    graph.Insert(8);
    graph.Insert(10);
    graph.Insert(12);
    graph.Insert(14);
    graph.Insert(16);
    assert(graph.Median(1, 18) == 8);

    cout << "test 9 passed" << endl;
}
void test10() {
    Graph graph = Graph();
    bool p1 = false;
    try {
        graph.Median(15, 20);
    }
    catch (NotFoundException) {
        p1 = true;
    }
    assert(p1);

    cout << "test 10 passed" << endl;
}
void test11() {
    Graph graph = Graph();
    graph.Insert(1);
    assert(graph.Median(0, 2, true) == 1);

    cout << "test 11 passed" << endl;
}
void test12() {
    Graph graph = Graph();
    bool p1 = false;
    try {
        graph.Median(3, 3);
    }
    catch (NotFoundException) {
        p1 = true;
    }
    assert(p1);

    cout << "test 12 passed" << endl;
}

#endif /* __PROGTEST__ */

int main() {
    #ifndef __PROGTEST__
    test1();
    test2();
    test3();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    #endif /* __PROGTEST__ */

    Graph graph = Graph();
    int command = 0;
    bool looping = true;

    while(looping) {
        cin >> command;
            if (command == 1) {         // Insert
                int ins;
                cin >> ins;
                graph.Insert(ins);
            }
            else if (command == 2) {    // Delete

            }
            else if (command == 3) {    // Median
                int lower, upper, median;
                cin >> lower;
                cin >> upper;

                bool found = true;

                try {
                    median = graph.Median(lower, upper);
                } catch (NotFoundException) {
                    cout << "notfound" << endl;
                    found = false;
                }

                if (found) {
                    cout << median << endl;
                }
            }
            else if (command == 4) {    // Exit
                looping = false;
                break;
            }
            else {
                graph.PreorderPrint();
                graph.InorderPrint();
                graph.IndexPrint();
                break;
            }
    }
     /**/
}
#include <iostream>
#include <string.h>
#include <fstream>

const short KEY_SIZE = 257;
const  short CHARACTERISTIC_NUMBER = 5;

struct Pair {
    char key[KEY_SIZE] = "";
    unsigned long long value = 0;
};


namespace btree{
class Node {
    friend class BTree;

private:
    Pair* data;
    Node** child; 
    int n; 
    bool leaf; 
    int t; 

    Node(int, bool);
    Node* search(char*);
    // вспомогательные функции для вставки 
    void split_child(int, Node*); 
    void insert_non_full(Pair); 
    // вспомогательные функции для удаления
    void remove_main(char*); 
    void remove_from_leaf(int);
    void remove_from_non_leaf(int);
    void steal_from_next(int); 
    void steal_from_prev(int); 
    void fill_up_node(int); 
    void merge(int); 
    // вспомогательная функция для работы с файлом
    void save_node(std::ofstream&);
    void destroy_node();
};

class BTree {
    Node* root;
    int t;
    void insert_from_file(Pair);
public:
    BTree(int);
    void insert(Pair);
    void remove(char*);
    unsigned long long* search(char*);
    void save_to_file(char*);
    void load_from_file(char*);
    ~BTree();
};

//___________________________________________________RELALIZATION_OF_NODE_METHODS________________________________________________________________


Node::Node(int _t, bool is_leaf) {
    t = _t;
    leaf = is_leaf;
    data = new Pair[2 * t - 1];
    child = new Node*[2 * t];
    for (int i = 0; i < 2 * t; ++i) {
        child[i] = nullptr;
    }
    n = 0;
}

Node* Node::search(char* key) {
    int i = 0;
    while (i < n && strcmp(key, data[i].key) > 0) {
        i++;
    }

    if (i < n && strcmp(key, data[i].key) == 0) {
        return this;
    }

    if (leaf == true) {
        return nullptr;
    }
    return child[i]->search(key);
}  

void Node::split_child(int index, Node* full_node) {
  
    Node* new_node = new Node(full_node->t, full_node->leaf);
    new_node->n = t - 1;

    for (int i = 0; i < t - 1; ++i) {
        new_node->data[i] = full_node->data[i + t];
    }

    if (full_node->leaf == false) {
        for (int i = 0; i < t; ++i) {
            new_node->child[i] = full_node->child[i+t];
        }
    }
  
    full_node->n = t - 1;
    for (int i = n; i >= index + 1; --i) {
        child[i + 1] = child[i];
    }
    child[index + 1] = new_node;
    for (int i = n - 1; i >= index; --i) {
        data[i + 1] = data[i];
    }
    data[index] = full_node->data[t - 1];
    ++n;
}

void Node::insert_non_full(Pair inserted_elem) {
    int i = n - 1;

    if (leaf == true) {
        while (i >= 0 && strcmp(data[i].key, inserted_elem.key) > 0) {
            data[i+1] = data[i];
            --i;
        }
        data[i + 1] = inserted_elem;
        ++n;
    } else {
        while (i >= 0 && strcmp(data[i].key, inserted_elem.key) > 0) {
            --i;
        }

        if (child[i+1]->n == 2 * t - 1) {
            this->split_child(i + 1, child[i + 1]);

            if (strcmp(inserted_elem.key, data[i + 1].key) > 0) {
                ++i;
            }
        }
        child[i+1]->insert_non_full(inserted_elem);
    }
}

void Node::remove_main(char* key) {
    int idx = 0;
    while (idx < n && strcmp(key, data[idx].key) > 0) {
        ++idx;
    }

    if (idx < n && strcmp(key, data[idx].key) == 0) {
        if (leaf) {
            remove_from_leaf(idx);
        } else {
            remove_from_non_leaf(idx); 
        }
    } else {
        if (child[idx]->n < t) {
            fill_up_node(idx);
        }

        if (idx > n) {
            child[idx-1]->remove_main(key);
        } else {
            child[idx]->remove_main(key);
        }
    }
}

void Node::remove_from_leaf(int index) {
    for (int i = index + 1; i < n; i++) {
        data[i-1] = data[i];
    }
    --n;
}

void Node::remove_from_non_leaf(int index) {
    char key[KEY_SIZE]; 
    strcpy(key, data[index].key);
    Pair new_parent;
    if (child[index]->n >= t) {
        Node* max_node = child[index];
        while (!max_node->leaf) {
            max_node = max_node->child[max_node->n];
        }
        new_parent = max_node->data[max_node->n-1];
        data[index] = new_parent;
        child[index]->remove_main(new_parent.key);
    } else if (child[index+1]->n >= t) {
        Node* min_node = child[index+1];
        while(!min_node->leaf) {
            min_node = min_node->child[0];
        }
        new_parent = min_node->data[0];
        data[index] = new_parent;
        child[index+1]->remove_main(new_parent.key);
    } else {
        merge(index);
        child[index]->remove_main(key);
    }
}

void Node::fill_up_node(int index) {
    if (index != 0 && child[index-1]->n >= t) {
        steal_from_prev(index);
    } else if (index != n && child[index+1]->n >= t) {
        steal_from_next(index);
    } else {
        if (index == n) {
            merge(index-1);
        } else {
            merge(index);
        }
    }
}

void Node::steal_from_next(int index) {
    Node* current = child[index];
    Node* next = child[index+1];
    current->data[current->n] = data[index];
    data[index] = next->data[0];
    if (!current->leaf) {
        current->child[(current->n) + 1] = next->child[0];
    }

    for (int i = 0; i < next->n - 1; ++i) {
        next->data[i] = next->data[i+1];
    }
    if (!next->leaf) {
        for (int i = 0; i < next->n; ++i) {
            next->child[i] = next->child[i+1]; 
        }
    }
    current->n += 1;
    next->n -= 1;
}

void Node::steal_from_prev(int index) {
    Node* current = child[index];
    Node* prev = child[index-1];

    for (int i = current->n; i >= 1; --i) {
        current->data[i] = current->data[i-1];
    }
    current->data[0] = data[index-1];
    if (!current->leaf) {
        for (int i = current->n + 1; i >= 1; --i) {
            current->child[i] = current->child[i-1];
        }
        current->child[0] = prev->child[prev->n];
    }
    data[index-1] = prev->data[prev->n - 1];
    
    current->n += 1;
    prev->n -= 1;
}

void Node::merge(int index) {
    Node* left_child = child[index];
    Node* right_child = child[index+1];

    left_child->data[t-1] = data[index];
    for (int i = 0; i < t - 1; ++i) {
        left_child->data[i+t] = right_child->data[i]; 
    }   
    if (!left_child->leaf) {
        for (int i = 0; i < t; ++i) {
            left_child->child[i + t] = right_child->child[i];
        }
    }
    for (int i = index + 1; i < n; ++i) {
        data[i-1] = data[i];
    }

    for (int i = index + 2; i <= n; ++i) {
        child[i-1] = child[i];
    }

    left_child->n = 2*t - 1;
    --n;
    delete[] right_child->data;
    delete[] right_child->child;
    delete right_child;
}

void Node::save_node(std::ofstream &out) {
    if (child[0] == nullptr) {
        for (int i = 0; i < n; ++i) {
            out.write(data[i].key, sizeof(char) * (strlen(data[i].key) + 1));  
            out.write((char*)&data[i].value, sizeof(long long int));
        }
    } else {
        for (int i = 0; i < n; ++i) {
            child[i]->save_node(out);
            out.write(data[i].key, sizeof(char) * (strlen(data[i].key) + 1));  
            out.write((char*)&data[i].value, sizeof(long long int));
        }
        child[n]->save_node(out);
    }
}

void Node::destroy_node() {
    delete[] data;
    if (child[0] == nullptr) {
        delete[] child;
        return;
    }
    for (int i = 0; i <= n; ++i) {
        if (child[i] != nullptr) {
            child[i]->destroy_node();
        }
        delete child[i];
    }
    delete[] child;
} 


//___________________________________________________RELALIZATION_OF_BTREE_METHODS_______________________________________________________________



BTree::BTree(int _t = 2) {
    root = nullptr;
    t = _t;
}

BTree::~BTree() {
    if (root != nullptr) {
        root->destroy_node();
    }
    delete root;
}

unsigned long long* BTree::search(char* key) {
    if (root == nullptr) {
        return nullptr;
    }

    Node* result = root->search(key);
    if (result == nullptr){
        return nullptr;
    }
    else{
        int i = 0;
        while (i < result->n && strcmp(key, result->data[i].key) > 0) {
            i++;
        }
        return &result->data[i].value;
    }

}

void BTree::insert(Pair elem) {
    if (root == nullptr) {
        root = new Node(t, true);
        root->data[0] = elem;
        root->n = 1;
        std::cout << "OK\n";
        return;
    }
    if (root->search(elem.key) != nullptr) {
        std::cout << "Exist\n";
        return;
    }
    if (root->n == 2*t-1) {
        Node* new_root = new Node(t, false);
        new_root->child[0] = root;
        new_root->split_child(0, root);
        int i = 0;
        if (strcmp(elem.key, new_root->data[0].key) > 0) {
            ++i;
        }
        new_root->insert_non_full(elem);
        root = new_root;
    } else { 
        root->insert_non_full(elem);
    }
    std::cout << "OK\n";
}

void BTree::remove(char* key) {
    if (root == nullptr) {
        std::cout << "NoSuchWord\n";
        return;
    }
    
    if (root->search(key) == nullptr) {
        std::cout << "NoSuchWord\n";
        return;
    }

    root->remove_main(key);
    if (root->n == 0) {
        Node* old_root = root;
        if (root->leaf) {
            root = nullptr;
        } else {
            root = root->child[0];
        }
        delete[] old_root->data;
        delete[] old_root->child;
        delete old_root;
    }
    std::cout << "OK\n";

}

void BTree::save_to_file(char* path) {
    std::ofstream out(path, std::ios::binary);
    short is_tree = 1;
    if (root == nullptr) {
        is_tree = 0;
        out.write((char*)&is_tree, sizeof(short));
        return;
    }

    out.write((char*)&is_tree, sizeof(short));
    root->save_node(out);
    char end_token = '$';
    out.write((char*)&end_token, sizeof(char));
    out.close();
}

void BTree::load_from_file(char* path) {
    if (root != nullptr) {
        root->destroy_node();
        delete root;
        root = nullptr;
    }

    std::ifstream in(path, std::ios::binary);
    short is_tree;
    in.read((char*)&is_tree, sizeof(short));
    char c;
    if (is_tree) {
        while (true) {
            in.read((char*)&c, sizeof(char));
            if (c == '$') {
                break;
            }
            Pair inserted_elem;
            inserted_elem.key[0] = c;
            for (int i = 1; c != '\0'; ++i) {
                in.read((char*)&c, sizeof(char));
                inserted_elem.key[i] = c;
            }
            in.read((char*)&inserted_elem.value, sizeof(unsigned long long));
            this->insert_from_file(inserted_elem);
        }
    }
    in.close();
}

void BTree::insert_from_file(Pair elem) {
    if (root == nullptr) {
        root = new Node(t, true);
        root->data[0] = elem;
        root->n = 1;
        return;
    }
    if (root->n == 2*t-1) {
        Node* new_root = new Node(t, false);
        new_root->child[0] = root;
        new_root->split_child(0, root);
        int i = 0;
        if (strcmp(elem.key, new_root->data[0].key) > 0) {
            ++i;
        }
        new_root->insert_non_full(elem);
        root = new_root;
    } else { 
        root->insert_non_full(elem);
    }
}
}


void to_lover(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL); 
    std::cout.tie(NULL);

    btree::BTree Tree(CHARACTERISTIC_NUMBER);
    Pair data;
    char path[KEY_SIZE];
    char buffer[KEY_SIZE];
    while(std::cin >> buffer) {
        switch (buffer[0]) {
            case '+':
                std::cin >> data.key >> data.value;
                to_lover(data.key);
                Tree.insert(data);
                break;
            case '-':
                std::cin >> buffer;
                to_lover(buffer);
                Tree.remove(buffer);
                break;
            case '!':
                std::cin >> buffer >> path;
                if (!strcmp(buffer, "Save")) {
                    Tree.save_to_file(path);
                } else {
                    Tree.load_from_file(path);
                }
                std::cout << "OK\n";
                break;
            default:
                to_lover(buffer);
                unsigned long long* res;
                res = Tree.search(buffer);
                if (res == nullptr){
                    std::cout << "NoSuchWord\n";
                }
                else{
                    std::cout << "OK: " << *res << "\n";
                }
                break;
        }
    }
    return 0;
}
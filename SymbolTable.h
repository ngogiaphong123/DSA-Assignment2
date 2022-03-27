#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
struct Data {
    string name;
    string value;
    int level;
    string stt;
    string type;
    Data() {
        this->name = "";
        this->value = "";
        this->level = -1;
        this->stt = "false";
        this->type = "";
    }
    Data(string name, string value, int level, string stt, string type) {
        this->name = name;
        this->value = value;
        this->level = level;
        this->stt = stt;
        this->type = type;
    }
};
struct Node {
    Data data;
    Node* left;
    Node* right;
    Node* parent;
    Node() {
        this->data = Data();
        this->left = NULL;
        this->right = NULL;
        this->parent = NULL;
    }
    Node(Data data, Node* left, Node* right, Node* parent) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
};
struct SplayTree {
    Node* root;
    SplayTree() {
        this->root = NULL;
    }
    void leftRotate(Node* node);
    void rightRotate(Node* node);
    bool splay(Node* node);
    bool insert(string name, string type, string stt, int currLevel,int& numComp,int& numSplay);
    bool insertRec(Node* node,int& numComp, int& numSplay);
    Node* search(string name);
    Node* searchRec(Node* node,string name); 
    void remove(Node* node,int& numSplay);
    Node* findMax(Node* node);
    int level(Node* node);
    void print();
    void printRec(Node* node,bool& flag);
    void clearAll(Node* root);
    ~SplayTree() {
        clearAll(this->root);
        this->root = NULL;
    }
};
#endif
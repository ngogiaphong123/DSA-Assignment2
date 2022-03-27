#include "SymbolTable.h"
//--------- TOKENIZE ----------------------------------------------------------------
int numberOfWords(string input,char del = ' ') {
    if(input == "()") return 0;
    int count = 0;
    int n = input.length();
    for(int i = 0 ; i < n ; i++) {
        if(input[i] == del) count++;
    }
    return count+1;
}
string* tokenize(string input, string del = " ") {
    char s = del[0];
    int numberOfWord = numberOfWords(input,s);
    int i=0;
    string *cmd = new string[numberOfWord];
    int start = 0;
    int end = (int)input.find(del); 
    while (end != -1) {
        cmd[i] = input.substr(start, end - start);
        start = (int)end + (int)del.size();
        end = (int)input.find(del, start);
        i++;
    }
    cmd[i] = input.substr(start, end - start);
    return cmd;
}
//---------- CHECK CONDITION ---------------------------------------------------------
bool checkStringValue(string value) {
    regex archetype("\\'[a-zA-Z0-9 ]*\\'");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkNumberValue(string value) {
    regex archetype("\\d+");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkFunctionValue(string value) {
    regex archetype("[a-z]\\w*(\\(\\)|\\(([a-z]\\w*|\\d+|\\'[a-zA-Z0-9 ]*\\')(,([a-z]\\w*|\\d+|\\'[a-zA-Z0-9 ]*\\'))*\\))");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkIdentifierName(string value) {
    if(value == "string" || value == "number" || value == "true" || value == "false") return false;
    regex archetype("[a-z]\\w*");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkPrimitiveType(string value) {
    if(value == "number" || value == "string") return true;
    return false;
}
bool checkFunctionType(string value) {
    regex archetype("(\\(\\)|\\((number|string)(,(number|string))*\\))(->(number|string))");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
//---------SPLAY TREE OPERATIONS -----------------------------------------------------
void SplayTree :: clearAll(Node* node) {
    if(node == NULL) return;
    clearAll(node->left);
    clearAll(node->right);
    Node* del = node;
    del->parent = NULL;
    del->left = NULL;
    del->right = NULL;
    delete del;
    del = NULL;
    node=NULL;
}
void SplayTree :: remove(Node* node,int& numSplay) {
    if(splay(node) == true) numSplay++;
    Node* del = this->root;
    if(this->root != NULL) {
        if(this->root->left == NULL && this->root->right == NULL) {
            delete this->root;
            this->root = NULL;
        }
        else if(this->root->left == NULL) {
            this->root = this->root->right;
            delete del;
            del = NULL;
            this->root->parent = NULL;
        }
        else if(this->root->right == NULL) {
            this->root = this->root->left;
            delete del;
            del = NULL;
            this->root->parent = NULL;
        }
        else {
            Node* max = findMax(this->root->left);
            del = this->root;
            this->root = this->root->left;
            this->root->parent = NULL;
            del->left = NULL;
            if(splay(max) == true) numSplay++;
            this->root->right = del->right;
            del->right->parent = this->root;
            del->right = NULL;
            delete del;
            del = NULL;
        }
    }
}
Node* SplayTree :: findMax(Node* node) {
    while(node->right != NULL) {
        node = node->right;
    }
    return node;
}
int SplayTree :: level(Node* node) {
    int count = 0;
    while(node->parent != NULL) {
        node = node->parent;
        count++;
    }
    return count;
}
Node* SplayTree :: searchRec(Node* node,string name) {
    if(node == NULL) return NULL;
    Node* result = NULL;
    if(node->data.name == name) result = node;
    Node* left = searchRec(node->left,name);
    Node* right = searchRec(node->right,name);
    if(left != NULL && right != NULL) {
        if(left->data.level > right->data.level) {
            result = left;
        }
        else result = right;
    }
    else if(left != NULL && right == NULL) {
        result = left;
    }
    else if(left == NULL && right != NULL) {
        result = right;
    }
    if(result != NULL && node->data.name == name) {
        if(result->data.level < node->data.level) {
            result = node;
        }
    }
    else if(result == NULL && node->data.name == name) {
        result = node;
    }
    return result;
}
Node* SplayTree :: search(string name) {
    return searchRec(this->root,name);
}
void SplayTree :: leftRotate(Node* node) {
    Node *y = node->right;
    node->right = y->left;
    if (y->left != NULL) {
        y->left->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL) { 
        this->root = y;
    }
    else if (node == node->parent->left) { 
        node->parent->left = y;
    }
    else { 
        node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
}
void SplayTree :: rightRotate(Node* node) {
    Node *y = node->left;
    node->left = y->right;
    if (y->right != NULL) {
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL) { 
        this->root = y;
    }
    else if (node == node->parent->right) { 
        node->parent->right = y;
    }
    else { 
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}
bool SplayTree :: splay(Node* node) {
    if(node->parent == NULL) return false;
    while (node->parent != NULL) { 
        if (node->parent == this->root) { 
            if (node == node->parent->left) {
                rightRotate(node->parent);
            }
            else {
                leftRotate(node->parent);
            }
        }
        else {
            Node *p = node->parent;
            Node *gp = p->parent; 

            if (p->left == node && gp->left == p) { 
                rightRotate(gp);
                rightRotate(p);
            }
            else if (p->right == node && gp->right == p) { 
                leftRotate(gp);
                leftRotate(p);
            }
            else if (p->right == node && gp->left == p) {
                leftRotate(p);
                rightRotate(gp);
            }
            else if (p->left == node && gp->right == p) {
                rightRotate(p);
                leftRotate(gp);
            }
        }
    }
    return true;
}
bool SplayTree :: insert(string name, string type, string stt, int currLevel,int& numComp, int& numSplay) {
    if(stt == "true") currLevel = 0;
    Data data(name,"",currLevel,stt,type);
    Node* newNode = new Node(data,NULL,NULL,NULL);
    if(insertRec(newNode,numComp,numSplay) == true) {
        return true;
    }
    else {
        delete newNode;
        return false;
    }
}
bool SplayTree :: insertRec(Node* node,int& numComp, int& numSplay) {
    Node* y = NULL;
    Node* temp = this->root;
    while(temp != NULL) {
        y = temp;
        if(node->data.level < temp->data.level) {
            numComp++;
            temp = temp->left;
        }
        else if(node->data.level == temp->data.level) {
            if(node->data.name < temp->data.name) {
                numComp++;
                temp = temp -> left;
            }
            else if(node->data.name > temp->data.name) {
                numComp++;
                temp = temp -> right;
            }
            else return false;
        }
        else if(node->data.level > temp->data.level) {
            numComp++;
            temp = temp -> right;
        }
    }
    node->parent = y;
    if(y==NULL) {
        this->root = node;
        return true;
    }
    else {
        if(y->data.level > node->data.level) {
            y->left = node;
        }
        else if(y->data.level == node->data.level) {
            if(y->data.name < node->data.name) {
                y->right = node;
            }
            else y->left = node;
        }
        else if(y->data.level < node->data.level) {
            y->right = node;
        }
    }
    if(splay(node)==true) numSplay++;
    return true;
}
void SplayTree :: printRec(Node* root,bool& flag) {
    if(root == NULL) return;
    Node* temp = root;
    if(flag == true) {
        cout << temp->data.name<<"//"<<temp->data.level;
        flag = false;
    }
    else cout << " " << temp->data.name<<"//"<<temp->data.level;
    printRec(temp->left,flag);
    printRec(temp->right,flag);
}
void SplayTree :: print() {
    if(root == NULL) return;
    bool flag = true;
    printRec(root,flag);
    cout << endl;
}
// --------- MAIN PROGRAM -----------------------------------------
void SymbolTable::run(string filename)
{
    int currLevel = 0;
    SplayTree tree;
    string line="";
    string tmp = "";
    ifstream file(filename);
    while(!file.eof()) {
        getline(file, tmp);
        if(tmp == "") {
            throw InvalidInstruction("");
        }
        string* cmd = tokenize(tmp);
        if(cmd[0] == "INSERT") {
            if(checkIdentifierName(cmd[1]) == false) {
                delete[] cmd;
                throw InvalidInstruction(tmp);
            }
            if(checkPrimitiveType(cmd[2]) == true) {
                int numComp = 0;
                int numSplay = 0;
                if(tree.insert(cmd[1],cmd[2],cmd[3],currLevel,numComp,numSplay) == false) {
                    delete[] cmd;
                    throw Redeclared(tmp);
                }
                else {
                    if(currLevel != 0) {
                        if(cmd[3] == "false") {
                            line +=cmd[1] + ",";
                        }
                    }
                    cout << numComp << " " << numSplay << endl;     
                }             
            }
            else if(checkFunctionType(cmd[2]) == true) {
                if(currLevel != 0) {
                    if(cmd[3] != "true") {
                        delete[] cmd;
                        throw InvalidDeclaration(tmp);
                    }
                }
                int numComp = 0;
                int numSplay = 0;
                if(tree.insert(cmd[1],cmd[2],cmd[3],currLevel,numComp,numSplay) == false) {
                    delete[] cmd;
                    throw Redeclared(tmp);
                }
                else cout << numComp << " " << numSplay << endl; 
            }
            else {
                delete[] cmd;
                throw InvalidInstruction(tmp);
            }
        }
        else if(cmd[0] == "ASSIGN") {
            int numberWordsAssign = numberOfWords(tmp);
            if(numberWordsAssign > 3) {
                for(int i = 3 ; i < numberWordsAssign ; i++) {
                    cmd[2] = cmd[2] + " " + cmd[i];
                }
                numberWordsAssign = 3;
            }
            if(checkIdentifierName(cmd[1]) == false) {
                delete[] cmd;
                throw InvalidInstruction(tmp);
            }
            if(checkStringValue(cmd[2]) == true) {
                int numComp = 0;
                int numSplay = 0;
                Node* result = tree.search(cmd[1]);
                if(result == NULL) {
                    delete[] cmd;
                    throw Undeclared(tmp);
                }
                if(result->data.type != "string") {
                    delete[] cmd;
                    throw TypeMismatch(tmp);
                }
                numComp = 1 + tree.level(result);
                if(tree.splay(result) == true) numSplay++;
                cout << numComp << " " << numSplay << endl;
            }
            else if(checkNumberValue(cmd[2]) == true) {
                int numComp = 0;
                int numSplay = 0;
                Node* result = tree.search(cmd[1]);
                if(result == NULL) {
                    delete[] cmd;
                    throw Undeclared(tmp);
                }
                if(result->data.type != "number") {
                    delete[] cmd;
                    throw TypeMismatch(tmp);
                }
                result->data.value = cmd[2];
                numComp = 1 + tree.level(result);
                if(tree.splay(result) == true) numSplay++;
                cout << numComp << " " << numSplay << endl;
            }
            else if(checkFunctionValue(cmd[2]) == true) {
                int numComp = 0;
                int numSplay = 0;
                string temp = cmd[2];
                int openBracket = temp.find("(");
                string functionName = temp.substr(0, openBracket);
                temp = temp.substr(openBracket);
                int sizeArg = numberOfWords(temp,',');
                temp = temp.substr(1,temp.length()-2);
                string* arg = tokenize(temp,",");
                Node* result = tree.search(functionName);
                if(result == NULL) {
                    delete[] arg;
                    delete[] cmd;
                    throw Undeclared(tmp);
                }
                if(result->data.type == "string" || result->data.type == "number") {
                    delete[] arg;
                    delete[] cmd;
                    throw TypeMismatch(tmp);
                }
                numComp += 1 + tree.level(result);
                if(tree.splay(result) == true) numSplay++;
                string typeResult = result->data.type;
                int arrowSignal = typeResult.find("->");
                string returnType = typeResult.substr(arrowSignal+2);
                typeResult = typeResult.substr(0,arrowSignal);
                int sizeType = numberOfWords(typeResult,',');
                typeResult = typeResult.substr(1,typeResult.length()-2);
                string* type = tokenize(typeResult, ",");
                if(sizeType != sizeArg) {
                    delete[] type;
                    delete[] cmd;
                    delete[] arg;
                    throw TypeMismatch(tmp);
                }
                else {
                    for(int i = 0 ; i < sizeArg ; i++) {
                        if(checkNumberValue(arg[i]) == true) {
                            if(type[i] != "number") {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(tmp);
                            }
                        }
                        else if(checkStringValue(arg[i]) == true) {
                            if(type[i] != "string") {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(tmp);
                            }
                        }
                        else if(checkIdentifierName(arg[i])==true) {
                            Node* var = tree.search(arg[i]);
                            if(var == NULL) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw Undeclared(tmp);
                            }
                            else {
                                if(var->data.type != type[i]) {
                                    delete[] type;
                                    delete[] arg;
                                    delete[] cmd;
                                    throw TypeMismatch(tmp);
                                }
                            }
                            numComp += 1 + tree.level(var);
                            if(tree.splay(var) == true) numSplay++;
                        }
                    }
                }
                Node* assigned = tree.search(cmd[1]);
                if(assigned == NULL) {
                    delete[] arg;
                    delete[] type;
                    delete[] cmd;
                    throw Undeclared(tmp);
                }
                if(assigned->data.type != returnType) {
                    delete[] type;
                    delete[] cmd;
                    delete[] arg;
                    throw TypeMismatch(tmp);
                }
                numComp += 1 + tree.level(assigned);
                if(tree.splay(assigned) == true) numSplay++;
                cout << numComp <<" "<< numSplay <<endl;
                delete[] arg;
                delete[] type;
            }
            else if(checkIdentifierName(cmd[2]) == true) {
                int numComp = 0;
                int numSplay = 0;
                Node* var = tree.search(cmd[2]);
                Node* assigned = tree.search(cmd[1]);
                if(var == NULL || assigned == NULL) {
                    delete[] cmd;
                    throw Undeclared(tmp);
                }
                if(var->data.type != assigned->data.type) {
                    delete[] cmd;
                    throw TypeMismatch(tmp);
                }
                if(assigned->data.type != "number" && assigned->data.type != "string") {
                    delete[] cmd;
                    throw TypeMismatch(tmp);
                }
                if(var->data.type != "number" && var->data.type != "string") {
                    delete[] cmd;
                    throw TypeMismatch(tmp);
                }
                assigned->data.value = var->data.value;
                numComp += 1 + tree.level(var);
                if(tree.splay(var) == true) numSplay++;
                numComp += 1 + tree.level(assigned);
                if(tree.splay(assigned) == true) numSplay++;
                cout << numComp << " " << numSplay << endl;
            }
            else {
                delete[] cmd;
                throw InvalidInstruction(tmp);
            }
        }
        else if(cmd[0] == "BEGIN") {
            currLevel++;
            line += "|";
        }
        else if(cmd[0] == "END") {
            currLevel --;
            if(currLevel < 0) {
                delete[] cmd;
                throw UnknownBlock();
            }
            int i = 0;
            for(i = line.length(); i >= 0; i--) {
                if(line[i] == '|') break;
            }
            string temp = line.substr(i+1);
            if(temp == "") {
                line = line.substr(0, i);
                delete[] cmd;
                continue;
            }
            temp = temp.erase(temp.length()-1,1);
            int sizeBeRemoved = numberOfWords(temp,',');
            int numSplay = 0;
            string* beRemoved = tokenize(temp,",");
            for(int j = 0 ; j < sizeBeRemoved ; j++) {
                Node* node = tree.search(beRemoved[j]);
                tree.remove(node,numSplay);
            }
            line = line.substr(0,i);
            delete[] beRemoved;
        }   
        else if(cmd[0] == "LOOKUP") {
            if(checkIdentifierName(cmd[1]) == false) {
                delete[] cmd;
                throw InvalidInstruction(tmp);
            }
            Node* result = tree.search(cmd[1]);
            if(result == NULL) {
                delete[] cmd;
                throw Undeclared(tmp);
            }
            tree.splay(result);
            cout << result->data.level << endl;
        }
        else if(cmd[0] == "PRINT") {
            tree.print();
        }
        else {
            delete[] cmd;
            throw InvalidInstruction(tmp);
        }
        delete[] cmd;
    }
    if(currLevel > 0) throw UnclosedBlock(currLevel);
}
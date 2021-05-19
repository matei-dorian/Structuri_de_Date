#include <bits/stdc++.h>

using namespace std;
ifstream fin("abce.in");
ofstream fout("abce.out");

const int C = 100001;
int Q;

struct Node{
    int key;
    int priority;
    Node *left, *right;

    /// constructor
    Node(int value){
        key = value;
        priority = rand() % C;
        left = right = NULL;
    }

}*root;


inline Node *rotate_right(Node *parent){
/*
        5              3
       / \            / \
      3   8   --->   1   5
     / \                / \
    1   4              4   8

    parent -> 5
    left_son -> 3
    right_grandson -> 4 (aici avem doar noduri dar pointerii pot arata si catre subarbori)
    */

    Node *left_son = parent->left,  *right_grandson = left_son->right;

    left_son->right = parent;
    parent->left = right_grandson;

    // returnam noua radacina
    return left_son;
}

inline Node *rotate_left(Node *parent){
/*
        3              5
       / \            / \
      1   5   --->   3   8
         / \        / \
        4   8      1   4

    parent -> 3
    right_son -> 3
    left_grandson -> 4
    */

    Node *right_son = parent->right,  *left_grandson = right_son->left;

    right_son->left = parent;
    parent->right = left_grandson;

    // returnam noua radacina
    return right_son;
}

void insert_node(Node* &root, int x){

    if(!root){ /// cazul cu rezolvare imediata
        root = new Node(x);
        return;
    }

    if(x < root->key){ /// daca x e mai mic decat radacina -> mergem in subarborele stang
        insert_node(root->left, x);
        if(root->left != NULL && root->left->priority > root->priority) /// dupa ce am inserat nodul avem grija sa mentinem si structura de max-heap
            root = rotate_right(root);
    }

    else{ /// altfel mergem in subarborele drept
        insert_node(root->right, x);
        if(root->right != NULL && root->right->priority > root->priority)
            root = rotate_left(root);
    }

}

bool search_value(Node* root, int v){
    /// cautarea se realizeaza la fel ca in orice arbore de cautare
    if (root == NULL)
        return 0;
    if(root->key == v)
        return 1;

    return (v > root->key) ? search_value(root->right, v) : search_value(root->left, v);
}

void print(Node* root){
    if(root->left) print(root->left);
    cout<<root->priority<<" : "<<root->key<<"\n";
    if(root->right) print(root->right);
}

void print_interval(Node* root, int x, int y){
    if(root->left) print_interval(root->left, x, y);
    if(root->key >= x && root->key <= y)
        fout<<root->key<<" ";
    if(root->right) print_interval(root->right, x, y);
}

void delete_node(Node* &root, int x){
    if(root == NULL) /// valoarea pe care vrem sa o cautam nu se afla in treap
        return;

    if (x > root->key)  /// cautam nodul pe care vrem sa l stergem
        delete_node(root->right, x);
    else if (x < root->key)
        delete_node(root->left, x);

        else{ /// daca am ajuns pana aici stim ca x == root->key
            /// Caz 1: nodul este frunza -> il stergem direct
            if(root->right == NULL && root->left == NULL){
                delete root;
                root = NULL;
            }
            else if(root->left && root->right)
            { /// Caz 2: nodul are 2 fii -> facem o rotatie pentru a transforma radacina in frunza
                if(root->right->priority > root->left->priority){
                    root = rotate_left(root);
                    delete_node(root->left, x);
                }
                else{
                        root = rotate_right(root);
                        delete_node(root->right, x);
                }
            }
            else{/// Caz 3: nodul are un singur fiu -> facem swap
                Node *child = (root->right) ? root->right : root->left;
                Node *temp = root;

                root = child;
                delete temp;
            }
        }
}

int predecessor(Node *root, int x){
    /// functia returneaza predecesorul lui x din treap
    if(root == NULL)
        return -1; /// nu exista predecesor
    if(root->key == x)
        return x;

    if(root->key < x){ /// cautam in subarborele drept
        int k = predecessor(root->right, x);
        if(k == -1)
            return root->key;
        return k;
    }

    if (root->key > x)
        return predecessor(root->left, x);

}

int successor(Node *root, int x){
    /// functia returneaza succesorul lui x din treap
    if(root == NULL)
        return -1; /// nu exista predecesor
    if(root->key == x)
        return x;

    if(root->key > x){ /// cautam in subarborele stang
        int k = successor(root->left, x);
        if(k == -1)
            return root->key;
        return k;
    }

    if(root->key < x)
        return successor(root->right, x);
}


int main()
{
    int cmd, X, Y;

    srand(time(NULL)); /// pentru a obtine prioritati diferite din functia rand() la fiecare rulare a programului
    fin>>Q;

    for(int i = 1; i <= Q; ++ i){
        fin>>cmd;
        if(cmd == 6) fin>>X>>Y;
        else fin>>X;

        switch(cmd){
            case 1: insert_node(root, X);
                    break;
            case 2: delete_node(root, X);
                    break;
            case 3: fout<<search_value(root, X)<<"\n";
                    break;
            case 4: fout<<predecessor(root, X)<<"\n";
                    break;
            case 5: fout<<successor(root, X)<<"\n";
                    break;
            case 6: print_interval(root, X, Y);
                    fout<<"\n";
                    break;
            default:
                    break;
        }

    }
    return 0;
}

#include <fstream>
#include <iostream>
enum COLOR{RED, BLACK};

using namespace std;

ifstream fin("abce.in");
ofstream gout("abce.out");

struct Node{
    COLOR color;
    int key;
    Node *left, *right, *parent;

    Node(COLOR _color, int _key, Node* _left, Node* _right, Node* _parent){
        color = _color;
        key = _key;
        left = _left;
        right = _right;
        parent = _parent;
    }
};

typedef struct Node* Nod;

class RBT{
private:
    Nod root;
    Nod NIL;

    Nod TreeMinimum(Nod x);
    Nod TreeMaximum(Nod x);
    Nod SearchNode(int val);
    void LeftRotate(Nod x);
    void RightRotate(Nod x);
    void InsertNode(Nod z);
    void InsertFixup(Nod z);
    void Replace(Nod u, Nod v);
    void DeleteNode(Nod z);
    void DeleteFixup(Nod x);

public:
    RBT(){
        NIL = new Node(BLACK, 0, nullptr, nullptr, nullptr);
        root = NIL;
    }
    int Minimum();
    int Maximum();
    bool Search(int val);
    void Insert(int val);
    void Delete(int val);
    int Successor(int val);
    int Predecessor(int val);
    void Sort(int l, int r);
};

Nod RBT::TreeMinimum(Nod x){
    while(x -> left != NIL)
        x = x -> left;
    return x;
}
Nod RBT::TreeMaximum(Nod x){
    while(x -> right != NIL)
        x = x -> right;
    return x;
}
int RBT::Minimum(){
    return TreeMinimum(root) -> key;
}
int RBT::Maximum(){
    return TreeMaximum(root) -> key;
}
Nod RBT::SearchNode(int val){
    Nod x = root;
    while(x != NIL && x -> key != val){
        if(val < x -> key)
            x = x -> left;
        else
            x = x -> right;
    }
    return x;
}
bool RBT::Search(int val){
    return SearchNode(val) != NIL;
}
void RBT::LeftRotate(Nod x){
    Nod y = x -> right;
    x -> right = y -> left;
    if(y -> left != NIL)
        y -> left -> parent = x;
    y -> parent = x -> parent;
    if(x -> parent == NIL)
        root = y;
    else if(x == x -> parent -> left)
        x -> parent -> left = y;
    else
        x -> parent -> right = y;
    y -> left = x;
    x -> parent = y;
}
void RBT::RightRotate(Nod x){
    Nod y = x -> left;
    x -> left = y -> right;
    if(y -> right != NIL)
        y -> right -> parent = x;
    y -> parent = x -> parent;
    if(x -> parent == NIL)
        root = y;
    else if(x == x -> parent -> right)
        x -> parent -> right = y;
    else
        x -> parent -> left = y;
    y -> right = x;
    x -> parent = y;
}
void RBT::InsertNode(Nod z){
    Nod y = NIL;
    Nod x = root;
    while(x != NIL){
        y = x;
        if(z -> key < x -> key)
            x = x -> left;
        else
            x = x -> right;
    }
    z -> parent = y;
    if(y == NIL)
        root = z;
    else if(z -> key < y -> key)
        y -> left = z;
    else
        y -> right = z;
    z -> left = NIL;
    z -> right = NIL;
    z -> color = RED;
    InsertFixup(z);
}
void RBT::InsertFixup(Nod z){
    while(z -> parent -> color == RED)
        if(z -> parent == z -> parent -> parent -> left){
            Nod y = z -> parent -> parent -> right;
            if(y -> color == RED){                  /// case 1: z.uncle = RED -> RECOLOR
                z -> parent -> color = BLACK;
                y -> color = BLACK;
                z -> parent -> parent -> color = RED;
                z = z -> parent -> parent;
            }
            else{
                if(z == z -> parent -> right){                /// case 2: z.uncle = black (trinagle) -> rotate z.parent
                    z = z -> parent;
                    LeftRotate(z);
                }
                z -> parent -> color = BLACK;
                z -> parent -> parent -> color = RED;           /// case 3: z.uncle = black (line) -> rotate z.grandparent
                RightRotate(z -> parent -> parent);             ///       and RECOLOR
            }
        }
        else{
            Nod y = z -> parent -> parent -> left;
            if(y -> color == RED){                              /// case 1: z.uncle = RED -> RECOLOR
                z -> parent -> color = BLACK;
                y -> color = BLACK;
                z -> parent -> parent -> color = RED;
                z = z -> parent -> parent;
            }                                                   ///  SYMMETRIC
            else{
                if(z == z -> parent -> left){
                    z = z -> parent;                             /// case 2: z.uncle = black (trinagle) -> rotate z.parent
                    RightRotate(z);
                }
                z -> parent -> color = BLACK;
                z -> parent -> parent -> color = RED;           /// case 3: z.uncle = black (line) -> rotate z.grandparent
                LeftRotate(z -> parent -> parent);              ///        and RECOLOR
            }
        }
    root->color = BLACK;                                             /// case 0
}
void RBT::Insert(int val){
    Nod z = new Node(RED, val, NIL, NIL, NIL);
    InsertNode(z);
}
void RBT::Replace(Nod u, Nod v){
    if(u -> parent == NIL)
        root = v;
    else if(u == u -> parent -> left)
        u -> parent -> left = v;
    else
        u -> parent -> right = v;
    v -> parent = u -> parent;
}
void RBT::DeleteNode(Nod z){
    Nod x, y = z;
    COLOR yOriginalColor = y -> color;
    if(z -> left == NIL){                                      /// doar un fiu drept
        x = z -> right;
        Replace(z, z -> right);
    }
    else if(z -> right == NIL){                                 /// doar un fiu stang
        x = z -> left;
        Replace(z, z -> left);

    }
    else{                                                       /// doi fii
        y = TreeMinimum(z -> right);
        yOriginalColor = y -> color;
        x = y -> right;

        if(y -> parent == z)
            x -> parent = y;
        else{
            Replace(y, y -> right);
            y -> right = z -> right;
            y -> right -> parent = y;

        }
        Replace(z, y);
        y -> left = z -> left;
        y -> left -> parent = y;
        y -> color = z -> color;
    }
    delete z;
    if(yOriginalColor == BLACK)
        DeleteFixup(x);
}
void RBT::DeleteFixup(Nod x){
    while(x != root && x -> color == BLACK){
        if(x == x -> parent -> left){
            Nod w = x -> parent -> right;
            if(w -> color == RED){                              /// case 1
                w -> color = BLACK;
                x -> parent -> color = RED;
                LeftRotate(x -> parent);
                w = x -> parent -> right;
            }
            if(w -> left -> color == BLACK && w -> right -> color == BLACK){            /// case 2
                w -> color = RED;
                x = x -> parent;
            }
            else{
                if(w -> right -> color == BLACK){
                    w -> left -> color = BLACK;
                    w -> color = RED;                   /// case 3
                    RightRotate(w);
                    w = x -> parent -> right;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = BLACK;           /// case 4
                w -> right -> color = BLACK;
                LeftRotate(x -> parent);
                x = root;
            }
        }
        else{                                           /// SYMMETRIC
            Nod w = x -> parent -> left;
            if(w -> color == RED){
                w -> color = BLACK;                             /// case 1
                x -> parent -> color = RED;
                RightRotate(x -> parent);
                w = x -> parent -> left;
            }
            if(w -> left -> color == BLACK && w -> right -> color == BLACK){      /// case 2
                w -> color = RED;
                x = x -> parent;
            }
            else{
                if(w -> left -> color == BLACK){
                    w -> right -> color = BLACK;
                    w -> color = RED;                           /// case 3
                    LeftRotate(w);
                    w = x -> parent -> left;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = BLACK;
                w -> left -> color = BLACK;                     /// case 4
                RightRotate(x -> parent);
                x = root;
            }
        }
    }
    x -> color = BLACK;     /// case 0
}
void RBT::Delete(int val){
    Nod x = SearchNode(val);
    if(x != NIL)
        DeleteNode(x);
}
int RBT::Successor(int val){
    Nod x = root;
    int ans;
    while(x != NIL){
        if(x -> key == val)
            return x -> key;
        if(x -> key < val)
            x = x -> right;
        else{
            ans = x -> key;
            x = x -> left;
        }
    }
    return ans;
}
int RBT::Predecessor(int val){
    Nod x = root;
    int ans;
    while(x != NIL){
        if(x -> key == val)
            return x -> key;
        if(x -> key > val)
            x = x -> left;
        else{
            ans = x -> key;
            x = x -> right;
        }
    }
    return ans;
}
void RBT::Sort(int l, int r){
    int next = Successor(l);
    while(next <= r and RBT::Search(next)){
        gout << next << " ";
        next = Successor(next + 1);
    }
}

int main(){
    RBT *T = new RBT();
    int q; //numarul de cerinte
    fin>>q;
    for (int i=0; i<q; i++)
    {
        int type, x, y;
        fin>>type;
        switch(type)
        {
    case 1:
        fin>>x;
        T->Insert(x);
        break;

    case 2:
        fin>>x;
        T->Delete(x);
        break;
    case 3:
        fin>>x;
        gout<<T->Search(x)<<"\n";
        break;

    case 4:
        fin>>x;
        gout<<T->Predecessor(x)<<"\n";
        break;

    case 5:
        fin>>x;
        gout<<T->Successor(x)<<"\n";
        break;

    case 6:
        fin>>x>>y;
        T->Sort(x, y);
        gout<<"\n";
        break;
        }
    }
    return 0;
}

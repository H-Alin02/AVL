#include "../HeaderFiles/AVL.h"
#include <iostream>
using namespace std;

AVL::AVL() : root(nullptr){}

AVL::~AVL(){
    deleteTree(root);
}

void AVL::insert(int key){
    root = insert(root,key);
}

AVLNode* AVL::insert(AVLNode* node, int key){
    // empty tree
    if(!node) return new AVLNode(key);

    // Finding the right place to insert
    if(key < node->key)
        node->left = insert(node->left,key);
    else if(key > node->key)
        node->right = insert(node->right,key);
    else {
        return node; 
    }

    // updateHeight
    updateHeight(node);

    // Balance controll and Rotation
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVL::deleteKey(int key){
    root = deleteNode(root,key);
}

AVLNode* AVL::deleteNode(AVLNode* node, int key){
    // empty tree
    if(!node) return node;

    // Find node to delete
    if(key < node->key)
        node->left = deleteNode(node->left,key);
    else if(key > node->key)
        node->right = deleteNode(node->right,key);
    else { // key found
        if(!node->left || !node->right){
            AVLNode* tmp = node->left ? node->left : node->right;
            if(!tmp){ 
                // Leaf
                tmp = node;
                node = nullptr;
            }else{
                // Single child
                *node = *tmp;
            }

            delete tmp;
        }else{
            // Two children
            AVLNode* tmp = minValueNode(node->right);
            node->key = tmp->key;
            node->right = deleteNode(node->right, tmp->key);
        }
    }

    // if the current node is deleted       
    if(!node) return node;

    // updateHeight
    updateHeight(root);

    // Balance controll and Rotation
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return node;
}

AVLNode* AVL::rotateLeft(AVLNode* x){
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

AVLNode* AVL::rotateRight(AVLNode* y){
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

int AVL::getHeight(AVLNode* node) const{
    return node? node->height : 0;
}

int AVL::getBalance(AVLNode* node) const{
    return node ? getHeight(node->left) - getHeight(node->right): 0;
}

void AVL::updateHeight(AVLNode* node){
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

AVLNode* AVL::minValueNode(AVLNode* node){
    AVLNode* current = node;
    while(current && current->left){
        current = current->left;
    }
    return current;
}

bool AVL::search(int key) const{
    AVLNode* current = root;
    while(current){
        if(key == current->key)
            return true;
        else if(key < current->key)
            current = current->left;
        current = current->right;
    }
    return false;
}

void AVL::printInOrder() const{
    inOrderTraversal(root);
    cout << "\n\n";
}

void AVL::inOrderTraversal(AVLNode* node) const{
    if(node){
        inOrderTraversal(node->left);
        cout << node->key << " -> ";
        inOrderTraversal(node->right);
    }
}

void AVL::deleteTree(AVLNode* node){
    if(!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void AVL::printTree() const {
    cout<<"\n\n-----------------------------\n\n";
    printTree(root, 0);
    cout<<"\n-------------------------------\n\n";

}

void AVL::printTree(AVLNode* node, int depth) const {
   if (node == nullptr) return;

    // Right subtree
    printTree(node->right, depth + 1);

    // Print identation and the key
    for (int i = 0; i < depth; ++i) {
        cout << "\t"; // Indentazione
    }
    cout << node->key << endl;

    // Left subtree
    printTree(node->left, depth + 1);
}
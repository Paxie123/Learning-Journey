#include<iostream>
#include<string>
#define RED 0
#define BLACK 1

typedef int KEY_TYPE;

typedef struct _rbtree_node{
    KEY_TYPE key;
    void* value;
    int color;
    rbtree_node* left;
    rbtree_node* right;
    rbtree_node* parent;
}rbtree_node;

typedef struct _rbtree{
    rbtree_node* root;
    rbtree_node* nil;
}rbtree;

void rbtree_left_rotate(rbtree* T, rbtree_node* x){
    rbtree_node* y = x->right;

    //x的右子树应变为y的左子树(即便为空也直接变)，但是y的左子树给父节点要先判断y是否有左子树
    x->right = y->left;
    if(y->left != T->nil){
        y->left->parent = x;
    }

    //x的parent的子节点变为y,就要考虑是左节点还是右节点
    y->parent = x->parent;
    if(x == T->root){
        T->root = y;
    }else if(x == x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }

    //y的左子树应变为x，x的父节点变为y
    y->left = x;
    x->parent = y;
}

void rbtree_right_rotate(rbtree* T, rbtree_node* y){
    rbtree_node* x = y->right;

    y->left = x->right;
    if(x->right != T->nil){
        x->right->parent = y;
    }

    x->parent = y->parent;
    if(y == T->root){
        T->root = x;
    }else if(y == y->parent->right){
        y->parent->right = x;
    }else{
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void rbtree_insert(rbtree* T, rbtree_node* z){
    rbtree_node* x = T->root;
    rbtree_node* y = T->nil;
    while(x != T->nil){
        y=x;
        if(z->key > x->key){
            x = x->right;
        }else if(z->key < x->key){
            x=x->left;
        }else{
            return;
        }
    }
    if(y == T->nil){
        T->root = x;
    }else{
        if(z->key > y->key){
            y->right = z;
        }else{
            y->left = z;
        }
    }
    z->parent = y;
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    return;
}

void rbtree_insert_fixup(rbtree* T, rbtree_node* z){
    while(z->parent->color == RED){
        //叔父节点在右边
        if(z->parent == z->parent->parent->left){
            rbtree_node* y = z->parent->parent->right; // 叔父节点
            //叔父节点是红色
            if(y->color == RED){
                z->parent->parent->color = RED;
                z->parent->color = BLACK;
                y->color = BLACK;
                z=z->parent->parent;
            }else{ //叔父节点是黑色,如果z是右子树，需要先围绕z的父节点左旋
                if(z==z->parent->right){
                    z=z->parent;
                    rbtree_left_rotate(T,z);
                }
                //如果z是左子树，父节点改为黑色，祖父节点改为红色，围绕祖父节点进行右旋
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_right_rotate(T,z->parent->parent);
                z=z->parent->parent;
            }
        }
    }
}
#include<iostream>
#include<string>
#define RED 1
#define BLACK 0

#define RBTREE_ENTRY(name,type) \
    struct name{\
        struct type *right;\
        struct type *left; \
        struct type *parent;\
        unsigned char color; \
    }
    

typedef int KEY_TYPE;


typedef struct _rbtree_node{ //不可复用，定义完了之后不可迁移。
   
    KEY_TYPE key;
    void* value;
#if 0
    struct _rbtree_node *right;
        struct _rbtree_node *left; 
        struct _rbtree_node *parent;
        unsigned char color; 
#else
    RBTREE_ENTRY(,_rbtree_node) ready;
#endif
    
} _rbtree_node;

typedef struct _rbtree{

    struct _rbtree_node* root;
    struct _rbtree_node* nil;
}rbtree;

void rbtree_left_rotate(rbtree *T, _rbtree_node* x){
    _rbtree_node *y = x->ready.right;
    x->ready.right = y->ready.left;
    if(y->ready.left!=T->nil){
        x->ready.right = y->ready.left;
    }
    y->ready.parent = x->ready.parent;
    if(x==T->root){
        T->root = y;
    }else if(x == x->ready.parent->ready.left){
        x->ready.parent->ready.left = y;
    }else{
        x->ready.parent->ready.right = y;
    }

    y->ready.left = x;
    x->ready.parent = y;
}

void rbtree_right_rotate(rbtree *T, _rbtree_node* y){
    _rbtree_node *x = y->ready.left;
    y->ready.left = x->ready.right;
    if(x->ready.right!=T->nil){
        y->ready.left = x->ready.right;
    }
    x->ready.parent = y->ready.parent;
    if(y==T->root){
        T->root = x;
    }else if(y == x->ready.parent->ready.right){
        y->ready.parent->ready.right = x;
    }else{
        y->ready.parent->ready.left = x;
    }

    x->ready.right = y;
    y->ready.parent = x;
}

void rbtree_insert(rbtree *T, _rbtree_node* z){
    _rbtree_node* x = T->root;
    _rbtree_node* y = T->nil;
    while(x!=T->nil){
        y=x;
        if(z->key < x->key){
            x = x->ready.left;
        }else if(z->key > x->key){
            x = x->ready.right;
        }else{//exist
            return;
        }
    }
    if(y == T->nil){
        T->root = z;
    }
    else{
        if (y->key > z->key){
            y->ready.left = z;
        }else{
            y->ready.right = z;
        }
    }
    z->ready.parent = y;
    z->ready.left = T->nil;
    z->ready.right = T->nil;
    z->ready.color = RED;
    return;
}

void rbtree_insert_fixup(rbtree* T, _rbtree_node* z){
    while(z->ready.parent->ready.color == RED){
        if(z->ready.parent == z->ready.parent->ready.left){ //叔父节点在右边
            _rbtree_node *y = z->ready.parent->ready.right;
            if(y->ready.color == RED){
                z->ready.color = BLACK;
                y->ready.color = BLACK;
                z->ready.parent->ready.parent->ready.color = RED;
                z=z->ready.parent->ready.parent;
            }else{ // y == BLACK
                if(z == z->ready.parent->ready.right){ // 右边重，直接父节点左旋
                    z=z->ready.parent;
                    rbtree_left_rotate(T,z->ready.parent);
                }
                // 左边重，z的父节点改为黑色，祖父节点改为红色，进行右旋
                z->ready.parent->ready.color = BLACK;
                z->ready.parent->ready.parent->ready.color = RED;
                rbtree_right_rotate(T,z->ready.parent->ready.parent);
            }
        }else{
            _rbtree_node *y = z->ready.parent->ready.left;
            if(y->ready.color == RED){
                z->ready.color = BLACK;
                y->ready.color = BLACK;
                z->ready.parent->ready.parent->ready.color = RED;
            }
        }
    }
}
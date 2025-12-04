
// 1、每个节点有至多拥有M个孩子节点，M-1个关键字
// 2、根节点至少拥有两颗子树
// 3、除了根节点以外，其余每个分支节点至少拥有M/2棵子树
// 4、所有的叶节点都在同一层（平衡因子等于0的多路查找树）
// 5、有K棵子树的分支节点则存在k-1个关键字，关键字按照递增顺序进行排序
// 6、关键字数量满足ceil(M/2)-1 <= N <=M-1

#include<iostream>
using namespace std;

#define SUB_M 3
//SUB_M = M/2

typedef struct _btree_node{
    int keys[2 * SUB_M - 1];            // 最多M-1个关键字,5
    btree_node * childrens[2*SUB_M];    // 最多M个孩子节点,6
    int num;                            //keys里面存储的数量
    int leaf;                           //是否为叶子节点
}btree_node;

typedef struct _btree{
    btree_node* root;
}btree;

btree_node* btree_create_node(int leaf){
    btree_node* node = (btree_node*)calloc(1,sizeof(btree_node));
    if(node == NULL) return NULL;

    node->leaf = leaf;

    node->keys = (int*)calloc(2*SUB_M-1, sizeof(int));
    if (node->keys == NULL) {
        free(node);
        return NULL;
    }

    node->childrens = (btree_node**)calloc(2*SUB_M, sizeof(btree_node*));
    if (node->childrens == NULL) {
        free(node->keys);
        free(node);
        return NULL;
    }

    node->num = 0;
    return node;

}

void btree_destory_node(btree_node* node){
    free(node->childrens);
    free(node->keys);
    free(node);
}

void btree_split_child(btree* T, btree_node* x, int idx){ //T表示树，x表示的被分裂节点的父节点，i表示被分裂节点在其父节点的位置
    btree_node* y = x->childrens[idx];
    btree_node* z = btree_create_node(y->leaf);
    
    //子节点分裂
    z->num = SUB_M -1;
    for(int i = 0; i< z->num; i++){
        z->keys[i] = y->keys[SUB_M+i];
    }
    if(y->leaf == 0){
        for(int i = 0;i<SUB_M;i++){
            z->childrens[i] = y->childrens[SUB_M+i];
        }
    }
    y->num = SUB_M;

    //父节点插入
    for(int i = x->num; i >= idx+1; i--){
        x->childrens[i+1] = x->childrens[i];
    }
    x->childrens[idx] = z;
    for(int i = x->num; i >= idx+1; i--){
        x->keys[i+1] = x->keys[i];
    }
    x->keys[idx] = y->keys[SUB_M];
    x->num+=1;

}

//根节点分裂先创建一个空节点，然后子树指向根节点，再像上方那样分裂
void btree_insert(btree* T, int key){
    btree_node* r = T->root;
    if(r->num == 2*SUB_M - 1){
        btree_node * node = btree_create_node(0);
        T->root = node;
        node->childrens[0] = r;
        btree_split_child(T,node,0);
    }
}

//合并
void btree_merge(btree* T, btree_node* x, int idx){
    btree_node* left = x->childrens[idx];
    btree_node* right = x->childrens[idx+1];
    left->keys[left->num] = x->keys[idx];

    for(int i = 0; i< right->num; i++){
        left->keys[SUB_M+i] = right->keys[i];
    }

    if(!left->leaf){
        for(int i = 0;i<SUB_M;i++){
            left->childrens[SUB_M+i] = right->childrens[i];
        }
    }
    left->num += SUB_M;
    btree_destory_node(right);

    for(int i = idx+1;i<x->num;i++){
        x->keys[i-1] = x->keys[i];
        x->childrens[i] = x->childrens[i+1];//因为合并后是idx+1的位置被合并到idx上了，所以从idx+1合并
    }
}
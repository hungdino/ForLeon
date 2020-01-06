#include <stdio.h>
#include <stdlib.h>

#define RED  0
#define BLACK 1

typedef struct Node_t{
    int data;
    int color;
    struct Node_t *parent;
    struct Node_t *left;
    struct Node_t *right;
}Node;
typedef struct{
    Node *root;
}RedBlackTree;

void left_rotate(RedBlackTree *tree, Node *x){
    Node *y = x->right;

    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        tree->root=y;
    else{
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
void right_rotate(RedBlackTree *tree, Node *y){
    Node *x = y->left;

    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        tree->root = x;
    else{
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}
void insert_fixup(RedBlackTree *tree, Node *node){
    Node *parent;
    Node *pparent;

    while (node->parent!=NULL && node->parent->color==RED){
        parent = node->parent;
        pparent = parent->parent;

        if (parent == pparent->left){
            Node *uncle = pparent->right;
            if (uncle!=NULL && uncle->color==RED){
                uncle->color=BLACK;
                parent->color=BLACK;
                pparent->color=RED;
                node = pparent;
                continue;
            }
            if (parent->right == node){
                Node *tmp;
                left_rotate(tree, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->color =BLACK;
            pparent->color =RED;
            right_rotate(tree, pparent);
        }else{
            Node *uncle = pparent->left;
            if (uncle!=NULL && uncle->color==RED){
                uncle->color=BLACK;
                parent->color=BLACK;
                pparent->color=RED;
                node = pparent;
                continue;
            }
            if (parent->left == node){
                Node *tmp;
                right_rotate(tree, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->color =BLACK;
            pparent->color =RED;
            left_rotate(tree, pparent);
        }
    }
    tree->root->color=BLACK;
}
void insert(RedBlackTree *tree, Node *node){
    Node *y = NULL;
    Node *x = tree->root;

    for(x=tree->root;x!=NULL;){
        y = x;
        if (node->data < x->data)
            x = x->left;
        else
            x = x->right;
    }
    node->parent=y;
    if (y != NULL){
        if (node->data < y->data)
            y->left = node;
        else
            y->right = node;
    }
    else
        tree->root = node;
    node->left = NULL;
    node->right = NULL;
    node->color = RED;
    insert_fixup(tree, node);
}
void delete_fixup(RedBlackTree *tree, Node *node, Node *parent){
    Node *other;

    while ((!node || node->color==BLACK) && node != tree->root){
        if (parent->left == node){
            other = parent->right;
            if (other->color==RED){
                other->color=BLACK;
                parent->color=RED;
                left_rotate(tree, parent);
                other = parent->right;
            }
            if ((!other->left || other->left->color==BLACK) &&
                (!other->right || other->right->color==BLACK)){
                other->color=RED;
                node = parent;
                parent = node->parent;
            } else{
                if (!other->right || other->right->color==BLACK){
                    other->left->color=BLACK;
                    other->color=RED;
                    right_rotate(tree, other);
                    other = parent->right;
                }
                other->color=parent->color;
                parent->color=BLACK;
                other->right->color=BLACK;
                left_rotate(tree, parent);
                node = tree->root;
                break;
            }
        } else {
            other = parent->left;
            if (other->color==RED){
                other->color=BLACK;
                parent->color=RED;
                right_rotate(tree, parent);
                other = parent->left;
            }
            if ((!other->left || other->left->color==BLACK) &&
                (!other->right || other->right->color==BLACK)){
                other->color=RED;
                node = parent;
                parent = node->parent;
            }else{
                if (!other->left || other->left->color==BLACK){
                    other->right->color=BLACK;
                    other->color=RED;
                    left_rotate(tree, other);
                    other = parent->left;
                }
                other->color=parent->color;
                parent->color=BLACK;
                other->left->color=BLACK;
                right_rotate(tree, parent);
                node = tree->root;
                break;
            }
        }
    }
    if (node)
        node->color=BLACK;
}
void Delete(RedBlackTree *tree, Node *node){
    Node *child, *parent;
    int color;

    if (node->left!=NULL && node->right!=NULL) {
        Node *replace = node;
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;
        if (node->parent!=NULL){
            if (node->parent->left == node)
                node->parent->left = replace;
            else
                node->parent->right = replace;
        }
        else
            tree->root = replace;
        child = replace->right;
        parent = replace->parent;
        color = replace->color;
        if (parent == node)
            parent = replace;
        else{
            if (child)
                child->parent=parent;
            parent->left = child;
            replace->right = node->right;
            node->right->parent=replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if (color == BLACK)
            delete_fixup(tree, child, parent);

        return ;
    }
    if (node->left !=NULL)
        child = node->left;
    else
        child = node->right;
    parent = node->parent;
    color = node->color;
    if (child)
        child->parent = parent;
    if (parent){
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        tree->root = child;
    if (color == BLACK)
        delete_fixup(tree, child, parent);

}
Node* search(RedBlackTree *tree, int k){
    Node *x;

    for(x=tree->root;x!=NULL;){
        if (k < x->data)
            x = x->left;
        else if(k> x->data)
            x = x->right;
        else {
            return x;
        }
    }
    return NULL;
}
void inorder_traversal(Node *node,FILE *fout){
    if(node->left!=NULL)
        inorder_traversal(node->left,fout);
    if(node->parent!=NULL)
        printf("key: %d parent: %d color: %s\n",node->data,node->parent->data,node->color==RED?"red":"black");
    else
        printf("key: %d parent:   color: %s\n",node->data,node->color==RED?"red":"black");
    if(node->right!=NULL)
        inorder_traversal(node->right,fout);
}
int main(int argc,char *argv[]){
    FILE *fin;
    FILE *fout;
    int num;
    int op;
    int data;
    int start;
    char c;
    RedBlackTree tree;

    fin=fopen("input.txt","r");


    tree.root=NULL;
    fscanf(fin,"%d",&num);
    while(num>0){
        fscanf(fin,"%d",&op);
        start=0;
        do{
            fscanf(fin,"%d",&data);
            if(op==1){
                if(start==0){
                    printf("Insert: %d",data);
                    start=1;
                }else
                    printf(", %d",data);
                Node* node=(Node*)malloc(sizeof(Node));
                //memset(node,0,sizeof(Node));
                node->data=data;
                insert(&tree,node);

            }else{
                if(start==0){
                    printf("Delete: %d",data);
                    start=1;
                }else
                    printf(", %d",data);
                Node* node=search(&tree, data);
                if(node!=NULL){
                    Delete(&tree,node);
                }
            }
        }while((c=getc(fin)) != EOF && c != '\n');
        printf("\n");
        inorder_traversal(tree.root,fout);
        num--;
    }

    fclose(fin);
    return 0;
}

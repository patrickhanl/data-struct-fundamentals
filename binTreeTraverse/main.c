//
//  main.c
//  binTreeTraverse
//
//  Created by Patrick Hanley on 6/24/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode {
    int key;
    int left;
    int right;
} treeNode;

void inOrder (treeNode * tree, int index);
void preOrder (treeNode * tree, int index);
void postOrder (treeNode * tree, int index);

int main(void) {
    freopen("input.txt", "r", stdin);
    int numNodes, i;
    scanf("%d", &numNodes);
    
    treeNode * tree;
    tree = (treeNode *)malloc(sizeof(treeNode) * numNodes);
    for (i = 0; i < numNodes; i++) {
        scanf("%d%d%d", &tree[i].key, &tree[i].left, &tree[i].right);
    }
    
    inOrder(tree, 0);
    printf("\n");
    preOrder(tree, 0);
    printf("\n");
    postOrder(tree, 0);
    
    return 0;
}

void inOrder (treeNode * tree, int index) {
    if (tree[index].left != -1) {
        inOrder(tree, tree[index].left);
    }
    printf("%d ", tree[index].key);
    if (tree[index].right != -1) {
        inOrder(tree, tree[index].right);
    }
}

void preOrder (treeNode * tree, int index) {
    printf("%d ", tree[index].key);
    if (tree[index].left != -1) {
        preOrder(tree, tree[index].left);
    }
    if (tree[index].right != -1) {
        preOrder(tree, tree[index].right);
    }
}

void postOrder (treeNode * tree, int index) {
    if (tree[index].left != -1) {
        postOrder(tree, tree[index].left);
    }
    if (tree[index].right != -1) {
        postOrder(tree, tree[index].right);
    }
    printf("%d ", tree[index].key);
}

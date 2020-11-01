//
//  main.c
//  validateBinTree
//
//  Created by Patrick Hanley on 6/25/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode {
    int key;
    int left;
    int right;
} treeNode;

void validate (treeNode * tree, int index, int * valid, int * last);

int main(void) {
    freopen("input.txt", "r", stdin);
    int numNodes, i, valid = 1, last = -2147483648;
    scanf("%d", &numNodes);
    
    if (numNodes <= 1) {
        printf("CORRECT");
        return 0;
    }
    
    treeNode * tree;
    tree = (treeNode *)malloc(sizeof(treeNode) * numNodes);
    for (i = 0; i < numNodes; i++) {
        scanf("%d%d%d", &tree[i].key, &tree[i].left, &tree[i].right);
    }
    
    validate(tree, 0, &valid, &last);
    
    if (valid) {
        printf("CORRECT");
    } else {
        printf("INCORRECT");
    }
    
    return 0;
}

void validate (treeNode * tree, int index, int * valid, int * last) {
    if (!valid) {
        return;
    }
    if (tree[index].left != -1) {
        validate(tree, tree[index].left, valid, last);
    }
    
    if (*last > tree[index].key) {
        *valid = 0;
    } else {
        *last = tree[index].key;
    }
    
    /*if ((tree[index].left != -1 && tree[tree[index].left].key > tree[index].key) || (tree[index].right != -1 && tree[tree[index].right].key < tree[index].key)) {
        *valid = 0;
    }*/
    
    if (tree[index].right != -1) {
        validate(tree, tree[index].right, valid, last);
    }
}

//
//  main.c
//  treeHeight
//
//  Created by Patrick Hanley on 6/4/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//
// Algorithm reads in a rooted tree and outputs the height (single node has height 1).

#include <stdio.h>
#include <stdlib.h>

//each node contains parent index, number of children and pointer of pointer to array of children
typedef struct treeNode {
    int index;
    int childCount;
    struct treeNode ** childArray;
} treeNode;

//function definitions
int treeHeight (treeNode * root, int height);

int main(void) {
    int treeSize, i, parentIndex;
    treeNode * root, * tree;
    
    scanf("%d", &treeSize);
    
    //trees of size 1 or 2 have height = size
    if (treeSize == 1 || treeSize == 2) {
        return treeSize;
    }
    
    //allocate memory for tree
    tree = (treeNode *)malloc(sizeof(treeNode) * treeSize);
    
    //create empty nodes for all tree nodes
    for (i = 0; i < treeSize; i++) {
        treeNode newNode;
        newNode.index = i;
        newNode.childCount = 0;
        newNode.childArray = NULL;
        tree[i] = newNode;
    }
    
    //this loop reads in tree
    for (i = 0; i < treeSize; i++) {
        scanf("%d", &parentIndex);
        
        if (parentIndex == -1) {
            //sets root
            root = &tree[i];
        } else {
            //if not root, add node to array of children in parent node. each node keeps track of number of children
            tree[parentIndex].childArray = (treeNode **)realloc(tree[parentIndex].childArray, sizeof(treeNode *) * (tree[parentIndex].childCount + 1));
            tree[parentIndex].childArray[tree[parentIndex].childCount] = &tree[i];
            tree[parentIndex].childCount++;
        }
    }
    
    //print result of helper function that calculates height
    printf("%d", treeHeight(root, 0));
    
    free(tree);
    
    return 0;
}

//function takes in root of tree with inital height. then checks each child node to find the one with the greatest height and returns that height + 1 to account for the parent node
int treeHeight (treeNode * root, int height) {
    int i, subHeight = 0, testHeight;
    //enter loop if there are children, else return height 1 for single node
    if (root->childArray != NULL) {
        
        for (i = 0; i < root->childCount; i++) {
            //gets height of each child and saves greatest height
            testHeight = treeHeight(root->childArray[i], height);
            if (testHeight > subHeight) {
                subHeight = testHeight;
            }
        }
    }
    return subHeight + 1;
}

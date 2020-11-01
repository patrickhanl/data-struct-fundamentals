//
//  main.c
//  rope
//
//  Created by Patrick Hanley on 7/11/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct ropeNode {
    char character;
    int subTreeSize;
    struct ropeNode * parent;
    struct ropeNode * left;
    struct ropeNode * right;
} ropeNode;

ropeNode * findKthNode (ropeNode * currentNode, int k);

ropeNode * splay(ropeNode * splayNode);

ropeNode * zigLeft(ropeNode * splayNode);
ropeNode * zigRight(ropeNode * splayNode);

ropeNode * zigZigLeft(ropeNode * splayNode);
ropeNode * zigZagLeft(ropeNode * splayNode);

ropeNode * zigZigRight(ropeNode * splayNode);
ropeNode * zigZagRight(ropeNode * splayNode);

void newSum(ropeNode *);

ropeNode * splitLeft(ropeNode * root);
ropeNode * splitRight(ropeNode * root);

ropeNode * mergeRopes(ropeNode * leftChild, ropeNode * rightChild);

ropeNode * mergeThree(ropeNode * root, ropeNode * fragmentRoot, ropeNode * rightChild);
void printRope(ropeNode *);

int main(void) {
    freopen("input.txt", "r", stdin);
    char string[300001];
    int numOperations, i = 1, left, right, insert;
    ropeNode * root;
    
    root = (ropeNode *)malloc(sizeof(ropeNode));
    
    scanf("%s", string);

    root->character = string[0];
    root->subTreeSize = 1;
    root->parent = NULL;
    root->left = NULL;
    root->right = NULL;
    
    while (string[i] != '\0') {
        ropeNode * newNode;
        newNode = (ropeNode *)malloc(sizeof(ropeNode));
        
        newNode->character = string[i];
        newNode->subTreeSize = root->subTreeSize + 1;
        newNode->parent = NULL;
        newNode->left = root;
        newNode->right = NULL;
        
        root->parent = newNode;
        root = newNode;
        i++;
    }
    
    scanf("%d", &numOperations);
    
    for (i = 0; i < numOperations; i++) {
        ropeNode * leftChild, * rightChild, * fragmentRoot;
        //leftChild = (ropeNode *)malloc(sizeof(ropeNode));
        //rightChild = (ropeNode *)malloc(sizeof(ropeNode));
        //fragmentRoot = (ropeNode *)malloc(sizeof(ropeNode));
        
        scanf("%d%d%d", &left, &right, &insert);
        if (left == insert == 0) {
            continue;
        }
        root = findKthNode(root, left + 1);
        while (root->parent != NULL) {
            splay(root);
        }
        
        leftChild = splitLeft(root);
        
        root = findKthNode(root, right - left + 1);
        while (root->parent != NULL) {
            splay(root);
        }
        
        rightChild = splitRight(root);
        fragmentRoot = root;
        root = mergeRopes(leftChild, rightChild);
        
        root = findKthNode(root, insert);
        while (root->parent != NULL) {
            splay(root);
        }
        
        if (insert != 0) {
            rightChild = splitRight(root);
        } else  {
            rightChild = root;
            root = NULL;
        }
        
        /*printf("\n\nROOT:");
        printRope(root);
        printf("\nMIDDLE");
        printRope(fragmentRoot);
        printf("\nRIGHT:");
        printRope(rightChild);
         */
        
        root = mergeThree(root, fragmentRoot, rightChild);
        
        printRope(root);
        printf("\n");
    }
    
    //printRope(root);
    
    return 0;
}

ropeNode * findKthNode (ropeNode * currentNode, int k) {
    int leftSize = 0;
    if (currentNode->left != NULL) {
        leftSize += currentNode->left->subTreeSize;
    }
    if (k == leftSize + 1) {
        return currentNode;
        
    } else if (k < leftSize + 1) {
        if (currentNode->left != NULL) {
            return findKthNode(currentNode->left, k);
        } else {
            return currentNode;
        }
        
    } else {
        if (currentNode->right != NULL) {
            return findKthNode(currentNode->right, k - leftSize - 1);
        } else {
            return currentNode;
        }
    }
}

ropeNode * splay(ropeNode * splayNode) {
    if (splayNode->parent == NULL) {
        return splayNode;
    }
    if (splayNode->parent->parent == NULL) {
        if (splayNode->parent->left == splayNode) {
            zigRight(splayNode);
        } else {
            zigLeft(splayNode);
        }
    } else {
        if (splayNode->parent->left == splayNode) {
            if (splayNode->parent->parent->left == splayNode->parent) {
                zigZigRight(splayNode);
            } else {
                zigZagLeft(splayNode);
            }
            
        } else {
            if (splayNode->parent->parent->right == splayNode->parent) {
                zigZigLeft(splayNode);
            } else {
                zigZagRight(splayNode);
            }
        }
    }
    
    return splayNode;
}

ropeNode * zigLeft (ropeNode * currentNode) {
    ropeNode * leftChild = currentNode->left, * root = currentNode->parent;
    
    if (leftChild != NULL) {
        leftChild->parent = root;
    }
    root->right = leftChild;
    
    root->parent = currentNode;
    currentNode->left = root;
    
    newSum(root);
    
    currentNode->parent = NULL;
    
    newSum(currentNode);
    
    return currentNode;
}

ropeNode * zigRight (ropeNode * currentNode) {
    ropeNode * rightChild = currentNode->right, * root = currentNode->parent;
    
    if (rightChild != NULL) {
        rightChild->parent = root;
    }
    root->left = rightChild;
    
    root->parent = currentNode;
    currentNode->right = root;
    
    newSum(root);
    
    currentNode->parent = NULL;
    
    newSum(currentNode);
    
    return currentNode;
}

ropeNode * zigZagRight(ropeNode * currentNode) {
    ropeNode * parent = currentNode->parent, * rightChild = currentNode->right, * leftChild = currentNode->left, * grandParent = currentNode->parent->parent;
    
    if (leftChild != NULL) {
        leftChild->parent = parent;
        
    }
    parent->right = leftChild;
    
    parent->parent = currentNode;
    
    if (rightChild != NULL) {
        rightChild->parent = grandParent;
    }
    
    currentNode->left = parent;
    currentNode->right = grandParent;
    currentNode->parent = grandParent->parent;
    
    if (grandParent->parent != NULL) {
        if (grandParent->parent->right == grandParent) {
            grandParent->parent->right = currentNode;
        } else {
            grandParent->parent->left = currentNode;
        }
    }
    
    grandParent->left = rightChild;
    grandParent->parent = currentNode;
    
    newSum(parent);
    newSum(grandParent);
    newSum(currentNode);
    
    return currentNode;
    
}

ropeNode * zigZagLeft(ropeNode * currentNode) {
    ropeNode * parent = currentNode->parent, * rightChild = currentNode->right, * leftChild = currentNode->left, * grandParent = currentNode->parent->parent;
    
    if (rightChild != NULL) {
        rightChild->parent = parent;
        
    }
    parent->left = rightChild;
    
    parent->parent = currentNode;
    
    if (leftChild != NULL) {
        leftChild->parent = grandParent;
    }
    
    currentNode->right = parent;
    currentNode->left = grandParent;
    currentNode->parent = grandParent->parent;
    
    if (grandParent->parent != NULL) {
        if (grandParent->parent->right == grandParent) {
            grandParent->parent->right = currentNode;
        } else {
            grandParent->parent->left = currentNode;
        }
    }
    
    grandParent->right = leftChild;
    grandParent->parent = currentNode;
    
    newSum(parent);
    newSum(grandParent);
    newSum(currentNode);
    
    return currentNode;
    
}

ropeNode * zigZigLeft(ropeNode * currentNode) {
    ropeNode * parent = currentNode->parent, * leftChild = currentNode->left, * parentLeftChild = currentNode->parent->left, * grandParent = currentNode->parent->parent;
    
    currentNode->parent = grandParent->parent;
    currentNode->left = parent;
    
    parent->parent = currentNode;
    parent->right = leftChild;
    parent->left = grandParent;
    
    if (parentLeftChild != NULL) {
        parentLeftChild->parent = grandParent;
    }
    
    if (leftChild != NULL) {
        leftChild->parent = parent;
    }
    
    if (grandParent->parent != NULL) {
        if (grandParent->parent->right == grandParent) {
            grandParent->parent->right = currentNode;
        } else {
            grandParent->parent->left = currentNode;
        }
    }
    grandParent->right = parentLeftChild;
    grandParent->parent = parent;
    
    newSum(grandParent);
    newSum(parent);
    newSum(currentNode);
    
    return currentNode;
}

ropeNode * zigZigRight(ropeNode * currentNode) {
    ropeNode * parent = currentNode->parent, * rightChild = currentNode->right, * parentRightChild = currentNode->parent->right, * grandParent = currentNode->parent->parent;
    
    currentNode->parent = grandParent->parent;
    currentNode->right = parent;
    
    parent->parent = currentNode;
    parent->left = rightChild;
    parent->right = grandParent;
    
    if (parentRightChild != NULL) {
        parentRightChild->parent = grandParent;
    }
    
    if (rightChild != NULL) {
        rightChild->parent = parent;
    }
    
    if (grandParent->parent != NULL) {
        if (grandParent->parent->right == grandParent) {
            grandParent->parent->right = currentNode;
        } else {
            grandParent->parent->left = currentNode;
        }
    }
    grandParent->left = parentRightChild;
    grandParent->parent = parent;
    
    newSum(grandParent);
    newSum(parent);
    newSum(currentNode);
    
    return currentNode;
}

void newSum(ropeNode * currentNode) {
    currentNode->subTreeSize = 1;
    
    if (currentNode->right != NULL) {
        currentNode->subTreeSize += currentNode->right->subTreeSize;
    }
    
    if (currentNode->left != NULL) {
        currentNode->subTreeSize += currentNode->left->subTreeSize;
    }
    
}

ropeNode * splitLeft(ropeNode * root) {
    ropeNode * leftChild = root->left;
    
    if (leftChild != NULL) {
        leftChild->parent = NULL;
        root->subTreeSize -= leftChild->subTreeSize;
    }
    root->left = NULL;
    
    
    return leftChild;
    
}

ropeNode * splitRight(ropeNode * root) {
    ropeNode * rightChild = root->right;
    if (rightChild != NULL) {
        rightChild->parent = NULL;
        
        root->right = NULL;
        root->subTreeSize -= rightChild->subTreeSize;
    }
    
    return rightChild;
    
}

ropeNode * mergeRopes(ropeNode * leftChild, ropeNode * rightChild) {
    if (leftChild == NULL) {return rightChild;}
    if (rightChild == NULL) {return leftChild;}
    leftChild = findKthNode(leftChild, 400000);
    while (leftChild->parent != NULL) {
        splay(leftChild);
    }
    rightChild = findKthNode(rightChild, 1);
    while (rightChild->parent != NULL) {
        splay(rightChild);
    }
    leftChild->right = rightChild;
    rightChild->parent = leftChild;
    leftChild->subTreeSize += rightChild->subTreeSize;
    
    return leftChild;
}

ropeNode * mergeThree(ropeNode * root, ropeNode * fragmentRoot, ropeNode * rightChild) {
    //add conditions for if inserting at beginning or end, may need conditions in main
    if (rightChild == NULL && root == NULL) {
        return fragmentRoot;
    } else if (rightChild == NULL) {
        return mergeRopes(root, fragmentRoot);
    } else if (root == NULL) {
        return mergeRopes(fragmentRoot, rightChild);
    }
    
    fragmentRoot->parent = root;
    root->right = fragmentRoot;
    root->subTreeSize += fragmentRoot->subTreeSize;
    
    root = findKthNode(root, 300002);
    while (root->parent != NULL) {
        splay(root);
    }
    root->right = rightChild;
    rightChild->parent = root;
    root->subTreeSize += rightChild->subTreeSize;
    return root;
}

void printRope (ropeNode * tree) {
    if (tree->left != NULL) {
        printRope(tree->left);
    }
    printf("%c", tree->character);
    if (tree->right != NULL) {
        printRope(tree->right);
    }
}

//
//  main.c
//  setRangeSum
//
//  Created by Patrick Hanley on 6/26/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode {
    long long int key;
    long long int sum;
    struct treeNode * parent;
    struct treeNode * left;
    struct treeNode * right;
} treeNode;

treeNode * find (long long int, treeNode * searchNode);
treeNode * splayFind(long long int key, treeNode * root);

treeNode * splay(treeNode * splayNode);

treeNode * zigLeft(treeNode * splayNode);
treeNode * zigRight(treeNode * splayNode);

treeNode * zigZigLeft(treeNode * splayNode);
treeNode * zigZagLeft(treeNode * splayNode);

treeNode * zigZigRight(treeNode * splayNode);
treeNode * zigZagRight(treeNode * splayNode);

treeNode * next(treeNode * currentNode);
treeNode * leftDescendent(treeNode * currentNode);
treeNode * rightAncestor(treeNode * currentNode);

void validate (treeNode * tree, int * valid, long long int * last);

void newSum(treeNode * currentNode);


int main(void) {
    freopen("input.txt", "r", stdin);
    treeNode * root;
    int numOperations, i, M = 1000000001, treeEmpty = 1, valid = 1;
    long long int integer, lowerRange, upperRange, x[1];
    char operation[5];
    
    x[0] = 0;
    
    root = (treeNode *)malloc(sizeof(treeNode));
    root->parent = NULL;
    root->left = NULL;
    root->right = NULL;
    root->key = -1;
    root->sum = -1;
    
    scanf("%d", &numOperations);
    
    for (i = 0; i < numOperations; i++) {
        scanf("%s", operation);
        
        if (operation[0] == '+') {
            long long int key;
            treeNode * leaf,* newNode;
            scanf("%lld", &integer);
            key = (integer + x[0]) % M;
            if (treeEmpty) {
                root->key = key;
                root->sum = key;
                treeEmpty = 0;
            } else {
                //should this be a splay find????
                leaf = find(key, root);
                if (leaf->key == key) {
                    while(leaf->parent != NULL) {
                        root = splay(leaf);
                    }
                    continue;
                }
                newNode = (treeNode *)malloc(sizeof(treeNode));
                newNode->parent = leaf;
                newNode->key = key;
                newNode->left = NULL;
                newNode->right = NULL;
                newNode->sum = key;
                if (key > leaf->key) {
                    leaf->right = newNode;
                } else {
                    leaf->left = newNode;
                }
                while (newNode->parent != NULL){
                    splay(newNode);
                }
                root = newNode;
            }
        }
        
        else if (operation[0] == '-') {
            long long int key;
            scanf("%lld", &integer);
            key = (integer + x[0]) % M;
            treeNode * foundNode = find(key, root);
            if (foundNode->key != key) {
                while (foundNode->parent != NULL) {
                    foundNode = splay(foundNode);
                }
                root = foundNode;
                continue;
            }
            //do we need to check if there is no sucessor??
            root = splayFind(next(foundNode)->key, root);
            root = splayFind(foundNode->key, root);
            treeNode * leftChild = root->left, * rightChild = root->right;
            
            if (rightChild == NULL) {
                if (leftChild == NULL) {
                    root->key = -1;
                    root->sum = -1;
                    treeEmpty = 1;
                } else {
                    free(root);
                    root = leftChild;
                }
            } else {
                rightChild->left = leftChild;
                root = rightChild;
                
                if (leftChild != NULL) {
                    leftChild->parent = rightChild;
                    rightChild->sum = leftChild->sum + rightChild->sum;
                }
                
                if (root->parent != NULL) {
                    free(root->parent);
                }
            }
            root->parent = NULL;
        }
        
        else if (operation[0] == '?') {
            scanf("%lld", &integer);
            long long int key = (integer + x[0]) % M;
            root = splayFind(key, root);
            if (root->key == key) {
                printf("Found\n");
            } else {
                printf("Not found\n");
            }
        }
        
        else if (operation[0] == 's') {
            scanf("%lld%lld", &lowerRange, &upperRange);
            long long int lowerKey = (lowerRange + x[0]) % M;
            long long int upperKey = (upperRange + x[0]) % M;
            
            root = splayFind(lowerKey, root);
            
            if (root->key < lowerKey) {
                //this is suspect, don't set root to non root
                root = next(root);
                while (root->parent != NULL) {
                    root = splay(root);
                }
            }
            
            if (root->key > upperKey || root->key < lowerKey || root->key == -1) {
                printf("0\n");
                x[0] = 0;
                continue;
            }
            
            treeNode * lowerNode = root;
            
            treeNode * leftChild = root->left;
            
            root->left = NULL;
            if (leftChild != NULL) {
                leftChild->parent = NULL;
                root->sum -= leftChild->sum;
            }
            
            root = splayFind(upperKey, root);
            //if upperKey is greater than root but less than next largest value, next largest value is being splayed to root and added to sum. need condition to handle this.
            x[0] = root->sum;
            if (root->right != NULL) {
                x[0] -= root->right->sum;
            }
            
            if (root->key > upperKey) {
                x[0] -= root->key;
            }
            
            if (leftChild != NULL) {
                lowerNode->left = leftChild;
                leftChild->parent = lowerNode;
                lowerNode->sum += leftChild->sum;
            }
            
            printf("%lld\n", x[0]);
        }
        long long int last  = -9223372036854775807;
        validate(root, &valid, &last);
    }
    
    return 0;
}

treeNode * splayFind(long long int key, treeNode * root) {
    treeNode * foundNode = find(key, root);
    while (foundNode->parent != NULL) {
        foundNode = splay(foundNode);
    }
    return foundNode;
}

treeNode * find (long long int key, treeNode * currentNode) {
    if (currentNode->key == key || (currentNode->key > key && currentNode->left == NULL) || (currentNode->key < key && currentNode->right == NULL)) {
        return currentNode;
    }
    else if (currentNode->key > key) {
        return find(key, currentNode->left);
    }
    else {
        return find(key, currentNode->right);
    }
}

treeNode * splay(treeNode * splayNode) {
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

treeNode * zigLeft (treeNode * currentNode) {
    treeNode * leftChild = currentNode->left, * root = currentNode->parent;
    
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

treeNode * zigRight (treeNode * currentNode) {
    treeNode * rightChild = currentNode->right, * root = currentNode->parent;
    
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

treeNode * zigZagRight(treeNode * currentNode) {
    treeNode * parent = currentNode->parent, * rightChild = currentNode->right, * leftChild = currentNode->left, * grandParent = currentNode->parent->parent;
    
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

treeNode * zigZagLeft(treeNode * currentNode) {
    treeNode * parent = currentNode->parent, * rightChild = currentNode->right, * leftChild = currentNode->left, * grandParent = currentNode->parent->parent;
    
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

treeNode * zigZigLeft(treeNode * currentNode) {
    treeNode * parent = currentNode->parent, * leftChild = currentNode->left, * parentLeftChild = currentNode->parent->left, * grandParent = currentNode->parent->parent;
    
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
        if (currentNode->key > grandParent->parent->key) {
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

treeNode * zigZigRight(treeNode * currentNode) {
    treeNode * parent = currentNode->parent, * rightChild = currentNode->right, * parentRightChild = currentNode->parent->right, * grandParent = currentNode->parent->parent;
    
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
        if (currentNode->key > grandParent->parent->key) {
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

void newSum(treeNode * currentNode) {
    currentNode->sum = currentNode->key;
    
    if (currentNode->right != NULL) {
        currentNode->sum += currentNode->right->sum;
    }
    
    if (currentNode->left != NULL) {
        currentNode->sum += currentNode->left->sum;
    }
    
}

treeNode * next(treeNode * currentNode) {
    if (currentNode->right != NULL) {
        return leftDescendent(currentNode->right);
    } else {
        return rightAncestor(currentNode);
    }
}

treeNode * leftDescendent(treeNode * currentNode) {
    if (currentNode->left == NULL) {
        return currentNode;
    } else {
        return leftDescendent(currentNode->left);
    }
}

treeNode * rightAncestor(treeNode * currentNode) {
    if (currentNode->parent == NULL) {
        return currentNode;
    }
    else if (currentNode->key < currentNode->parent->key) {
        return currentNode->parent;
    } else {
        return rightAncestor(currentNode->parent);
    }
}

void validate (treeNode * tree, int * valid, long long int * last) {
    if (!valid) {
        return;
    }
    if (tree->left != NULL) {
        validate(tree->left, valid, last);
    }
    
    if (*last > tree->key) {
        *valid = 0;
    } else {
        *last = tree->key;
    }
    
    if (tree->right != NULL) {
        validate(tree->right, valid, last);
    }
}

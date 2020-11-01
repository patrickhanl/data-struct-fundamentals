//
//  main.c
//  hashWithChains
//
//  Created by Patrick Hanley on 6/17/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stringNode {
    char string[16];
    struct stringNode * next;
    struct stringNode * previous;
} stringNode;

long long unsigned int intPow (int base, int exp);
int customHash (char * string, int lenString, int m);

int main(void) {
    //freopen("input.txt", "r", stdin);
    int m, N, i, j, hashIndex;
    char command[6], input[16];
    stringNode * table;
    
    scanf("%d%d", &m, &N);
    
    table = (stringNode *)malloc(sizeof(stringNode) * m);
    
    for (i = 0; i < m; i++) {
        for (j = 0; j < 16; j++) {
            table[i].string[j] = '\0';
        }
        table[i].next = NULL;
        table[i].previous = NULL;
    }
    
    for (i = 0; i < N; i++) {
        scanf("%s", command);
        
        if (command[0] == 'a') {
            int alreadyContains = 0;
            scanf("%s", input);
            hashIndex = customHash(input, (int)strlen(input), m);
            if (table[hashIndex].string[0] == '\0') {
                for (j = 0; j < 16; j++) {
                    table[hashIndex].string[j] = input[j];
                }
            } else {
                if (strcmp(table[hashIndex].string, input) == 0) {
                    continue;
                }
                stringNode * currentNode = table[hashIndex].next;
                while (currentNode != NULL) {
                    if (strcmp(currentNode->string, input) == 0) {
                        currentNode = NULL;
                        alreadyContains = 1;
                        continue;
                        
                    }
                    currentNode = currentNode->next;
                }
                if (alreadyContains) {
                    continue;
                }
                stringNode * oldNode = (stringNode *)malloc(sizeof(stringNode)), newNode;
                
                for (j = 0; j < 16; j++) {
                    oldNode->string[j] = table[hashIndex].string[j];
                }
                oldNode->next = table[hashIndex].next;
                
                newNode.next = oldNode;
                newNode.previous = NULL;
                for (j = 0; j < 16; j++) {
                    newNode.string[j] = input[j];
                }
                table[hashIndex] = newNode;
                
                oldNode->previous = &table[hashIndex];
                if (oldNode->next != NULL) {
                    oldNode->next->previous = oldNode;
                }
            }
            
        } else if (command[0] == 'd') {
            scanf("%s", input);
            hashIndex = customHash(input, (int)strlen(input), m);
            stringNode * currentNode = &table[hashIndex];
            if (strcmp(input, table[hashIndex].string) == 0) {
                if (table[hashIndex].next == NULL) {
                    for (j = 0; j < 16; j++) {
                        table[hashIndex].string[j] = '\0';
                    }
                } else {
                    if (table[hashIndex].next->next != NULL) {
                        table[hashIndex].next->next->previous = &table[hashIndex];
                    }
                    table[hashIndex] = *table[hashIndex].next;
                    table[hashIndex].previous = NULL;
                }
            } else {
                while (currentNode != NULL) {
                    if (strcmp(input, currentNode->string) == 0) {
                        currentNode->previous->next = currentNode->next;
                        if (currentNode->next != NULL) {
                            currentNode->next->previous = currentNode->previous;
                        }
                        free(currentNode);
                    }
                    currentNode = currentNode->next;
                }
            }
            
        } else if (command[0] == 'f') {
            int found = 0;
            scanf("%s", input);
            hashIndex = customHash(input, (int)strlen(input), m);
            stringNode * currentNode = &table[hashIndex];
            if (strcmp(input, table[hashIndex].string) == 0) {
                found = 1;
                printf("yes\n");
            } else {
                while (currentNode != NULL) {
                    if (strcmp(input, currentNode->string) == 0) {
                        found = 1;
                        printf("yes\n");
                    }
                    currentNode = currentNode->next;
                }
            }
            if (found == 0) {printf("no\n");}
        } else {
            scanf("%d", &j);
            printf("%s ", table[j].string);
            stringNode * currentNode = table[j].next;
            while(currentNode != NULL) {
                printf("%s ", currentNode->string);
                currentNode = currentNode->next;
            }
            printf("\n");
        }
    }
    
    return 0;
}

int customHash (char * string, int lenString, int m) {
    int i, x = 263, p = 1000000007;
    long long unsigned int result = 0;
    
    for (i = lenString - 1; i > -1; i --) {
        result = (((result * x + string[i]) % p) + p) % p;
    }
    result = result % m;
    
    //printf("%lld", result);
    
    return result;
}

long long unsigned int intPow (int base, int exp) {
    long long unsigned int result = 1;
    int i;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    
    return result;
}

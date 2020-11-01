//
//  main.c
//  mergeTables
//
//  Created by Patrick Hanley on 6/14/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct tableNode {
    int parent;
    int size;
} tableNode;

int findRoot(int childNode, tableNode * tables);
int tableUnion(int destination, int source, tableNode * tables, int * tableRank, int maxSize);

int main(void) {
    //freopen("input.txt", "r", stdin);
    int numTables, numMerges, i, * tableRank, source, destination, maxSize = 0;
    tableNode * tables;
    scanf("%d%d", &numTables, &numMerges);
    
    tables = (tableNode *)malloc(sizeof(tableNode) * numTables);
    tableRank = (int *)calloc(numTables, sizeof(int));
    
    for (i = 0; i < numTables; i++) {
        tables[i].parent = i;
        scanf("%d", &tables[i].size);
        if (tables[i].size > maxSize) {maxSize = tables[i].size;}
    }
    
    for (i = 0; i < numMerges; i++) {
        scanf("%d%d", &destination, &source);
        destination --;
        source --;
        maxSize = tableUnion(destination, source, tables, tableRank, maxSize);
        printf("%d\n", maxSize);
    }
    
    return 0;
}

int tableUnion(int destination, int source, tableNode * tables, int * tableRank, int maxSize) {
    int rootDestination, rootSource;
    rootDestination = findRoot(destination, tables);
    rootSource = findRoot(source, tables);
    
    if (rootDestination == rootSource) {return maxSize;}
    
    if (tableRank[rootDestination] > tableRank[rootSource]) {
        tables[rootSource].parent = rootDestination;
        tables[rootDestination].size = tables[rootSource].size + tables[rootDestination].size;
        if (tables[rootDestination].size > maxSize) {maxSize = tables[rootDestination].size;}
    } else {
        tables[rootDestination].parent = rootSource;
        if (tableRank[rootDestination] == tableRank[rootSource]) {
            tableRank[rootSource] = tableRank[source] + 1;
        }
        tables[rootSource].size = tables[rootSource].size + tables[rootDestination].size;
        if (tables[rootSource].size > maxSize) {maxSize = tables[rootSource].size;}
    }
    
    return maxSize;
}

int findRoot(int childNode, tableNode * tables) {
    while (tables[childNode].parent != childNode) {
        childNode = tables[childNode].parent;
    }
    return childNode;
}

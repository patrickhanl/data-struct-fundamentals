//
//  main.c
//  minHeapSwaps
//
//  Created by Patrick Hanley on 6/9/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int getParent(int i);
int getLeftChild (int i);
int getRightChild (int i);
void siftDown(int i, int heapSize, int * heap, int *swapsI, int * swapsJ, int * numSwaps);
void swap (int i, int j, int * heap);

int main(void) {
    int i, heapSize, * heap, numSwaps = 0, * swapsI, * swapsJ;
    
    scanf("%d", &heapSize);
    
    heap = (int *)malloc(sizeof(int) * heapSize);
    swapsI = (int *)malloc(sizeof(int) + (4 * heapSize));
    swapsJ = (int *)malloc(sizeof(int) + (4 * heapSize));
    
    for (i = 0; i < heapSize; i++) {
        scanf("%d", &heap[i]);
    }
    
    for (i = ((heapSize - 1) / 2); i >= 0; i--) {
        siftDown(i, heapSize, heap, swapsI, swapsJ, &numSwaps);
    }
    
    printf("%d\n", numSwaps);
    
    for (i = 0; i <  numSwaps; i++) {
        printf("%d %d\n", swapsI[i], swapsJ[i]);
    }
    
    return 0;
}

void siftDown(int i, int heapSize, int * heap, int *swapsI, int * swapsJ, int * numSwaps) {
    //this is sifting only once, need to make it siftDown for each child
    int parentVal, rightIndex, leftIndex, minIndex;
    minIndex = i;
    parentVal = heap[i];
    rightIndex = getRightChild(i);
    leftIndex = getLeftChild(i);
    
    if (leftIndex < heapSize && heap[leftIndex] < heap[minIndex]) {
        minIndex = leftIndex;
    }
    
    if (rightIndex < heapSize && heap[rightIndex] < heap[minIndex]) {
        minIndex = rightIndex;
    }
    
    if (minIndex != i) {
        swapsI[*numSwaps] = i;
        swapsJ[*numSwaps] = minIndex;
        *numSwaps += 1;
        swap(i, minIndex, heap);
        siftDown(minIndex, heapSize, heap, swapsI, swapsJ, numSwaps);
    }
    
}

int getParent(int i) {
    return (i - 1) / 2;
}

int getLeftChild (int i) {
    return (2*i + 1);
}

int getRightChild (int i) {
    return (2*i + 2);
}

void swap (int i, int j, int * heap) {
    int tmp;
    tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

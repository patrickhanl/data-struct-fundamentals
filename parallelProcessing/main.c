//
//  main.c
//  parallelProcessing
//
//  Created by Patrick Hanley on 6/12/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

typedef struct threadNode {
    int threadNum;
    long long int currentJobEndTime;
    
} threadNode;

int getParent(int i);
int getLeftChild (int i);
int getRightChild (int i);
void swap (int i, int j, threadNode * heap);
void siftDown(int i, int heapSize, threadNode * heap);
int min (int firstVal, int secondVal);


#include <stdio.h>
#include <stdlib.h>

int main(void) {
    //freopen("test.txt", "r", stdin);
    int numThreads, numJobs, i;
    long long unsigned int time = 0, scannedTime;
    threadNode * threads;
    scanf("%d%d", &numThreads, &numJobs);
    
    threads = (threadNode *)malloc(sizeof(threadNode) * numThreads);
    
    for (i = 0; i < min(numJobs, numThreads); i++) {
        scanf("%llu", &threads[i].currentJobEndTime);
        threads[i].threadNum = i;
        printf("%d %llu\n", i, time);
    }
    
    for (i = ((numThreads - 1) / 2); i >= 0; i--) {
        siftDown(i, numThreads, threads);
    }
    
    for (i = numThreads; i < numJobs; i++) {
        printf("%d %llu\n", threads[0].threadNum, threads[0].currentJobEndTime);
        scanf("%llu", &scannedTime);
        threads[0].currentJobEndTime += scannedTime;
        siftDown(0, numThreads, threads);
    }
    
    return 0;
}

void siftDown(int i, int heapSize, threadNode * heap) {

    int rightIndex, leftIndex, minIndex;
    long long unsigned int parentVal = heap[i].currentJobEndTime;
    minIndex = i;
    parentVal = heap[i].currentJobEndTime;
    rightIndex = getRightChild(i);
    leftIndex = getLeftChild(i);
    
    if (leftIndex < heapSize) {
        if (heap[leftIndex].currentJobEndTime < heap[minIndex].currentJobEndTime || (heap[leftIndex].currentJobEndTime == heap[minIndex].currentJobEndTime && heap[leftIndex].threadNum < heap[minIndex].threadNum)) {
                minIndex = leftIndex;
        }
    }
    
    if (rightIndex < heapSize) {
        if (heap[rightIndex].currentJobEndTime < heap[minIndex].currentJobEndTime || (heap[rightIndex].currentJobEndTime == heap[minIndex].currentJobEndTime && heap[rightIndex].threadNum < heap[minIndex].threadNum)) {
                minIndex = rightIndex;
        }
    }
    
    if (minIndex != i) {
        swap(i, minIndex, heap);
        siftDown(minIndex, heapSize, heap);
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

void swap (int i, int j, threadNode * heap) {
    threadNode tmp;
    tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

int min (int firstVal, int secondVal) {
    return (firstVal <= secondVal) ? firstVal : secondVal;
}

//
//  main.c
//  networkPackets
//
//  Created by Patrick Hanley on 6/6/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//
// Algorith to process (simulated) network packets using a queue

#include <stdio.h>
#include <stdlib.h>

//struct for each packet, stores each packets arrival time, processing time, and the time it would be finished process
typedef struct packet {
    int arrivalTime;
    int processingTime;
    int endTime;
} packet;


//function definitions
int enqueue (packet * queue, packet newPacket, int * readP, int * writeP, int bufferSize, int time);
void dequeue (packet * queue, int time, int * readP, int * writeP, int bufferSize);

int main(void) {
    int bufferSize, numPackets, read = 0, write = 0, time = 0, i;
    packet * queue;
    
    int *readP = &read;
    int *writeP = &write;
    
    //read in the number of packets that can be stored in the buffer
    scanf("%d", &bufferSize);
    if (bufferSize < 1) {
        return 0;
    }
    
    //number of packets to process
    scanf("%d", &numPackets);
    queue = (packet *)malloc(sizeof(packet) * (bufferSize + 1));
    
    for (i = 0; i < numPackets; i++) {
        packet newPacket;
        scanf("%d%d", &newPacket.arrivalTime, &newPacket.processingTime);
        
        while (newPacket.arrivalTime > time) {
            dequeue(queue, time, readP, writeP, bufferSize);
            time ++;
        }
        
        dequeue(queue, time, readP, writeP, bufferSize);
        
        if (newPacket.processingTime > 0) {
            printf("%d\n", enqueue(queue, newPacket, readP, writeP, bufferSize, time));
        } else {
            if (*writeP + 1 == *readP || (*readP == 0 && *writeP == bufferSize)) {
                printf("-1\n");
            } else {
                printf("%d\n", enqueue(queue, newPacket, readP, writeP, bufferSize, time));
            }
        }
        
    }
    
    return 0;
}

int enqueue (packet * queue, packet newPacket, int * readP, int * writeP, int bufferSize, int time) {
    int readVal = *readP, writeVal = *writeP, startTime;
    if (writeVal + 1 == readVal || (readVal == 0 && writeVal == bufferSize)) {
        return -1;
    } else {
        if (readVal == writeVal) {
            newPacket.endTime = newPacket.processingTime + time;
            startTime = time;
        } else {
            if (writeVal == 0) {
                newPacket.endTime = queue[bufferSize].endTime + newPacket.processingTime;
                startTime = queue[bufferSize].endTime;
            } else {
                startTime = queue[writeVal - 1].endTime;
                newPacket.endTime = queue[writeVal - 1].endTime + newPacket.processingTime;
            }
        }
        queue[writeVal] = newPacket;
        if (writeVal == bufferSize) {writeVal = -1;}
        *writeP = writeVal + 1;
    }
    return startTime;
}

void dequeue (packet * queue, int time, int * readP, int * writeP, int bufferSize) {
    int readVal = *readP, writeVal = *writeP;
    while (readVal != writeVal && queue[readVal].endTime == time) {
        if (readVal == bufferSize) {
            readVal = -1;
        }
        readVal ++;
        *readP = readVal;
    }
}

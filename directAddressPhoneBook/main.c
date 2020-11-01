//
//  main.c
//  directAddressPhoneBook
//
//  Created by Patrick Hanley on 6/16/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record {
    char name[16];
} record;


int main(void) {
    //freopen("input.txt", "r", stdin);
    int numRecords, i, readNumber;
    char command[5], readName[16];
    record * book;
    book = (record *)malloc(sizeof(record) * 10000000);
    
    for (i = 0; i < 10000000; i++) {
        strcpy(book[i].name,"not found");
    }
    
    scanf("%d", &numRecords);
    
    for (i = 0; i < numRecords; i++) {
        scanf("%s", command);
        if (command[0] == 'a') {
            scanf("%d%s", &readNumber, readName);
            strcpy(book[readNumber].name, readName);
        } else if (command[0] == 'd'){
            scanf("%d", &readNumber);
            strcpy(book[readNumber].name, "not found");
        } else {
            scanf("%d", &readNumber);
            printf("%s\n", book[readNumber].name);
        }
    }
    return 0;
}

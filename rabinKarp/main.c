//
//  main.c
//  rabinKarp
//
//  Created by Patrick Hanley on 6/21/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void preComputeHashes (long long unsigned int * hashes, int strLen, int subStringLen, char * string, int x, int p);
long long unsigned int customHash (char * string, int lenString, int x, int p);
int areEqual (char * string1, char * string2, int strLen);

int main(void) {
    //freopen("input.txt", "r", stdin);
    
    char readSubString[500001], readString[500001], * string, * subString, * testSubString;
    
    scanf("%s%s", readSubString, readString);
    
    int subStringLen = strlen(readSubString), strLen = strlen(readString), i, j;
    
    string = (char *)malloc(sizeof(char) * (strLen + 1));
    subString = (char *)malloc(sizeof(char) * (subStringLen + 1));
    testSubString = (char *)malloc(sizeof(char) * (subStringLen + 1));
    
    for (i = 0; i < strLen + 1; i++) {
        string[i] = readString[i];
    }
    
    for (i = 0; i < subStringLen + 1; i++) {
        subString[i] = readSubString[i];
    }
    
    int x, p = 1000000007, * positions;
    long long unsigned int subStringHash, * hashes;
    x = rand() % p + 1;
    positions = (int *)calloc(strLen, sizeof(int));
    
    hashes = (long long unsigned int *)malloc(sizeof(long long unsigned int) * (strLen - subStringLen + 1));
    
    subStringHash = customHash(subString, subStringLen, x, p);
    
    preComputeHashes(hashes, strLen, subStringLen, string, x, p);
    
    for (i = 0; i < strLen - subStringLen + 1; i++) {
        if (subStringHash != hashes[i]) {
            continue;
        }
        for (j = 0; j < subStringLen; j++) {
            testSubString[j] = string[i + j];
        }
        if (areEqual(subString, testSubString, subStringLen)) {
            positions[i] = 1;
        }
    }
    
    for (i = 0; i < strLen; i++) {
        if (positions[i] != 0) {
            printf("%d ", i);
        }
    }
    
    free(string);
    free(subString);
    free(testSubString);
    free(positions);
    
    return 0;
}

void preComputeHashes (long long unsigned int * hashes, int strLen, int subStringLen, char * string, int x, int p) {
    int i, subStringPos = 0;
    long long unsigned int y = 1;
    char lastSubString[subStringLen + 1];
    
    for (i = strLen - subStringLen; i < strLen; i++) {
        lastSubString[subStringPos] = string[i];
        subStringPos++;
    }
    
    hashes[strLen - subStringLen] = customHash(lastSubString, subStringLen, x, p);
    
    for (i = 0; i < subStringLen; i++) {
        y = (((y * x) % p) + p) % p;
    }
    
    for (i = strLen - subStringLen - 1; i > -1; i--) {
        hashes[i] = ((x * hashes[i+1]) + (int)string[i] - (y * (int)string[i+subStringLen])) % p;
    }
}

long long unsigned int customHash (char * string, int lenString, int x, int p) {
    int i;
    long long unsigned int result = 0;

    for (i = lenString - 1; i > -1; i --) {
        result = (((result * x + (int)string[i]) % p) + p) % p;
    }
    //result = result % m;

    //printf("%lld", result);

    return result;
}

int areEqual (char * string1, char * string2, int strLen) {
    int i;
    
    for (i = 0; i < strLen; i ++) {
        if (string1[i] != string2[i]) {
            return 0;
        }
    }
    
    return 1;
}

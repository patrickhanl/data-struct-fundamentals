//
//  main.c
//  brackets
//
//  Created by Patrick Hanley on 6/1/20.
//  Copyright © 2020 Patrick Hanley. All rights reserved.
//
// Algorithm that reads a string and outputs a success message if sting contains a closing brackt for every opening bracket, and the opening and closing bracket types match, strings without brackets also return success message. If there is a mismatch, prints the position of the first unmatched closing brackts, or if there are no unmatched closing brackets the position of the first unmatched opening bracket

#include <stdio.h>
#include <stdlib.h>

//Stack node contains the pointer to the next stack node, the bracket char and the position in the string (1 based)
typedef struct bracketNode {
    struct bracketNode * next;
    char bracket;
    int position;
} bracketNode;

//Function declarations
int isOpening (char testChar);
int isClosing (char testChar);
bracketNode * push (char newNodeKey, bracketNode * oldHead, int position);
bracketNode * pop (bracketNode * oldHead);
char matchingBracket (char closingBracket);

int main(void) {
    int position = 0;
    char currentChar = 'p';
    bracketNode * head = NULL;
    
    //loop reads each character of the string until reaching end or new line
    while (currentChar != '\0' && currentChar != '\n') {
        scanf("%c", &currentChar);
        position ++;
        
        if (isOpening(currentChar)) {
        //if bracket is opening bracket, push onto stack to keep track of most recently seen opening bracket
            head = push(currentChar, head, position);
        //otherwise do nothing unless character is a closing bracket
        } else if (isClosing(currentChar)) {
            //if a closing bracket check to see if it is the same type of bracket as the most recently seen opening bracket
            if (head == NULL || head->bracket != matchingBracket(currentChar)) {
                //if not, or there is not an opening bracket on the stack, there is a mismatch so print the current position and exit algorithm
                printf("%d", position);
                return 0;
            } else {
                //if the brackets match, remove the opening bracket from the stack so we check the previously seen bracket
                head = pop(head);
            }
        }
    }
    
    //if we've read through all the characters in the string and there is nothing on the stack, the brackets are balanced, output success!
    if (head == NULL) {
        printf("Success");
    } else {
        //otherwise, unwind the stack to get the first opening bracket position (this would be unnecessary if we kept track of the stack tail)
        while (head->next != NULL) {
            head = pop(head);
        }
        printf("%d", head->position);
    }

    return 0;
}

// isOpening fuction returns true if character matches one of the opening bracket characters, false otherwise
int isOpening (char testChar) {
    return (testChar == '{' || testChar == '(' || testChar == '[') ? 1 : 0;
}

//isClosing function returns true if character matches one of the closing bracket characters, false otherwise
int isClosing (char testChar) {
    return (testChar == '}' || testChar == ')' || testChar == ']') ? 1 : 0;
}

//removes the node on the top of the stack and returns it
bracketNode * pop (bracketNode * head) {
    bracketNode * newHead = head->next;
    free(head);
    return newHead;
}

//creates a new stack node and adds it to the top of the stack
bracketNode * push (char newNodeKey, bracketNode * oldHead, int position) {
    bracketNode * newNodePointer;
    newNodePointer = (bracketNode *)malloc(sizeof(bracketNode));
    newNodePointer->bracket = newNodeKey;
    newNodePointer->next = oldHead;
    newNodePointer->position = position;
    return newNodePointer;
}

//returns the matching opening bracket for a given closing bracket
char matchingBracket (char closingBracket) {
    if (closingBracket == '}') {
        return '{';
    } else if (closingBracket == ')') {
        return '(';
    } else {
        return '[';
    }
}

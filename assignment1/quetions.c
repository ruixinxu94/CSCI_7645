#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define MAX_MESSAGE_SIZE 100
#define MESSAGE_COUNT 3

void transformArrayOfStrings(
        char messages[MESSAGE_COUNT][MAX_MESSAGE_SIZE],
        int messageCount,
        int (*visitor)(int)) {
    /*
        TODO:
        Iterate through all the messages.
        For each message, apply visitor to each character
        until the end of string character \0 is found.

    */

    for (int i = 0; i < messageCount; i++) {
        for (int j = 0; messages[i][j] != '\0'; j++) {
            messages[i][j] = (char) visitor(messages[i][j]);
        }
    }
}


int main(int argc, char *argv[]) {
    char messages[MESSAGE_COUNT][MAX_MESSAGE_SIZE];
    int index;

    /*
        TODO:
        Handle command line parameters per the given requirements

    */
    if (argc != 5 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: ./assignment1 <mode> <message_1> <message_2> <message_3>");
        exit(EXIT_FAILURE);
    }

    /*
        TODO:
        Populate the messages array from the command line parameters
    */
    for (int i = 0; i < MESSAGE_COUNT; i++) {
        strcpy(messages[i], argv[i + 2]);
    }


    if (strcmp(argv[1], "upper") == 0) {
        transformArrayOfStrings(
                messages,
                MESSAGE_COUNT,
                toupper);
    } else if (strcmp(argv[1], "lower") == 0) {
        transformArrayOfStrings(
                messages,
                MESSAGE_COUNT,
                tolower);
    } else {
        printf("mode must be upper or lower\n");
        exit(EXIT_FAILURE);
    }

    /*
        TODO:
        Print out the transformed messages, one message per line
    */
    for (int i = 0; i < MESSAGE_COUNT; i++) {
        printf("%s\n", messages[i]);
    }

    exit(EXIT_SUCCESS);
}
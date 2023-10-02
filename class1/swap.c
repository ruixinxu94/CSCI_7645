#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char firstName[40];
    char lastName[40];

} Person;

struct Person1{
    char a[10];
};

void testPerson1(struct Person1* a) {
    struct Person1 temp = *a;
    printf("Person1 %s",temp.a);
}
// not only restricted datatype, but also your own datatype
// there are duplicates for swap, how to make it generics
void swapPersons(Person* first, Person* second) {
    Person temp;
    temp = *first;
    *first = *second;
    *second = temp;
}

// void* mean it doesn't know the datatype, the size
void genericSwap(void* firstObject, void* secondObject, size_t objectSize) {
    void* temp = malloc(objectSize);
    if (temp == NULL) {
        printf("failed to exit memory\n");
        exit(EXIT_FAILURE);
    }
    memcpy(temp, firstObject, objectSize);
    memcpy(firstObject, secondObject, objectSize);
    memcpy(secondObject, temp, objectSize);
    free(temp);
}

/**
 * swap 40 50
 * after
 * 50 40
 * @param argc
 * @param argv
 * @return
 */
void swap (int* first, int* second) {
//    int temp = first;
//    first = second;
//    second = temp;
// why it didn't work
// the value of x copied into first, y into second
// these copies swap, but not the origin values
// this is called copied by value
// int * pointer to an integer variable
    int temp = *first;
    *first = *second;
    *second = temp;
}
int main(int argc, char* argv[]) {
    int x, y;

    Person alice, bob;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <first number> <second number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    x = atoi(argv[1]);
    y = atoi(argv[2]);
    printf("before execute x = %d, y = %d\n", x, y);
//    temp = x;
//    x = y;
//    y = temp;
// &x is the address of x
//    swap(&x, &y);
    genericSwap(&x, &y, sizeof(int));
    printf("after execute, x = %d, y = %d\n", x, y);


    strcpy(alice.firstName,"Alice");
    strcpy(alice.lastName,"Admas");

    strcpy(bob.firstName, "Bob");
    strcpy(bob.lastName, "Bergman");

    printf("before swap: alice last name is %s, bob last name is %s\n", alice.lastName, bob.lastName);
//    swapPersons(&alice, &bob);
    genericSwap(&alice, &bob, sizeof(Person));
    printf("after swap: alice last name is %s, bob last name is %s\n", alice.lastName, bob.lastName);

    exit(EXIT_SUCCESS);
}
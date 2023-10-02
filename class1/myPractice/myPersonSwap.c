#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[40];
    int age;
} Person;

void swap(Person* firstP, Person* secondP) {
    Person temp = *firstP;
    *firstP = *secondP;
    *secondP = temp;
}


int main () {
    Person alice, bob;
    strcpy(alice.name, "alice");
    alice.age =  10;
    stpcpy(bob.name, "bob");
    bob.age = 11;
    printf("before swap, alice's age is%d, bob's age is %d\n", alice.age, bob.age);
    swap(&alice, &bob);
    printf("after swap, alice's age is%d, bob's age is %d\n", alice.age, bob.age);

    exit(EXIT_SUCCESS);
}
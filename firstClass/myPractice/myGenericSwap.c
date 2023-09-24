#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char name[40];
    int age;
} Person;

void generalSwap(void* firstObject, void* secondObject, size_t size) {
    void* temp = malloc(size);
    if (temp == NULL) {
        printf("cannot allocate memory");
        exit(EXIT_FAILURE);
    }
    memcpy(temp, firstObject, size);
    memcpy(firstObject, secondObject, size);
    memcpy(secondObject, temp, size);
    free(temp);
}

int main() {
    Person alice, bob;
    int num1 = 0;
    int num2 = 1;
    printf("before swap, num1 is %d, num2 is %d\n", num1, num2);
    generalSwap(&num1,&num2, sizeof(int));
    printf("after swap, num1 is %d, num2 is %d\n", num1, num2);
    alice.age = 10;
    strcpy(alice.name, "alice");
    bob.age = 5;
    stpcpy(bob.name, "bob");
    printf("before swap, alice's age is %d, bob's age is %d\n", alice.age, bob.age);
    generalSwap(&alice, &bob, sizeof(Person));
    printf("after swap, alice's age is %d, bob's age is %d\n", alice.age, bob.age);
    exit(EXIT_SUCCESS);

}
#include <stdio.h>

int main() {
    printf("hello world\n");
    int index;
    // %d is a placeholder
    for (index = 0; index < 10; index++) {
        printf("the number is %d\n", index+1);
    }
}
#include <stdio.h>
#include <string.h>
#include <malloc/_malloc.h>

/**
 * Function Arguments:
When passing strings to functions, you often see char* as the parameter type because arrays decay to pointers when
 passed to functions. This is more flexible because the function can then accept both arrays and pointers.
 * @return
 */
int main() {
    char str1[] = "Hello";  // str1 is an array of characters
    char* str2 = "World";   // str2 is a pointer to a character

    printf("%s %s\n", str1, str2);
    //
    //To break it down:
    //
    //- `str2` is a variable of type `char*`, which means it's a pointer to a character.
    //- The value stored in `str2` is the address of the first character ('W') of the string "World".
    //- The string "World" itself (i.e., the sequence of characters 'W', 'o', 'r', 'l', 'd', and '\0') is typically
    // stored in a read-only section of memory when it's a string literal.
    //- When you use `str2` in a context where a `char*` is expected (like with `printf` using `%s`), it starts
    // reading characters from that address until it encounters the null terminator (`'\0'`), which signifies the
    // end of the string.
    //
    //So, in essence, `str2` points to the beginning of the string "World", and by following that pointer
    // (and subsequent memory locations), you can access the entire string.
    printf("Value of str2 (address of \"World\"): %p\n", (char*)str2);
    printf("Address of str2 itself: %p\n", (char*)&str2);
    str2 = "changed";
    printf("After changed, Value of str2 (address of \"changed\"): %p\n", (char*)str2);
    // the value of str2 stays unchanged
    printf("After changed, Address of str2 itself: %p\n", (char*)&str2);

    str1[0] = 'w';
    printf("After changed, str1 is: %s\n", str1);


    // str2[0] = 'w';If you had done the above modification with a pointer to a string literal,
    // it would have been undefined behavior:

    char* str3 = malloc(4);  // Allocate memory for 5 characters + null terminator
    if (str3) {
        strcpy(str3, "man");
        str3[0] = 'w';  // This is fine since we've allocated memory
        // ... use the string ...
    }
    printf("str3 is %s\n", str3);
    free(str3);




    return 0;
}

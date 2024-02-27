
// ex2.c:
#include <stdio.h>
#include <unistd.h>
int main()
{
    char a = 46;
    char *ptr;
    ptr = &a;
    *ptr = 47;
    a = 48;
    printf("%d, %d\n", *ptr, a);
    a = '0';
    printf("%d, %c, %x\n", a, a, a);
    return (0);
}
/*
    The given C code snippet manipulates a character variable a and a pointer ptr. Let’s break down the steps:

char a = 46;: Initializes a with the ASCII value of the character '.', which is 46.
char *ptr;: Declares a pointer ptr to a character.
ptr = &a;: Assigns the address of a to ptr.
*ptr = 47;: Changes the value at the memory location pointed to by ptr to 47 (ASCII value of '/').
a = 48;: Updates the value of a to 48 (ASCII value of '0').
printf("%d, %d\n", *ptr, a);: Prints the values pointed to by ptr and the updated value of a. The output will be 47, 48.
a = '0';: Sets a to the character '0'.
printf("%d, %c, %x\n", a, a, a);: Prints the ASCII value, character, and hexadecimal value of a. The output will be 48, 0, 30.
In summary:

The value of a changes from '.' to '0'.
The value pointed to by ptr changes from '.' to '/'.
*/
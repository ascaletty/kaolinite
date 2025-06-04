#include <stdio.h>
#include <string.h>

int main() {
    char s1[] = "Hello, Geeks!";
  
    // Ensure destination has enough space
    char s2[50];  

    // Copying string using strcpy
    strcpy(s2, s1);

    printf("%s", s2);
    return 0;
}

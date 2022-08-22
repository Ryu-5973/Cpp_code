#include <stdio.h>

#include <stdlib.h>


int main() {
    char *p1 = (char*)malloc(40);
    char *p2 = (char*)malloc(50);
    char *p3 = (char*)malloc(60);
    // char *p3 = (char*)malloc(100);
    char *p4 = (char*)malloc(70);
    printf("%lu\n", (unsigned long)p1);
    printf("%lu\n", (unsigned long)p2);
    printf("%lu\n", (unsigned long)(p2-p1));
    printf("%lu\n", (unsigned long)p3);
    printf("%lu\n", (unsigned long)p4);
    printf("%lu\n", (unsigned long)(p4-p3));
    return 0;
}
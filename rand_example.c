#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int i;
    printf("RAND_MAX равно %u\n", RAND_MAX);
    srand(time(0));

    printf("случайные значения от 0 до RANDOM_MAX\n");
    for(i=0; i < 8; i++)
        printf("%d\n", rand());
    printf("случайные значения от 1 до 20\n");
    for (i = 0; i < 8; i++){
        printf("%d\n", (rand()%20)+1);
    }   
}
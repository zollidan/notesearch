#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void fatal(const char *fmt, ...) {
    char error_message[256];
    va_list args;

    strcpy(error_message, "[!!] Критическая ошибка: ");

    va_start(args, fmt);
    vsnprintf(error_message + strlen(error_message),
              sizeof(error_message) - strlen(error_message),
              fmt,
              args);
    va_end(args);

    perror(error_message);
    exit(EXIT_FAILURE);
}
void *ec_malloc(unsigned int size) {
    void *ptr;
    ptr = malloc(size);
    if(ptr == NULL) {
        fatal("в функции ec_malloc() при выделении памяти" );
    }
    return ptr;
}

void usage(char *prog_name, char *filename) {
    printf("Usage: %s <data to add to %s >\n ", prog_name, filename);
    exit(0);
}

void debug(char *name, void *addr, char *value) {
    printf("[DEBUG] %s @ %p: '%s'\n", name, addr, value);
}
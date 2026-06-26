void fatal(char *message){
    char error_message [100]; 
    strcpy(error_message, "[!!]Критическая ошибка");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
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
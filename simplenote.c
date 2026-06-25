#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
void usage(char *prog_name, char *filename) {
    printf("Usage: %s <data to add to %s >\n ", prog_name, filename);
    exit(0);
}

void debug(char *name, void *addr, char *value) {
    printf("[DEBUG] %s @ %p: '%s'\n", name, addr, value);
}

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

int main(int argc, char *argv[]){
    
    int fd; // дескриптор файла
    char * buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);

    strcpy(datafile, "tmp/notes");

    if (argc < 2)
        usage(argv[0], datafile);

    strcpy(buffer, argv[1]);

    debug("buffer", buffer, buffer);
    debug("datafile", datafile, datafile);

    strncat(buffer, "\n", 1);

    // открываю файл 
    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if (fd == -1){
        fatal("ошибка при открытии файла в дескриптор");
    }

    //запись данных
    if(write(fd, buffer, strlen(buffer)) == -1 ){
        fatal("ошибка записи данных в дескриптор");
    }

    // закрываю файл
    if (close(fd) == -1){
        fatal("ошибка при закрытии файла");
    }

    printf("заметка сохранена.\n");
    free(buffer);
    free(datafile);

    return 0;
}
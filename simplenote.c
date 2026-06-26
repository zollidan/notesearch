#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "hacking.h"

#define FILENAME "/var/notes"

int print_notes(int, int, char *);
int find_user_note(int, int);
void fatal(char *);
void *ec_malloc(unsigned int);

int main(int argc, char *argv[]){
    int fd, userId; // дескриптор файла и userId
    char * buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);

    strcpy(datafile, "var/notes");

    if (argc < 2)
        usage(argv[0], datafile);

    strcpy(buffer, argv[1]);

    debug("buffer", buffer, buffer);
    debug("datafile", datafile, datafile);

    // открываю файл 
    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if (fd == -1){
        fatal("ошибка при открытии файла в дескриптор");
    }
    printf("[DEBUG]: дескриптор файла: %d\n", fd);

    userId = getuid();

    if(write(fd, &userId, 4) == -1)
        fatal("в функции main() при записи userId");
    write(fd, "\n", 1);
    
    //запись данных
    if(write(fd, buffer, strlen(buffer)) == -1 )
        fatal("в функции main() при записи буфера в дескриптор");
    write(fd, "\n", 1);

    // закрываю файл
    if (close(fd) == -1){
        fatal("ошибка при закрытии файла");
    }

    printf("заметка сохранена.\n");
    free(buffer);
    free(datafile);

    return 0;
}
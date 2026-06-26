#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

#define FILENAME "var/notes"

int print_notes(int, int, char *); // вывод заметов в консоль
int find_user_note(int, int); // поиск в файле заметок пользователя
int search_note(char *, char *); // поиск по ключевым словам
void fatal(char *); // обработка критических ошибок

int main(int argc, char const *argv[])
{
    int userId, printing = 1, fd;
    char searchstring[100];

    if(argc > 1){
        strcpy(searchstring, argv[1]);
    }
    else
        searchstring[0] = 0;

    userId = getuid();

    fd = open(FILENAME, O_RDONLY);
    if(fd == -1)
        fatal("в функции main ошибка открытия дескриптора");

    while(printing)
        printing = print_notes(fd, userId, searchstring);
    printf("------[конец данных по заметке]------\n");
    close(fd);

    return 0;
}

int print_notes(int fd, int uid, char *searchstring){
    int note_length;
    char byte=0, note_buffer[100];

    note_length = find_user_note(fd, uid);

    if(note_length == -1)
        return 0;
    
    read(fd, note_buffer, note_length);
    note_buffer[note_length] = 0;

    if(search_note(note_buffer, searchstring))
        printf(note_buffer);

    return 1;
}

// Функция поиска следующей заметки для указаного userId
// возвращает -1 при достижении конца файла 
// в противном случае возвращает длину найденой заметки
int find_user_note(int fd, int user_uid){
    int note_uid = -1;
    unsigned char byte;
    int length;
    while(note_uid != user_uid) {
        if(read(fd, &note_uid, 4) != 4) // читаю uid пользователя
            return -1;

        if(read(fd, &byte, 1) != 1) // читаю символ перевода строки
            return -1;
        
        byte = length = 0;
        while(byte != '\n') { // смотрим сколько до конца строки
            if(read(fd, &byte, 1) != 1) // читаем один байт 
                return -1; 
            length++;
        }
    }
    lseek(fd, length * -1, SEEK_CUR); // смещаем позицию чтения на length байтов

    printf("[DEBUG] обнаружена заметка длиной %d байтов для id %d\n", length, note_uid);

    return length;
}

// Функция поиска заметки по ключевому слову
// возвращает 1 при обнаружении совпадений и 0, если их нет
int search_note(char *note, char *keyword) {
    int i, keyword_length, match=0;

    keyword_length = strlen(keyword);
    if(keyword_length == 0) // если поисковой строки нет 
        return 1; // всегда совпадение
    
    for(i=0; i<strlen(note); i++) { // побайтовый просмотр заметки
        if(note[i] == keyword[i]){
            match++;
        }
        else{
            if(note[i] == keyword[0]){
                match = 1;
            }else{
                match = 0;
            }
        }
        if (match == keyword_length)
            return -1;
    }

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "hacking.h"

#define DATAFILE "var/chance.data" // файл данных пользователя

struct user {
    int uid;
    int credits;
    int highscore;
    char name[100];
    int (*current_game) ();
};

int get_player_data();
void register_new_player();
void input_name();

struct user player;

int main() {
    int choice, last_game;

    srand(time(0)); // передача в генератор текущее время как начальное значение

    if(get_player_data() == -1) // пытаемся читать данные игрока из файла
        register_new_player(); // если данных нет запуск регистрации

    
}

int get_player_data() {
    int fd, uid, read_bytes;
    struct user entry;

    uid = getuid();

    fd = open(DATAFILE, O_RDONLY);
    if(fd == -1)
        return -1;

    read_bytes = read(fd, &entry, sizeof(struct user)); // читаю первый блок
    while(entry.uid != uid && read_bytes > 0) { // повтор пока не найду нужный uid
        read_bytes = read(fd, &entry, sizeof(struct user)); // продолжение чтения 
    }

    close(fd);
    if(read_bytes < sizeof(struct user))
        return -1;
    else
        player = entry;
    return 1;
}

void register_new_player() {
    int fd;

    printf("-=-={ Регистрация нового игрока }=-=-\n");
    printf("Введите свое имя: ");
    input_name();

    player.uid = getuid();
    player.highscore = player.credits = 100;

    fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if(fd == -1) 
        fatal("ошибка в функции register_new_player при создании нового пользователя");
    write(fd, &player, sizeof(struct user));
    close(fd);

    printf("Добро пожаловать в игру %s.\n", player.name);
    printf("Вам выдано %u кредитов.\n", player.credits);
}

void input_name(){
    char *name_ptr, input_char='\n';
    while(input_char == '\n')
        scanf("%c", &input_char);

    name_ptr = (char *) &(player.name);
    while(input_char != '\n') {
        *name_ptr = input_char;
        scanf("%c", &input_char);
        name_ptr++;
    }
    *name_ptr = 0;
}
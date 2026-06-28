#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "hacking.h"

#define DATAFILE "var/chance.data" // файл данных пользователя

// 120 bytes user
struct user {
    int uid;                 // 4 bytes
    int credits;             // 4 bytes
    int highscore;           // 4 bytes
    char name[100];          // 100 bytes
    int (*current_game)();   // 8 bytes
};

int get_player_data();
void register_new_player();
void input_name();
int take_wager(int, int);
void play_the_game();
void pick_a_number();
void dealer_no_match();
void find_the_ace();


struct user player;

int main() {
    int choice, last_game;

    srand(time(0)); // передача в генератор текущее время как начальное значение

    if(get_player_data() == -1) // пытаемся читать данные игрока из файла
        register_new_player(); // если данных нет запуск регистрации


    while(choice != 7) {
        printf("-=[ Меню игр ]=-\n");
        printf("1 - Игра Угадай число\n");
        printf("2 - Игра Без совпадений\n");
        printf("3 - Игра Найди туза\n");
        printf("4 - Текущий рекорд\n");
        printf("5 - Сменить пользователя\n");
        printf("6 - Вернуть учетную запись к 100 кредитам\n");
        printf("7 - Выход\n");

        printf("[Имя: %s]\n", player.name);
        printf("[У вас %u кредитов] -> ", player.credits);

        scanf("%d", &choice);

        if((choice < 1) || (choice > 7)) {
            printf("\n[!!] Число %d недопустимо.\n\n", choice);
        } else if(choice < 4) {
            if(choice != last_game) {
                if(choice == 1) {
                    player.current_game = pick_a_number;
                }
                else if(choice == 2) {
                    player.current_game = dealer_no_match;
                } 
                else {
                    player.current_game = find_the_ace;
                }

                last_game = choice;
            }
            play_the_game();
        } else if(choice == 4) {
            show_highscore();
        } else if(choice == 5){
            printf("\nДругой пользователь\n");
            printf("Укажите новое имя: ");
            input_name();
        } else if(choice == 6){
            printf("\nВаше счет возвращен к 100 кредитам. \n\n");
            player.credits = 100;
        }
    }
    update_player_data();
    printf("Спасибо за игру! Пока.\n");
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

void update_player_data() {
    int fd, i, read_uid;
    char burned_byte;

    fd = open(DATAFILE, O_RDWR);
    if(fd == -1) {
        fatal("ошибка при открытии fd в %s", __FUNCTION__);
    }
    read(fd, &read_uid, 4); // читаю uid из первой строки
    while(read_uid != player.uid) { // читаю пока не найду нужный id
        for (i = 0; i < sizeof(struct user) - 4; i++)
        {
            read(fd, &burned_byte, 1);
        }

        read(fd, &read_uid, 4);
    }
    write(fd, &(player.credits), 4);
    write(fd, &(player.highscore), 4);
    write(fd, &(player.name), 100);
    close(fd);
}

// отображение рекордов и имени установившего его человека
void show_highscore() {
    unsigned int top_score = 0;
    char top_name[100];
    struct user entry;
    int fd;

    printf("\n=================| РЕКОРДЫ |=================\n");
    fd = open(DATAFILE, O_RDONLY);
    if(fd == -1){
        fatal("ошибка при открытии fd в %s", __FUNCTION__);
    }
    while(read(fd, &entry, sizeof(struct user)) > 0) {
        if(entry.highscore > top_score) { // если есть результат лучше  
            top_score = entry.highscore; // то берем его в top_score
            strcpy(top_name, entry.name); 
        }
    }
    close(fd);
    if(top_score > player.highscore)
        printf("%s установил рекорд %u\n", top_name, top_score);
    else
        printf("Сейчас у вас рекорд %u кредитов!\n", player.highscore);
    printf("============================================================");
}

void jackpot() {
    printf("+=+=+=++=+=+=++=+=+=+ ДЖЕКПОТ +=+=+=++=+=+=++=+=+=+");
    printf("Вы выиграли джекпот 100 кредитов!");
    player.credits += 100;
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

void print_cards(char *messages, char *cards, int user_pick) {
    int i;

    printf("\n\t*** %s ***\n", messages);
    printf("    \t._.\t._.\t._.\n");

    
    if(user_pick == -1) {
        print(" 1 \t 2\t 3\t\n");
    }else {
        for (i = 0; i < user_pick; i++)
            printf("\t");        
        printf(" вы выбрали\n");
    }
}

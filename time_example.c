#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    
    long int seconds_since_epoch;
    struct tm current_time, *time_ptr;
    int hour, minute, second, day, month, year;

    seconds_since_epoch = time(0); // передаю 0 указатель 

    printf("time() секунд с начала эры: %ld\n", seconds_since_epoch);

    time_ptr = &current_time; // установка time_ptr на адрес структуры current_time

    localtime_r(&seconds_since_epoch, time_ptr);

    hour = current_time.tm_hour; // прямой доступ 
    minute = time_ptr->tm_min; // доступ по указателю
    second = *((int *) time_ptr);

    printf("Текущее время: %02d:%02d:%02d\n", hour, minute, second);

    return 0;
}

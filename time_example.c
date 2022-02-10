// 構造体の使用例
#include <stdio.h>
#include <time.h>

/*
/usr/include/time.h
struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
}
*/

int main() {
    long int seconds_since_epoch;
    struct tm current_time, *time_ptr;
    int hour, minute, second, day, month, year;

    seconds_since_epoch = time(0);
    printf("time() - total seconds from epoch: %ld\n", seconds_since_epoch);

    time_ptr = &current_time; // time_ptrにcurrent_time構造体の先頭アドレスを設定する。
    localtime_r(&seconds_since_epoch, time_ptr); // seconds_since_epochの情報をcurrent_timeへ

    hour = current_time.tm_hour; // 直接アクセス
    minute = time_ptr->tm_min;   // ポインタ経由
    second = *((int *) time_ptr);// 整数へのポインタにキャストして参照外し　該当ポインタのアドレス上のデータ（tm構造体最初の要素）を取得

    printf("current time: %02d:%02d:%02d\n", hour, minute, second);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"
#define FILENAME "/var/notes"

int print_notes(int, int, char *); //メモ出力
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[]) {
    int userid, printing=1, fd;
    char searchstring[100];

    if(argc > 1) strcpy(searchstring, argv[1]);
    else searchstring[0] = 0;
    
    userid = getuid(); // 実ユーザID
    fd = open(FILENAME, O_RDONLY); // read only
    if(fd == -1) fatal("An error occurred while reading file in main.");
    
    while(printing) printing = print_notes(fd, userid, searchstring);
    printf("-------[end of memo]-------\n");
    close(fd);
}

// 特定ユーザの（検索文字列を含む）メモを1行表示する
int print_notes(int fd, int uid, char *searchstring) {
    int note_length;
    char byte=0, note_buffer[100];

    note_length = find_user_note(fd, uid);
    if(note_length == -1) return 0; // メモが無ければ0

    read(fd, note_buffer, note_length);
    note_buffer[note_length] = 0;

    if(search_note(note_buffer, searchstring)) printf(note_buffer);
    return 1; // 特定ユーザのメモがあれば1
}

// 特定uidのメモを検索し長さを返す
int find_user_note(int fd, int user_uid) {
    int note_uid=-1;
    unsigned char byte;
    int length;

    while(note_uid != user_uid) {
        if(read(fd, &note_uid, 4) != 4) return -1; // 4byte読み込みnote_uidに代入
        if(read(fd, &byte, 1) != 1) return -1; // 1byte読み込み\nをbyteに代入

        byte = length = 0;
        while(byte != '\n') { // 行末まで
            if(read(fd, &byte, 1) != 1) return -1; // 1byteずつ読み込む
            length++;
        }
    }
    lseek(fd, length * -1L, SEEK_CUR); // 読み込み位置を(long型)-length変化させる

    printf("[DEBUG] uid %dの%dバイトのメモを見つけました。\n", note_uid, length);
    return length;
}

// 検索文字列
int search_note(char *note, char *keyword) {
    int i, keyword_length, match=0;

    keyword_length = strlen(keyword);
    if(keyword_length == 0) return 1; // 検索文字列がないときは成功扱い

    for(i=0; i<strlen(note); i++) {
        if(note[i] == keyword[match]) match++;
        else {
            if(note[i] == keyword[0]) match = 1;
            else match = 0;
        }
        if(match == keyword_length) return 1;
    }
    return 0;
}
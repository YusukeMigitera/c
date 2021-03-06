#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

void usage(char *prog_name, char *filename) {
    printf("使用方法: %s <%sに追加するデータ>\n", prog_name, filename);
    exit(0);
}

void fatal(char *);
void *ec_malloc(unsigned int); // error checked malloc

int main(int argc, char *argv[]) { // argc引数の数, argv[0] ./a.out, argv[1] 第一引数
    int userid, fd; //ファイル記述子
    char *buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "/var/notes");

    if(argc < 2) usage(argv[0], datafile);

    strcpy(buffer, argv[1]);

    printf("[DEBUG]buffer   @ %p: \'%s\'\n", buffer, buffer);
    printf("[DEBUG]datafile @ %p: \'%s\'\n", datafile, datafile);

    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if(fd == -1) fatal("An error occurred while opening file in main.");
    printf("[DEBUG]file descriptor: %d\n", fd);
    userid = getuid(); // 実ユーザID

    if(write(fd, &userid, 4) == -1) fatal("An error occurred while writing user ID to file in main.");
    write(fd, "\n", 1);
    if(write(fd, buffer, strlen(buffer)) == -1) fatal("An error occurred while writing buffer to file in main.");
    write(fd, "\n", 1);

    if(close(fd) == -1) fatal("An error occurred while closing file in main.");

    printf("メモが保存されました。\n");
    free(buffer);
    free(datafile);
}
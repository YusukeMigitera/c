#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void usage(char *prog_name, char *filename) {
    printf("使用方法: %s <%sに追加するデータ>\n", prog_name, filename);
    exit(0);
}

void fatal(char *);
void *ec_malloc(unsigned int); // error checked malloc

int main(int argc, char *argv[]) { // argc引数の数, argv[0] ./a.out, argv[1] 第一引数
    int fd; //ファイル記述子
    char *buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "/tmp/notes");

    if(argc < 2) usage(argv[0], datafile);

    strcpy(buffer, argv[1]);

    printf("[DEBUG]buffer   @ %p: \'%s\'\n", buffer, buffer);
    printf("[DEBUG]datafile @ %p: \'%s\'\n", datafile, datafile);

    strncat(buffer, "\n", 1); // 行末に改行を追加

    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if(fd == -1) fatal("An error occurred while opening file in main.");
    printf("[DEBUG]file descriptor: %d\n", fd);

    if(write(fd, buffer, strlen(buffer)) == -1) fatal("An error occurred while writing buffer to file in main.");

    if(close(fd) == -1) fatal("An error occurred while closing file in main.");

    printf("\n");
    free(buffer);
    free(datafile);
}

void fatal(char *message) {
    char error_message[100];

    strcpy(error_message, "[!!] fatal error:");
    strncat(error_message, message, 79);
    perror(error_message); // print error
    exit(-1);
}

void *ec_malloc(unsigned int size) {
    void *ptr;
    ptr = malloc(size);
    if(ptr == NULL) fatal("An error occurred while allocating memory in ec_malloc().");
    return ptr;
}
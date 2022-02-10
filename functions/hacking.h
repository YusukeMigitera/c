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

// 生のメモリを1バイトずつ16進数でダンプする
void dump(const unsigned char *data_buffer, const unsigned int length) {
    unsigned char byte;
    unsigned int i, j;
    for(i=0; i<length; i++) {
        byte = data_buffer[i];
        printf("%02x ", data_buffer[i]); // バイトを16進数で表示する
        if(((i%16)==15) || (i==length-1)) {
            for(j=0; j<15-(i%16); j++) printf("   ");
            printf("| ");
            for(j=(i-(i%16)); j<=i; j++) { // 行内の印字可能なバイトを表示する
                byte = data_buffer[j];
                if((byte > 31) && (byte < 127)) printf("%c", byte); // 印字可能な文字の範囲にない場合
                else printf(".");
            }
            printf("\n"); // 行(16バイト)の終了
        }
    }
}
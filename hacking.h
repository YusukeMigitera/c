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
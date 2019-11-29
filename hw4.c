#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Only one arg is accepted.
    if(argc != 2){
        fprintf(stderr, "Please only provide one argument.\n");
        exit(1);
    }

    FILE *file_stream;
    file_stream = fopen(argv[1], "r");
    if(file_stream == NULL){
        fprintf(stderr, "The file does not exist.\n");
        exit(1);
    }
}




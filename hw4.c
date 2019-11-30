#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define _GNU_SOURCE
#include <string.h>

int field_count;
char** fields;
int name_index;

// invalid exit
void exit_invalid(FILE* stream){
    if(stream != NULL){
        fclose(stream);
    }

    fprintf(stderr, "The file is not valid.\n");
    exit(1);
}

bool header_validator(char* header_line){
    char *token = strtok (header_line,",");
    field_count = 0;
    int name_count = 0;
    while(token != NULL){
        // printf("%s\n", token);
        if(strcmp(token,"name") == 0 || 
            strcmp(token,"\"name\"") == 0){
            name_count ++;
            name_index = field_count;
        }
        token = strtok(NULL, ",");
        field_count ++;
    }

    if(name_count != 1){
        return false;
    }
    return true;
}

void fields_creation(char* header_line){
    char *token = strtok (header_line,",");
    fields = malloc(field_count * sizeof(char*));
    int index = 0;
    // printf("%s\n", header_line);
    while(token != NULL){
        // printf("gONe");
        if(index != field_count - 1){
            fields[index] = malloc(strlen(token));
            strcpy(fields[index], token);
        }
        else{
            fields[index] = malloc(strlen(token) - 2);
            strncpy(fields[index], token, strlen(token) - 2);
        }
        index ++;
        token = strtok(NULL, ",");
    }
    // printf("%s\n", (char*)fields[15]);
}

int main(int argc, char *argv[]) {
    // Only one arg is accepted.
    if(argc != 2){
        fprintf(stderr, "Please only provide one argument.\n");
        exit(1);
    }

    // Open the file with fopen().
    FILE *file_stream;
    file_stream = fopen(argv[1], "r");
    ssize_t read_bytes;
    size_t len = 0;
    char *header_line;

    // Check if the file exist or not.
    if(file_stream == NULL){
        exit_invalid(file_stream);
    }

    // Go to the end of file.
    fseek(file_stream, 0, SEEK_END);
    // Get the postion of offset which is the same as file size.
    if(ftell(file_stream) == 0){
        exit_invalid(file_stream);
    }

    // Restore the offset to the begining.
    fseek(file_stream, 0, SEEK_SET);

    // Take the header and check if it is valid.
    if((read_bytes = getline(&header_line, &len, file_stream)) != -1){
        if(read_bytes > 1024){
            exit_invalid(file_stream);
        }
        char* temp = malloc(strlen(header_line));
        strcpy(temp, header_line);

        if(!header_validator(temp)){
            exit_invalid(file_stream);
        }

        free(temp);
    }
    else{
        exit_invalid(file_stream);
    }

    fields_creation(header_line);

    for(int i; i < field_count; i ++){
        printf("%s\n", fields[i]);
    }
    // printf("%s\n", fields[7]);
    // printf("%s\n", fields[15]);
    // printf("%d\n", name_index);
    // // printf("%d\n", strcmp(fields[15],"name"));
    if(strcmp(fields[7],"name") == 0){
        printf("7 WIN\n");
    }

    if(strcmp(fields[15],"name") == 0){
        printf("15 WIN\n");
    }


    free(header_line);
}
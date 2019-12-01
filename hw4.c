#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define _GNU_SOURCE
#include <string.h>

int field_count;
char** fields;
int name_index;

struct count_node
{
    char* name;
    int number;
    struct count_node * next;
};


// Problem: \0 strlen?

// invalid exit
void exit_invalid(FILE* stream){
    if(stream != NULL){
        fclose(stream);
    }

    fprintf(stderr, "The file is not valid.\n");
    exit(1);
}

void fields_creation(char* header_line){
    int comma_count = 0;
    for(int i = 0; i < strlen(header_line); i ++){
        if(header_line[i] == ','){
            comma_count ++;
        }
    }

    field_count = comma_count + 1;
    fields = malloc(field_count * sizeof(char*));
    int index = 0;
    int prev = 0;
    for(int i = 0; i < strlen(header_line); i ++){
        if(header_line[i] == ','){
            fields[index] = malloc(i - prev + 1);
            memcpy(fields[index], &header_line[prev], i - prev + 1);
            fields[index][i - prev] = '\0';
            index ++;
            prev = i + 1;
        }
    }

    // The last field
    fields[index] = malloc(strlen(header_line) - prev + 1);
    memcpy(fields[index], &header_line[prev], strlen(header_line) - prev + 1);
    fields[index][strlen(header_line) - prev] = '\0';
}

// validate the content and store the name and tweets count
bool content_validator_collector(char* line){
    int comma_count = 0;
    for(int i = 0; i < strlen(line); i ++){
        if(line[i] == ','){
            comma_count ++;
        }
    }

    int item_count = comma_count + 1;
    if(item_count != field_count){
        return false;
    }
    char** items = malloc(item_count * sizeof(char*));
    int index = 0;
    int prev = 0;
    for(int i = 0; i < strlen(line); i ++){
        if(line[i] == ','){
            items[index] = malloc(i - prev + 1);
            memcpy(items[index], &line[prev], i - prev + 1);
            items[index][i - prev] = '\0';
            index ++;
            prev = i + 1;
        }
    }

    // The last field
    items[index] = malloc(strlen(line) - prev + 1);
    memcpy(items[index], &line[prev], strlen(line) - prev + 1);
    items[index][strlen(line) - prev] = '\0';

    for(int i = 0; i < field_count; i ++){
        int length = strlen(items[i]);
        if (items[i][length - 1] == '\"' ^ items[i][0] == '\"'){
            return false;
        }
    }


    for(int i = 0; i < field_count; i ++){
        if(fields[i][0] == '\"'){
            if(!(items[i][0] == '\"')){
                return false;
            }
        }
        else{
            if(!(items[i][0] != '\"')){
                return false;
            }
        }
    }

    // Store the name and add the count


    // Free the memory
    for(int i = 0; i < field_count; i ++){
        free(items[i]);
    }

    free(items);
    return true;
}

bool fields_validator(){
    int name_count = 0;
    for(int i = 0; i < field_count; i ++){
        // printf("%s\n", fields[i]);
        int length = strlen(fields[i]);
        if (fields[i][length - 1] == '\"' ^ fields[i][0] == '\"'){
            return false;
        }

        if(strcmp(fields[i], "name") == 0 || strcmp(fields[i], "\"name\"") == 0){
            name_count ++;
            // Get the name index
            name_index = i;
        }
    }

    if(name_count != 1){
        return false;
    }

    return true;
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
    char *line;

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
    if((read_bytes = getline(&line, &len, file_stream)) != -1){
        if(read_bytes > 1024){
            exit_invalid(file_stream);
        }

        char* temp = malloc(strlen(line));
        strcpy(temp, line);
        strtok(temp, "\r\n");
        strtok(NULL, "\n");

        // Empty line: what to do? invalid?
        if(strcmp(line, "") == 0){
            exit_invalid(file_stream);
        }

        fields_creation(temp);
        free(temp);
    }
    else{
        exit_invalid(file_stream);
    }

    if(!fields_validator()){
        exit_invalid(file_stream);
    }

    int line_number = 1;
    while((read_bytes = getline(&line, &len, file_stream)) != -1){
        if(read_bytes > 1024){
            exit_invalid(file_stream);
        }

        char* temp = malloc(strlen(line));
        strcpy(temp, line);
        strtok(temp, "\r\n");
        strtok(NULL, "\n");

        // Empty line: what to do? invalid?
        if(strcmp(line, "") == 0){
            exit_invalid(file_stream);
        }

        if(!content_validator_collector(temp)){
            exit_invalid(file_stream);
        }

        free(temp);
    }
}
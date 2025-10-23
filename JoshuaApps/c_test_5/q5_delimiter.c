#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


void check_digit(char* token)
{
    int is_num = 1;
    int value = 0;
    int i = 0;
    
    for(i = 0; token[i] != '\0'; i++){
        if(!isdigit(token[i])){
            is_num = 0;
            break;
        }
    }

    if(is_num){
        value = atoi(token);
        printf("Digit: %d\n", value);
    }else{
        printf("Char: %s\n", token);
    }
}

void method_1(char *str, int length)
{
    int i = 0;
    int value = 0;
    char *buffer = malloc(length + 1);
    char *token = NULL;

    if(!buffer){
        perror("fail to allocate memory\n exit\n");
        exit(1);
    }

    strcpy(buffer, str);
    token = strtok(buffer, ",");

    while (token != NULL) {
        //delete first space
        while (*token == ' ') {
            token++;
        }

        //delete space after comma
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') {
            *end = '\0';
            end--;
        }

        check_digit(token);
        token = strtok(NULL, ",");
    }

    free(buffer);
}

void method_2(char *str, int length)
{
    int i = 0;
    char *start = NULL;
    char *end = NULL;
    char *buffer = malloc(length + 1);
    if(!buffer){
        perror("fail to allocate memory\n exit\n");
        exit(1);
    }

    strncpy(buffer, str, length);
    buffer[length] = '\0';  // ensure null-termination

    start = buffer;

    for(i = 0; i <= length; i++){
        if(buffer[i] == ',' || buffer[i] == '\0'){
            buffer[i] = '\0';
            char *parse = start;
            while(*parse == ' '){
                parse++;
            }

            end = parse + strlen(parse) - 1;
            while (end > parse && *end == ' ') {
                *end = '\0';
                end--;
            }

            check_digit(parse);
            start = buffer + i + 1;
        }
    }    

    free(buffer);

}

int main() 
{
    char str[] = "Embedded, Linux, Software, Engineer, 2025, 06, 02";
    // char *token = strtok(str, ",");
    int len =  strlen(str); 

    method_1(str, len);
    method_2(str, len);

    return 0;
}
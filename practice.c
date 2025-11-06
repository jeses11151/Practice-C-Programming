#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void save_to_buffer(const char *source)
{
    int len = 0;
    char *dest_space = NULL;

    len = strlen(source);
    dest_space = malloc(len + 1);
    if(dest_space == NULL){
        perror("failed to allocate memory");
        exit(1);
    }

    strcpy(destination, source)
}

void save_to_file()
{
    FILE *fp = fopen(filename, "w");
    if (!fp){
        perror("Failed to open file");
        exit(1);
    }

    fputs(data, fp);
    fputs("\n", fp);

    fclose(fp);
}

void reverse_index_swap(const char *original, char *output)
{
    int len = strlen(original);
    int i = 0;

    for (i = 0; i < len; i++){
        output[i] = original[len - 1 - i];
    }

    output[len] = '\0';
}

void reverse_string(int method, const char *original, char *output)
{
    const char *end = original + strlen(original) - 1;
    char *out = output;

    while (end >= original) {
        *out++ = *end--;
    }

    *out = '\0';
}


void foo(){
    int arr[] = {5, 2, 9, 1, 3};
    int size = sizeof(arr) / sizeof(arr[0]);
    int temp = 0;
    int i = 0; 
    int j = 0;

    for(i=0; i<size-1; i++){
        for(j=0; j < size-1-i; j++){
            if(arr[j] > arr[j+1]){  //ascending
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1]=temp;
            }
        }

    }

    for(i = 0; i < size -1; i++){
        for(j=0; j < size - 1 - i; j++){
            if(arr[j] < arr[j+1]){  //descending
                temp = arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }

    int largest = arr[n-1]; //set largest to 9
    int second_largest = -1;

    for(i = n-2; i >= 0; i--){
        if(arr[i] != largest){  //if arr[n-2] = 9, continue loop
            second_largest = arr[i];
            break;
        }
    }
    printf("2nd largest = %d\n", second_largest);


    int smallest = arr[0];
    int second_smallest = -1;

    for(i = 1; i < size; i++){
        if(arr[i] != smallest){
            second_smallest = arr[i];
            break;
        }
    }
    printf("2nd smallest is : %d\n", second_smallest);
}

char *getChar(){
    char words[20];
    strcpy(words, "hello");
    return words;
}

int sum(int *arr, int size){
    int sum;
    for(int i=0; i<=size; i++){
        sum += arr[i]; // sum = sum + arr[i]
    }
    return sum;
}

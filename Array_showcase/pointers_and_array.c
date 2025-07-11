#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

void demo_integer_array() 
{
    int numbers[5] = {10, 20, 30, 40, 50};

    printf("\n=== Integer Array ===\n");
    for (int i = 0; i < 5; i++) {
        printf("numbers[%d] = %d, address = %p\n", i, numbers[i], (void*)&numbers[i]);
    }
}

void demo_character_array() 
{
    char letters[] = {'A', 'B', 'C', 'D', 'E', '\0'};

    printf("\n=== Character Array ===\n");
    printf("letters as string: %s\n", letters);  // Print as string
    for (int i = 0; i < 5; i++) {
        printf("letters[%d] = %c, address = %p\n", i, letters[i], (void*)&letters[i]);
    }
}

void demo_string_array() 
{
    const char *words[] = {"Hello", "World", "Array", "of", "Strings"};
    printf("\n=== Array of Strings ===\n");
    for (int i = 0; i < 5; i++) {
        printf("words[%d] = %s, address = %p (pointer), first char = %c\n",
               i, words[i], (void*)&words[i], words[i][0]);
    }
}

#define ARRAY_SIZE 5        
void demo_dynamic_array() 
{
    int *dyn = NULL;

    dyn = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!dyn) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    // Init array
    memset(dyn, 0, ARRAY_SIZE * sizeof(int)); 

    printf("\n=== Dynamic Integer Array ===\n");
    if (dyn) {
        for (int i = 0; i < 5; i++) {
            dyn[i] = i * 100;
            printf("dyn[%d] = %d, address = %p\n", i, dyn[i], (void*)&dyn[i]);
        }
        free(dyn);
    } else {
        printf("Memory allocation failed!\n");
    }
}

void demo_uninit_array() 
{
    int uninit[5];

    printf("\n=== Uninitialized Integer Array ===\n");
    for (int i = 0; i < 5; i++) {
        printf("uninit[%d] = %d, address = %p (value is garbage)\n", i, uninit[i], (void*)&uninit[i]);
    }
}

void demo_runtime_array() 
{
    int n = 0;
    int runtime[n];  // Variable Length Array (C99)
    memset(runtime, 0, n * sizeof(int));

    printf("\nEnter size of runtime array: ");
    scanf("%d", &n);

    printf("\n=== Runtime-sized Array (VLA) ===\n");
    for (int i = 0; i < n; i++) {
        runtime[i] = i + 1;
        printf("runtime[%d] = %d, address = %p\n", i, runtime[i], (void*)&runtime[i]);
    }
}

void demo_double_pointer() 
{
    int x = 42;
    int *p = &x;
    int **pp = &p;

    printf("\n=== Double Pointer ===\n");
    printf("x = %d\n", x);
    printf("p (address of x) = %p\n", (void*)p);
    printf("*p (value at p) = %d\n", *p);
    printf("pp (address of p) = %p\n", (void*)pp);
    printf("*pp (value at pp = p) = %p\n", (void*)*pp);
    printf("**pp (value at *pp = *p = x) = %d\n", **pp);
}

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a + b; }
int multiply(int a, int b){ return a * b; }
int divide(int a, int b){ return a / b; }

int my_math_array[] = 
{
    {"plus", add},
    {"minus", subtract},
    {"times", multiply},
    {"divide", divide}
};

void demo_function_pointer() //function pointer or callback
{
    int a = 0;
    int b = 0;
    char exp[20];

    printf("\n=== Function Pointer ===\n");
    printf("type in a simple math expression: \n");
    printf("eg: 5 plus 3 ");
    printf("eg: 5 minus 3 ");
    printf("eg: 5 times 3 ");
    printf("eg: 5 divide 3 ");
    scanf("%d %o %d", &a, exp, &b);

    //check expression match with my_math_array[]
    for (int i = 0; i < sizeof(my_math_array)/sizeof(my_math_array[]); i++){
        
    }
}

int (*func_ptr)(int, int);

void demo_function_pointer(int a, int b, func_ptr cb) 
{
    int ret = cb(a, b);


}

int main() {
    int choice;

    do {
        printf("\nChoose an array demo to run:\n");
        printf("1. Integer Array\n");
        printf("2. Character Array\n");
        printf("3. Array of Strings\n");
        printf("4. Dynamic Array (malloc)\n");
        printf("5. Uninitialized Array\n");
        printf("6. Runtime-sized Array (VLA)\n");
        printf("7. Double Pointer Demo\n");
        printf("8. Function Pointer Demo\n");
        printf("0. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: demo_integer_array(); break;
            case 2: demo_character_array(); break;
            case 3: demo_string_array(); break;
            case 4: demo_dynamic_array(); break;
            case 5: demo_uninit_array(); break;
            case 6: demo_runtime_array(); break;
            case 7: demo_double_pointer(); break;
            case 8: demo_function_pointer(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}

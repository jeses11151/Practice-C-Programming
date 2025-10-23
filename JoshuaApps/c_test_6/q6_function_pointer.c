#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/ sizeof((arr)[0]))

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b){ return a * b; }

int divide(int a, int b)
{ 
    if(b == 0){
        printf("cannot divide by 0\n");
        return 0;
    }
    return a / b; 
}

void default_handler()
{
    printf("expression error, please try again.\n");
    return;
}

const char * op_name[] = {"plus", "minus", "times", "divide"};
int (*op_func[])(int, int) = {add, subtract, multiply, divide};

int main() //function pointer or callback
{
    int a = 0;
    int b = 0;
    char exp[20];
    int (*fptr)(int, int);
    int results = 0;
    int i = 0;
    int check_input = 3;
    int check_expr = 1;

    printf("\n=== Function Pointer ===\n");
    printf("type in a simple math expression: \n");
    printf("eg: 5 plus 3 \n");
    printf("eg: 5 minus 3 \n");
    printf("eg: 5 times 3 \n");
    printf("eg: 5 divide 3 \n");
    check_input = scanf("%d %s %d", &a, exp, &b);

    if(check_input != 3){
        printf("wrong input, please try again\n");
        return 1;
    }


    //check expression match with my_math_array[]
    for (i = 0; i < ARRAY_SIZE(op_name); i++){
        if(strncasecmp(exp, op_name[i], strlen(op_name[i])) == 0)
        {
            results  = op_func[i](a, b);    //calls the function at the same name as the expression
            //printf("find cmd %d \n", i);
            printf("results: %d \n\n", results);
            check_expr = 0;
            break;
        }
    }

    if(check_expr == 1){
        default_handler();
    }

}
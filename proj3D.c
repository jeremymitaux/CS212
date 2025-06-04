#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 10

typedef enum
{
   ADD,
   MULT,
   SUBTRACT,
   UNSUPPORTED
} MathOperation;

typedef struct 
{
        double numbers[MAX_SIZE];
        int top;
} 
Stack;

void initstack(Stack *s) 
{
    s->top = -1;
}


bool isFull(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, double num)
 {
    if (isFull(s)) {
        printf("Error: Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->numbers[++s->top] = num;
}

double pop(Stack *s) {
    if (isEmpty(s)) {
        printf("Error: Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->numbers[s->top--];
}

double stringToDouble(const char *str) 
{
    double number = 0.0;
    int sign = 1;  
    double factor = 1.0;  
    if (*str == '-') 
    {
        sign = -1;
        ++str;
    }

    // Process each character of the string
    while (*str != '\0') 
    {
        if (*str >= '0' && *str <= '9') {
            number = number * 10.0 + (*str - '0');
        } else if (*str == '.') {
            str++;
            while (*str >= '0' && *str <= '9') 
            {
                factor /= 10.0;
                number += (*str - '0') * factor;
                str++;
            }
            break;
        } else 
        {
            printf("Error: Invalid character in number '%s'\n", str);
            exit(EXIT_FAILURE);
        }
        str++;
    }

    return number * sign;
}


MathOperation getOperation(const char *op) 
{
    if (op[0] == '+' && op[1] == '\0') {
        return ADD;
    } else if (op[0] == '-' && op[1] == '\0') 
    {
        return SUBTRACT;
    } else if (op[0] == 'x' && op[1] == '\0') 
    {
        return MULT;
    } else {
        return UNSUPPORTED;
    }
}

int main(int argc, char *argv[]) 
{
    Stack stack;
    initstack(&stack);

    for (int i = 1; i < argc; i++) 
    {
        if (argv[i][0] == '+' || argv[i][0] == '-' || argv[i][0] == 'x') 
        {
            if (stack.top < 1)
             {
                printf("Error: Insufficient operands\n");
                exit(EXIT_FAILURE);
            }

            double a = pop(&stack);
            double b = pop(&stack);
            double result;

            switch (getOperation(argv[i]))
             {
                case ADD:
                    result = b + a;
                    break;
                case MULT:
                    result = b * a;
                    break;
                case SUBTRACT:
                    result = b - a;
                    break;
                default:
                    printf("Unsupported operation\n");
                    exit(EXIT_FAILURE);
                }
             push(&stack, result);

        } else {
            double num = stringToDouble(argv[i]);
            push(&stack, num);
        }
    }

    if (!isEmpty(&stack)) {
        printf("The total is %f\n", pop(&stack));
    } else {
        printf("No result to display\n");
    }


    return 0;
} 
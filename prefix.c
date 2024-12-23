#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10

int top = -1;
int stack[SIZE];

void push(int item) {
    if (top >= SIZE - 1) {
        printf("Stack Overflow\n");
        
    }
    stack[++top] = item;
}

int pop() {
    if (top < 0) {
        printf("Stack Underflow\n");
        return 1;
    }
    return stack[top--];
}

int isOperand(char c) {
    return (c >= '0' && c <= '9');
}

int testPrefix(char* exp) {
    int i, x, y;
 for (i = strlen(exp) - 1; i >= 0; i--) {
     if (isOperand(exp[i])) {
  push(exp[i] - '0');
    } else {
 x = pop();
 y = pop();
 switch (exp[i]) {
    case '+':
push(x + y);
break;
     case '-':
push(x - y);
break;
     case '*':
push(x * y);
 break;
     case '/':
if (y == 0) {
 printf("Division by zero error\n");
    return 0;
    }
 push(x / y);
 break;
default:
 printf("Invalid operator\n");
return 1;
 }
    }
}

 return pop();
}

int main() {
    char exp[SIZE];

    printf("Enter prefix expression: ");
    scanf("%s", exp);

    int result = testPrefix(exp);
    printf("Result: %d\n", result);

    return 0;
}
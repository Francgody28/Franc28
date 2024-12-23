#include <stdio.h>
#define SIZE 10

int top = -1;
char stack[SIZE];

void push(char item) {
if (top >= SIZE - 1) {
 printf("Stack Overflow\n");
 return;
}
    stack[++top] = item;
}
char pop() {
if (top < 0) {
 printf("Stack Underflow\n");
 return -1; 
 }
    return stack[top--];
}
int isEmpty() {
return (top < 0);
}

int parenthiss(char* bracket) {
    top = -1; 
for (int i = 0; bracket[i] != '\0'; i++) {
 if (bracket[i] == '(') {
  push('(');
   } else if (bracket[i] == ')') {
if (isEmpty()) {
 return 0; 
}
pop();
 }
  }

return isEmpty(); 
}

int main() {
char bracket[SIZE];

 printf("Enter a string: ");
    scanf("%s", bracket);

 if (parenthiss(bracket)) {
 printf("parenthiss are balanced.\n");
} else {
 printf(" parenthiss are not balanced.\n");
 }
return 0;
}
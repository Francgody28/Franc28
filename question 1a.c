#include <stdio.h>
#define SIZE 5

int top = -1;
int stack[SIZE];

void push(int item) {
    if (top >= SIZE - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack[++top] = item;
}

int pop() {
    if (top < 0) {
        printf("Stack Underflow\n");
        return 0;
    }
    return stack[top--];
}

void reverseList(int arr[], int size) {
    top = -1; 
    for (int i = 0; i < size; i++) {
        push(arr[i]);
    }
    for (int i = 0; i < size; i++) {
        arr[i] = pop();
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    reverseList(arr, size);

    printf("Reversed array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
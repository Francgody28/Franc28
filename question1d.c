#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

int topA = -1;
int stackA[SIZE];
int topB = -1;
int stackB[SIZE];

void pushA(int data) {
if (topA >= SIZE - 1) {
printf("Stack 1 Overflow\n");
 return;
}
stackA[++topA] = data;
}
int popA() {
 if (topA < 0) {
 printf("Stack A Underflow\n");
  return 0; 
}
return stackA[topA--];
 }
void pushB(int data) {
 if (topB >= SIZE - 1) {
 printf("Stack B Overflow\n");
  return;
 }
stackB[++topB] = data;
}
int popB() {
 if (topB < 0) {
 printf("Stack B Underflow\n");
return 0; 
 }
 return stackB[topB--];
}
int compareStacks() {
 if (topA != topB) {
  return 0; 
 }
while (topA >= 0) {
  if (stackA[topA] != stackB[topB]) {
 return 0; 
 }
 topA--; topB--;
 }
  return 1; 
}
int main() {   
pushA(1);
pushA(2);
 pushA(3);

pushB(1);
 pushB(2);
pushB(3);

if (compareStacks()) {
printf("Stacks are equal.\n");
}
 else
 {
  printf("Stacks are not equal.\n");
 }
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10
#define  NAME 5

struct Student {
    char name[NAME];
    int age;
};
struct Student items[SIZE];
int top = -1;

void push(struct Student student) {
if (top >= SIZE - 1) {
 printf("Stack Overflow\n");
 return;
   }
 items[++top] = student;
}
struct Student pop() {
 if (top < 0) {
 printf("Stack Underflow\n");
 struct Student emptyStudent;
emptyStudent.age = -1; 
return emptyStudent; 
  }
 return items[top--];
}
int main() {
struct Student student1 = {"Juma", 20};
struct Student student2 = {"Asha", 22};

push(student1);
push(student2);
while (top >= 0) {
 struct Student poppedStudent = pop();
 printf("Name: %s, Age: %d\n", poppedStudent.name, poppedStudent.age);
  }
 return 0;
}
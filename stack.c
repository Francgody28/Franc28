#include <stdio.h>
#include <stdlib.h>
#define size 5
int top = -1;
struct stack{
    int s[size];
    int top;
};
struct stack st;
void push(int item)
{
    st.top++;
    st.s[st.top] = item;
}

int pop()
{
    int item = st.s[st.top];
    st.top--;
    return item;
}

int peek()
{
    if (isEmpty())
    {
        printf("stack is empty\n");
        return -1;
    }
    printf("the peek item is:%d ",st.s[st.top]);
    return st.s[st.top];
}
int isEmpty()
{
    if (st.top == -1)
    {
        printf("stack is empty!\n");
        return 1;
    }
    return 0;
}

void display()
{
    if (st.top == -1)
    {
        printf("stack is empty\n");
        return;
    }
    for (int i = st.top; i > 0; i--)
    {
        printf("%d",st.s[i]);
    }

}

int main() {
    int choice = 1;
    int value;
    while (choice ==1)
    {
        printf("enter your choice: \n");
        printf("1.push \n");
        printf("2.pop \n");
        printf("3.peek \n");
        printf("4.display \n");
        scanf("%d",&choice);

        switch (choice)
        {
            case 1:
                printf("enter your value: \n");
            scanf("%d",&value);
            push(value);
            break;
            case 2:
                value = pop();
            if (value != -1)
                printf("the deleted value is: %d",value);
            break;

            case 3:
                value = peek();
            if (value != -1)
                printf("the top value is: %d",value);
            break;
            case 4:
                display();
            break;
            default:
                break;
        }
        printf("\nDo you want to continue? (1 for Yes / 0 for No): ");
        scanf("%d", &choice);
    }

    return 0;

}

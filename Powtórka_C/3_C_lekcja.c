#include <stdio.h>

int a , b;

int main()
{

    printf("Podaj warto�� czynnika a\n");
    scanf("%d",&a);
    printf("Podaj warto�� czynnika b\n");
    scanf("%d",&b);

    if(a>b || a==b)
        printf("A wi�krze od B");
        else
            printf("B wi�krze od A");


    return 0;
}

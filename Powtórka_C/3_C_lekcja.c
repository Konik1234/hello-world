#include <stdio.h>

int a , b;

int main()
{

    printf("Podaj wartoœæ czynnika a\n");
    scanf("%d",&a);
    printf("Podaj wartoœæ czynnika b\n");
    scanf("%d",&b);

    if(a>b || a==b)
        printf("A wiêkrze od B");
        else
            printf("B wiêkrze od A");


    return 0;
}

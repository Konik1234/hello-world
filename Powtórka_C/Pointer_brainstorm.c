#include <stdio.h>

void divide(int*x,int*z,int by);
void switch1 (int*a,int*b);

int main()
{
int z;
int b;

printf("Podaj wartosc zmiennej\n");
scanf("%d%d",&z,&b);

printf("%d i %d \n",z,b);

switch1(&z,&b);

printf("%d i %d \n",z,b);

    return 0;
}

void divide(int*x,int*z,int by)
{
    *x/=by;
    *z/=by;
}

void switch1(int*a,int*b)
{
    int z;
    z=*a;
    *a=*b;
    *b=z;
}

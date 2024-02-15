#include <stdio.h>

#define A 5
#define B 12
int ob;


int main()
{
    printf("Siema jak sie masz %d\n\n",A);
    printf("to jest wartosc \t %d",B);

    printf("A teraz podaj wartosc liczby do wyœwietlenia\n\n");
    scanf("%d",&ob);
    printf("To jest twoja wartosc %d\n",ob);

    return 0;
}

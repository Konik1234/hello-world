#include <stdio.h>
#include <string.h>



struct person
    {
        int age;
        char name[40];
        float many;
    };

    typedef struct
    {
        int quantyty;
        float cost;
    } product;

int main ()
{
    struct person a;
    struct person b = {29,"Tomek",400.5};
    product d ={12,60.5};


    printf("Podaj swoje dane!!! \n");

    scanf("%d",&a.age);
    scanf("%s",a.name);
    scanf("%f",&a.many);

    strcpy(b.name,"Agata");




    printf("Wiek: %d\n\n",a.age);
        printf("Imie: %s\n\n",a.name);
        printf("Srodki na kacie: %4.3f\n\n",a.many);

        printf("Wiek: %d\n\n",b.age);
        printf("Imie: %s\n\n",b.name);
        printf("Srodki na kacie: %4.3f\n\n",b.many);

         printf("Ilosc kupionego produktu: %d\n\n",d.quantyty);

        printf("Koszt produktu: %4.3f\n\n",d.cost);


    return 0;
}

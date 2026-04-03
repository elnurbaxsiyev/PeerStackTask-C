#include <stdio.h>

int main(void) {
    int choice;

    printf("Student Manager\n");
    printf("1 -- Istifadeci elave et\n");
    printf("2 -- Istifadecini sil\n");
    printf("3 -- Istifadecileri goster\n");
    printf("4 -- Secilmis istifadecini yenile\n");
    printf("5 -- Proqramdan cix\n");

    printf("Seciminizi daxil edin: ");
    scanf("%d", &choice);

    printf("Siz %d secdiniz.\n", choice);

    return 0;
}
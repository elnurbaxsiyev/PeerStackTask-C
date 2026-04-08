#include <stdio.h>

#define NAME_SIZE 50
#define SURNAME_SIZE 50
#define EMAIL_SIZE 100

typedef struct {
    int id;
    char name[NAME_SIZE];
    char surname[SURNAME_SIZE];
    int age;
    char email[EMAIL_SIZE];
} Student;

void showMenu(void);
void handleChoice(int choice);

void showMenu(void) {
    printf("\nStudent Manager\n");
    printf("1 -- Istifadeci elave et\n");
    printf("2 -- Istifadecini sil\n");
    printf("3 -- Istifadecileri goster\n");
    printf("4 -- Secilmis istifadecini yenile\n");
    printf("5 -- Proqramdan cix\n");
}

void handleChoice(int choice) {
    switch (choice) {
        case 1:
            printf("Add hissesi sonra yazilacaq.\n");
            break;
        case 2:
            printf("Delete hissesi sonra yazilacaq.\n");
            break;
        case 3:
            printf("Show hissesi sonra yazilacaq.\n");
            break;
        case 4:
            printf("Update hissesi sonra yazilacaq.\n");
            break;
        case 5:
            printf("Programdan cixilir...\n");
            break;
        default:
            printf("Yanlis secim etdiniz.\n");
    }
}

int main(void) {
    int choice = 0;

    while (choice != 5) {
        showMenu();
        printf("Seciminizi daxil edin: ");
        scanf("%d", &choice);

        handleChoice(choice);
    }

    return 0;
}
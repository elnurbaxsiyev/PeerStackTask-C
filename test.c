#include <stdio.h>

#define MAX_STUDENTS 100
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

Student students[MAX_STUDENTS];
int studentCount = 0;
int nextId = 1;

void showMenu(void);
void handleChoice(int choice);
void addStudent(void);
void showStudents(void);
int findStudentById(int id);
void updateStudent(void);
void deleteStudent(void);

void showMenu(void) {
    printf("\n=== Student Manager ===\n");
    printf("1 -- Istifadeci elave et\n");
    printf("2 -- Istifadecini sil\n");
    printf("3 -- Istifadecileri goster\n");
    printf("4 -- Secilmis istifadecini yenile\n");
    printf("5 -- Proqramdan cix\n");
}

int findStudentById(int id) {
    int i;

    for (i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return i;
        }
    }

    return -1;
}

void addStudent(void) {
    if (studentCount >= MAX_STUDENTS) {
        printf("Yer yoxdur. Yeni student elave etmek olmur.\n");
        return;
    }

    students[studentCount].id = nextId;

    printf("Adi daxil edin: ");
    scanf("%49s", students[studentCount].name);

    printf("Soyadi daxil edin: ");
    scanf("%49s", students[studentCount].surname);

    printf("Yasi daxil edin: ");
    scanf("%d", &students[studentCount].age);

    printf("Email daxil edin: ");
    scanf("%99s", students[studentCount].email);

    printf("Student elave olundu. ID = %d\n", students[studentCount].id);

    studentCount++;
    nextId++;
}

void showStudents(void) {
    int i;

    if (studentCount == 0) {
        printf("Hech bir student yoxdur.\n");
        return;
    }

    printf("\n=== Student List ===\n");
    for (i = 0; i < studentCount; i++) {
        printf("ID: %d\n", students[i].id);
        printf("Ad: %s\n", students[i].name);
        printf("Soyad: %s\n", students[i].surname);
        printf("Yas: %d\n", students[i].age);
        printf("Email: %s\n", students[i].email);
        printf("--------------------\n");
    }
}

void updateStudent(void) {
    int id;
    int index;

    if (studentCount == 0) {
        printf("Yenilemek ucun student yoxdur.\n");
        return;
    }

    printf("Yenilenecek studentin ID-sini daxil edin: ");
    scanf("%d", &id);

    index = findStudentById(id);

    if (index == -1) {
        printf("Bu ID ile student tapilmadi.\n");
        return;
    }

    printf("Yeni adi daxil edin: ");
    scanf("%49s", students[index].name);

    printf("Yeni soyadi daxil edin: ");
    scanf("%49s", students[index].surname);

    printf("Yeni yasi daxil edin: ");
    scanf("%d", &students[index].age);

    printf("Yeni email daxil edin: ");
    scanf("%99s", students[index].email);

    printf("Student yenilendi.\n");
}

void deleteStudent(void) {
    int id;
    int index;
    int i;

    if (studentCount == 0) {
        printf("Silmek ucun student yoxdur.\n");
        return;
    }

    printf("Silinecek studentin ID-sini daxil edin: ");
    scanf("%d", &id);

    index = findStudentById(id);

    if (index == -1) {
        printf("Bu ID ile student tapilmadi.\n");
        return;
    }

    for (i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }

    studentCount--;

    printf("Student silindi.\n");
}

void handleChoice(int choice) {
    switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            deleteStudent();
            break;
        case 3:
            showStudents();
            break;
        case 4:
            updateStudent();
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
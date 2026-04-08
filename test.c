#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define SURNAME_SIZE 50
#define EMAIL_SIZE 100
#define DATA_FILE "students.csv"

#define VALIDATE_TEXT  1
#define VALIDATE_AGE   2
#define VALIDATE_EMAIL 3

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
void saveStudents(void);
void loadStudents(void);

int isBlank(const char *s);
int containsComma(const char *s);
int isNumber(const char *s);
int validateInput(const char *value, int type);

void showMenu(void) {
    printf("\n=== Student Manager ===\n");
    printf("1 -- Istifadeci elave et\n");
    printf("2 -- Istifadecini sil\n");
    printf("3 -- Istifadecileri goster\n");
    printf("4 -- Secilmis istifadecini yenile\n");
    printf("5 -- Proqramdan cix\n");
}

int isBlank(const char *s) {
    return s[0] == '\0';
}

int containsComma(const char *s) {
    while (*s) {
        if (*s == ',') {
            return 1;
        }
        s++;
    }
    return 0;
}

int isNumber(const char *s) {
    int i = 0;

    if (s[0] == '\0') {
        return 0;
    }

    while (s[i]) {
        if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
        i++;
    }

    return 1;
}

int validateInput(const char *value, int type) {
    int age;

    switch (type) {
        case VALIDATE_TEXT:
            return !isBlank(value) && !containsComma(value);

        case VALIDATE_AGE:
            if (!isNumber(value)) {
                return 0;
            }
            age = atoi(value);
            return age > 0 && age <= 150;

        case VALIDATE_EMAIL:
            return !isBlank(value) &&
                   !containsComma(value) &&
                   strchr(value, '@') != NULL;

        default:
            return 0;
    }
}

void saveStudents(void) {
    FILE *file;
    int i;

    file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Fayl acilmadi.\n");
        return;
    }

    for (i = 0; i < studentCount; i++) {
        fprintf(file, "%d,%s,%s,%d,%s\n",
                students[i].id,
                students[i].name,
                students[i].surname,
                students[i].age,
                students[i].email);
    }

    fclose(file);
}

void loadStudents(void) {
    FILE *file;
    char line[256];

    file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        return;
    }

    studentCount = 0;
    nextId = 1;

    while (fgets(line, sizeof(line), file) != NULL && studentCount < MAX_STUDENTS) {
        int result;

        result = sscanf(line, "%d,%49[^,],%49[^,],%d,%99[^\n]",
                        &students[studentCount].id,
                        students[studentCount].name,
                        students[studentCount].surname,
                        &students[studentCount].age,
                        students[studentCount].email);

        if (result == 5) {
            if (students[studentCount].id >= nextId) {
                nextId = students[studentCount].id + 1;
            }
            studentCount++;
        }
    }

    fclose(file);
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
    char ageStr[20];

    if (studentCount >= MAX_STUDENTS) {
        printf("Yer yoxdur. Yeni student elave etmek olmur.\n");
        return;
    }

    students[studentCount].id = nextId;

    do {
        printf("Adi daxil edin: ");
        scanf("%49s", students[studentCount].name);

        if (!validateInput(students[studentCount].name, VALIDATE_TEXT)) {
            printf("Ad yanlisdir.\n");
        }
    } while (!validateInput(students[studentCount].name, VALIDATE_TEXT));

    do {
        printf("Soyadi daxil edin: ");
        scanf("%49s", students[studentCount].surname);

        if (!validateInput(students[studentCount].surname, VALIDATE_TEXT)) {
            printf("Soyad yanlisdir.\n");
        }
    } while (!validateInput(students[studentCount].surname, VALIDATE_TEXT));

    do {
        printf("Yasi daxil edin: ");
        scanf("%19s", ageStr);

        if (!validateInput(ageStr, VALIDATE_AGE)) {
            printf("Yas yanlisdir.\n");
        }
    } while (!validateInput(ageStr, VALIDATE_AGE));
    students[studentCount].age = atoi(ageStr);

    do {
        printf("Email daxil edin: ");
        scanf("%99s", students[studentCount].email);

        if (!validateInput(students[studentCount].email, VALIDATE_EMAIL)) {
            printf("Email yanlisdir.\n");
        }
    } while (!validateInput(students[studentCount].email, VALIDATE_EMAIL));

    printf("Student elave olundu. ID = %d\n", students[studentCount].id);

    studentCount++;
    nextId++;

    saveStudents();
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
    char ageStr[20];

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

    do {
        printf("Yeni adi daxil edin: ");
        scanf("%49s", students[index].name);

        if (!validateInput(students[index].name, VALIDATE_TEXT)) {
            printf("Ad yanlisdir.\n");
        }
    } while (!validateInput(students[index].name, VALIDATE_TEXT));

    do {
        printf("Yeni soyadi daxil edin: ");
        scanf("%49s", students[index].surname);

        if (!validateInput(students[index].surname, VALIDATE_TEXT)) {
            printf("Soyad yanlisdir.\n");
        }
    } while (!validateInput(students[index].surname, VALIDATE_TEXT));

    do {
        printf("Yeni yasi daxil edin: ");
        scanf("%19s", ageStr);

        if (!validateInput(ageStr, VALIDATE_AGE)) {
            printf("Yas yanlisdir.\n");
        }
    } while (!validateInput(ageStr, VALIDATE_AGE));
    students[index].age = atoi(ageStr);

    do {
        printf("Yeni email daxil edin: ");
        scanf("%99s", students[index].email);

        if (!validateInput(students[index].email, VALIDATE_EMAIL)) {
            printf("Email yanlisdir.\n");
        }
    } while (!validateInput(students[index].email, VALIDATE_EMAIL));

    printf("Student yenilendi.\n");

    saveStudents();
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

    saveStudents();
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

    loadStudents();

    while (choice != 5) {
        showMenu();
        printf("Seciminizi daxil edin: ");
        scanf("%d", &choice);

        handleChoice(choice);
    }

    return 0;
}
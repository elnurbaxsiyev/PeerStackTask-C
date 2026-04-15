#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <io.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define SURNAME_SIZE 50
#define EMAIL_SIZE 100
#define DATA_FILE "students.txt"

#define VALIDATE_TEXT  1
#define VALIDATE_AGE   2
#define VALIDATE_EMAIL 3

#define ID_WIDTH 5
#define AGE_WIDTH 3

#define HEADER_FORMAT "%-5s|%-50s|%-50s|%-3s|%-100s\n"
#define RECORD_FORMAT "%05d|%-50s|%-50s|%03d|%-100s\n"

#define ID_START 0
#define NAME_START (ID_START + ID_WIDTH + 1)
#define SURNAME_START (NAME_START + NAME_SIZE + 1)
#define AGE_START (SURNAME_START + SURNAME_SIZE + 1)
#define EMAIL_START (AGE_START + AGE_WIDTH + 1)

#define RECORD_SIZE (ID_WIDTH + 1 + NAME_SIZE + 1 + SURNAME_SIZE + 1 + AGE_WIDTH + 1 + EMAIL_SIZE + 1)
#define HEADER_SIZE RECORD_SIZE

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
void sortStudentsById(void);

int isBlank(const char *s);
int containsComma(const char *s);
int containsPipe(const char *s);
int isNumber(const char *s);
int isOnlyLetters(const char *s);
int validateIdString(const char *s);
int validateInput(const char *value, int type);
void trimRight(char *str);
void copyFixedField(char *dest, int destSize, const char *src, int start, int width);
void normalizeName(char *s);

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
        if (*s == ',') return 1;
        s++;
    }
    return 0;
}

int containsPipe(const char *s) {
    while (*s) {
        if (*s == '|') return 1;
        s++;
    }
    return 0;
}

int isNumber(const char *s) {
    int i = 0;

    if (s[0] == '\0') return 0;

    while (s[i]) {
        if (!isdigit((unsigned char)s[i])) return 0;
        i++;
    }

    return 1;
}

int isOnlyLetters(const char *s) {
    int i = 0;

    if (s[0] == '\0') return 0;

    while (s[i]) {
        if (!isalpha((unsigned char)s[i])) return 0;
        i++;
    }

    return 1;
}

void normalizeName(char *s) {
    int i;

    if (s[0] == '\0') return;

    s[0] = toupper((unsigned char)s[0]);

    for (i = 1; s[i] != '\0'; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

int validateIdString(const char *s) {
    return isNumber(s);
}

int validateInput(const char *value, int type) {
    int age;

    switch (type) {
        case VALIDATE_TEXT:
            return !isBlank(value) &&
                   !containsComma(value) &&
                   !containsPipe(value) &&
                   isOnlyLetters(value);

        case VALIDATE_AGE:
            if (!isNumber(value)) return 0;
            age = atoi(value);
            return age > 0 && age <= 150;

        case VALIDATE_EMAIL:
            return !isBlank(value) &&
                   !containsComma(value) &&
                   !containsPipe(value) &&
                   strchr(value, '@') != NULL;

        default:
            return 0;
    }
}

void trimRight(char *str) {
    int len = strlen(str);

    while (len > 0 &&
           (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

void copyFixedField(char *dest, int destSize, const char *src, int start, int width) {
    int copyLen = width;

    if (copyLen > destSize - 1) copyLen = destSize - 1;

    strncpy(dest, src + start, copyLen);
    dest[copyLen] = '\0';
    trimRight(dest);
}

void sortStudentsById(void) {
    int i, j;
    Student temp;

    for (i = 0; i < studentCount - 1; i++) {
        for (j = i + 1; j < studentCount; j++) {
            if (students[i].id > students[j].id) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

void saveStudents(void) {
    FILE *file;
    int i;

    sortStudentsById();

    file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        printf("Fayl acilmadi.\n");
        return;
    }

    fprintf(file, HEADER_FORMAT, "ID", "NAME", "SURNAME", "AGE", "EMAIL");

    for (i = 0; i < studentCount; i++) {
        fprintf(file, RECORD_FORMAT,
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
    char line[RECORD_SIZE + 5];

    file = fopen(DATA_FILE, "rb");
    if (file == NULL) return;

    studentCount = 0;
    nextId = 1;

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL && studentCount < MAX_STUDENTS) {
        char idStr[ID_WIDTH + 1];
        char ageStr[AGE_WIDTH + 1];

        copyFixedField(idStr, sizeof(idStr), line, ID_START, ID_WIDTH);
        copyFixedField(students[studentCount].name, NAME_SIZE, line, NAME_START, NAME_SIZE);
        copyFixedField(students[studentCount].surname, SURNAME_SIZE, line, SURNAME_START, SURNAME_SIZE);
        copyFixedField(ageStr, sizeof(ageStr), line, AGE_START, AGE_WIDTH);
        copyFixedField(students[studentCount].email, EMAIL_SIZE, line, EMAIL_START, EMAIL_SIZE);

        students[studentCount].id = atoi(idStr);
        students[studentCount].age = atoi(ageStr);

        if (students[studentCount].id > 0) {
            normalizeName(students[studentCount].name);
            normalizeName(students[studentCount].surname);

            if (students[studentCount].id >= nextId) {
                nextId = students[studentCount].id + 1;
            }
            studentCount++;
        }
    }

    sortStudentsById();
    fclose(file);
}

int findStudentById(int id) {
    int left = 0;
    int right = studentCount - 1;
    int mid;

    while (left <= right) {
        mid = left + (right - left) / 2;

        if (students[mid].id == id) return mid;
        if (students[mid].id < id) left = mid + 1;
        else right = mid - 1;
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
        normalizeName(students[studentCount].name);

        if (!validateInput(students[studentCount].name, VALIDATE_TEXT)) {
            printf("Ad yalniz herflerden ibaret olmalidir.\n");
        }
    } while (!validateInput(students[studentCount].name, VALIDATE_TEXT));

    do {
        printf("Soyadi daxil edin: ");
        scanf("%49s", students[studentCount].surname);
        normalizeName(students[studentCount].surname);

        if (!validateInput(students[studentCount].surname, VALIDATE_TEXT)) {
            printf("Soyad yalniz herflerden ibaret olmalidir.\n");
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

    printf("Student elave olundu. ID = %05d\n", students[studentCount].id);

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
        printf("RecordNo: %d\n", i);
        printf("ID: %05d\n", students[i].id);
        printf("Ad: %s\n", students[i].name);
        printf("Soyad: %s\n", students[i].surname);
        printf("Yas: %d\n", students[i].age);
        printf("Email: %s\n", students[i].email);
        printf("--------------------\n");
    }
}

void updateStudent(void) {
    int id, index;
    char idStr[20];
    char ageStr[20];
    FILE *file;

    if (studentCount == 0) {
        printf("Yenilemek ucun student yoxdur.\n");
        return;
    }

    do {
        printf("Yenilenecek studentin ID-sini daxil edin: ");
        scanf("%19s", idStr);

        if (!validateIdString(idStr)) {
            printf("ID yalniz reqemlerden ibaret olmalidir.\n");
        }
    } while (!validateIdString(idStr));

    id = atoi(idStr);

    index = findStudentById(id);
    if (index == -1) {
        printf("Bu ID ile student tapilmadi.\n");
        return;
    }

    do {
        printf("Yeni adi daxil edin: ");
        scanf("%49s", students[index].name);
        normalizeName(students[index].name);

        if (!validateInput(students[index].name, VALIDATE_TEXT)) {
            printf("Ad yalniz herflerden ibaret olmalidir.\n");
        }
    } while (!validateInput(students[index].name, VALIDATE_TEXT));

    do {
        printf("Yeni soyadi daxil edin: ");
        scanf("%49s", students[index].surname);
        normalizeName(students[index].surname);

        if (!validateInput(students[index].surname, VALIDATE_TEXT)) {
            printf("Soyad yalniz herflerden ibaret olmalidir.\n");
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

    file = fopen(DATA_FILE, "rb+");
    if (file == NULL) {
        printf("Fayl acilmadi.\n");
        return;
    }

    fseek(file, (long)HEADER_SIZE + (long)index * RECORD_SIZE, SEEK_SET);
    fprintf(file, RECORD_FORMAT,
            students[index].id,
            students[index].name,
            students[index].surname,
            students[index].age,
            students[index].email);

    fclose(file);
    printf("Student yenilendi.\n");
}

void deleteStudent(void) {
    int id, index, i;
    char idStr[20];
    FILE *file;
    char line[RECORD_SIZE + 5];

    if (studentCount == 0) {
        printf("Silmek ucun student yoxdur.\n");
        return;
    }

    do {
        printf("Silinecek studentin ID-sini daxil edin: ");
        scanf("%19s", idStr);

        if (!validateIdString(idStr)) {
            printf("ID yalniz reqemlerden ibaret olmalidir.\n");
        }
    } while (!validateIdString(idStr));

    id = atoi(idStr);

    index = findStudentById(id);
    if (index == -1) {
        printf("Bu ID ile student tapilmadi.\n");
        return;
    }

    file = fopen(DATA_FILE, "rb+");
    if (file == NULL) {
        printf("Fayl acilmadi.\n");
        return;
    }

    for (i = index + 1; i < studentCount; i++) {
        fseek(file, (long)HEADER_SIZE + (long)i * RECORD_SIZE, SEEK_SET);
        fread(line, 1, RECORD_SIZE, file);

        fseek(file, (long)HEADER_SIZE + (long)(i - 1) * RECORD_SIZE, SEEK_SET);
        fwrite(line, 1, RECORD_SIZE, file);
    }

    fflush(file);
    _chsize(fileno(file), (long)HEADER_SIZE + (long)(studentCount - 1) * RECORD_SIZE);
    fclose(file);

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

    loadStudents();

    while (choice != 5) {
        showMenu();
        printf("Seciminizi daxil edin: ");

        if (scanf("%d", &choice) != 1) {
            choice = 0;
            while (getchar() != '\n');
        }

        handleChoice(choice);
    }

    return 0;
}
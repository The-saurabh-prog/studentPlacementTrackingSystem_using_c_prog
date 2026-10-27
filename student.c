
#include "placement.h"

/* Dynamic array of student records */
static Student *students = NULL;
static size_t studentCount = 0;
static size_t studentCapacity = 0;

/* Load saved students when the program starts */
void initializeStudentStorage(void) {
    Student *loadedStudents = NULL;
    size_t loadedCount = 0;
    size_t loadedCapacity = 0;

    if (!loadStudents(&loadedStudents,
                      &loadedCount,
                      &loadedCapacity)) {
        printf("Warning: Could not load student records.\n");
        printf("Check students.dat before continuing.\n");
        return;
    }

    free(students);

    students = loadedStudents;
    studentCount = loadedCount;
    studentCapacity = loadedCapacity;

    printf("Loaded %zu student record(s).\n", studentCount);
}

/* Save the current student array */
int persistStudentRecords(void) {
    return saveStudents(students, studentCount);
}

/* Read a complete line safely */
static void readLine(const char *prompt, char *buffer, size_t size) {
    int ch;
    size_t len;

    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Discard excess characters */
        }
    }
}

/* Read and validate an integer */
static int readInt(const char *prompt, int *value) {
    char line[100];
    char *end;
    long result;

    readLine(prompt, line, sizeof(line));

    if (line[0] == '\0')
        return 0;

    result = strtol(line, &end, 10);

    while (*end == ' ' || *end == '\t')
        end++;

    if (*end != '\0' || end == line ||
        result < -2147483647L - 1 ||
        result > 2147483647L)
        return 0;

    *value = (int)result;
    return 1;
}

/* Read and validate a floating-point number */
static int readFloat(const char *prompt, float *value) {
    char line[100];
    char *end;
    float result;

    readLine(prompt, line, sizeof(line));

    if (line[0] == '\0')
        return 0;

    result = strtof(line, &end);

    while (*end == ' ' || *end == '\t')
        end++;

    if (end == line || *end != '\0')
        return 0;

    *value = result;
    return 1;
}

/* Find a student by ID */
static int findStudentIndex(int id) {
    size_t i;

    for (i = 0; i < studentCount; i++) {
        if (students[i].studentId == id)
            return (int)i;
    }

    return -1;
}

/* Expand the dynamic array when required */
static int ensureCapacity(void) {
    size_t newCapacity;
    Student *temp;

    if (studentCount < studentCapacity)
        return 1;

    newCapacity = (studentCapacity == 0)
                    ? INITIAL_CAPACITY
                    : studentCapacity * 2;

    if (newCapacity > (size_t)-1 / sizeof(Student))
        return 0;

    temp = realloc(students, newCapacity * sizeof(Student));

    if (temp == NULL)
        return 0;

    students = temp;
    studentCapacity = newCapacity;

    return 1;
}

/* Add a student */
void addStudent(void) {
    Student s;

    printf("\n========== ADD STUDENT ==========\n");

    if (!readInt("Enter student ID: ", &s.studentId) ||
        s.studentId <= 0) {
        printf("Invalid student ID.\n");
        return;
    }

    if (findStudentIndex(s.studentId) != -1) {
        printf("This student ID already exists.\n");
        return;
    }

    readLine("Enter student name: ", s.name, sizeof(s.name));
    readLine("Enter branch: ", s.branch, sizeof(s.branch));

    if (s.name[0] == '\0' || s.branch[0] == '\0') {
        printf("Name and branch cannot be empty.\n");
        return;
    }

    if (!readFloat("Enter CGPA (0-10): ", &s.cgpa) ||
        s.cgpa < 0.0f || s.cgpa > 10.0f) {
        printf("Invalid CGPA.\n");
        return;
    }

    if (!readInt("Enter number of backlogs: ", &s.backlogs) ||
        s.backlogs < 0) {
        printf("Invalid backlog count.\n");
        return;
    }

    readLine("Enter skills: ", s.skills, sizeof(s.skills));

    s.placed = 0;
    s.packageLPA = 0.0f;

    if (!ensureCapacity()) {
        printf("Memory allocation failed. Student not added.\n");
        return;
    }

    students[studentCount] = s;
studentCount++;

if (persistStudentRecords()) {
    printf("Student added and saved successfully!\n");
} else {
    printf("Student added in memory, but saving failed!\n");
}
}

/* Display all student records */
void displayStudents(void) {
    size_t i;

    printf("\n================ STUDENT RECORDS ================\n");

    if (studentCount == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("%-8s %-20s %-12s %-7s %-9s\n",
           "ID", "Name", "Branch", "CGPA", "Backlogs");

    printf("--------------------------------------------------\n");

    for (i = 0; i < studentCount; i++) {
        printf("%-8d %-20.20s %-12.12s %-7.2f %-9d\n",
               students[i].studentId,
               students[i].name,
               students[i].branch,
               students[i].cgpa,
               students[i].backlogs);
    }

    printf("Total students: %zu\n", studentCount);
}

/* Search for a student */
void searchStudent(void) {
    int id;
    int index;

    if (!readInt("Enter student ID to search: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    index = findStudentIndex(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("\nStudent ID : %d\n", students[index].studentId);
    printf("Name       : %s\n", students[index].name);
    printf("Branch     : %s\n", students[index].branch);
    printf("CGPA       : %.2f\n", students[index].cgpa);
    printf("Backlogs   : %d\n", students[index].backlogs);
    printf("Skills     : %s\n", students[index].skills);
}

/* Update student details */
void updateStudent(void) {
    int id;
    int index;
    int choice;
    float cgpa;
    int backlogs;
    char buffer[MAX_SKILLS];

    if (!readInt("Enter student ID to update: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    index = findStudentIndex(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("\n1. Update name\n");
    printf("2. Update branch\n");
    printf("3. Update CGPA\n");
    printf("4. Update backlogs\n");
    printf("5. Update skills\n");

    if (!readInt("Enter your choice: ", &choice)) {
        printf("Invalid choice.\n");
        return;
    }

    switch (choice) {
        case 1:
            readLine("Enter new name: ", buffer, MAX_NAME);
            if (buffer[0] == '\0') {
                printf("Name cannot be empty.\n");
                return;
            }
            strcpy(students[index].name, buffer);
            break;

        case 2:
            readLine("Enter new branch: ", buffer, MAX_BRANCH);
            if (buffer[0] == '\0') {
                printf("Branch cannot be empty.\n");
                return;
            }
            strcpy(students[index].branch, buffer);
            break;

        case 3:
            if (!readFloat("Enter new CGPA: ", &cgpa) ||
                cgpa < 0.0f || cgpa > 10.0f) {
                printf("Invalid CGPA.\n");
                return;
            }
            students[index].cgpa = cgpa;
            break;

        case 4:
            if (!readInt("Enter new backlog count: ", &backlogs) ||
                backlogs < 0) {
                printf("Invalid backlog count.\n");
                return;
            }
            students[index].backlogs = backlogs;
            break;

        case 5:
            readLine("Enter new skills: ",
                     students[index].skills,
                     sizeof(students[index].skills));
            break;

        default:
            printf("Invalid update option.\n");
            return;
    }

   if (persistStudentRecords()) {
    printf("Student updated and saved successfully!\n");
} else {
    printf("Update made in memory, but saving failed!\n");
}
}

/* Delete a student */
void deleteStudent(void) {
    int id;
    int index;
    size_t i;

    if (!readInt("Enter student ID to delete: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    index = findStudentIndex(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    for (i = (size_t)index; i + 1 < studentCount; i++) {
        students[i] = students[i + 1];
    }

    studentCount--;

    if (persistStudentRecords()) {
    printf("Student deleted and changes saved!\n");
} else {
    printf("Deletion made in memory, but saving failed!\n");
}
}

/* Student management submenu */
void studentManagementMenu(void) {
    int choice;

    do {
        printf("\n========== STUDENT MANAGEMENT ==========\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("0. Back to Main Menu\n");

        if (!readInt("Enter your choice: ", &choice)) {
            printf("Invalid input. Enter a number.\n");
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choice != 0);
}
int getStudentById(int studentId, Student *result)
{
    int index = findStudentIndex(studentId);

    if (index == -1)
        return 0;

    *result = students[index];
    return 1;
}
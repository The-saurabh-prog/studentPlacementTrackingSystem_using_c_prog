
#include "placement.h"

/* Save records using a temporary file */
int saveStudents(const Student *records, size_t count) {
    FILE *fp;
    int closeResult;

    fp = fopen("students.tmp", "wb");

    if (fp == NULL) {
        perror("Cannot create temporary file");
        return 0;
    }

    if (count > 0 &&
        fwrite(records, sizeof(Student), count, fp) != count) {
        perror("Error writing student records");
        fclose(fp);
        remove("students.tmp");
        return 0;
    }

    closeResult = fclose(fp);

    if (closeResult != 0) {
        perror("Error closing temporary file");
        remove("students.tmp");
        return 0;
    }

    /*
       Replace the previous data file.
       A production application would use a platform-specific
       atomic replacement strategy for stronger crash safety.
    */
    if (remove("students.dat") != 0) {
        FILE *check = fopen("students.dat", "rb");

        if (check != NULL) {
            fclose(check);
            perror("Cannot replace students.dat");
            remove("students.tmp");
            return 0;
        }
    }

    if (rename("students.tmp", "students.dat") != 0) {
        perror("Cannot rename temporary file");
        return 0;
    }

    return 1;
}

/* Load saved records into dynamically allocated memory */
int loadStudents(Student **records, size_t *count,
                 size_t *capacity) {
    FILE *fp;
    Student record;
    Student *temp;
    size_t newCapacity;

    *records = NULL;
    *count = 0;
    *capacity = 0;

    fp = fopen("students.dat", "rb");

    /* First run: no data file exists yet */
    if (fp == NULL)
        return 1;

    while (fread(&record, sizeof(Student), 1, fp) == 1) {
        if (*count == *capacity) {
            newCapacity = (*capacity == 0)
                            ? 2
                            : (*capacity * 2);

            if (newCapacity > (size_t)-1 / sizeof(Student)) {
                fclose(fp);
                free(*records);
                *records = NULL;
                *count = 0;
                *capacity = 0;
                return 0;
            }

            temp = realloc(*records,
                           newCapacity * sizeof(Student));

            if (temp == NULL) {
                fclose(fp);
                free(*records);
                *records = NULL;
                *count = 0;
                *capacity = 0;
                return 0;
            }

            *records = temp;
            *capacity = newCapacity;
        }

        (*records)[*count] = record;
        (*count)++;
    }

    if (ferror(fp)) {
        perror("Error reading students.dat");
        fclose(fp);
        free(*records);
        *records = NULL;
        *count = 0;
        *capacity = 0;
        return 0;
    }

    /*
       Detect an incomplete final record rather than silently
       accepting a truncated binary file.
    */
    {
        long position = ftell(fp);

        if (position < 0 || fseek(fp, 0, SEEK_END) != 0) {
            fclose(fp);
            free(*records);
            *records = NULL;
            *count = 0;
            *capacity = 0;
            return 0;
        }

        if (ftell(fp) != position) {
            fclose(fp);
            free(*records);
            *records = NULL;
            *count = 0;
            *capacity = 0;
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

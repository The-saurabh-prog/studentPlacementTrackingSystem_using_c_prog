
#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_BRANCH 30
#define MAX_SKILLS 150
#define MAX_ROLE 50

#define INITIAL_CAPACITY 2

typedef enum {
    APPLIED,
    SHORTLISTED,
    INTERVIEW,
    SELECTED,
    REJECTED
} ApplicationStatus;

typedef struct {
    int studentId;
    char name[MAX_NAME];
    char branch[MAX_BRANCH];
    float cgpa;
    int backlogs;
    char skills[MAX_SKILLS];
    int placed;
    float packageLPA;
} Student;

typedef struct {
    int companyId;
    char companyName[MAX_NAME];
    char jobRole[MAX_ROLE];
    float packageLPA;
    float minimumCGPA;
    int maxBacklogs;
    int vacancies;
} Company;

typedef struct {
    int applicationId;
    int studentId;
    int companyId;
    ApplicationStatus status;
} Application;

/* Menu functions */
void displayWelcome(void);
void showMainMenu(void);
void applicationManagementMenu(void);
void applyForCompany(void);
void displayApplications(void);
void updateApplicationStatus(void);
void checkCompanyEligibility(void);
int getStudentById(int studentId, Student *result);
int getCompanyById(int companyId, Company *result);
/* Student management functions */
void studentManagementMenu(void);
void companyManagementMenu(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

/* Permanent student storage */
void initializeStudentStorage(void);
int saveStudents(const Student *records, size_t count);
int loadStudents(Student **records, size_t *count,
                 size_t *capacity);
int persistStudentRecords(void);

#endif
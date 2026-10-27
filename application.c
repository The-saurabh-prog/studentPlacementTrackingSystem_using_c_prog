
#include "placement.h"

static Application *applications = NULL;
static size_t applicationCount = 0;
static size_t applicationCapacity = 0;
static int nextApplicationId = 1;

static int ensureApplicationCapacity(void)
{
    if (applicationCount < applicationCapacity)
        return 1;

    size_t newCapacity =
        (applicationCapacity == 0) ? INITIAL_CAPACITY
                                   : applicationCapacity * 2;

    Application *temp = realloc(
        applications, newCapacity * sizeof(Application));

    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }

    applications = temp;
    applicationCapacity = newCapacity;
    return 1;
}

static const char *statusName(ApplicationStatus status)
{
    switch (status) {
        case APPLIED:    return "Applied";
        case SHORTLISTED:return "Shortlisted";
        case INTERVIEW:  return "Interview";
        case SELECTED:   return "Selected";
        case REJECTED:   return "Rejected";
        default:         return "Unknown";
    }
}

void applyForCompany(void)
{
    int studentId, companyId;
    Student student;
    Company company;

    printf("\n--- Apply for a Company ---\n");

    printf("Enter Student ID: ");
    if (scanf("%d", &studentId) != 1) {
        printf("Invalid Student ID.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Company ID: ");
    if (scanf("%d", &companyId) != 1) {
        printf("Invalid Company ID.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    /* Find the student */
    if (!getStudentById(studentId, &student)) {
        printf("Student not found!\n");
        return;
    }

    /* Find the company */
    if (!getCompanyById(companyId, &company)) {
        printf("Company not found!\n");
        return;
    }

    /* Check if already placed */
    if (student.placed) {
        printf("This student is already placed.\n");
        return;
    }

    /* Check CGPA eligibility */
    if (student.cgpa < company.minimumCGPA) {
        printf("Application rejected: CGPA is below the minimum %.2f.\n",
               company.minimumCGPA);
        return;
    }

    /* Check backlog eligibility */
    if (student.backlogs > company.maxBacklogs) {
        printf("Application rejected: Too many backlogs.\n");
        return;
    }

    /* Check vacancies */
    if (company.vacancies <= 0) {
        printf("No vacancies are available for this company.\n");
        return;
    }

    /* Prevent duplicate applications */
    for (size_t i = 0; i < applicationCount; i++) {
        if (applications[i].studentId == studentId &&
            applications[i].companyId == companyId) {
            printf("This student has already applied to this company.\n");
            return;
        }
    }

    /* Allocate memory for a new application */
    if (!ensureApplicationCapacity()) {
        return;
    }

    Application app;
    app.applicationId = nextApplicationId++;
    app.studentId = studentId;
    app.companyId = companyId;
    app.status = APPLIED;

    applications[applicationCount++] = app;

    printf("\nApplication submitted successfully!\n");
    printf("Application ID: %d\n", app.applicationId);
    printf("Student: %s\n", student.name);
    printf("Company: %s\n", company.companyName);
    printf("Job Role: %s\n", company.jobRole);
    printf("Package: %.2f LPA\n", company.packageLPA);
}


void displayApplications(void)
{
    printf("\n--- Placement Applications ---\n");

    if (applicationCount == 0) {
        printf("No applications available.\n");
        return;
    }

    printf("%-12s %-12s %-12s %-15s\n",
           "App ID", "Student ID", "Company ID", "Status");

    for (size_t i = 0; i < applicationCount; i++) {
        printf("%-12d %-12d %-12d %-15s\n",
               applications[i].applicationId,
               applications[i].studentId,
               applications[i].companyId,
               statusName(applications[i].status));
    }
}

void updateApplicationStatus(void)
{
    int id, choice;
    int found = 0;

    if (applicationCount == 0) {
        printf("\nNo applications available to update.\n");
        return;
    }

    printf("\nEnter Application ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid Application ID.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    for (size_t i = 0; i < applicationCount; i++) {
        if (applications[i].applicationId == id) {
            found = 1;

            printf("\n1. Applied\n");
            printf("2. Shortlisted\n");
            printf("3. Interview\n");
            printf("4. Selected\n");
            printf("5. Rejected\n");
            printf("Choose new status: ");

            if (scanf("%d", &choice) != 1) {
                printf("Invalid choice.\n");
                while (getchar() != '\n');
                return;
            }

            while (getchar() != '\n');

            switch (choice) {
                case 1: applications[i].status = APPLIED; break;
                case 2: applications[i].status = SHORTLISTED; break;
                case 3: applications[i].status = INTERVIEW; break;
                case 4: applications[i].status = SELECTED; break;
                case 5: applications[i].status = REJECTED; break;
                default:
                    printf("Invalid status choice.\n");
                    return;
            }

            printf("Application status updated successfully.\n");
            break;
        }
    }

    if (!found)
        printf("Application ID not found.\n");
}

void applicationManagementMenu(void)
{
    int choice;

    do {
        printf("\n===== APPLICATION MANAGEMENT =====\n");
        printf("1. Apply for Company\n");
        printf("2. Display Applications\n");
        printf("3. Update Application Status\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        switch (choice) {
            case 1: applyForCompany(); break;
            case 2: displayApplications(); break;
            case 3: updateApplicationStatus(); break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}

void checkCompanyEligibility(void)
{
    int studentId, companyId;
    Student student;
    Company company;

    printf("\n===== CHECK COMPANY ELIGIBILITY =====\n");

    printf("Enter Student ID: ");
    if (scanf("%d", &studentId) != 1) {
        printf("Invalid Student ID.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Company ID: ");
    if (scanf("%d", &companyId) != 1) {
        printf("Invalid Company ID.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    if (!getStudentById(studentId, &student)) {
        printf("Student not found.\n");
        return;
    }

    if (!getCompanyById(companyId, &company)) {
        printf("Company not found.\n");
        return;
    }

    printf("\nStudent: %s\n", student.name);
    printf("Company: %s\n", company.companyName);
    printf("Student CGPA: %.2f\n", student.cgpa);
    printf("Required CGPA: %.2f\n", company.minimumCGPA);
    printf("Student backlogs: %d\n", student.backlogs);
    printf("Maximum allowed backlogs: %d\n", company.maxBacklogs);
    printf("Available vacancies: %d\n", company.vacancies);

    if (student.placed) {
        printf("\nResult: NOT ELIGIBLE - Student is already placed.\n");
    } else if (student.cgpa < company.minimumCGPA) {
        printf("\nResult: NOT ELIGIBLE - CGPA requirement not met.\n");
    } else if (student.backlogs > company.maxBacklogs) {
        printf("\nResult: NOT ELIGIBLE - Too many backlogs.\n");
    } else if (company.vacancies <= 0) {
        printf("\nResult: NOT ELIGIBLE - No vacancies available.\n");
    } else {
        printf("\nResult: ELIGIBLE to apply for this company!\n");
    }
}


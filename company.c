
#include "placement.h"

/* Dynamic array of company records */
static Company *companies = NULL;
static size_t companyCount = 0;
static size_t companyCapacity = 0;

/* Read a line of text */
static void readCompanyLine(const char *prompt, char *buffer,
                            size_t size) {
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
            /* Discard extra characters */
        }
    }
}

/* Read an integer */
static int readCompanyInt(const char *prompt, int *value) {
    char line[100];
    char *end;
    long result;

    readCompanyLine(prompt, line, sizeof(line));

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

/* Read a floating-point value */
static int readCompanyFloat(const char *prompt, float *value) {
    char line[100];
    char *end;
    float result;

    readCompanyLine(prompt, line, sizeof(line));

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

/* Find a company by ID */
static int findCompanyIndex(int id) {
    size_t i;

    for (i = 0; i < companyCount; i++) {
        if (companies[i].companyId == id)
            return (int)i;
    }

    return -1;
}

/* Expand company array when needed */
static int ensureCompanyCapacity(void) {
    size_t newCapacity;
    Company *temp;

    if (companyCount < companyCapacity)
        return 1;

    newCapacity = (companyCapacity == 0)
                    ? INITIAL_CAPACITY
                    : companyCapacity * 2;

    if (newCapacity > (size_t)-1 / sizeof(Company))
        return 0;

    temp = realloc(companies, newCapacity * sizeof(Company));

    if (temp == NULL)
        return 0;

    companies = temp;
    companyCapacity = newCapacity;

    return 1;
}

/* Add a company */
static void addCompany(void) {
    Company c;

    printf("\n========== ADD COMPANY ==========\n");

    if (!readCompanyInt("Enter company ID: ", &c.companyId) ||
        c.companyId <= 0) {
        printf("Invalid company ID.\n");
        return;
    }

    if (findCompanyIndex(c.companyId) != -1) {
        printf("Company ID already exists.\n");
        return;
    }

    readCompanyLine("Enter company name: ",
                    c.companyName, sizeof(c.companyName));

    readCompanyLine("Enter job role: ",
                    c.jobRole, sizeof(c.jobRole));

    if (c.companyName[0] == '\0' || c.jobRole[0] == '\0') {
        printf("Company name and role cannot be empty.\n");
        return;
    }

    if (!readCompanyFloat("Enter package (LPA): ", &c.packageLPA) ||
        c.packageLPA < 0.0f) {
        printf("Invalid package.\n");
        return;
    }

    if (!readCompanyFloat("Enter minimum CGPA (0-10): ",
                          &c.minimumCGPA) ||
        c.minimumCGPA < 0.0f || c.minimumCGPA > 10.0f) {
        printf("Invalid minimum CGPA.\n");
        return;
    }

    if (!readCompanyInt("Enter maximum allowed backlogs: ",
                        &c.maxBacklogs) ||
        c.maxBacklogs < 0) {
        printf("Invalid backlog limit.\n");
        return;
    }

    if (!readCompanyInt("Enter number of vacancies: ",
                        &c.vacancies) ||
        c.vacancies < 0) {
        printf("Invalid vacancy count.\n");
        return;
    }

    if (!ensureCompanyCapacity()) {
        printf("Memory allocation failed.\n");
        return;
    }

    companies[companyCount++] = c;

    printf("Company added successfully!\n");
}

/* Display all companies */
static void displayCompanies(void) {
    size_t i;

    printf("\n================ REGISTERED COMPANIES ================\n");

    if (companyCount == 0) {
        printf("No companies registered yet.\n");
        return;
    }

    printf("%-6s %-20s %-18s %-10s %-10s %-10s %-10s\n",
           "ID", "Company", "Role", "Package",
           "Min CGPA", "Backlogs", "Vacancies");

    for (i = 0; i < companyCount; i++) {
        printf("%-6d %-20.20s %-18.18s %-10.2f %-10.2f %-10d %-10d\n",
               companies[i].companyId,
               companies[i].companyName,
               companies[i].jobRole,
               companies[i].packageLPA,
               companies[i].minimumCGPA,
               companies[i].maxBacklogs,
               companies[i].vacancies);
    }

    printf("Total companies: %zu\n", companyCount);
}

/* Search for a company */
static void searchCompany(void) {
    int id;
    int index;

    if (!readCompanyInt("Enter company ID to search: ", &id)) {
        printf("Invalid company ID.\n");
        return;
    }

    index = findCompanyIndex(id);

    if (index == -1) {
        printf("Company not found.\n");
        return;
    }

    printf("\nCompany ID : %d\n", companies[index].companyId);
    printf("Name       : %s\n", companies[index].companyName);
    printf("Role       : %s\n", companies[index].jobRole);
    printf("Package    : %.2f LPA\n", companies[index].packageLPA);
    printf("Minimum CGPA: %.2f\n", companies[index].minimumCGPA);
    printf("Max backlogs: %d\n", companies[index].maxBacklogs);
    printf("Vacancies  : %d\n", companies[index].vacancies);
}

/* Company management submenu */
void companyManagementMenu(void) {
    int choice;

    do {
        printf("\n========== COMPANY MANAGEMENT ==========\n");
        printf("1. Add Company\n");
        printf("2. Display All Companies\n");
        printf("3. Search Company\n");
        printf("0. Back to Main Menu\n");

        if (!readCompanyInt("Enter your choice: ", &choice)) {
            printf("Invalid input. Enter a number.\n");
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1:
                addCompany();
                break;
            case 2:
                displayCompanies();
                break;
            case 3:
                searchCompany();
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}
int getCompanyById(int companyId, Company *result)
{
    int index = findCompanyIndex(companyId);

    if (index == -1)
        return 0;

    *result = companies[index];
    return 1;
}
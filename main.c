
#include "placement.h"

void displayWelcome(void) {
    printf("\n");
    printf("============================================\n");
    printf("   STUDENT PLACEMENT TRACKING SYSTEM\n");
    printf("============================================\n");
    printf("   BE-CSE | C Programming Project\n");
    printf("============================================\n");
}

void showMainMenu(void) {
    printf("\n--------------- MAIN MENU -----------------\n");
    printf("1. Student Management\n");
    printf("2. Company Management\n");
    printf("3. Check Company Eligibility\n");
    printf("4. Apply for Placement\n");
    printf("5. Track Application Status\n");
    printf("6. Placement Reports\n");
    printf("0. Exit\n");
    printf("--------------------------------------------\n");
    printf("Enter your choice: ");
}

int main(void) {
    int choice;
    int ch;

    initializeStudentStorage();
displayWelcome();

    do {
        showMainMenu();

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number.\n");

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Clear invalid input */
            }

            choice = -1;
            continue;
        }

        /* Clear the newline left by scanf */
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Discard extra input */
        }

        switch (choice) {
            case 1:
                studentManagementMenu();
                break;

            case 2:
                companyManagementMenu();
                break;

            case 3:
                checkCompanyEligibility();
                 break;

           case 4:
                 applicationManagementMenu();
                  break;

            case 5:
                printf("Application tracking will be added in Phase 3.\n");
                break;

            case 6:
                printf("Reports module will be added in Phase 5.\n");
                break;

            case 0:
                printf("Thank you for using SPTS!\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 0);

    return 0;
}

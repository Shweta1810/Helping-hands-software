/* main.c */
#include <stdio.h>
#include "project.h"

void printMenu() {
    printf("\n===== HELPING HANDS =====\n");
    printf("1. Enter branch name\n");
    printf("2. Remove branch\n");
    printf("3. Register helpers\n");
    printf("4. Remove helper\n");
    printf("5. View branches\n");
    printf("6. View helpers in branch\n");
    printf("7. Total helpers per branch\n");
    printf("8. Search helper\n");
    printf("9. Connect branches\n");
    printf("10. Exit\n");
    printf("Choose: ");
}

int main() {
    int ch = 0;

    while (1) {
        printMenu();

        /* validate menu choice using scanf-only approach */
        while (scanf("%d", &ch) != 1 || ch < 1 || ch > 10) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number between %d and %d: ", 1, 10);
        }
        clearInputBuffer();

        switch (ch) {
            case 1: optionAddBranch(); break;
            case 2: optionRemoveBranch(); break;
            case 3: optionRegisterHelpers(); break;
            case 4: optionRemoveHelperByID(); break;
            case 5: optionViewBranches(); break;
            case 6: optionHelpersByBranch(); break;
            case 7: optionTotalHelpersDetails(); break;
            case 8: optionSearchHelperByNumber(); break;
            case 9: optionConnectTwoBranches(); break;
            case 10:
                printf("Thank you!\n");
                return 0;
            default:
                /* unreachable due to validation */
                printf("Invalid option\n");
        }
    }
}

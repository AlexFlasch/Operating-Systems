#include "FileUtils.h"

#include <stdio.h>

Student *students;

int main() {
    // create a buffer in console so output is more easily readable
    printf("\n\n");

    students = convertFileToUsers();
    // generate initial user struct via read data.
    // calculate all required user data
    // output user data

    // create another buffer in console again for readability
    printf("\n\n");
}

void printGradingSchemeForUser() {
    printf("%-15s%-20s%-20s%-25s\n", "Category", "Category Percent", "#Items in Category", "Points/Item in Category");
    printf("------------------------------------------------------------------------------\n");
}

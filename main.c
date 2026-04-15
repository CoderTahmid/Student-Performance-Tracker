/* #################### Student Marks Organizer Project DIU #################### */
/* ----------------- Developer: Md. Delower Sarker ----------------- */

/* ----------------- Libraries (3) ----------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ----------------- Structures (1) ----------------- */

struct Student
{
    int id;
    char name[50];
    char section;
    int courses[5];
    int total_marks;
    struct Student *next;
};

struct Student *start = NULL;

const char *courseNames[5] = {
    "Discrete Mathematics (CSE212)",
    "Engineering Mathematics (MAT211)",
    "Bangladesh Studies (BNS101)",
    "Algorithm (CSE213)",
    "Algorithm Lab (CSE214)"};

const char *courseCodes[5] = {
    "CSE212",
    "MAT211",
    "BNS101",
    "CSE213",
    "CSE214"};

int isValidName(const char *name)
{
    int hasLetter = 0;

    for (int i = 0; name[i] != '\0'; i++)
    {
        unsigned char ch = (unsigned char)name[i];

        if (isalpha(ch))
        {
            hasLetter = 1;
        }
        else if (!isspace(ch))
        {
            return 0;
        }
    }

    return hasLetter;
}

/* ----------------- File Operations (2) ----------------- */

void saveData() // For auto-saving data when a new student is added
{
    FILE *fp = fopen("students_db.txt", "w");
    if (fp == NULL)
        return;

    struct Student *i = start;
    while (i != NULL)
    {
        fprintf(fp, "%d;%s;%c;%d;%d;%d;%d;%d;%d\n",
                i->id, i->name, i->section,
            i->courses[0], i->courses[1], i->courses[2], i->courses[3], i->courses[4],
                i->total_marks);
        i = i->next;
    }
    fclose(fp);
}

void loadData() // For loading all previous student data at startup
{
    FILE *fp = fopen("students_db.txt", "r");
    if (fp == NULL)
        return;

    char line[200];
    while (fgets(line, sizeof(line), fp))
    {
        int id, c1, c2, c3, c4, c5, total;
        char name[50], section;

        if (sscanf(line, "%d;%49[^;];%c;%d;%d;%d;%d;%d;%d", &id, name, &section, &c1, &c2, &c3, &c4, &c5, &total) == 9)
        {
            struct Student *newnode = (struct Student *)malloc(sizeof(struct Student));
            newnode->id = id;
            strcpy(newnode->name, name);
            newnode->section = section;
            newnode->courses[0] = c1;
            newnode->courses[1] = c2;
            newnode->courses[2] = c3;
            newnode->courses[3] = c4;
            newnode->courses[4] = c5;
            newnode->total_marks = total;
            newnode->next = NULL;

            // Reconstruct the list (already sorted from file)
            if (start == NULL)
            {
                start = newnode;
            }
            else
            {
                struct Student *t = start;
                while (t->next != NULL)
                    t = t->next;
                t->next = newnode;
            }
        }
        else if (sscanf(line, "%d;%49[^;];%c;%d;%d;%d;%d", &id, name, &section, &c1, &c2, &c3, &total) == 7)
        {
            // Backward compatibility for older data files that stored 3 courses.
            struct Student *newnode = (struct Student *)malloc(sizeof(struct Student));
            newnode->id = id;
            strcpy(newnode->name, name);
            newnode->section = section;
            newnode->courses[0] = c1;
            newnode->courses[1] = c2;
            newnode->courses[2] = c3;
            newnode->courses[3] = 0;
            newnode->courses[4] = 0;
            newnode->total_marks = newnode->courses[0] + newnode->courses[1] + newnode->courses[2] + newnode->courses[3] + newnode->courses[4];
            newnode->next = NULL;

            if (start == NULL)
            {
                start = newnode;
            }
            else
            {
                struct Student *t = start;
                while (t->next != NULL)
                    t = t->next;
                t->next = newnode;
            }
        }
    }
    fclose(fp);
}

/* #################### SINGLY LINKED LIST & CORE OPERATIONS (4) #################### */

/* ----------------- Insert Function (Sorted) (1) ----------------- */

void addStudentSorted() // Inserts a student keeping the list constantly sorted by ID
{
    struct Student *newnode = (struct Student *)malloc(sizeof(struct Student));
    struct Student *temp;
    int exists;

    printf("\n--- [ ADD NEW STUDENT ] ---\n");

    // STEP 1: ID Validation Loop
    while (1)
    {
        printf("Enter Student ID: ");
        if (scanf("%d", &newnode->id) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        temp = start;
        exists = 0;
        while (temp != NULL)
        {
            if (temp->id == newnode->id)
            {
                exists = 1;
                break;
            }
            temp = temp->next;
        }

        if (exists)
            printf("Error: This ID already exists! Please enter a unique ID.\n");
        else
            break;
    }

    // STEP 2: General Info
    while (1)
    {
        printf("Enter Student Name: ");
        scanf(" %49[^\n]", newnode->name);

        if (isValidName(newnode->name))
            break;

        printf("Invalid name! Please use letters and spaces only.\n");
    }

    printf("Enter Section (e.g., A, B, C): ");
    scanf(" %c", &newnode->section);

    // STEP 3: Marks & Total
    printf("\n-- Enter Course Marks --\n");
    for (int c = 0; c < 5; c++)
    {
        printf("%s: ", courseNames[c]);
        scanf("%d", &newnode->courses[c]);
    }

    newnode->total_marks = 0;
    for (int c = 0; c < 5; c++)
    {
        newnode->total_marks += newnode->courses[c];
    }
    printf("\nCalculating Total Marks... Total: %d\n", newnode->total_marks);

    newnode->next = NULL;

    // STEP 4: Sorted Insertion Logic (By ID)
    if (start == NULL || start->id >= newnode->id)
    {
        newnode->next = start;
        start = newnode;
    }
    else
    {
        struct Student *current = start;
        while (current->next != NULL && current->next->id < newnode->id)
        {
            current = current->next;
        }
        newnode->next = current->next;
        current->next = newnode;
    }

    printf("\n[v] Student added to the linked list successfully!\n");
    printf("[v] Inserted in sorted order by ID.\n");
    printf("[v] Data auto-saved to 'students_db.txt'.\n");

    saveData(); // Auto-save trigger
}

/* ----------------- Section Explorer (1) ----------------- */

void exploreBySection()
{
    char sec;
    printf("\n--- [ SECTION EXPLORER ] ---\n");
    printf("Enter Section to view (e.g., A, B, C): ");
    scanf(" %c", &sec);

    printf("\nFetching data for Section %c...\n", sec);
    printf("Sorting course data...\n\n");
    printf("--- [ SECTION %c: COURSE WISE LIST ] ---\n", sec);
    printf("%-6s | %-20s | %-7s | %-7s | %-7s | %-7s | %-7s | %-5s\n", "ID", "Name", "CSE212", "MAT211", "BNS101", "CSE213", "CSE214", "Total");
    printf("----------------------------------------------------------------------------------------------\n");

    struct Student *i = start;
    int found = 0;
    while (i != NULL)
    {
        if (i->section == sec || i->section == sec - 32 || i->section == sec + 32) // Basic case insensitivity
        {
             printf("%-6d | %-20s | %-7d | %-7d | %-7d | %-7d | %-7d | %-5d\n",
                 i->id, i->name, i->courses[0], i->courses[1], i->courses[2], i->courses[3], i->courses[4], i->total_marks);
            found = 1;
        }
        i = i->next;
    }

    if (!found)
        printf("No students found in Section %c.\n", sec);
    printf("----------------------------------------------------------------------------------------------\n");
}

/* #################### ALGORITHMS (QUICK SORT & BINARY SEARCH) (4) #################### */

/* ----------------- Utilities for Algorithms (1) ----------------- */

int countStudents()
{
    int count = 0;
    struct Student *i = start;
    while (i != NULL)
    {
        count++;
        i = i->next;
    }
    return count;
}

/* ----------------- Quick Sort Implementation (Top 3) (2) ----------------- */

void swap(struct Student **a, struct Student **b)
{
    struct Student *t = *a;
    *a = *b;
    *b = t;
}

int partition(struct Student *arr[], int low, int high)
{
    int pivot = arr[high]->total_marks;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j]->total_marks >= pivot) // Descending order
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(struct Student *arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void viewTopThree()
{
    int n = countStudents();
    if (n == 0)
    {
        printf("\nNo student records available.\n");
        return;
    }

    // Dynamic array of pointers to the linked list nodes
    struct Student **arr = malloc(n * sizeof(struct Student *));
    struct Student *curr = start;
    for (int i = 0; i < n; i++)
    {
        arr[i] = curr;
        curr = curr->next;
    }

    printf("\n--- [ HALL OF FAME: TOP 3 STUDENTS ] ---\n");
    printf("Applying Quick Sort algorithm...\n\n");

    quickSort(arr, 0, n - 1);

    printf("%-5s | %-6s | %-20s | %-3s | %-5s\n", "RANK", "ID", "NAME", "SEC", "TOTAL");
    printf("---------------------------------------------------------\n");

    int limit = (n < 3) ? n : 3;
    for (int i = 0; i < limit; i++)
    {
        printf(" #%-3d | %-6d | %-20s | %-3c | %-5d\n",
               i + 1, arr[i]->id, arr[i]->name, arr[i]->section, arr[i]->total_marks);
    }
    printf("---------------------------------------------------------\n");
    free(arr);
}

/* ----------------- Binary Search Implementation (Bar Graph) (2) ----------------- */

int binarySearch(struct Student *arr[], int low, int high, int target)
{
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (arr[mid]->id == target)
            return mid;
        if (arr[mid]->id < target)
            low = mid + 1; // Array is sorted by ID ascending
        else
            high = mid - 1;
    }
    return -1;
}

void generateBarGraph()
{
    int target_id;
    printf("\n--- [ MARKS VISUALIZER ] ---\n");
    printf("Enter Student ID to search: ");
    scanf("%d", &target_id);

    int n = countStudents();
    if (n == 0)
    {
        printf("\nDatabase is empty.\n");
        return;
    }

    // Since our linked list is ALWAYS sorted by ID, copying to array guarantees sorted array
    struct Student **arr = malloc(n * sizeof(struct Student *));
    struct Student *curr = start;
    for (int i = 0; i < n; i++)
    {
        arr[i] = curr;
        curr = curr->next;
    }

    printf("\nExecuting Binary Search... ");
    int index = binarySearch(arr, 0, n - 1, target_id);

    if (index != -1)
    {
        printf("Student Found!\n\n");
        printf("[ Name: %s | Sec: %c | ID: %d ]\n\n", arr[index]->name, arr[index]->section, arr[index]->id);

        for (int c = 0; c < 5; c++)
        {
            printf("%s [%3d] : ", courseCodes[c], arr[index]->courses[c]);
            int bars = arr[index]->courses[c] / 2; // Scaling down for visual fit
            for (int b = 0; b < bars; b++)
            {
                printf("#"); // Using standard ASCII character for broad compatibility
            }
            printf("\n");
        }
    }
    else
    {
        printf("Student not found!\n");
    }

    free(arr);
}

/* #################### MAIN MENU (1) #################### */

int main()
{
    loadData();
    int choice;

    while (1)
    {
        printf("\n======================================================\n");
        printf("           STUDENT MARKS ORGANIZER v1.0\n");
        printf("======================================================\n");
        printf(" [1] Add a New Student Record\n");
        printf(" [2] View Top 3 Students (Quick Sort)\n");
        printf(" [3] Explore Course Data by Section\n");
        printf(" [4] Generate Student Marks Bar Graph (Binary Search)\n");
        printf(" [0] Save & Exit\n");
        printf("======================================================\n");
        printf(" Select an option: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            addStudentSorted();
            break;
        case 2:
            viewTopThree();
            break;
        case 3:
            exploreBySection();
            break;
        case 4:
            generateBarGraph();
            break;
        case 0:
            saveData();
            printf("\nData saved. Exiting Student Marks Organizer...\n");
            exit(0);
        default:
            printf("Invalid Option. Please try again.\n");
            break;
        }
    }
    return 0;
}
    
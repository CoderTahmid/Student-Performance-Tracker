#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

struct Student
{
    int id;
    char name[50];
    char section[15];
    int courses[5];
    int total_marks;
    struct Student *next;
};

struct Student *start = NULL;

void saveData()
{
    FILE *fp = fopen("cse_students_db.txt", "w");
    if(fp == NULL) return;

    struct Student *i = start;
    while(i != NULL)
    {
        fprintf(fp, "%d;%s;%s;%d;%d;%d;%d;%d;%d\n",
                i->id, i->name, i->section,
                i->courses[0], i->courses[1], i->courses[2],
                i->courses[3], i->courses[4], i->total_marks);
        i = i->next;
    }
    fclose(fp);
}

void loadData()
{
    FILE *fp = fopen("cse_students_db.txt", "r");
    if(fp == NULL) return;

    char line[200];
    while(fgets(line, sizeof(line), fp))
    {
        int id, c1, c2, c3, c4, c5, total;
        char name[50], section[15];

        if(sscanf(line, "%d;%49[^;];%14[^;];%d;%d;%d;%d;%d;%d", &id, name, section, &c1, &c2, &c3, &c4, &c5, &total) == 9)
        {
            struct Student *newnode = (struct Student*)malloc(sizeof(struct Student));
            newnode->id = id;
            strcpy(newnode->name, name);
            strcpy(newnode->section, section);
            newnode->courses[0] = c1;
            newnode->courses[1] = c2;
            newnode->courses[2] = c3;
            newnode->courses[3] = c4;
            newnode->courses[4] = c5;
            newnode->total_marks = total;
            newnode->next = NULL;

            if(start == NULL) start = newnode;
            else
            {
                struct Student *t = start;
                while(t->next != NULL) t = t->next;
                t->next = newnode;
            }
        }
    }
    fclose(fp);
}

void addStudentSorted()
{
    struct Student *newnode = (struct Student*)malloc(sizeof(struct Student));
    struct Student *temp;
    int exists, valid;

    printf("\n--- [ ADD NEW CSE STUDENT ] ---\n");

    while(1)
    {
        printf("Enter Student ID: ");
        if(scanf("%d", &newnode->id) != 1)
        {
            while(getchar()!='\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        temp = start;
        exists = 0;
        while(temp != NULL)
        {
            if(temp->id == newnode->id)
            {
                exists = 1;
                break;
            }
            temp = temp->next;
        }

        if(exists) printf("Error: This ID already exists! Please enter a unique ID.\n");
        else break;
    }

    while(1)
    {
        printf("Enter Student Name: ");
        scanf(" %[^\n]", newnode->name);

        valid = 1;
        for(int i = 0; newnode->name[i] != '\0'; i++)
        {
            if(!isalpha(newnode->name[i]) && !isspace(newnode->name[i]) && newnode->name[i] != '.')
            {
                valid = 0;
                break;
            }
        }

        if(!valid) printf("Error: Name must contain only letters and spaces (Ex: Mohammad).\n");
        else break;
    }

    printf("Enter Section (Ex: 68_A, 68_B): ");
    scanf("%14s", newnode->section);

    const char *course_names[] =
    {
        "Algorithms(CSE213)", "Algorithms Lab(CSE214)", "Engineering Mathematics(MAT211)",
        "Discrete Mathematics(CSE212)", "Bangladesh Studies(BNS101)"
    };

    printf("\n-- Enter Course Marks (0 - 100) --\n");
    for(int c = 0; c < 5; c++)
    {
        while(1)
        {
            printf("%s: ", course_names[c]);
            if(scanf("%d", &newnode->courses[c]) != 1)
            {
                while(getchar()!='\n');
                printf("Invalid input! Please enter a number.\n");
                continue;
            }

            if(newnode->courses[c] < 0 || newnode->courses[c] > 100)
            {
                printf("Error: Marks must be between 0 and 100. Please try again.\n");
            }
            else break;
        }
    }

    newnode->total_marks = newnode->courses[0] + newnode->courses[1] + newnode->courses[2] + newnode->courses[3] + newnode->courses[4];


    newnode->next = NULL;

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

    printf("\n Student added successfully!\n");


    saveData();
}

void exploreBySection()
{
    char sec[15];
    printf("\n--- [ CSE SECTION EXPLORER ] ---\n");
    printf("Enter Section to view ( Ex: 68_A): ");
    scanf("%14s", sec);

    printf("\nFetching data for Section %s...\n", sec);
    printf("--- [ SECTION %s: COURSE WISE LIST ] ---\n", sec);

    printf("%-6s | %-18s | %-3s | %-3s | %-3s | %-3s | %-3s | %-5s\n", "ID", "Name", "Alg", "Alg_L", "EM", "DM", "BNS", "Total");
    printf("-----------------------------------------------------------------\n");

    struct Student *i = start;
    int found = 0;
    while(i != NULL)
    {
        if(strcmp(i->section, sec) == 0)
        {
            printf("%-6d | %-18s | %-3d | %-5d | %-3d | %-3d | %-3d | %-5d\n",
                   i->id, i->name, i->courses[0], i->courses[1], i->courses[2], i->courses[3], i->courses[4], i->total_marks);
            found = 1;
        }
        i = i->next;
    }

    if(!found) printf("No students found in Section %s.\n", sec);
    printf("-----------------------------------------------------------------\n");
}

void removeStudentByID()
{
    int target_id;
    printf("\n--- [ REMOVE STUDENT RECORD ] ---\n");
    printf("Enter Student ID to remove: ");

    if(scanf("%d", &target_id) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid input! Please enter a valid ID number.\n");
        return;
    }

    if(start == NULL)
    {
        printf("\nDatabase is empty. No students to remove.\n");
        return;
    }

    struct Student *temp = start;
    struct Student *prev = NULL;

    if (temp != NULL && temp->id == target_id)
    {
        start = temp->next;  
        free(temp);           
        printf("\n[v] Student with ID %d has been removed successfully!\n", target_id);
        saveData();
        return;
    }

    while (temp != NULL && temp->id != target_id)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("\n[x] Student with ID %d not found in the database.\n", target_id);
        return;
    }

    prev->next = temp->next;
    free(temp);

    printf("\n[v] Student with ID %d has been removed successfully!\n", target_id);
    saveData(); 
}

int countStudents()
{
    int count = 0;
    struct Student *i = start;
    while(i != NULL)
    {
        count++;
        i = i->next;
    }
    return count;
}

void swap(struct Student **a, struct Student **b)
{
    struct Student *temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Student *arr[], int low, int high)
{
    int pivot = arr[low]->total_marks;
    int i = low;
    int j = high;

    while (i < j)
    {
        while (arr[i]->total_marks >= pivot && i <= high - 1) i++;
        while (arr[j]->total_marks < pivot && j >= low + 1) j--;
        if (i < j) swap(&arr[i], &arr[j]);
    }
    swap(&arr[low], &arr[j]);
    return j;
}

void quickSort(struct Student *arr[], int low, int high)
{
    if (low < high)
    {
        int pivot_idx = partition(arr, low, high);
        quickSort(arr, low, pivot_idx - 1);
        quickSort(arr, pivot_idx + 1, high);
    }
}

void viewTopThree()
{
    int n = countStudents();
    if(n == 0)
    {
        printf("\nNo student records available.\n");
        return;
    }

    struct Student **arr = malloc(n * sizeof(struct Student*));
    struct Student *curr = start;
    for(int i = 0; i < n; i++)
    {
        arr[i] = curr;
        curr = curr->next;
    }

    printf("\n--- [ HALL OF FAME: TOP 3 CSE STUDENTS ] ---\n");
    printf("Applying Quick Sort algorithm...\n\n");

    quickSort(arr, 0, n - 1);

    printf("%-5s | %-6s | %-20s | %-7s | %-5s\n", "RANK", "ID", "NAME", "SECTION", "TOTAL");
    printf("------------------------------------------------------------\n");

    int limit = (n < 3) ? n : 3;
    for(int i = 0; i < limit; i++)
    {
        printf(" #%-3d | %-6d | %-20s | %-7s | %-5d\n",
               i + 1, arr[i]->id, arr[i]->name, arr[i]->section, arr[i]->total_marks);
    }
    printf("------------------------------------------------------------\n");
    free(arr);
}

int binarySearch(struct Student *arr[], int low, int high, int target)
{
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (arr[mid]->id == target) return mid;
        if (arr[mid]->id < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void searchStudentByID()
{
    int target_id;
    printf("\n--- [ SEARCH STUDENT ] ---\n");
    printf("Enter Student ID to search: ");
    scanf("%d", &target_id);

    int n = countStudents();
    if(n == 0)
    {
        printf("\nDatabase is empty.\n");
        return;
    }

    struct Student **arr = malloc(n * sizeof(struct Student*));
    struct Student *curr = start;
    for(int i = 0; i < n; i++)
    {
        arr[i] = curr;
        curr = curr->next;
    }

    printf("\nExecuting Binary Search... ");
    int index = binarySearch(arr, 0, n - 1, target_id);

    if(index != -1)
    {
        printf("Student Found!\n\n");
        printf("ID: %d | Name: %s | Section: %s | Total Marks: %d\n",
               arr[index]->id, arr[index]->name, arr[index]->section, arr[index]->total_marks);
    }
    else
    {
        printf("Student not found!\n");
    }

    free(arr);
}

void generateSectionBarGraph()
{
    char sec[15];
    printf("\n--- [ SECTION AVERAGE BAR GRAPH ] ---\n");
    printf("Enter Section to analyze (Ex: 68_A): ");
    scanf("%14s", sec);

    int sum[5] = {0, 0, 0, 0, 0};
    int count = 0;

    struct Student *i = start;
    while(i != NULL)
    {
        if(strcmp(i->section, sec) == 0)
        {
            for(int c = 0; c < 5; c++)
            {
                sum[c] += i->courses[c];
            }
            count++;
        }
        i = i->next;
    }

    if(count == 0)
    {
        printf("\nNo students found in Section %s to generate a graph.\n", sec);
        return;
    }

    printf("\nGenerating Bar Graph for Section %s (Total Students: %d)\n\n", sec, count);

    const char *course_names[] =
    {
        "Algorithms(CSE213)                 ",
        "Algorithms Lab(CSE214)             ",
        "Engineering Mathematics(MAT211)    ",
        "Discrete Mathematics(CSE212)       ",
        "Bangladesh Studies(BNS101)         "
    };

    for(int c = 0; c < 5; c++)
    {
        float avg = (float)sum[c] / count;
        printf("%s [Avg: %5.1f] : ", course_names[c], avg);

        int bars = (int)(avg / 2); 
        for(int b = 0; b < bars; b++)
        {
            printf("#");
        }
        printf("\n");
    }
}

int main()
{
    loadData();
    int choice;

    while(1)
    {
        printf("\n===========================================\n");
        printf("      SMART STUDENT MARKS ORGANIZER    \n");
        printf("===========================================\n");
        printf(" [1] Add Student Record\n");
        printf(" [2] Top 3 Ranking (CSE Dept.)\n");
        printf(" [3] Explore by Section\n");
        printf(" [4] View Section Wise Courses (Bar Graph)\n");
        printf(" [5] Find Student\n");
        printf(" [6] Remove Student\n");     
        printf(" [0] Exit\n");
        printf("===========================================\n");
        printf(" Select an option: ");

        if(scanf("%d", &choice) != 1)
        {
            while(getchar()!='\n');
            continue;
        }

        switch(choice)
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
            generateSectionBarGraph();
            break;
        case 5:
            searchStudentByID();
            break;
        case 6:
            removeStudentByID();
            break;
        case 0:
            saveData();
            printf("\n Exiting Smart Student marks Organizer..\n");
            exit(0);
        default:
            printf("Invalid Option. Please try again.\n");
            break;
        }
    }
    return 0;
}

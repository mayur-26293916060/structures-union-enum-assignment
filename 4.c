// Structures - 1
// Extended: Menu-driven student records system with validation, search, update, delete, sorting, and GPA analytics.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int roll;
    char name[50];
    int age;
    float gpa;
} Student;

Student* create_student_list(int n) {
    if (n <= 0) {
        n = 2;
    }
    Student *studs = (Student*) malloc(n * sizeof(Student));
    return studs;
}

void add_student(Student *studs, int i, int roll, char *name, int age, float gpa) {
    studs[i].roll = roll;
    strcpy(studs[i].name, name);
    studs[i].age = age;
    studs[i].gpa = gpa;
}

void display_students(Student *students, int size) {
    if (size == 0) {
        printf("No student records available.\n");
        return;
    }
    printf("\n--- All Students (%d) ---\n", size);
    for (int i = 0; i < size; i++) {
        printf("Roll: %d, Name: %s, Age: %d, GPA: %.2f\n",
               students[i].roll, students[i].name, students[i].age, students[i].gpa);
    }
}

void free_student_list(Student *students) {
    free(students);
}

int find_student(Student *students, int size, int roll) {
    for (int i = 0; i < size; i++) {
        if (students[i].roll == roll) {
            return i;
        }
    }
    return -1;
}

void search_student(Student *students, int size) {
    if (size == 0) {
        printf("List is empty.\n");
        return;
    }

    int roll;
    printf("Enter roll number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = find_student(students, size, roll);
    if (idx == -1) {
        printf("Student with roll number %d not found.\n", roll);
    } else {
        printf("Found: Roll: %d, Name: %s, Age: %d, GPA: %.2f\n",
               students[idx].roll, students[idx].name, students[idx].age, students[idx].gpa);
    }
}

void update_student(Student *students, int size) {
    if (size == 0) {
        printf("List is empty.\n");
        return;
    }

    int roll;
    printf("Enter roll number to update: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = find_student(students, size, roll);
    if (idx == -1) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }

    printf("Current details: Roll: %d, Name: %s, Age: %d, GPA: %.2f\n",
           students[idx].roll, students[idx].name, students[idx].age, students[idx].gpa);

    char name[50];
    int age;
    float gpa;

    printf("Enter new name: ");
    scanf(" %[^\n]", name);

    printf("Enter new age (16-100): ");
    if (scanf("%d", &age) != 1 || age < 16 || age > 100) {
        printf("Invalid age entered. Update cancelled.\n");
        return;
    }

    printf("Enter new GPA (0.00-4.00): ");
    if (scanf("%f", &gpa) != 1 || gpa < 0.0f || gpa > 4.0f) {
        printf("Invalid GPA entered. Update cancelled.\n");
        return;
    }

    strcpy(students[idx].name, name);
    students[idx].age = age;
    students[idx].gpa = gpa;

    printf("Student details updated successfully.\n");
}

void delete_student(Student *students, int *size) {
    if (*size == 0) {
        printf("List is empty.\n");
        return;
    }

    int roll;
    printf("Enter roll number to delete: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = find_student(students, *size, roll);
    if (idx == -1) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }

    for (int i = idx; i < *size - 1; i++) {
        students[i] = students[i + 1];
    }
    (*size)--;

    printf("Student with roll %d deleted successfully.\n", roll);
}

void show_analytics(Student *students, int size) {
    if (size == 0) {
        printf("No records available for analytics.\n");
        return;
    }

    int high_idx = 0;
    int low_idx = 0;
    float sum = 0.0f;

    for (int i = 0; i < size; i++) {
        sum += students[i].gpa;
        if (students[i].gpa > students[high_idx].gpa) {
            high_idx = i;
        }
        if (students[i].gpa < students[low_idx].gpa) {
            low_idx = i;
        }
    }

    printf("\n--- Academic Analytics ---\n");
    printf("Highest GPA: %.2f (%s, Roll: %d)\n", students[high_idx].gpa, students[high_idx].name, students[high_idx].roll);
    printf("Lowest GPA : %.2f (%s, Roll: %d)\n", students[low_idx].gpa, students[low_idx].name, students[low_idx].roll);
    printf("Average GPA: %.2f\n", sum / size);
}

void display_sorted(Student *students, int size) {
    if (size == 0) {
        printf("No student records to sort.\n");
        return;
    }

    Student *copy = (Student *)malloc(size * sizeof(Student));
    if (copy == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        copy[i] = students[i];
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (copy[j].gpa < copy[j + 1].gpa ||
               (copy[j].gpa == copy[j + 1].gpa && copy[j].roll > copy[j + 1].roll)) {
                Student temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
        }
    }

    printf("\n--- Students Sorted by GPA (Descending) ---\n");
    for (int i = 0; i < size; i++) {
        printf("Roll: %d, Name: %s, Age: %d, GPA: %.2f\n",
               copy[i].roll, copy[i].name, copy[i].age, copy[i].gpa);
    }

    free(copy);
}

int main(void) {
    int capacity = 0;
    int count = 0;

    printf("Enter initial student capacity: ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity. Defaulting to 2.\n");
        capacity = 2;
    }

    Student *student_list = create_student_list(capacity);
    if (student_list == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int choice = 0;
    while (1) {
        printf("\n=== Student Records Menu ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Academic Analytics (Min/Max/Avg GPA)\n");
        printf("7. Display Students Sorted by GPA\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            break;
        }

        switch (choice) {
            case 1: {
                int roll;
                char name[50];
                int age;
                float gpa;

                printf("Enter roll number (positive integer): ");
                if (scanf("%d", &roll) != 1 || roll <= 0) {
                    printf("Invalid roll number.\n");
                    break;
                }

                if (find_student(student_list, count, roll) != -1) {
                    printf("Error: Student with roll number %d already exists.\n", roll);
                    break;
                }

                printf("Enter name: ");
                scanf(" %[^\n]", name);

                printf("Enter age (16-100): ");
                if (scanf("%d", &age) != 1 || age < 16 || age > 100) {
                    printf("Invalid age. Must be 16-100.\n");
                    break;
                }

                printf("Enter GPA (0.00-4.00): ");
                if (scanf("%f", &gpa) != 1 || gpa < 0.0f || gpa > 4.0f) {
                    printf("Invalid GPA. Must be 0.00-4.00.\n");
                    break;
                }

                if (count >= capacity) {
                    int new_capacity = capacity * 2;
                    Student *temp = (Student *)realloc(student_list, new_capacity * sizeof(Student));
                    if (temp == NULL) {
                        printf("Failed to grow array.\n");
                        break;
                    }
                    student_list = temp;
                    capacity = new_capacity;
                    printf("Capacity expanded to %d.\n", capacity);
                }

                add_student(student_list, count, roll, name, age, gpa);
                count++;

                printf("Student added successfully.\n");
                break;
            }
            case 2: {
                display_students(student_list, count);
                break;
            }
            case 3: {
                search_student(student_list, count);
                break;
            }
            case 4: {
                update_student(student_list, count);
                break;
            }
            case 5: {
                delete_student(student_list, &count);
                break;
            }
            case 6: {
                show_analytics(student_list, count);
                break;
            }
            case 7: {
                display_sorted(student_list, count);
                break;
            }
            case 8: {
                printf("Freeing memory and exiting...\n");
                free_student_list(student_list);
                return 0;
            }
            default: {
                printf("Invalid choice. Please select 1 to 8.\n");
                break;
            }
        }
    }

    free_student_list(student_list);
    return 0;
}

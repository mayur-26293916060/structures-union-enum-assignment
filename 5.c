// Structures - 2
// Extended: Interactive rectangle inventory with area, perimeter, square classification, sorting, and filtering.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RECTANGLES 1000
#define TOLERANCE 0.00001f

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;

void calculate_area(Rectangle *r) {
    r->area = r->width * r->height;
    r->perimeter = 2.0f * (r->width + r->height);
}

Rectangle* create_rectangle_array(int size) {
    if (size <= 0) {
        size = 2;
    }
    Rectangle *rectangles = (Rectangle *)malloc(size * sizeof(Rectangle));
    return rectangles;
}

void add_rectangle(Rectangle *rectangles, int index, int id, float width, float height) {
    rectangles[index].id = id;
    rectangles[index].width = width;
    rectangles[index].height = height;
    calculate_area(&rectangles[index]);
}

int is_square(Rectangle *r) {
    return fabsf(r->width - r->height) < TOLERANCE;
}

void display_rectangles(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }
    printf("\n--- Rectangle Inventory (%d) ---\n", size);
    for (int i = 0; i < size; i++) {
        printf("Rectangle %d: ID = %d, Width = %.2f, Height = %.2f, Area = %.2f, Perimeter = %.2f (%s)\n",
               i, rectangles[i].id, rectangles[i].width, rectangles[i].height,
               rectangles[i].area, rectangles[i].perimeter,
               is_square(&rectangles[i]) ? "Square" : "Non-Square");
    }
}

void free_rectangle_array(Rectangle *rectangles) {
    free(rectangles);
}

int find_rectangle(Rectangle *rectangles, int size, int id) {
    for (int i = 0; i < size; i++) {
        if (rectangles[i].id == id) {
            return i;
        }
    }
    return -1;
}

void search_rectangle(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("Enter rectangle ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = find_rectangle(rectangles, size, id);
    if (idx == -1) {
        printf("Rectangle with ID %d not found.\n", id);
    } else {
        printf("Found: ID = %d, Width = %.2f, Height = %.2f, Area = %.2f, Perimeter = %.2f (%s)\n",
               rectangles[idx].id, rectangles[idx].width, rectangles[idx].height,
               rectangles[idx].area, rectangles[idx].perimeter,
               is_square(&rectangles[idx]) ? "Square" : "Non-Square");
    }
}

void update_rectangle(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("Enter rectangle ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = find_rectangle(rectangles, size, id);
    if (idx == -1) {
        printf("Rectangle with ID %d not found.\n", id);
        return;
    }

    printf("Current details: ID = %d, Width = %.2f, Height = %.2f\n",
           rectangles[idx].id, rectangles[idx].width, rectangles[idx].height);

    float w;
    float h;
    printf("Enter new width: ");
    if (scanf("%f", &w) != 1 || !isfinite(w) || w <= 0.0f) {
        printf("Invalid width. Update cancelled.\n");
        return;
    }

    printf("Enter new height: ");
    if (scanf("%f", &h) != 1 || !isfinite(h) || h <= 0.0f) {
        printf("Invalid height. Update cancelled.\n");
        return;
    }

    rectangles[idx].width = w;
    rectangles[idx].height = h;
    calculate_area(&rectangles[idx]);

    printf("Rectangle %d updated successfully.\n", id);
}

void delete_rectangle(Rectangle *rectangles, int *size) {
    if (*size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("Enter rectangle ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = find_rectangle(rectangles, *size, id);
    if (idx == -1) {
        printf("Rectangle with ID %d not found.\n", id);
        return;
    }

    for (int i = idx; i < *size - 1; i++) {
        rectangles[i] = rectangles[i + 1];
    }
    (*size)--;

    printf("Rectangle %d deleted successfully.\n", id);
}

void show_extremes(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int min_idx = 0;
    int max_idx = 0;
    for (int i = 1; i < size; i++) {
        if (rectangles[i].area < rectangles[min_idx].area) {
            min_idx = i;
        }
        if (rectangles[i].area > rectangles[max_idx].area) {
            max_idx = i;
        }
    }

    printf("\n--- Area Extremes ---\n");
    printf("Largest Area : %.2f (ID: %d)\n", rectangles[max_idx].area, rectangles[max_idx].id);
    printf("Smallest Area: %.2f (ID: %d)\n", rectangles[min_idx].area, rectangles[min_idx].id);
}

void show_totals(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    float total = 0.0f;
    for (int i = 0; i < size; i++) {
        total += rectangles[i].area;
    }

    printf("\n--- Area Statistics ---\n");
    printf("Total Area  : %.2f\n", total);
    printf("Average Area: %.2f\n", total / size);
}

void classify_shapes(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    printf("\n--- Shape Classifications ---\n");
    for (int i = 0; i < size; i++) {
        printf("Rectangle ID %d: %s (width: %.2f, height: %.2f)\n",
               rectangles[i].id, is_square(&rectangles[i]) ? "SQUARE" : "NON-SQUARE",
               rectangles[i].width, rectangles[i].height);
    }
}

void display_sorted(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    Rectangle *copy = (Rectangle *)malloc(size * sizeof(Rectangle));
    if (copy == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        copy[i] = rectangles[i];
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (copy[j].area > copy[j + 1].area ||
               (copy[j].area == copy[j + 1].area && copy[j].id > copy[j + 1].id)) {
                Rectangle temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
        }
    }

    printf("\n--- Rectangles Sorted by Area (Ascending) ---\n");
    for (int i = 0; i < size; i++) {
        printf("ID = %d, Width = %.2f, Height = %.2f, Area = %.2f, Perimeter = %.2f (%s)\n",
               copy[i].id, copy[i].width, copy[i].height,
               copy[i].area, copy[i].perimeter,
               is_square(&copy[i]) ? "Square" : "Non-Square");
    }

    free(copy);
}

void filter_by_threshold(Rectangle *rectangles, int size) {
    if (size == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    float threshold;
    printf("Enter minimum area threshold: ");
    if (scanf("%f", &threshold) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int matches = 0;
    printf("\n--- Rectangles with Area > %.2f ---\n", threshold);
    for (int i = 0; i < size; i++) {
        if (rectangles[i].area > threshold) {
            printf("ID = %d, Width = %.2f, Height = %.2f, Area = %.2f, Perimeter = %.2f\n",
                   rectangles[i].id, rectangles[i].width, rectangles[i].height,
                   rectangles[i].area, rectangles[i].perimeter);
            matches++;
        }
    }

    if (matches == 0) {
        printf("No rectangles match the given threshold.\n");
    }
}

int main(void) {
    int capacity = 0;
    int count = 0;

    printf("Enter initial rectangle capacity (1-%d): ", MAX_RECTANGLES);
    if (scanf("%d", &capacity) != 1 || capacity < 1 || capacity > MAX_RECTANGLES) {
        printf("Invalid capacity. Defaulting to 4.\n");
        capacity = 4;
    }

    Rectangle *rectangles = create_rectangle_array(capacity);
    if (rectangles == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int choice = 0;
    while (1) {
        printf("\n=== Rectangle Inventory Menu ===\n");
        printf("1. Add Rectangle\n");
        printf("2. Display All Rectangles\n");
        printf("3. Search Rectangle by ID\n");
        printf("4. Update Dimensions by ID\n");
        printf("5. Delete Rectangle by ID\n");
        printf("6. Show Largest and Smallest Area\n");
        printf("7. Show Total and Average Area\n");
        printf("8. Classify Squares vs Non-Squares\n");
        printf("9. Display Rectangles Sorted by Area\n");
        printf("10. Filter Rectangles by Area Threshold\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            break;
        }

        switch (choice) {
            case 1: {
                if (count >= MAX_RECTANGLES) {
                    printf("Maximum limit of %d reached.\n", MAX_RECTANGLES);
                    break;
                }

                int id;
                float w;
                float h;

                printf("Enter rectangle ID (positive integer): ");
                if (scanf("%d", &id) != 1 || id <= 0) {
                    printf("Invalid ID.\n");
                    break;
                }

                if (find_rectangle(rectangles, count, id) != -1) {
                    printf("Error: Rectangle with ID %d already exists.\n", id);
                    break;
                }

                printf("Enter width: ");
                if (scanf("%f", &w) != 1 || !isfinite(w) || w <= 0.0f) {
                    printf("Invalid width. Must be positive.\n");
                    break;
                }

                printf("Enter height: ");
                if (scanf("%f", &h) != 1 || !isfinite(h) || h <= 0.0f) {
                    printf("Invalid height. Must be positive.\n");
                    break;
                }

                if (count >= capacity) {
                    int new_capacity = capacity * 2;
                    if (new_capacity > MAX_RECTANGLES) {
                        new_capacity = MAX_RECTANGLES;
                    }
                    Rectangle *temp = (Rectangle *)realloc(rectangles, new_capacity * sizeof(Rectangle));
                    if (temp == NULL) {
                        printf("Failed to grow array.\n");
                        break;
                    }
                    rectangles = temp;
                    capacity = new_capacity;
                }

                add_rectangle(rectangles, count, id, w, h);
                count++;

                printf("Rectangle %d added successfully.\n", id);
                break;
            }
            case 2: {
                display_rectangles(rectangles, count);
                break;
            }
            case 3: {
                search_rectangle(rectangles, count);
                break;
            }
            case 4: {
                update_rectangle(rectangles, count);
                break;
            }
            case 5: {
                delete_rectangle(rectangles, &count);
                break;
            }
            case 6: {
                show_extremes(rectangles, count);
                break;
            }
            case 7: {
                show_totals(rectangles, count);
                break;
            }
            case 8: {
                classify_shapes(rectangles, count);
                break;
            }
            case 9: {
                display_sorted(rectangles, count);
                break;
            }
            case 10: {
                filter_by_threshold(rectangles, count);
                break;
            }
            case 11: {
                printf("Freeing memory and exiting...\n");
                free_rectangle_array(rectangles);
                return 0;
            }
            default: {
                printf("Invalid choice. Please choose 1 to 11.\n");
                break;
            }
        }
    }

    free_rectangle_array(rectangles);
    return 0;
}

// Extended: Interactive task-list manager supporting ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    STATUS_PENDING,
    STATUS_DONE
} TaskStatus;

// Enum for command types
typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

typedef struct {
    int id;
    char description[100];
    TaskStatus status;
} Task;

void remove_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int command_equals(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (tolower((unsigned char)s1[i]) != tolower((unsigned char)s2[i])) {
            return 0;
        }
        i++;
    }
    return (s1[i] == '\0' && s2[i] == '\0');
}

// Function to convert string to CommandType
CommandType get_command_type(const char* commandStr) {
    if (command_equals(commandStr, "ADD")) return CMD_ADD;
    if (command_equals(commandStr, "DELETE")) return CMD_DELETE;
    if (command_equals(commandStr, "VIEW")) return CMD_VIEW;
    if (command_equals(commandStr, "UPDATE")) return CMD_UPDATE;
    if (command_equals(commandStr, "COMPLETE")) return CMD_COMPLETE;
    if (command_equals(commandStr, "FIND")) return CMD_FIND;
    if (command_equals(commandStr, "HELP")) return CMD_HELP;
    if (command_equals(commandStr, "EXIT")) return CMD_EXIT;
    return CMD_UNKNOWN;
}

int find_task(Task *arr, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (arr[i].id == id) {
            return i;
        }
    }
    return -1;
}

void show_help(void) {
    printf("\nSupported Commands:\n");
    printf("  ADD      - Add a new task\n");
    printf("  DELETE   - Delete task by ID\n");
    printf("  VIEW     - List all tasks\n");
    printf("  UPDATE   - Update description of a task\n");
    printf("  COMPLETE - Mark a task as completed\n");
    printf("  FIND     - Find a task by ID\n");
    printf("  HELP     - Show command list\n");
    printf("  EXIT     - Free memory and exit\n\n");
}

void view_tasks(Task *arr, int count) {
    if (count == 0) {
        printf("Task list is empty.\n");
        return;
    }
    printf("\n--- Task List (%d) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("ID: %-5d | Status: %-7s | Description: %s\n",
               arr[i].id,
               arr[i].status == STATUS_DONE ? "DONE" : "PENDING",
               arr[i].description);
    }
    printf("\n");
}

// Main program
int main(void) {
    int capacity = 4;
    int count = 0;
    Task *tasks = (Task *)malloc(capacity * sizeof(Task));
    if (tasks == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    char input[100];
    CommandType cmd;

    while (1) {
        printf("Enter command (ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting program.\n");
            break;
        }
        remove_newline(input);

        if (strlen(input) == 0) {
            continue;
        }

        cmd = get_command_type(input);

        switch (cmd) {
            case CMD_ADD: {
                printf("Add command selected.\n");
                // Add functionality here
                int id;
                char desc[100];

                printf("Enter task ID (positive integer): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                remove_newline(input);

                if (sscanf(input, "%d", &id) != 1 || id <= 0) {
                    printf("Invalid task ID.\n");
                    break;
                }

                if (find_task(tasks, count, id) != -1) {
                    printf("Error: Task with ID %d already exists.\n", id);
                    break;
                }

                printf("Enter task description: ");
                if (fgets(desc, sizeof(desc), stdin) == NULL) {
                    break;
                }
                remove_newline(desc);

                if (strlen(desc) == 0) {
                    printf("Description cannot be empty.\n");
                    break;
                }

                if (count >= capacity) {
                    int new_capacity = capacity * 2;
                    Task *temp = (Task *)realloc(tasks, new_capacity * sizeof(Task));
                    if (temp == NULL) {
                        printf("Failed to grow task array.\n");
                        break;
                    }
                    tasks = temp;
                    capacity = new_capacity;
                }

                tasks[count].id = id;
                strcpy(tasks[count].description, desc);
                tasks[count].status = STATUS_PENDING;
                count++;

                printf("Task %d added successfully.\n", id);
                break;
            }
            case CMD_DELETE: {
                printf("Delete command selected.\n");
                // Delete functionality here
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }

                int id;
                printf("Enter task ID to delete: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                remove_newline(input);

                if (sscanf(input, "%d", &id) != 1 || id <= 0) {
                    printf("Invalid task ID.\n");
                    break;
                }

                int idx = find_task(tasks, count, id);
                if (idx == -1) {
                    printf("Task with ID %d not found.\n", id);
                    break;
                }

                for (int i = idx; i < count - 1; i++) {
                    tasks[i] = tasks[i + 1];
                }
                count--;

                printf("Task %d deleted successfully.\n", id);
                break;
            }
            case CMD_VIEW: {
                printf("View command selected.\n");
                // View functionality here
                view_tasks(tasks, count);
                break;
            }
            case CMD_UPDATE: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }

                int id;
                printf("Enter task ID to update: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                remove_newline(input);

                if (sscanf(input, "%d", &id) != 1 || id <= 0) {
                    printf("Invalid task ID.\n");
                    break;
                }

                int idx = find_task(tasks, count, id);
                if (idx == -1) {
                    printf("Task with ID %d not found.\n", id);
                    break;
                }

                char desc[100];
                printf("Current description: %s\n", tasks[idx].description);
                printf("Enter new description: ");
                if (fgets(desc, sizeof(desc), stdin) == NULL) {
                    break;
                }
                remove_newline(desc);

                if (strlen(desc) == 0) {
                    printf("Description cannot be empty.\n");
                    break;
                }

                strcpy(tasks[idx].description, desc);
                printf("Task %d updated successfully.\n", id);
                break;
            }
            case CMD_COMPLETE: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }

                int id;
                printf("Enter task ID to complete: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                remove_newline(input);

                if (sscanf(input, "%d", &id) != 1 || id <= 0) {
                    printf("Invalid task ID.\n");
                    break;
                }

                int idx = find_task(tasks, count, id);
                if (idx == -1) {
                    printf("Task with ID %d not found.\n", id);
                    break;
                }

                tasks[idx].status = STATUS_DONE;
                printf("Task %d marked as DONE.\n", id);
                break;
            }
            case CMD_FIND: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }

                int id;
                printf("Enter task ID to find: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                remove_newline(input);

                if (sscanf(input, "%d", &id) != 1 || id <= 0) {
                    printf("Invalid task ID.\n");
                    break;
                }

                int idx = find_task(tasks, count, id);
                if (idx == -1) {
                    printf("Task with ID %d not found.\n", id);
                    break;
                }

                printf("Found -> ID: %d | Status: %s | Description: %s\n",
                       tasks[idx].id,
                       tasks[idx].status == STATUS_DONE ? "DONE" : "PENDING",
                       tasks[idx].description);
                break;
            }
            case CMD_HELP: {
                show_help();
                break;
            }
            case CMD_EXIT: {
                printf("Exiting program.\n");
                free(tasks);
                return 0;
            }
            default: {
                printf("Unknown command.\n");
                break;
            }
        }
    }

    free(tasks);
    return 0;
}
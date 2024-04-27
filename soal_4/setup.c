#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_APPS 50
#define MAX_APP_NAME 50

pid_t running[MAX_APPS];
int pidcount = 0;

void add_global(pid_t pid) {
    if (pidcount < MAX_APPS) {
        running[pidcount++] = pid;
    }
}

void read_temp() {
    FILE *input_file = fopen(".temp.txt", "r");
    if (input_file == NULL) {
        printf("Error: Failed to open temp\n");
        return;
    }

    fscanf(input_file, "%d", &pidcount);
    for (int i = 0; i < pidcount; i++) {
        fscanf(input_file, "%d", &running[i]);
    }

    fclose(input_file);
}

void write_temp() {
    FILE *output_file = fopen(".temp.txt", "w");
    if (output_file == NULL) {
        printf("Error: Failed to open temp\n");
        return;
    }

    fprintf(output_file, "%d\n", pidcount);
    for (int i = 0; i < pidcount; i++) {
        fprintf(output_file, "%d\n", running[i]);
    }

    fclose(output_file);
}

void kill_temp() {
    for (int i = 0; i < pidcount; i++) {
        if (kill(running[i], SIGTERM) == -1) {
            perror("Error");
        }
    }
}

void kill_from_file(char *filename) {
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Error: Failed to open file\n");
        return;
    }

    char app[MAX_APP_NAME];
    int num;
    while (fscanf(input_file, "%s %d", app, &num) == 2) {
        for (int i = 0; i < num; i++) {
            pid_t pid = fork();
            if (pid < 0) {
                printf("Error: Fork failed\n");
                exit(1);
            }
            if (pid == 0) {
                execlp("pkill", "pkill", app, NULL);
                perror("Error");
                exit(1);
            }
        }
    }

    fclose(input_file);
}

void open_apps(int argc, char *argv[]) {
    for (int i = 2; i < argc; i += 2) {
        char *app_name = argv[i];
        int num_instances = atoi(argv[i + 1]);
        for (int j = 0; j < num_instances; j++) {
            pid_t pid = fork();
            if (pid < 0) {
                printf("Error: Fork failed\n");
                exit(1);
            }
            if (pid == 0) {
                execlp(app_name, app_name, NULL);
                perror("Error");
                exit(1);
            } else {
                add_global(pid);
            }
        }
    }
}

void open_apps_from_file(char *filename) {
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Error: Failed to open file\n");
        return;
    }

    char app[MAX_APP_NAME];
    int num;
    while (fscanf(input_file, "%s %d", app, &num) == 2) {
        for (int i = 0; i < num; i++) {
            pid_t pid = fork();
            if (pid < 0) {
                printf("Error: Fork failed\n");
                exit(1);
            }
            if (pid == 0) {
                execlp(app, app, NULL);
                perror("Error");
                exit(1);
            } else {
                add_global(pid);
            }
        }
    }

    fclose(input_file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [-o <app1> <num1> <app2> <num2> ...] [-f file.conf] [-k] [-k file.conf]\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-o") == 0 && argc > 2) {
        open_apps(argc, argv);
        write_temp();
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        open_apps_from_file(argv[2]);
        write_temp();
    } else if (strcmp(argv[1], "-k") == 0) {
        if (argc == 2) {
            kill_temp();
        } else if (argc == 3) {
            kill_from_file(argv[2]);
        } else {
            printf("Invalid arguments\n");
            printf("Usage: %s [-o <app1> <num1> <app2> <num2> ...] [-f file.conf] [-k] [-k file.conf]\n", argv[0]);
            exit(1);
        }
    } else {
        printf("Invalid arguments\n");
        printf("Usage: %s [-o <app1> <num1> <app2> <num2> ...] [-f file.conf] [-k] [-k file.conf]\n", argv[0]);
        exit(1);
    }

    return 0;
}

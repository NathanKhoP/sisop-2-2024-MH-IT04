#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h> 

#define MAX_APPS 10 
#define MAX_APP_NAME 50 

void open_apps(char *apps[], int num_apps) {
    pid_t pid;

    for (int i = 0; i < num_apps; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            execlp(apps[i], apps[i], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_apps; i++) {
        wait(NULL);
    }
}

void close_apps(char *apps[], int num_apps) {
    for (int i = 0; i < num_apps; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            execlp("pkill", "pkill", "-f", apps[i], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_apps; i++) {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [-o <app1> <num1> <app2> <num2> ... <appN> <numN>] [-f file.conf] [-k]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-o") == 0 && argc > 3 && (argc - 2) % 2 == 0) {
        int num_apps = (argc - 2) / 2;
        char *apps[MAX_APPS];
        int app_index = 0;

        for (int i = 2; i < argc; i += 2) {
            char *app_name = argv[i];
            int num_windows = atoi(argv[i + 1]);

            for (int j = 0; j < num_windows; j++) {
                apps[app_index++] = app_name;
            }
        }

        open_apps(apps, app_index);
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        FILE *file = fopen(argv[2], "r");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char app[MAX_APP_NAME];
        int num_windows;

        while (fscanf(file, "%s %d", app, &num_windows) == 2) {
            char *apps[MAX_APPS];

            for (int i = 0; i < num_windows; i++) {
                apps[i] = strdup(app);
            }

            open_apps(apps, num_windows);
        }

        fclose(file);
    } else if (strcmp(argv[1], "-k") == 0 && (argc == 2 || (argc == 3 && strcmp(argv[2], "-f") == 0))) {
        char *apps[] = {"firefox", "chrome"};
        close_apps(apps, sizeof(apps) / sizeof(apps[0]));
    } else {
        printf("Invalid arguments\n");
        printf("Usage: %s [-o <app1> <num1> <app2> <num2> ... <appN> <numN>] [-f file.conf] [-k]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}


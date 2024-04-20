#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>

#define LINK "https://docs.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup" // library.zip fixed link
#define FOLDER_PATH "/home/etern1ty/sisop_works/modul_2/soal_2/library"
#define MAX_LEN 1024

// Debug mode and production mode for the code, respectively

// #define DEBUG
#define PROD

// Modes
volatile sig_atomic_t program_mode = 0;

// User and time for logging
char *user;
time_t T;
struct tm tm;

void signalin(int signum) {
    if (signum == SIGRTMIN) program_mode = 0;
    else if (signum == SIGUSR1) program_mode = 1;
    else if (signum == SIGUSR2) program_mode = 2;
    else if (signum == SIGTERM) exit(EXIT_SUCCESS);
}

int checker(const char *fname) {
    char name[MAX_LEN] = "/home/etern1ty/sisop_works/modul_2/soal_2/";
    strcat(name, fname);
    if (access(name, F_OK) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void download_file() {
    char *argv[] = {"wget", "--content-disposition", LINK, "-P", "/home/etern1ty/sisop_works/modul_2/soal_2/", NULL};
    execv("/bin/wget", argv);
}

char rot19_alg(char input) {
    // NOTE : This is a reversed ROT19 algorithm, or normally a ROT7 algorithm
    if (input >= 'A' && input <= 'Z') {
        return 'A' + (input - 'A' - 19 + 26) % 26; // Normally + 7
    } 
    else if (input >= 'a' && input <= 'z') {
        return 'a' + (input - 'a' - 19 + 26) % 26;
    } 
    else {
        return input;
    }
}

void decrypt_file(char *fileName) {
    for (int i = 0; fileName[i]; i++) {
        fileName[i] = rot19_alg(fileName[i]);
    }
}

void unzip_file() {
    char *argv[] = {"unzip", "/home/etern1ty/sisop_works/modul_2/soal_2/library.zip", "-d", "/home/etern1ty/sisop_works/modul_2/soal_2", NULL};
    execv("/bin/unzip", argv);
}

void file_operations() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir(FOLDER_PATH);
    FILE *log_file = fopen("/home/etern1ty/sisop_works/modul_2/soal_2/history.log", "a"); // append mode

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *old_name = ep->d_name;
            if (old_name[0] >= '0' && old_name[0] <= '9') continue;
            if (strstr(old_name, "d3Let3") != NULL || strstr(old_name, "m0V3") != NULL || strstr(old_name, "r3N4mE") != NULL) continue;
            if (strstr(old_name, "backup") != NULL || strstr(old_name, "helper") != NULL || strstr(old_name, "calculator") != NULL || strstr(old_name, "server") != NULL || strstr(old_name, "renamed") != NULL) continue;

            char old_path[MAX_LEN], new_path[MAX_LEN];
            snprintf(old_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);

            char decrypted_name[MAX_LEN];
            strncpy(decrypted_name, ep->d_name, MAX_LEN - 1);
            decrypted_name[MAX_LEN - 1] = '\0';
            decrypt_file(decrypted_name);

            snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", decrypted_name);
            rename(old_path, new_path);
            
            #ifdef DEBUG
            fprintf(log_file, "Renamed file %s to %s\n", old_path, new_path);
            #endif
        }
    }
    #ifdef DEBUG
    fprintf(log_file, "\n\n-----------------------------------------------------------\n");
    fprintf(log_file, "Files Decrypted");
    fprintf(log_file, "\n-----------------------------------------------------------\n\n");
    #endif
    closedir(dir);
    fclose(log_file);
}

void file_processing() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir(FOLDER_PATH);
    FILE *log_file = fopen("/home/etern1ty/sisop_works/modul_2/soal_2/history.log", "a"); // append mode

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            char old_path[MAX_LEN], new_path[MAX_LEN];
            snprintf(old_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);

            if (strstr(file_name, "d3Let3") != NULL) {
                remove(old_path);

                #ifdef DEBUG
                fprintf(log_file, "[Decrypted : %s] Deleted file %s \n", file_name, old_path);
                #endif

                #ifdef PROD
                fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File successfully deleted.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
                #endif
                continue;
            }

            if (strstr(file_name, "r3N4mE") != NULL) {
                char *ext = strrchr(file_name, '.');
                if (ext != NULL) {
                    if (strcmp(ext, ".ts") == 0) {
                        snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/helper.ts");
                    } 
                    else if (strcmp(ext, ".py") == 0) {
                        snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/calculator.py");
                    } 
                    else if (strcmp(ext, ".go") == 0) {
                        snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/server.go");
                    } 
                    else {
                        snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/renamed.file");
                    }
                } 
                else {
                    snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/renamed.file");
                }
                rename(old_path, new_path);

                #ifdef DEBUG
                fprintf(log_file, "[Decrypted : %s] Renamed file %s to %s\n\n", file_name, old_path, new_path);
                #endif

                #ifdef PROD
                fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File successfully renamed.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
                #endif
            }
        }
    }
}

void default_func() {
    int status;
    if (checker("library.zip") == 0) {
        pid_t download_id = fork();
        if (download_id < 0) {
            printf("Fork Failed!\n");
            exit(EXIT_FAILURE);
        }
        else if (download_id == 0) {
            download_file();
        }
    }
    wait(&status);

    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        if (checker("library") == 0) {
            pid_t unzip_id = fork();
            if (unzip_id < 0) {
                printf("Fork Failed!\n");
                exit(EXIT_FAILURE);
            }
            else if (unzip_id == 0) {
                unzip_file();
            }
        }
    }
    wait(&status);

    // if (defaultmode == 1) { 
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        if (checker("library") == 1) {
            pid_t opr_id = fork();
            if (opr_id < 0) {
                printf("Fork Failed!\n");
                exit(EXIT_FAILURE);
            }
            else if (opr_id == 0) {
                file_operations();
                exit(EXIT_SUCCESS);
            }
        }
    }
    wait(&status);

    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        if (checker("library") == 1) {
            pid_t pro_id = fork();
            if (pro_id < 0) {
                printf("Fork Failed!\n");
                exit(EXIT_FAILURE);
            }
            else if (pro_id == 0) {
                file_processing();
                exit(EXIT_SUCCESS);
            }
        }
    }
    wait(&status);
}

void backup_func() {
    DIR *dir;
    struct dirent *ep;

    char *backup_dir = "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup";

    // check / create backup dir
    dir = opendir(backup_dir);
    if (dir == NULL) {
        mkdir(backup_dir, 0777); // perm
    } 
    else closedir(dir);

    dir = opendir(FOLDER_PATH);
    FILE *log_file = fopen("/home/etern1ty/sisop_works/modul_2/soal_2/history.log", "a"); // append mode

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            if (strstr(file_name, "m0V3") != NULL) {
                char old_path[MAX_LEN], new_path[MAX_LEN];
                snprintf(old_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);
                snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup/%s", ep->d_name);
                rename(old_path, new_path);

                #ifdef DEBUG
                fprintf(log_file, "[Backup : %s] Moved file %s to %s\n\n", file_name, old_path, new_path);
                #endif

                #ifdef PROD
                fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File backed up.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
                #endif
            }
        }
    }
    #ifdef DEBUG
    fprintf(log_file, "\n\n-----------------------------------------------------------\n");
    fprintf(log_file, "Files Backed Up");
    fprintf(log_file, "\n-----------------------------------------------------------\n\n");
    #endif
    fclose(log_file);
    closedir(dir);
}

void restore_func() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir("/home/etern1ty/sisop_works/modul_2/soal_2/library/backup");
    FILE *log_file = fopen("/home/etern1ty/sisop_works/modul_2/soal_2/history.log", "a"); // append mode

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            char old_path[MAX_LEN], new_path[MAX_LEN];
            snprintf(old_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup/%s", ep->d_name);
            snprintf(new_path, MAX_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);
            rename(old_path, new_path);

            #ifdef DEBUG
            fprintf(log_file, "[Restore : %s] Moved file %s to %s\n\n", file_name, old_path, new_path);
            #endif

            #ifdef PROD
            fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File successfully restored from backup.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
            #endif
        }
    }
    #ifdef DEBUG
    fprintf(log_file, "\n\n-----------------------------------------------------------\n");
    fprintf(log_file, "Files Restored");
    fprintf(log_file, "\n-----------------------------------------------------------\n\n");
    #endif
    fclose(log_file);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    // Signals
    signal(SIGRTMIN, signalin);
    signal(SIGUSR1, signalin);
    signal(SIGUSR2, signalin);
    signal(SIGTERM, signalin);

    // Init data for logging 
    user = (char *)malloc(10*sizeof(char));
    user = getlogin();
    T = time(NULL);
    tm = *localtime(&T);

    // Handling args
    if (argc == 1) {
        program_mode = 0;
    }
    else if (argc > 1) {
        if (strcmp(argv[1], "-m") == 0 && argc > 2) {
            if (strcmp(argv[2], "backup") == 0) {
                program_mode = 1;
            } else if (strcmp(argv[2], "restore") == 0) {
                program_mode = 2;
            }
        }
    }

    // Making the program run as a daemon
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        printf("Fork Failed!\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    if (setsid() < 0) {
        perror("Error: setsid() failed");
    }
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        switch (program_mode) {
            case 0:
                default_func();
                break;

            case 1:
                backup_func();
                break;

            case 2:
                restore_func();
                break;
        }

        #ifdef DEBUG
        sleep(30);
        #endif

        #ifdef PROD
        sleep(1);
        #endif
    }
    return 0;
}
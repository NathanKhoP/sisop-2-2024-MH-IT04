#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>

#define BUFFER_SIZE 1000
#define TMP_FILE "/Users/macbook/Kuliah/Sistem Operasi/modul-sisop-2/soal_1/replace.tmp"
#define LOG_FILE "/Users/macbook/Kuliah/Sistem Operasi/modul-sisop-2/soal_1/virus.log"

void replaceAll(char* str, const char* oldWord, const char* newWord) {
  char* pos, temp[BUFFER_SIZE];
  int index = 0;
  int owlen;

  owlen = strlen(oldWord);

  if (!strcmp(oldWord, newWord)) {
    return;
    }

  while ((pos = strstr(str, oldWord)) != NULL) {
    strcpy(temp, str);
    index = pos - str;
    str[index] = '\0';
    strcat(str, newWord);
    strcat(str, temp + index + owlen);
    }
  }

void log_message(const char* file_name) {
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];
  char log_message[128];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, sizeof(buffer), "[%d-%m-%Y][%H:%M:%S] ", timeinfo);
  printf("%s", buffer);

  snprintf(log_message, sizeof(log_message), "Suspicious string at %s successfully replaced!\n", file_name);
  printf("%s", log_message);
  }


int main(int argc, char* argv[]) {
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process
  int log_fd;

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
    }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
    }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
    }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);


  while (1) {

    log_fd = open(LOG_FILE, O_CREAT | O_WRONLY | O_APPEND, 0640);
    if (log_fd < 0) {
      exit(EXIT_FAILURE);
      }

    // Mengarahkan stdout dan stderr ke file log
    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);

    // Buka file log
    if (argc < 2) {
      return 1;
      }

    char* path = argv[1];
    FILE* fPtr, * fTemp;
    char buffer[BUFFER_SIZE];

    fPtr = fopen(path, "r");
    fTemp = fopen(TMP_FILE, "w");

    if (fPtr == NULL || fTemp == NULL) {
      printf("\nUnable to open file.\n");
      printf("Please check whether file exists and you have read/write privilege.\n");
      return 1;
      }

    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL) {
      replaceAll(buffer, "m4LwAr3", "[MALWARE]");
      replaceAll(buffer, "5pYw4R3", "[SPYWARE]");
      replaceAll(buffer, "R4nS0mWaR3", "[RANSOMWARE]");
      fputs(buffer, fTemp);
      }

    log_message(path);

    fclose(fPtr);
    fclose(fTemp);

    remove(path);
    rename("/Users/macbook/Kuliah/Sistem Operasi/modul-sisop-2/soal_1/replace.tmp", path);

    sleep(15);
    }

  close(log_fd);
  exit(EXIT_SUCCESS);
  }
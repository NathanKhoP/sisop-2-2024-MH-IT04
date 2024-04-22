#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h> 


#define MAX_LINE_LENGTH 256

void monitorProcess(int monitorMode, const char* user) {
  char cmd[MAX_LINE_LENGTH];
  FILE* ps_output;
  char line[MAX_LINE_LENGTH];
  char comm[MAX_LINE_LENGTH];
  char log_filename[MAX_LINE_LENGTH];
  FILE* log_file;
  int pid_process;

  // Mengambil pid dan nama proses
  snprintf(cmd, sizeof(cmd), "ps -u %s -o pid,comm | awk 'NR>1 {print $1, $2}'", user);
  ps_output = popen(cmd, "r");
  if (ps_output == NULL) {
    perror("Failed to open pipe");
    exit(EXIT_FAILURE);
    }

  // Buka log file
  snprintf(log_filename, sizeof(log_filename), "%s.log", user);
  log_file = fopen(log_filename, "a");
  if (log_file == NULL) {
    perror("Failed to open log file");
    exit(EXIT_FAILURE);
    }

  // Baca output dari ps per baris
  while (fgets(line, sizeof(line), ps_output) != NULL) {
    // Parsing baris untuk mendapatkan PID dan nama proses
    if (sscanf(line, "%d %s", &pid_process, comm) != 2) {
      fprintf(stderr, "Failed to parse line: %s", line);
      continue;
      }

    // Mendapatkan waktu saat ini
    time_t now;
    struct tm* local_time;
    char timestamp[30];
    time(&now);
    local_time = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "[%d:%m:%Y]-[%H:%M:%S]", local_time);

    // Format data dan simpan ke log file
    if (monitorMode == 1) {
      fprintf(log_file, "%s-%d-%s_JALAN\n", timestamp, pid_process, comm);
      }
    else if (monitorMode == 2) {
      // Pastikan untuk gunakan user root untuk menjalankan program `su root`
      int result = kill(pid_process, SIGTERM);
      if (result == 0) {
        fprintf(log_file, "%s-%d-%s_GAGAL\n", timestamp, pid_process, comm);
        }
      else {
        perror("kill");
        fprintf(log_file, "GAGAL DI KILL%d-%s_\n", pid_process, comm);
        exit(EXIT_FAILURE);
        }
      }
    }

  // Tutup pipe dan log file
  pclose(ps_output);
  fclose(log_file);
  }

int main(int argc, char* argv[]) {
  char process_name[MAX_LINE_LENGTH];
  pid_t pid, sid;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <username> | -m <username> | -s <username> | -c <username> | -a <username>\n", argv[0]);
    exit(EXIT_FAILURE);
    }

  int monitorMode = 0;
  char* user = argv[1];

  if (argc == 2) {
    char* args[] = { "ps", "-u", user, NULL };
    execvp(args[0], args);
    perror("execvp failed");

    return 1;
    }

  if (argc == 2 && strcmp(argv[1], "-m") == 0) {
    fprintf(stderr, "Usage: %s -m <username>\n", argv[0]);
    exit(EXIT_FAILURE);
    }

  pid = fork(); // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    fprintf(stderr, "gagal membuat fork");
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

  if (argc == 3 && strcmp(argv[1], "-m") == 0) {
    fprintf(stderr, "Memulai proses monitoring user %s", argv[2]);
    monitorMode = 1;
    user = argv[2];
    }

  if (argc == 3 && strcmp(argv[1], "-s") == 0) {
    snprintf(process_name, sizeof(process_name), "%s -m %s", argv[0], argv[2]);
    char* args[] = { "pkill", "-f", process_name, NULL };
    execvp(args[0], args);
    perror("execvp failed");
    return 1;
    }

  if (argc == 3 && strcmp(argv[1], "-c") == 0) {
    fprintf(stderr, "Memulai proses gagalkan monitoring user %s", argv[2]);
    monitorMode = 2;
    user = argv[2];
    }

  if (argc == 3 && strcmp(argv[1], "-a") == 0) {
    snprintf(process_name, sizeof(process_name), "%s -c %s", argv[0], argv[2]);
    char* args[] = { "pkill", "-f", process_name, NULL };
    execvp(args[0], args);
    perror("execvp failed");
    return 1;
    }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1) {
    if (monitorMode == 1 || monitorMode == 2) {
      monitorProcess(monitorMode, user);
      }

    sleep(1);
    }

  exit(EXIT_SUCCESS);
  return 0;
  }

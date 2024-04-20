#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <user>\n", argv[0]);
    return 1;
    }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork failed");
    return 1;
    }

  if (pid == 0) {
    // Ini adalah proses anak
    char* args[] = { "ps", "-u", argv[1], NULL };
    execvp(args[0], args);
    perror("execvp failed"); // Jika execvp gagal, program akan mencapai baris ini
    return 1;
    }
  else {
    // Ini adalah proses induk
    int status;
    waitpid(pid, &status, 0); // Menunggu proses anak selesai
    return 0;
    }
  }

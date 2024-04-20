//Langkah Pertama,adalah membuat file yang nanti akan digunakan sebagai setup.c,
//membuat filenya,"gedit nama file(setup.c)"
//Langkah Kedua,
//untuk kode setup.c
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h> 

#define MAX_APPS 10 // Maksimum jumlah aplikasi yang dapat dibuka
#define MAX_APP_NAME 50 // Maksimum panjang nama aplikasi

// Fungsi untuk membuka aplikasi
void open_apps(char *apps[], int num_apps) {
    pid_t pid; // Variabel untuk menyimpan ID proses

    for (int i = 0; i < num_apps; i++) { // Loop untuk setiap aplikasi
        pid = fork(); // Membuat proses anak

        if (pid < 0) { // Jika gagal membuat proses anak
            perror("fork"); // Menampilkan pesan kesalahan
            exit(EXIT_FAILURE); // Keluar dari program dengan status kesalahan
        } else if (pid == 0) { // Jika proses anak
            execlp(apps[i], apps[i], NULL); // Menjalankan aplikasi
            perror("execlp"); // Menampilkan pesan kesalahan jika execlp gagal
            exit(EXIT_FAILURE); // Keluar dari proses anak dengan status kesalahan
        }
    }

    // Menunggu semua proses anak selesai
    for (int i = 0; i < num_apps; i++) {
        wait(NULL); // Menunggu proses anak selesai
    }
}

// Fungsi untuk menutup aplikasi
void close_apps(char *apps[], int num_apps) {
    for (int i = 0; i < num_apps; i++) { // Loop untuk setiap aplikasi
        pid_t pid = fork(); // Membuat proses anak

        if (pid < 0) { // Jika gagal membuat proses anak
            perror("fork"); // Menampilkan pesan kesalahan
            exit(EXIT_FAILURE); // Keluar dari program dengan status kesalahan
        } else if (pid == 0) { // Jika proses anak
            execlp("pkill", "pkill", "-f", apps[i], NULL); // Menutup aplikasi
            perror("execlp"); // Menampilkan pesan kesalahan jika pkill gagal
            exit(EXIT_FAILURE); // Keluar dari proses anak dengan status kesalahan
        }
    }

    // Menunggu semua proses anak selesai
    for (int i = 0; i < num_apps; i++) {
        wait(NULL); // Menunggu proses anak selesai
    }
}

// Fungsi utama
int main(int argc, char *argv[]) {
    // Menampilkan pesan jika argumen kurang dari 2
    if (argc < 2) {
        printf("Usage: %s [-o <app1> <num1> <app2> <num2> ... <appN> <numN>] [-f file.conf] [-k]\n", argv[0]);
        exit(EXIT_FAILURE); // Keluar dari program dengan status kesalahan
    }

    // Mengecek opsi untuk membuka aplikasi
    if (strcmp(argv[1], "-o") == 0 && argc > 3 && (argc - 2) % 2 == 0) {
        // Menghitung jumlah aplikasi yang akan dibuka
        int num_apps = (argc - 2) / 2;
        char *apps[MAX_APPS]; // Array untuk menyimpan nama aplikasi
        int app_index = 0;

        // Mengisi array apps dengan nama aplikasi sesuai argumen
        for (int i = 2; i < argc; i += 2) {
            char *app_name = argv[i]; // Nama aplikasi
            int num_windows = atoi(argv[i + 1]); // Jumlah jendela aplikasi

            // Menambahkan nama aplikasi ke dalam array sebanyak num_windows
            for (int j = 0; j < num_windows; j++) {
                apps[app_index++] = app_name;
            }
        }

        // Memanggil fungsi untuk membuka aplikasi
        open_apps(apps, app_index);
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        // Mengecek opsi untuk membuka aplikasi dari file konfigurasi
        FILE *file = fopen(argv[2], "r"); // Membuka file konfigurasi

        // Menampilkan pesan kesalahan jika file tidak dapat dibuka
        if (file == NULL) {
            perror("fopen"); // Menampilkan pesan kesalahan
            exit(EXIT_FAILURE); // Keluar dari program dengan status kesalahan
        }

        char app[MAX_APP_NAME]; // Variabel untuk menyimpan nama aplikasi
        int num_windows; // Variabel untuk menyimpan jumlah jendela

        // Membaca file konfigurasi dan membuka aplikasi sesuai dengan isi file
        while (fscanf(file, "%s %d", app, &num_windows) == 2) {
            char *apps[MAX_APPS]; // Array untuk menyimpan nama aplikasi

            // Menambahkan nama aplikasi ke dalam array sebanyak num_windows
            for (int i = 0; i < num_windows; i++) {
                apps[i] = strdup(app);
            }

            // Memanggil fungsi untuk membuka aplikasi
            open_apps(apps, num_windows);
        }

        fclose(file); // Menutup file konfigurasi setelah selesai membaca
    } else if (strcmp(argv[1], "-k") == 0 && (argc == 2 || (argc == 3 && strcmp(argv[2], "-f") == 0))) {
        // Mengecek opsi untuk menutup aplikasi
        if (argc == 2) {
            // Menutup semua aplikasi yang dibuka
            char *apps[] = {"firefox", "wireshark"}; // Menambahkan aplikasi lain jika diperlukan
            close_apps(apps, sizeof(apps) / sizeof(apps[0]));
        } else {
            // Menutup aplikasi sesuai dengan file konfigurasi
            FILE *file = fopen(argv[3], "r"); // Membuka file konfigurasi

            // Menampilkan pesan kesalahan jika file tidak dapat dibuka
            if (file == NULL) {
                perror("fopen"); // Menampilkan pesan kesalahan
                exit(EXIT_FAILURE); // Keluar dari program dengan status kesalahan
            }

            char app[MAX_APP_NAME]; // Variabel untuk menyimpan nama aplikasi
            int num_windows; // Variabel untuk menyimpan jumlah jendela

            // Membaca file konfigurasi dan menutup aplikasi sesuai dengan isi file
            while (fscanf(file, "%s %d", app, &num_windows) == 2) {
                char *apps[MAX_APPS]; // Array untuk menyimpan nama aplikasi

                // Menambahkan nama aplikasi ke dalam array sebanyak num_windows
                for (int i = 0; i < num_windows; i++) {
                    apps[i] = strdup(app);
                }

                // Memanggil fungsi untuk menutup aplikasi
                close_apps(apps, num_windows);
            }

            fclose(file); // Menutup file konfigurasi setelah selesai membaca
        }
    } else {
        // Menampilkan pesan jika argumen tidak valid
        printf("Invalid arguments\n");
        printf("Usage: %s [-o <app1> <num1> <app2> <num2> ... <appN> <numN>] [-f file.conf] [-k]\n", argv[0]);
        exit(EXIT_FAILURE); // Keluar dari program dengan status kesalahan
    }

    return 0; // Keluar dari program dengan status sukses
}

//untuk pengujiannya sebagai berikut,kita harus mencompiler kodenya dengan,
//(gcc setup.c -o setup)

//contoh pengujian pertama,
//(./setup -o firefox 2 wireshark 3)

//contoh pengujian kedua,
//(./setup -f file.conf)
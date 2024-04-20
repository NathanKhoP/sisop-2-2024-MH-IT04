# Repository Praktikum Sistem Operasi Modul 2 - IT04
## Anggota

| Nama                      | NRP        |
|---------------------------|------------|
|Nathan Kho Pancras         | 5027231002 |
|Athalla Barka Fadhil       | 5027231018 |
|Muhammad Ida Bagus Rafi H  | 5027221059 |


## Daftar Isi

- [Soal 1](#soal-1)
- [Soal 2](#soal-2)
- [Soal 3](#soal-3)
- [Soal 4](#soal-4)

# Soal 1

**Dikerjakan oleh Athalla Barka Fadhil (5027231018)**

## Deskripsi Soal

Gavriel adalah seorang cyber security enthusiast. Suatu hari, ia dikontrak oleh sebuah perusahaan ternama untuk membuat sebuah program yang cukup rumit dengan bayaran jutaan rupiah. Karena tergoda dengan nominal bayaran tersebut, Gavriel langsung menerima tawaran tersebut. Setelah mencoba membuat dan mengembangkan program tersebut selama seminggu, laptop yang digunakan Gavriel mengalami overheat dan mati total sehingga harus dilarikan ke tukang servis terdekat. Karena keterbatasan waktu dalam pembuatan program dan tidak ingin mengecewakan perusahaan, Gavriel meminta bantuan kalian untuk membuat program tersebut

### Catatan
- Struktur Folder:
```
    soal_1
    └── virus.c
```


- Beberapa defined variable:
```c
#define BUFFER_SIZE 1000
#define TMP_FILE "/Users/macbook/Kuliah/Sistem Operasi/modul-sisop-2/soal_1/replace.tmp"
#define LOG_FILE "/Users/macbook/Kuliah/Sistem Operasi/modul-sisop-2/soal_1/virus.log"
```
## Pengerjaan

```c
void replace_string(char* str, const char* oldWord, const char* newWord) {
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
```
Fungsi `replace_string` digunakan untuk mereplace string, menerima argumen `str` untuk buffer dan sesuai namanya `oldWord` dan `newWord`

---

```c
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
```
Fungsi `log_message` digunakan untuk mengformat timestamp dan format message log dan menerima argumen `file_name` yang nantinya akan diisi dengan nama file lognya

---

```c
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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
    }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  ....
}
```
Untuk detailnya sebenernya simpel saja, menggunakan `strftime` untuk melakukan format timestampnya lalu `snprintf` untuk mengformat log messagenya

Implementasi Daemon, disini saya ambil dari modul

---

```c
.... 
// didalam int main()
while (1) {

    log_fd = open(LOG_FILE, O_CREAT | O_WRONLY | O_APPEND, 0640);
    if (log_fd < 0) {
      exit(EXIT_FAILURE);
      }

    // Mengarahkan stdout dan stderr ke file log
    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);

    // Cek argumen command line
    if (argc < 2) {
      return 1;
      }
      ....
}
```
Sekarang kita masuk ke program intinya, block `while(1){...}` disini agar program berjalan terus menerus lalu untuk isinya kita mengassign `log_fd` dengan  `open()` yang digunakan untuk membuka log filenya, dan memberi akses untuk crete, write, dan append dengan mode 0640 lalu kita arahkan output`stdout` dan `stderr` ke file log, kita juga ada pengecekan argumen command linenya, kalo ga menspecify filenya yaitu `argv[1]` atau `argc < 2` maka kita `return 1`

---

```c
....
char* path = argv[1]; // Menggunakan argumen pertama sebagai path file
    FILE* fPtr, * fTemp;
    char buffer[BUFFER_SIZE];

    fPtr = fopen(path, "r");
    fTemp = fopen(TMP_FILE, "w");

    if (fPtr == NULL || fTemp == NULL) {
      printf("\nUnable to open file.\n");
      printf("Please check whether file exists and you have read/write privilege.\n");
      return 1;
      }
```
Disini kita buat argumen pertama dari cmd kita pake sebagai path file. Fungsi `fopen()` digunakan untuk membuka file dengan mode "r" untuk file utama dan "w" untuk file sementara. Misal salah satu dari kedua file tersebut gagal dibuka, program akan mencetak pesan kesalahan dan exit dengan  `return 1`.

---

```c
{
....
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL) {
      replace_string(buffer, "m4LwAr3", "[MALWARE]");
      replace_string(buffer, "5pYw4R3", "[SPYWARE]");
      replace_string(buffer, "R4nS0mWaR3", "[RANSOMWARE]");
      fputs(buffer, fTemp);
    }

    log_message(path); // Mencetak pesan log dengan format yang diminta

    fclose(fPtr);
    fclose(fTemp);

    remove(path);
    rename("/Users/macbook/Kuliah/Sistem Operasi/modul-sisop-2/soal_1/replace.tmp", path);

    sleep(15);
    }

close(log_fd);
exit(EXIT_SUCCESS);

```
Dalam bagian ini, program membaca file baris per baris dengan menggunakan `fgets()` dan menyimpannya di dalam buffer. Setiap baris kemudian diubah dengan menggunakan fungsi `replace_string()` untuk mengganti string tertentu dengan string pengganti yang sesuai. Hasilnya ditulis ke dalam file sementara.

Setelah selesai membaca dan mengubah semua baris, pesan log dicetak dengan memanggil fungsi `log_message()` dengan format yang diminta. File utama dan file sementara ditutup dengan `fclose()`.

Kemudian, file utama dihapus dan file sementara direname menjadi file utama menggunakan fungsi `remove()` dan `rename()`. Program kemudian tidur selama 15 detik sebelum mengulang prosesnya.

Setelah keluar dari loop, file log ditutup, dan program keluar dengan status keluaran yang sukses.

---

# Soal 2

**Dikerjakan oleh Nathan Kho Pancras (5027231002)**

## Deskripsi Soal

Paul adalah seorang mahasiswa semester 4 yang diterima magang di perusahaan XYZ. Pada hari pertama magang, ia diberi tugas oleh atasannya untuk membuat program manajemen file sederhana. Karena kurang terbiasa dengan bahasa C dan environment Linux, ia meminta bantuan kalian untuk mengembangkan program tersebut.

### Catatan
- Struktur folder:
```
    soal_2/
    ├── history.log
    ├── management.c
    └── library/
        └── backup/
```

- Beberapa defined variable:
```c
#define LINK "https://docs.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup"
#define FOLDER_PATH "/home/etern1ty/sisop_works/modul_2/soal_2/library"
#define MAX_PATH_LEN 1024
```

- Memakai fungsi rename() dan remove() dari C, bukan mv/rm menggunakan exec + fork
## Pengerjaan

> a. Atasannya meminta agar program tersebut dapat berjalan secara daemon dan dapat mengunduh serta melakukan unzip terhadap file berikut. Atasannya juga meminta program ini dibuat tanpa menggunakan command system()

Pertama, saya membuat program ini berjalan secara daemon sesuai dengan contoh yang diberikan di modul (menggunakan pid, sid, umask, close). Kemudian saya membuat kode untuk mengunduh dan unzip file yang telah disediakan didalam while (1) seperti ini:

```c
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
```

Sebelum melakukan download/unzip, dilakukan fork terlebih dahulu untuk membuat child process. Detail dari fungsi download/unzip file:

```c
void download_file() {
    char *argv[] = {"wget", "--content-disposition", LINK, "-P", "/home/etern1ty/sisop_works/modul_2/soal_2/", NULL};
    execv("/bin/wget", argv);
}

void unzip_file() {
    char *argv[] = {"unzip", "/home/etern1ty/sisop_works/modul_2/soal_2/library.zip", "-d", "/home/etern1ty/sisop_works/modul_2/soal_2", NULL};
    execv("/bin/unzip", argv);
}
```

**NOTE: Checker merupakan fungsi untuk melakukan checking apakah file di dir modul_2/soal_2/<...> ada atau tidak, agar tidak melakukan download/unzip berkali-kali.** Detail dari fungsi checker:

```c
int checker(const char *fname) {
    char name[MAX_PATH_LEN] = "/home/etern1ty/sisop_works/modul_2/soal_2/";
    strcat(name, fname);
    if (access(name, F_OK) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}
```

Dengan ini, subnomor a selesai dimana file library.zip sukses di download dan di unzip.

### Solusi

> b. Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma ROT19

### Solusi

Pertama, saya membuat fungsi file_operations, decrypt_file, dan rot19 untuk operasi-operasi directory listing dan melakukan decrypt (dengan ROT19, sebenarnya ROT7, tetapi ROT19 jika shift kebelakang) dan rename file setelah di decrypt.

```c
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

void file_operations() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir(FOLDER_PATH);

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *old_name = ep->d_name;
            if (old_name[0] >= '0' && old_name[0] <= '9') continue;
            if (strstr(old_name, "d3Let3") != NULL || strstr(old_name, "m0V3") != NULL || strstr(old_name, "r3N4mE") != NULL) continue;

            char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
            snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);

            char decrypted_name[MAX_PATH_LEN];
            strncpy(decrypted_name, ep->d_name, MAX_PATH_LEN - 1);
            decrypted_name[MAX_PATH_LEN - 1] = '\0';
            decrypt_file(decrypted_name);

            snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", decrypted_name);
            rename(old_path, new_path);
        }
    }
    closedir(dir);
}
```

if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue; dilakukan dikarenakan cara kerja readdir, dimana akan melakukan read pada directory awal (.) dan sebelum ini (..) sehingga dilakukan skip.

Kemudian menggunakan ep->d_name untuk mengakses nama file dan di declare ke old_name, kemudian dilakukan decrypt (menggunakan strncpy untuk decrpyted_name karena adanya mismatch tipe variabel) dan dilakukan rename dari old_path ke new_path.

Setelah mengamati file-file di folder library, dan karena keterbatasan readdir (mengambil file tidak dengan urutan yang alphabetically sorted) maka saya membuat solusi untuk menambah if statement berikut:

```c
if (old_name[0] >= '0' && old_name[0] <= '9') continue;
if (strstr(old_name, "d3Let3") != NULL || strstr(old_name, "m0V3") != NULL || strstr(old_name, "r3N4mE") != NULL) continue;
```

Karena file-file awal (alphabetically sorted) ada angka di depannya, maka saya set misal [0] berupa angka, skip ke file selanjutnya.
Kemudian jika file sudah memiliki kata-kata d3Let3, m0Ve, dan r3N4mE maka akan di skip agar tidak didecrypt ulang.

Penggunaan snprintf dilakukan agar tidak perlu inisialisasi variabel char[], dan bisa langsung melakukan rename dengan fungsi bawaan C.

Di int main:

```c
if (WIFEXITED(status) && !WEXITSTATUS(status)) {
    if (checker("library") == 1 && decmark == 0) {
        pid_t opr_id = fork();
        if (opr_id < 0) {
            printf("Fork Failed!\n");
            exit(EXIT_FAILURE);
        }
        else if (opr_id == 0) {
            file_operations();
            exit(0);
        }
        else {
            wait(NULL);
            decmark = 1;
        }
    }
}
wait(&status);
```

Adanya global variable baru, decmark dengan value awal 0. Berubah menjadi 1 saat file_operations dilakukan agar tidak melakukan decrypt ulang file.

> c. Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: r3N4mE, d3Let3, dan m0V3. Untuk setiap file dengan nama yang memuat kode d3Let3, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode r3N4mE, lakukan hal berikut:
>> Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
>
>> Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
>
>> Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
>
>> Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”

### Solusi

Saya pun membuat fungsi file_processsing dengan mekanisme dasar yang mirip dengan file_operations.

```c
void file_processing() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir(FOLDER_PATH);

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
            snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);

            if (strstr(file_name, "d3Let3") != NULL) {
                remove(old_path);
                continue;
            }

            if (strstr(file_name, "r3N4mE") != NULL) {
                char *ext = strrchr(file_name, '.');
                if (ext != NULL) {
                    if (strcmp(ext, ".ts") == 0) {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/helper.ts");
                    } 
                    else if (strcmp(ext, ".py") == 0) {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/calculator.py");
                    } 
                    else if (strcmp(ext, ".go") == 0) {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/server.go");
                    } 
                    else {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/renamed.file");
                    }
                } 
                else {
                    snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/renamed.file");
                }
                rename(old_path, new_path);
            }
        }
    }
}
```

Di int main:
```c
if (WIFEXITED(status) && !WEXITSTATUS(status)) {
    if (checker("library") == 1 && oprmark == 0) {
        pid_t pro_id = fork();
        if (pro_id < 0) {
            printf("Fork Failed!\n");
            exit(EXIT_FAILURE);
        }
        else if (pro_id == 0) {
            file_processing();
            oprmark = 1;
            exit(EXIT_SUCCESS);
        }
    }
}
wait(&status);
```

Variabel oprmark melakukan hal yang sama dengan decmark.

Untuk delete file, saya menggunakan strstr untuk mencari substring dari decrypted_file, jika mengandung d3L3te maka bisa menggunakan fungsi remove dari C ke old_path.

Untuk rename mirip, hanya saja ada penggunaan strrchr (mark lokasi extension yaitu .) dan strcmp (compare kata setelah lokasi extension).

> d. Atasan Paul juga meminta agar program ini dapat membackup dan merestore file. Oleh karena itu, bantulah Paul untuk membuat program ini menjadi 3 mode, yaitu:
>
>> default: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command ./management saja
>
>> backup: program memindahkan file dengan kode m0V3 ke sebuah folder bernama “backup”
>
>> restore: program mengembalikan file dengan kode m0V3 ke folder sebelum file tersebut dipindahkan
>
>> Contoh penggunaan: ./management -m backup

### Solusi

Melalui contoh penggunaan, dapat diketahui bahwa kita perlu memainkan argumen di int main.

```c
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
```

Karena mode backup dan restore hanya akan dijalankan ketika ada argumen, maka misal argc == 1 otomatis melakukan default mode. Kemudian dilakukan pengecekan argumen yang diinput jika argc > 1 terhadap argv untuk menentukan program berjalan dalam backup atau restore mode.

Membuat variabel mode (global):

```c
volatile sig_atomic_t program_mode = 0;
```

Didalam while(1):

```c
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
```

Saya memindahkan block fungsi yang telah saya buat untuk sub-sub nomor sebelumnya ke fungsi default_func, kemudian saya membuat fungsi backup dan restore.

```c
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

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            if (strstr(file_name, "m0V3") != NULL) {
                char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
                snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);
                snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup/%s", ep->d_name);
                rename(old_path, new_path);
            }
        }
    }
    closedir(dir);
}

void restore_func() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir("/home/etern1ty/sisop_works/modul_2/soal_2/library/backup");

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
            snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup/%s", ep->d_name);
            snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);
            rename(old_path, new_path);
        }
    }
    closedir(dir);
}
```

Untuk backup, saya menambahkan mekanisme untuk membuat folder backup jika belum ada. Kemudian untuk memindahkan file memakai fungsi rename, sehingga mirip dengan file_operations dan file_processing. Restore hanya membalikkan old_path dan new_path.

> e. Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
>> SIGRTMIN untuk mode default
>
>> SIGUSR1 untuk mode backup
>
>> SIGUSR2 untuk mode restore
>
>> Contoh penggunaan: kill -SIGUSR2 <pid_program>

### Solusi

Saya membuat fungsi baru untuk handling signal dan assign program_mode sesuai signal yang diterima oleh daemon.

```c
void signalin(int signum) {
    if (signum == SIGRTMIN) program_mode = 0;
    else if (signum == SIGUSR1) program_mode = 1;
    else if (signum == SIGUSR2) program_mode = 2;
}
```

Di int main:

```c
signal(SIGRTMIN, signalin);
signal(SIGUSR1, signalin);
signal(SIGUSR2, signalin);
```

> f. Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien

### Solusi

Saya hanya perlu untuk menambahkan signal baru (SIGTERM) agar bisa melakukan kill dengan aman.

```c
void signalin(int signum) {
    if (signum == SIGRTMIN) program_mode = 0;
    else if (signum == SIGUSR1) program_mode = 1;
    else if (signum == SIGUSR2) program_mode = 2;
    else if (signum == SIGTERM) exit(EXIT_SUCCESS);
}
```

Di int main:

```c
signal(SIGRTMIN, signalin);
signal(SIGUSR1, signalin);
signal(SIGUSR2, signalin);
signal(SIGTERM, signalin);
```

> g. Terakhir, program ini harus berjalan setiap detik dan mampu mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
>> Format: [nama_user][HH:MM:SS] - <nama_file> - <action>
>
>> nama_user adalah username yang melakukan action terhadap file
>
>> Format action untuk setiap kode:
>
>>> kode r3N4mE: Successfully renamed.
>
>>> kode d3Let3: Successfully deleted.
>
>>> mode backup: Successfully moved to backup.
>
>>> mode restore: Successfully restored from backup.
>
>> Contoh pesan log:
>>> [paul][00:00:00] - r3N4mE.ts - Successfully renamed.
>
>>> [paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.

### Solusi

Pertama, saya membuat beberapa global variable untuk user dan time dan melakukan initialization di int main.

```c
char *user;
time_t T;
struct tm tm;
```

Di int main:
```c
user = (char *)malloc(10*sizeof(char));
user = getlogin();
T = time(NULL);
tm = *localtime(&T);
```
Kemudian saya hanya perlu untuk membuat mekanisme untuk fopen ke history.log pada setiap fungsi yang melakukan rename, delete, backup, dan restore.

**file_processing**

```c
void file_processing() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir(FOLDER_PATH);
    FILE *log_file = fopen("/home/etern1ty/sisop_works/modul_2/soal_2/history.log", "a"); // append mode

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
            snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);

            if (strstr(file_name, "d3Let3") != NULL) {
                remove(old_path);
                fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File successfully deleted.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
                continue;
            }

            if (strstr(file_name, "r3N4mE") != NULL) {
                char *ext = strrchr(file_name, '.');
                if (ext != NULL) {
                    if (strcmp(ext, ".ts") == 0) {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/helper.ts");
                    } 
                    else if (strcmp(ext, ".py") == 0) {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/calculator.py");
                    } 
                    else if (strcmp(ext, ".go") == 0) {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/server.go");
                    } 
                    else {
                        snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/renamed.file");
                    }
                } 
                else {
                    snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/renamed.file");
                }
                rename(old_path, new_path);

                fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File successfully renamed.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
            }
        }
    }
}
```

**backup_func**

```c
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
                char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
                snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);
                snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup/%s", ep->d_name);
                rename(old_path, new_path);

                fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File backed up.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
            }
        }
    }
    fclose(log_file);
    closedir(dir);
}
```

**restore_func**

```c
void restore_func() {
    DIR *dir;
    struct dirent *ep;
    dir = opendir("/home/etern1ty/sisop_works/modul_2/soal_2/library/backup");
    FILE *log_file = fopen("/home/etern1ty/sisop_works/modul_2/soal_2/history.log", "a"); // append mode

    if (dir != NULL) {
        while ((ep = readdir(dir))) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

            char *file_name = ep->d_name;
            char old_path[MAX_PATH_LEN], new_path[MAX_PATH_LEN];
            snprintf(old_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/backup/%s", ep->d_name);
            snprintf(new_path, MAX_PATH_LEN, "/home/etern1ty/sisop_works/modul_2/soal_2/library/%s", ep->d_name);
            rename(old_path, new_path);

            fprintf(log_file, "[%s][%02d:%02d:%02d] - %s - File successfully restored from backup.\n", user, tm.tm_hour, tm.tm_min, tm.tm_sec, file_name);
        }
    }
    fclose(log_file);
    closedir(dir);
}
```

**NOTE: Di kode asli, terdapat ifdef untuk mode DEBUG dan PROD. Untuk mode DEBUG, saya menggunakan mode DEBUG untuk mengecek apakah file-file yang dicek sudah dilakukan operasi yang sesuai sebelum subnomor ini. Untuk mode PROD menggunakan format yang ditentukan oleh soal.**

# Soal 3

**Dikerjakan oleh Athalla Barka Fadhil (5027231018)**

## Deskripsi Soal

Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!

### Catatan

```c
#define MAX_LINE_LENGTH 256
```
```c
FILE* ps_output;
char cmd[MAX_LINE_LENGTH];
char line[MAX_LINE_LENGTH];
char comm[MAX_LINE_LENGTH];
char process_name[MAX_LINE_LENGTH];
char log_filename[MAX_LINE_LENGTH];
FILE* log_file;
pid_t pid, sid;
```
- MAX_LINE_LENGTH: Konstanta yang menyimpan nilai 256.
- ps_output: Pointer ke objek FILE untuk menyimpan output dari proses.
- cmd: Array karakter untuk menyimpan perintah yang akan dieksekusi.
- line: Array karakter untuk menyimpan baris hasil pembacaan output.
- comm: Array karakter untuk menyimpan nama perintah atau executable dari proses.
- process_name: Array karakter untuk menyimpan nama proses.
- log_filename: Array karakter untuk menyimpan nama file log.
- log_file: Pointer ke objek FILE untuk menyimpan file log.
- pid: Variabel untuk menyimpan PID (Process ID) dari proses.
- sid: Variabel untuk menyimpan SID (Session ID) dari proses.

## Pengerjaan

```c
if (argc < 2) {
    fprintf(stderr, "Usage: %s <username> | -m <username> | -s <username>\n", argv[0]);
    exit(EXIT_FAILURE);
    }
```
Disini kita memeriksa jumlah argumen yang diberikan saat menjalankan program. `argc` adalah jumlah argumen yang disediakan dalam pemanggilan program dari command line, sedangkan `argv` adalah array yang berisi argumen-argumen tersebut.

---
```c
int monitorMode = 0;
char* user = argv[1];
int pid_process;
```
Jika jumlah argumen kurang dari 2 maka akan `stderr` dan memberi info Usage
Disini kita menginisiasi beberapa variabel
- monitorMode: Variabel untuk menentukan mode operasi program. Nilai 0 menunjukkan mode standar.
- user: Variabel untuk menyimpan nama pengguna yang diberikan sebagai argumen pertama saat menjalankan program.
- pid_process: Variabel untuk menyimpan PID (Process ID) dari proses yang sedang diproses oleh program.

---
```c
if (argc == 2) {
    char* args[] = { "ps", "-u", user, NULL };
    execvp(args[0], args);
    perror("execvp failed");
    return 1;
    }
```
Mengecek apakah argumen command line adalah 2 (`argc == 2`) jika iya, maka akan membuat array `args` yang berisikan command yang akan diexecute `ps -u <user>` lalu akan dieksekusi menggunakan `execvp()`, jika ada error akan mengoutput error `execvp failed` dan `return 1`. Command ini `ps -u <user>` yang akan digunakan untuk menampilkan proses user 


```c
 if (argc == 2 && strcmp(argv[1], "-m") == 0) {
    fprintf(stderr, "Usage: %s -m <username>\n", argv[0]);
    exit(EXIT_FAILURE);
    }
```
Misalkan `argc` jumlahnya 2 (e.g ./admin -m) dan `argv[1]` sama dengan `-m` maka program akan exit dan memberikan output error Usage

---
```c
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
```
Fungsi diatas sama seperti dimodul, yaitu untuk mengimplementasi daemon, tapi disini tidak saya lakukan `chdir()` seperti dimodul

---

```c
if (argc == 3 && strcmp(argv[1], "-m") == 0) {
    fprintf(stderr, "Memulai proses monitoring user %s", argv[2]);
    monitorMode = 1;
    user = argv[2];
    }
```
Diatas itu simpel aja, yaitu mengecek jika argumen kedua adalah `"-m"` maka akan mengubah variabel `monitorMode = 1` dan `user = argv[2]` ini akan berfungsi nanti di fungsi `main`

---

```c
 if (argc == 3 && strcmp(argv[1], "-s") == 0) {
    snprintf(process_name, sizeof(process_name), "%s -m %s", argv[0], argv[2]);
    char* args[] = { "pkill", "-f", process_name, NULL };
    execvp(args[0], args);
    perror("execvp failed");
    return 1;
    }
```
Kalo ini mirip kyk sebelumnya, dia akan ngecek argumen kedua, setelah itu akan mengeksekusi command yang ada di `args[]` menggunakan `execvp`. Ini yang bertanggung jawab untuk mematikan proses `./admin -m <user>`

---
```c
 if (argc == 3 && strcmp(argv[1], "-c") == 0) {
    fprintf(stderr, "Memulai proses gagalkan monitoring user %s", argv[2]);
    monitorMode = 2;
    user = argv[2];
    }
```
Modifikasi dikit dari sebelumnya, ini yang akan bertanggung jawab untuk mengagalkan proses monitoring user, cukup perhatikan bahwa disini `monitorMode` diassign value `2`

---
```c
  if (argc == 3 && strcmp(argv[1], "-a") == 0) {
    snprintf(process_name, sizeof(process_name), "%s -c %s", argv[0], argv[2]);
    char* args[] = { "pkill", "-f", process_name, NULL };
    execvp(args[0], args);
    perror("execvp failed");
    return 1;
    }
```
Ini untuk mematikan proses sebelumnya `./admin -c <user>` yaitu dengan menggunakan `pkill -f <process_name>` tidak beda jauh dari fungsi yang `-s`.

---
```c
while (1) {

    if (monitorMode == 1) {
      // Mengambil pid dan nama proses
      snprintf(cmd, sizeof(cmd), "ps -u %s -o pid,comm | awk 'NR>1 {print $1, $2}'", user);
      ps_output = popen(cmd, "r");
      if (ps_output == NULL) {
        perror("Failed to open pipe");
        exit(EXIT_FAILURE);
        }
        ...}
    ...

    sleep(1); // Loop setiap 1 detik
    }
```        
Sekarang kita masuk ke loop, disini langsung ngecek misalkan `monitorMode` sama dengan 1 yaitu ketika kita `./admin -m <user>` akan mengexecute command `ps -u <user> -o pid,comm | awk 'NR>1 {print $1, $2}'` yang intinya dia akan menampilkan pid dan nama proses dari user.
Lalu command tersebut akan kita pipe menggunakan `popen()` dan kita masukkan outpunya ke `ps_output` lalu ada implementasi error handling biasa

---

```c
// Buka log file
      snprintf(log_filename, sizeof(log_filename), "%s.log", user);
      log_file = fopen(log_filename, "a");
      if (log_file == NULL) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
        }
```
Selanjutnya kita akan buka log file dan memberinya nama sesuai user menggunakan `snprintf` dan error handling biasa

---
```c
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
        fprintf(log_file, "%s-%d-%s_JALAN\n", timestamp, pid_process, comm);
        }
        // Tutup pipe dan log file
      pclose(ps_output);
      fclose(log_file);
```
Disini kita baca output dari `ps_output` tadi baris perbaris dan mendapatkan waktu serta melakukan format timestamp, lalu kita akan print ke `log_file`, setelah selesai kita akan menutup pipe dan log

---
```c
   if (monitorMode == 2) {
    ...
    fprintf(log_file, "%s-%d-%s_GAGAL\n", timestamp, pid_process, comm);
    ...
   }
```
Untuk kode saat `monitorMode == 2` tidak berbeda dengan `monitorMode == 1`, yang beda cuman saat log nya yaitu awalnya `xxxx_JALAN` sekarang kita gagalkan jadi `xxx_GAGAL` untuk logicnya sama seperti sebelumnya

---
```c

  exit(EXIT_SUCCESS);
  return 0;
  }
```
Setelah selesai kita akan exit









# Soal 4

## Deskripsi Soal

### Catatan

## Pengerjaan
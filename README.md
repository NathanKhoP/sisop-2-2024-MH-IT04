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

## Deskripsi Soal

### Catatan

## Pengerjaan

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

# Soal 3

## Deskripsi Soal

### Catatan

## Pengerjaan

# Soal 4

## Deskripsi Soal

### Catatan

## Pengerjaan
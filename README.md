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

# Soal 2

**Dikerjakan oleh Nathan Kho Pancras (5027231002)**

## Deskripsi Soal

Paul adalah seorang mahasiswa semester 4 yang diterima magang di perusahaan XYZ. Pada hari pertama magang, ia diberi tugas oleh atasannya untuk membuat program manajemen file sederhana. Karena kurang terbiasa dengan bahasa C dan environment Linux, ia meminta bantuan kalian untuk mengembangkan program tersebut.

### Catatan
Struktur folder:
```
    soal_2/
    ├── history.log
    ├── management.c
    └── library/
        └── backup/
```

## Pengerjaan

> a. Atasannya meminta agar program tersebut dapat berjalan secara daemon dan dapat mengunduh serta melakukan unzip terhadap file berikut. Atasannya juga meminta program ini dibuat tanpa menggunakan command system()

### Solusi

> b. Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma ROT19

### Solusi

> c. Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: r3N4mE, d3Let3, dan m0V3. Untuk setiap file dengan nama yang memuat kode d3Let3, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode r3N4mE, lakukan hal berikut:
>> Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
>
>> Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
>
>> Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
>
>> Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”

### Solusi

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

> e. Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
>> SIGRTMIN untuk mode default
>
>> SIGUSR1 untuk mode backup
>
>> SIGUSR2 untuk mode restore
>
>> Contoh penggunaan: kill -SIGUSR2 <pid_program>

### Solusi

> f. Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien

### Solusi

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

# Soal 4
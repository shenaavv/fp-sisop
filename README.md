# Final Project Sistem Operasi IT

## Peraturan

1. Waktu pengerjaan dimulai hari Kamis (19 Juni 2025) setelah soal dibagikan hingga hari Rabu (25 Juni 2025) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-FP-2025-IT-[Kelas][Kelompok]” (contoh:Sisop-FP-2025-IT-A01).
4. Setelah pengerjaan selesai, seluruh source code dan semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github dikumpulkan pada form yang disediakan. Pastikan github di setting ke publik.
5. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir.
6. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan.
7. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan.
8. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya.
9. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal final project, maka nilai dianggap 0.
10. Pengerjaan soal final project sesuai dengan modul yang telah diajarkan.

## Kelompok A07

Nama | NRP
--- | ---
Kanafira Vanesha Putri | 5027241010
Angga Firmansyah | 5027241062
Tiara Fatimah Azzahra | 5027241090
M. Alfaeran Auriga Ruswandi | 5027241115  

## Deskripsi Soal

> 19. Named Pipes - Pipe communication system.
Buatlah sebuah program sederhana yang dapat berkomunikasi satu sama lain menggunakan sebuah named pipe. Catat setiap komunikasi di masing - masing sisi di dalam file bernama history.log (tidak menggunakan waktu dan tanggal). 


### Catatan

Program ini mengimplementasikan sistem komunikasi menggunakan Named Pipes di Linux. Named Pipes memungkinkan komunikasi inter-process yang persistent dan dapat diakses melalui filesystem.
 Named Pipes menyediakan saluran komunikasi yang persistent dan dapat diakses seperti file di dalam filesystem. Dengan Named Pipes, dua proses terpisah bisa saling berkirim dan menerima pesan secara berurutan (First In First Out).


**Note**: Program ini dirancang untuk sistem operasi Linux/Unix. Untuk menjalankan di Windows, gunakan WSL (Windows Subsystem for Linux) atau Virtual Machine Linux.

Struktur repository:
```
.
├── README.md              # Laporan utama
├── src/                   # Source code C
│   ├── server.c           # Program server
│   └── client.c           # Program client  
├── scripts/               # Shell scripts
│   ├── build.sh           # Script kompilasi
│   ├── run_server.sh      # Script menjalankan server
│   ├── run_client.sh      # Script menjalankan client
│   ├── demo.sh            # Script demo otomatis
│   └── test.sh            # Script testing
├── Makefile               # Build automation
├── PROGRAM_README.md      # Dokumentasi program
└── history.log            # Log komunikasi (generated saat runtime)
```

## Pengerjaan

### 1. Implementasi Named Pipes Communication System

**Teori**

Named Pipes (FIFO - First In First Out) adalah salah satu metode Inter-Process Communication (IPC) di sistem operasi Unix/Linux yang memungkinkan proses yang berbeda untuk berkomunikasi satu sama lain. Berbeda dengan anonymous pipes yang hanya dapat digunakan oleh proses parent-child, named pipes dapat digunakan oleh proses yang tidak memiliki hubungan keluarga.

Karakteristik Named Pipes:
- Memiliki nama dalam filesystem (biasanya di /tmp/)
- Persistent sampai dihapus secara eksplisit
- Dapat diakses oleh multiple processes
- Data flow secara FIFO (First In First Out)
- Mendukung blocking dan non-blocking operations

Fungsi-fungsi utama yang digunakan:
- `mkfifo()`: Membuat named pipe
- `open()`: Membuka named pipe untuk read/write
- `read()/write()`: Membaca dan menulis data
- `unlink()`: Menghapus named pipe

**Solusi**

Implementasi terdiri dari dua program utama:

1. **Server Program (server.c)**:
   - Membuat named pipe menggunakan `mkfifo()`
   - Menunggu koneksi dari client
   - Menerima pesan dari client dan memberikan response
   - Mencatat semua komunikasi ke `history.log`
   - Cleanup otomatis saat program berakhir

2. **Client Program (client.c)**:
   - Terhubung ke named pipe yang sudah dibuat server
   - Mengirim pesan ke server dan menerima response
   - Mencatat semua komunikasi ke `history.log`
   - Dapat mengakhiri komunikasi dengan perintah "exit"

3. **Build dan Run Scripts**:
   - `build.sh`: Kompilasi kedua program
   - `run_server.sh`: Menjalankan server
   - `run_client.sh`: Menjalankan client

### 2. Logging System

**Teori**

Logging adalah proses pencatatan aktivitas dan events yang terjadi dalam sistem. Dalam konteks komunikasi antar proses, logging membantu untuk:

- Debugging dan troubleshooting
- Audit trail komunikasi
- Monitoring sistem
- Analisis performa

Studi empiris tentang praktik logging di kernel Linux, termasuk distribusi, perubahan, dan rekomendasi peningkatan kualitas log

**Solusi**

Sistem logging yang diimplementasikan:

- Setiap pesan yang dikirim dan diterima dicatat
- Format log: `[ROLE-ACTION] message`
- File log: `history.log`
- No timestamp sesuai permintaan soal

Format logging:

```
[CLIENT-SENT] Hello Server
[SERVER-RECEIVED] Hello Server
[SERVER-SENT] Hello Client
[CLIENT-RECEIVED] Hello Client
```

### 3. Error Handling dan Cleanup

**Teori**

Proper error handling dan cleanup sangat penting dalam system programming untuk:

- Mencegah resource leaks
- Memastikan konsistensi sistem
- Graceful program termination
- Recovery dari error conditions

**Solusi**

- Signal handlers untuk SIGINT dan SIGTERM
- Automatic cleanup named pipe saat program berakhir
- Error checking untuk semua system calls
- Proper file descriptor management
- Graceful shutdown sequence

**Video Menjalankan Program**




https://github.com/user-attachments/assets/20561d08-89e1-4b9d-8642-d47f456f5397




Untuk menjalankan program:

1. Kompilasi: `./scripts/build.sh` atau `make all`
2. Terminal 1: `./scripts/run_server.sh`
3. Terminal 2: `./scripts/run_client.sh`
4. Mulai komunikasi
5. Ketik "exit" di client untuk keluar

Alternatif testing:

- `make demo` - Demo otomatis
- `make test` - Testing komprehensif
- `./scripts/test.sh` - Manual testing

**Contoh Output History Log:**

```log
[CLIENT] Connected to server
[CLIENT-SENT] Hello Server!
[SERVER-RECEIVED] Hello Server!
[SERVER-SENT] Hello Client! Nice to meet you
[CLIENT-RECEIVED] Hello Client! Nice to meet you
[CLIENT-SENT] exit
[CLIENT] Exiting
[SERVER] Client requested exit
```

## Daftar Pustaka

1.	Karacalı, H., Donum, N., & Cebel, E. (2024). *Cryptographic enhancement of named pipes for secure process communication. The European Journal of Research and Development*, 4(2), 62–68.
 
2.	Karacalı, H., Cebel, E., & Donum, N. (2024). *Performance analysis of encryption algorithms in named pipe communication for Linux systems. The Eurasia Proceedings of Science, Technology, Engineering & Mathematics (EPSTEM)*, 27, 214–227.
 
3.	The Art of Machinery. (2020). *Robust and race‑free server logging using named pipes*. Retrieved from https://theartofmachinery.com/2020/10/10/logging_with_named_pipes.html
 
4. Merino, J. (2023). *Unit-testing shell scripts and tools with shtk*. Retrieved from https://jmmv.dev/2023/10/unit-testing-with-shtk.html

5. Vogel, F. (2024). *Automated testing of Linux shell scripts and applications with BATS*. *Medium*. Retrieved from https://medium.com/@fvogel_16801/automated-testing-of-linux-shell-scripts-and-applications-with-bats-230258682d6b

6. Hassan, A., Li, H., Chen, T., & Shang, W. (2018). Studying software logging using topic models. Empirical Software Engineering, 23, 2655-2694. https://doi.org/10.1007/s10664-018-9595-*8


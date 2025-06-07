## Tutorial membuat kernel module sederhana by hikmah gumelar


# karakter - Linux Kernel Module (karakter Device)

`karakter` adalah modul kernel Linux sederhana yang membuat **karakter device** virtual. Modul ini memungkinkan komunikasi antara user space dan kernel space melalui device file di `/dev/karakter`.
## 🧩 Komponen Penting Modul

### 1. Device File /dev/mychardev
    - Ini kayak "pintu masuk" dari user space ke kernel space.
    - Lo bisa pakai echo dan cat buat kirim/terima data dari situ.
### 2. Buffer Kernel
    - Modul nyimpen data dari user di dalam RAM kernel (pakai kmalloc)
    - Ukuran buffer: 1024 byte (1KB)


## 🔁 Siklus Hidup Modul ( lifecycle modul )


### 1. module_init() – Saat modul dimuat (insmod)

       - Alokasi buffer

       - Daftar char device

       - Bikin device di /dev

### 2. module_exit() – Saat modul dilepas (rmmod)

       - Hapus device & class

       - Free buffer

       - Unregister device


## 🧪 Cara Kerja (Contoh)


#### write() dipanggil → string "Halo" masuk ke buffer kernel.

```bash
write() dipanggil → string "Halo" masuk ke buffer kernel.
```

#### read() dipanggil → kernel kirim isi buffer ("Halo") ke user.
```bash
Virtual file seperti /proc atau /sys
```

## 🧠 Kenapa Ini Berguna?

### Ini dasar dari semua device driver di Linux (termasuk sensor, GPIO, dsb).
### Bisa jadi pondasi buat modul yang lebih kompleks, kayak:
    - Logging system
    - Interaksi hardware (GPIO, UART)
    - Virtual file seperti /proc atau /sys



---

## 🧰 Fitur

- Membuat character device `/dev/karakter`
- Mendukung operasi `open`, `read`, `write`, dan `release`
- Buffer memori kernel internal (1 KB)
- Logging via `dmesg` untuk monitoring aktivitas modul

---

## 📁 Struktur Proyek
```text
karakter/
├── karakter.c # Source code modul kernel
├── Makefile # Makefile untuk build modul
└── README.md # Dokumentasi proyek
```

---

---

## 🛠️ Build & Install

##  Build & Install

### 1. Build modul

```bash
make
```

### 2. Load module ke kernel

```bash
sudo insmod karakter.ko
```

### 3. Cek pesan kernel via dmesg

```bash

dmesg | tail
```

### 4. Buat device file (jika belum otomatis)
```bash
sudo mknod /dev/karakter c <major_number> 0 # Ganti <major_number> dengan nomor dari output dmesg
sudo chmod 666 /dev/karakter
```
## 🚀 Penggunaan

### 1. Tulis ke device
```bash
echo "Halo kernel" > /dev/karakter
```
## 🧼 Uninstall / Cleanup

### 1. Hapus modul

```bash
sudo rmmod karakter
```
### 2. Bersihkan file build

```bash
make clean
```

## 🐛 Troubleshooting

### 1. No such device atau Permission denied saat akses /dev/karakter

       - Pastikan device file sudah dibuat dengan mknod

       - Pastikan modul sudah dimuat (lsmod | grep karakter)

       - Pastikan permission file /dev/karakter sesuai

### 2. insmod gagal

       - Cek dengan dmesg dan pastikan tidak ada error seperti unresolved symbols


## 📄 Lisensi

#### Proyek ini menggunakan lisensi GPLv2 sesuai dengan ketentuan modul kernel Linux.


## 👨💻 Author

#### Created by Hikmah gumelar
#### feel free to fork, improve , or contribute



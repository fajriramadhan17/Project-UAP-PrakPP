# Project UAP PrakPP

Sebuah game terminal berbasis ncurses yang dibuat dalam bahasa C++. Game ini menyajikan permainan sederhana dengan kontrol keyboard, papan skor (highscore), dan tampilan ASCII art pada menu.

## Fitur
- Menu utama dengan pilihan: Play Game, High Score, Quit Game
- Gameplay interaktif menggunakan ncurses
- Sistem high score yang menyimpan nama pemain dan skornya
- Input nama setelah game over (dapat dibatalkan)
- Tampilan ASCII art saat menu

## Prasyarat
- Sistem operasi berbasis Unix / Linux (dapat dijalankan juga di macOS dengan penyesuaian)
- Compiler C++ (g++)
- Library ncurses (development headers)
  - Ubuntu/Debian: `sudo apt-get install libncurses5-dev libncursesw5-dev`
  - Fedora: `sudo dnf install ncurses-devel`
  - macOS (Homebrew): `brew install ncurses` (mungkin perlu menambahkan include/lib flags saat kompilasi)

## Struktur proyek (ringkasan)
- `main.cpp`            : Entrypoint aplikasi dan logika menu / kontrol utama
- `source/`             : Direktori sumber tambahan (implementasi game, ascii art, highscore, dsb.)
- `highscores.txt`      : File penyimpanan highscore (terenkripsi sederhana/teks)
- `README.md`           : Dokumen ini

> Catatan: Beberapa file header digunakan di `main.cpp`: `asciiArt.h`, `game.h`, `highscore.h`. Pastikan file-file implementasinya (.cpp) ada di folder `source/` sebelum kompilasi.

## Cara membangun (build)
1. Pastikan semua file sumber (.cpp) yang diperlukan berada di root atau di `source/`.
2. Kompilasi dengan g++ dan tautkan ncurses. Contoh perintah umum:

Linux/macOS:
```
g++ main.cpp source/*.cpp -lncurses -o uap-prakpp
```

Jika implementasi berada di tempat lain, sesuaikan path. Untuk macOS dengan Homebrew ncurses, Anda mungkin perlu menambahkan:
```
-I/usr/local/opt/ncurses/include -L/usr/local/opt/ncurses/lib
```
atau lokasi sesuai instalasi.

## Menjalankan
Setelah berhasil dikompilasi:
```
./uap-prakpp
```

## Kontrol permainan
- Menu:
  - Panah Atas / Bawah untuk navigasi
  - Enter untuk memilih
  - '1' / '2' / '3' sebagai shortcut ke menu item
  - 'q' atau 'Q' untuk keluar
- Saat bermain:
  - 'W' / 'w' atau Panah Atas : bergerak naik
  - 'S' / 's' atau Panah Bawah : bergerak turun
  - 'P' / 'p' : pause (tekan lagi untuk melanjutkan)
  - 'Q' / 'q' : quit (kembali ke menu atau keluar)
- Input nama saat menyimpan highscore:
  - Ketik nama (maks ~19 karakter)
  - Enter untuk konfirmasi
  - Backspace untuk menghapus karakter
  - ESC untuk membatalkan (tidak menyimpan)

## High Score
High score disimpan ke file `highscores.txt` (teks). Setelah game over dan pemain mengkonfirmasi nama, skor akan disimpan oleh manajer high score. Anda dapat membuka file tersebut untuk melihat daftar skor yang tersimpan.

## Debugging / Tips
- Jika terminal tampak "rusak" setelah program berhenti, jalankan `reset` atau pastikan program memanggil `endwin()` untuk ncurses. Kompilator/g++ memberikan pesan error terkait ncurses jika library tidak ditemukan.
- Jika ada error unresolved reference ke fungsi dari header (mis. `Game`, `HighScoreManager`, `tampilanMenu`), pastikan semua .cpp yang mengimplementasikan class/function tersebut dikompilasi dan dilink bersama `main.cpp`.

## Kontribusi
Jika ingin mengembangkan:
- Tambahkan issue / pull request di repository
- Pastikan menambahkan dokumentasi singkat pada setiap fitur baru
- Ikuti pola penamaan dan struktur yang ada

## Lisensi
Lisans belum ditentukan di repository. Jika Anda pemilik repo, pertimbangkan menambahkan file `LICENSE` (mis. MIT) bila ingin memperbolehkan penggunaan ulang.

## Kontak
Repository: https://github.com/fajriramadhan17/Project-UAP-PrakPP  
Pemilik: fajriramadhan17 (https://github.com/fajriramadhan17)

Selamat mencoba dan semoga berhasil!# Project-UAP-PrakPP

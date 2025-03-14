#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
// Struktur untuk merepresentasikan sebuah buku
struct Buku {
    int id;
    char judul[50];
    char penulis[50];
    int stok;
    Buku* next;
};
Buku* head = NULL;

// Fungsi untuk login sebelum mengakses sistem perpustakaan
bool login() {
    string username, password;
    while (true) {
        cout << "Selamat datang di database perpustakaan pei baca veteran!\n";
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;

        if (username == "pei" && password == "153") {
            cout << "Login berhasil!\n";
            return true;
        } else {
            cout << "Login gagal! Coba lagi.\n";
        }
    }
}

// Fungsi untuk menanyakan apakah user ingin kembali ke menu utama
bool kembaliKeMenu() {
    char pilihan;
    cout << "Kembali ke menu utama? (Y/N): ";
    cin >> pilihan;
    return !(pilihan == 'y' || pilihan == 'Y'); 
}

// Fungsi untuk menambahkan buku ke dalam database
void tambahBuku() {
    do {
        FILE* file = fopen("data.dat", "ab");// Membuka file dalam mode append binary
        Buku* buku = new Buku;

        cout << "Masukkan ID Buku: ";
        cin >> buku->id;
        cin.ignore();
        cout << "Masukkan Judul Buku: ";
        cin.getline(buku->judul, 50);
        cout << "Masukkan Penulis Buku: ";
        cin.getline(buku->penulis, 50);
        cout << "Masukkan Stok Buku: ";
        cin >> buku->stok;
        buku->next = head;
        head = buku;

        fwrite(buku, sizeof(Buku), 1, file);
        fclose(file);
        cout << "Buku berhasil ditambahkan!\n";
    } while (!kembaliKeMenu());
}

// Fungsi untuk menampilkan daftar buku yang tersimpan
void tampilkanBuku() {
    do {
        FILE* file = fopen("data.dat", "rb");
        if (!file) {
            cout << "File data tidak ditemukan!\n";
            return;
        }

        Buku* temp = NULL;
        Buku* curr;
        while (true) {
            Buku* buku = new Buku;
            if (fread(buku, sizeof(Buku), 1, file) == 0) break;
            buku->next = temp;
            temp = buku;
        }
        fclose(file);

        cout << "-------------------------------------------------------\n";
        cout << "ID   | Judul Buku           | Penulis       | Stok \n";
        cout << "-------------------------------------------------------\n";
        curr = temp;
        while (curr) {
            cout << curr->id << "  | " << curr->judul << " | " << curr->penulis << " | " << curr->stok << "\n";
            curr = curr->next;
        }
        cout << "-------------------------------------------------------\n";
    } while (!kembaliKeMenu());
}

// Fungsi untuk mencari buku berdasarkan judul
void cariBuku() {
    do {
        FILE* file = fopen("data.dat", "rb");
        if (!file) {
            cout << "File data tidak ditemukan!\n";
            return;
        }
        Buku buku;
        char keyword[50];
        bool ditemukan = false;

        cin.ignore();
        cout << "Masukkan judul buku yang dicari: ";
        cin.getline(keyword, 50);

        while (fread(&buku, sizeof(Buku), 1, file)) {
            if (strstr(buku.judul, keyword)) {
                cout << "Buku ditemukan: ID: " << buku.id << " | Judul: " << buku.judul
                     << " | Penulis: " << buku.penulis << " | Stok: " << buku.stok << endl;
                ditemukan = true;
            }
        }

        if (!ditemukan) {
            cout << "Buku tidak ditemukan!\n";
        }
        fclose(file);
    } while (!kembaliKeMenu());
}

// Fungsi untuk menghapus buku berdasarkan ID
void hapusBuku() {
    do {
        FILE* file = fopen("data.dat", "rb");
        FILE* temp = fopen("temp.dat", "wb");
        if (!file || !temp) {
            cout << "Gagal membuka file!\n";
            return;
        }
        Buku buku;
        int id;
        bool ditemukan = false;

        cout << "Masukkan ID buku yang ingin dihapus: ";
        cin >> id;

        while (fread(&buku, sizeof(Buku), 1, file)) {
            if (buku.id != id) {
                fwrite(&buku, sizeof(Buku), 1, temp);
            } else {
                ditemukan = true;
            }
        }
        fclose(file);
        fclose(temp);

        remove("data.dat");
        rename("temp.dat", "data.dat");

        if (ditemukan) {
            cout << "Buku berhasil dihapus!\n";
        } else {
            cout << "ID buku tidak ditemukan!\n";
        }
    } while (!kembaliKeMenu());
}

// Fungsi utama: Menampilkan menu utama
int main() {
    if (!login()) return 0;

    int pilihan;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku\n";
        cout << "3. Cari Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahBuku(); break;
            case 2: tampilkanBuku(); break;
            case 3: cariBuku(); break;
            case 4: hapusBuku(); break;
            case 5: cout << "Keluar dari program.\n"; return 0;
            default: cout << "Pilihan tidak valid!\n";
        }
    }
}

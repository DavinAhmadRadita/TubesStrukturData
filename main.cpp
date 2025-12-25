#include "library.h"

static void tampilkanMenu() {
    cout << "\n=====================================\n";
    cout << "       SISTEM DATA PERPUSTAKAAN      \n";
    cout << "=====================================\n";
    cout << "1. Tambah Buku\n";
    cout << "2. Tambah Peminjam\n";
    cout << "3. Peminjam Meminjam Buku\n";
    cout << "4. Hapus Buku\n";
    cout << "5. Hapus Peminjam\n";
    cout << "6. Tampilkan Semua Buku\n";
    cout << "7. Tampilkan Semua Peminjam\n";
    cout << "8. Tampilkan Semua Peminjam beserta Bukunya\n";
    cout << "9. Tampilkan Buku dari Peminjam tertentu\n";
    cout << "10. Tampilkan Peminjam dari Buku tertentu\n";
    cout << "11. Tampilkan Buku paling favorit & tidak favorit\n";
    cout << "0. Keluar\n";
    cout << "Pilihan: ";
}

int main() {
    ListBuku LB;
    ListPeminjam LP;
    createListBuku(LB);
    createListPeminjam(LP);

    int pilihan;
    do {
        tampilkanMenu();
        cin >> pilihan;
        cin.ignore(1000, '\n'); // penting biar getline aman

        switch (pilihan) {
            case 1: menuTambahBuku(LB); break;
            case 2: menuTambahPeminjam(LP); break;
            case 3: menuPinjamBuku(LP, LB); break;
            case 4: menuHapusBuku(LP, LB); break;
            case 5: menuHapusPeminjam(LP); break;
            case 6: tampilkanSemuaBuku(LB); break;
            case 7: tampilkanSemuaPeminjam(LP); break;
            case 8: tampilkanPeminjamDanBukunya(LP); break;
            case 9: tampilkanBukuDariPeminjam(LP); break;
            case 10: tampilkanPeminjamDariBuku(LP, LB); break;
            case 11: tampilkanBukuFavoritDanTidak(LB); break;
            case 0: cout << "Terima kasih!\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}

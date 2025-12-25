#include "library.h"

static bool isAngka(const string &s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}

static int inputAngka(const string &prompt, const string &pesanError) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);

        if (!isAngka(s)) {
            cout << pesanError << "\n";
        } else {
            return stoi(s);
        }
    }
}

void createListBuku(ListBuku &LB) { 
    LB.first = nullptr; 
}

void createListPeminjam(ListPeminjam &LP) { 
    LP.first = nullptr; 
    LP.last = nullptr; 
}

adrBuku alokasiBuku(const Buku &b) {
    adrBuku p = new elmBuku;
    p->info = b;
    p->next = nullptr;
    return p;
}

adrPeminjam alokasiPeminjam(const Peminjam &peminjam) {
    adrPeminjam p = new elmPeminjam;
    p->info = peminjam;
    p->next = nullptr;
    p->prev = nullptr;
    p->firstRelasi = nullptr;
    return p;
}

adrRelasi alokasiRelasi(adrBuku b) {
    adrRelasi r = new elmRelasi;
    r->buku = b;
    r->next = nullptr;
    return r;
}

void insertLastBuku(ListBuku &LB, adrBuku p) {
    if (LB.first == nullptr) LB.first = p;
    else {
        adrBuku q = LB.first;
        while (q->next != nullptr) q = q->next;
        q->next = p;
    }
}

adrBuku findBukuByID(ListBuku LB, const string &id) {
    adrBuku p = LB.first;
    while (p != nullptr) {
        if (p->info.idBuku == id) return p;
        p = p->next;
    }
    return nullptr;
}

adrBuku deleteBukuByID(ListBuku &LB, const string &id) {
    adrBuku p = LB.first;
    adrBuku prev = nullptr;

    while (p != nullptr && p->info.idBuku != id) {
        prev = p;
        p = p->next;
    }
    if (p == nullptr) return nullptr;

    if (prev == nullptr) LB.first = p->next;
    else prev->next = p->next;

    p->next = nullptr;
    return p;
}

void insertLastPeminjam(ListPeminjam &LP, adrPeminjam p) {
    if (LP.first == nullptr) {
        LP.first = LP.last = p;
    } else {
        LP.last->next = p;
        p->prev = LP.last;
        LP.last = p;
    }
}

adrPeminjam findPeminjamByID(ListPeminjam LP, const string &id) {
    adrPeminjam p = LP.first;
    while (p != nullptr) {
        if (p->info.idPeminjam == id) return p;
        p = p->next;
    }
    return nullptr;
}

adrPeminjam deletePeminjamByID(ListPeminjam &LP, const string &id) {
    adrPeminjam p = LP.first;
    while (p != nullptr && p->info.idPeminjam != id) p = p->next;
    if (p == nullptr) return nullptr;

    if (p == LP.first && p == LP.last) {
        LP.first = LP.last = nullptr;
    } else if (p == LP.first) {
        LP.first = p->next;
        LP.first->prev = nullptr;
    } else if (p == LP.last) {
        LP.last = p->prev;
        LP.last->next = nullptr;
    } else {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    }
    p->next = nullptr;
    p->prev = nullptr;
    return p;
}

static bool sudahMeminjam(adrPeminjam pem, adrBuku b) {
    adrRelasi r = pem->firstRelasi;
    while (r != nullptr) {
        if (r->buku == b) return true;
        r = r->next;
    }
    return false;
}

bool pinjamBuku(ListPeminjam &LP, ListBuku &LB,
                const string &idPeminjam, const string &idBuku) {
    adrPeminjam pem = findPeminjamByID(LP, idPeminjam);
    adrBuku b = findBukuByID(LB, idBuku);

    if (pem == nullptr || b == nullptr) {
        cout << "Peminjam atau Buku tidak ditemukan.\n";
        return false;
    }
    if (pem->info.jumlahPinjam >= 5) {
        cout << "Peminjam sudah mencapai batas maksimal 5 buku.\n";
        return false;
    }
    if (b->info.copyTersedia <= 0) {
        cout << "Stok buku tidak tersedia.\n";
        return false;
    }
    if (sudahMeminjam(pem, b)) {
        cout << "Peminjam sudah meminjam buku (judul sama tidak boleh).\n";
        return false;
    }

    adrRelasi r = alokasiRelasi(b);
    r->next = pem->firstRelasi;
    pem->firstRelasi = r;

    pem->info.jumlahPinjam++;
    b->info.copyTersedia--;
    b->info.totalDipinjam++;

    cout << "Berhasil: " << pem->info.nama << " meminjam \"" << b->info.judul << "\".\n";
    return true;
}

void hapusRelasiPeminjam(adrPeminjam pem) {
    adrRelasi r = pem->firstRelasi;
    while (r != nullptr) {
        r->buku->info.copyTersedia++;
        pem->info.jumlahPinjam--;
        adrRelasi del = r;
        r = r->next;
        delete del;
    }
    pem->firstRelasi = nullptr;
}

void hapusRelasiBuku(ListPeminjam &LP, adrBuku b) {
    adrPeminjam p = LP.first;
    while (p != nullptr) {
        adrRelasi r = p->firstRelasi;
        adrRelasi prev = nullptr;
        while (r != nullptr) {
            if (r->buku == b) {
                if (prev == nullptr) p->firstRelasi = r->next;
                else prev->next = r->next;

                p->info.jumlahPinjam--;
                adrRelasi del = r;
                r = r->next;
                delete del;
            } else {
                prev = r;
                r = r->next;
            }
        }
        p = p->next;
    }
}

void tampilkanSemuaBuku(ListBuku LB) {
    cout << "\n=== DATA BUKU ===\n";
    if (LB.first == nullptr) {
        cout << "Tidak ada data buku.\n";
        return;
    }
    adrBuku p = LB.first;
    while (p != nullptr) {
        cout << "ID      : " << p->info.idBuku << "\n";
        cout << "Judul   : " << p->info.judul << "\n";
        cout << "Penulis : " << p->info.penulis << "\n";
        cout << "Tahun   : " << p->info.tahun << "\n";
        cout << "Copy    : " << p->info.copyTersedia << " / " << p->info.totalCopy << "\n";
        cout << "Total Dipinjam: " << p->info.totalDipinjam << "\n";
        cout << "-----------------------------\n";
        p = p->next;
    }
}

void tampilkanSemuaPeminjam(ListPeminjam LP) {
    cout << "\n=== DATA PEMINJAM ===\n";
    if (LP.first == nullptr) {
        cout << "Tidak ada data peminjam.\n";
        return;
    }
    adrPeminjam p = LP.first;
    while (p != nullptr) {
        cout << "ID   : " << p->info.idPeminjam << "\n";
        cout << "Nama : " << p->info.nama << "\n";
        cout << "Pinjam: " << p->info.jumlahPinjam << " buku\n";
        cout << "-----------------------------\n";
        p = p->next;
    }
}

void tampilkanPeminjamDanBukunya(ListPeminjam LP) {
    cout << "\n=== PEMINJAM & BUKU DIPINJAM ===\n";
    if (LP.first == nullptr) {
        cout << "Tidak ada data peminjam.\n";
        return;
    }
    adrPeminjam p = LP.first;
    while (p != nullptr) {
        cout << "Peminjam: " << p->info.nama << " (ID: " << p->info.idPeminjam << ")\n";
        if (p->firstRelasi == nullptr) {
            cout << "  Tidak sedang meminjam.\n";
        } else {
            int i = 1;
            adrRelasi r = p->firstRelasi;
            while (r != nullptr) {
                cout << "  " << i++ << ". " << r->buku->info.judul
                    << " (ID: " << r->buku->info.idBuku << ")\n";
                r = r->next;
            }
        }
        cout << "-----------------------------\n";
        p = p->next;
    }
}

void tampilkanBukuDariPeminjam(ListPeminjam LP) {
    string id;
    cout << "Masukkan ID Peminjam: ";
    getline(cin, id);

    adrPeminjam pem = findPeminjamByID(LP, id);
    if (pem == nullptr) {
        cout << "Peminjam tidak ditemukan.\n";
        return;
    }

    cout << "\nPeminjam: " << pem->info.nama << "\n";
    if (pem->firstRelasi == nullptr) {
        cout << "Tidak sedang meminjam.\n";
        return;
    }

    int i = 1;
    adrRelasi r = pem->firstRelasi;
    while (r != nullptr) {
        cout << "  " << i++ << ". " << r->buku->info.judul
            << " (ID: " << r->buku->info.idBuku << ")\n";
        r = r->next;
    }
}

void tampilkanPeminjamDariBuku(ListPeminjam LP, ListBuku LB) {
    string idB;
    cout << "Masukkan ID Buku: ";
    getline(cin, idB);

    adrBuku b = findBukuByID(LB, idB);
    if (b == nullptr) {
        cout << "Buku tidak ditemukan.\n";
        return;
    }

    cout << "\nBuku: " << b->info.judul << "\n";
    bool ada = false;

    adrPeminjam p = LP.first;
    while (p != nullptr) {
        adrRelasi r = p->firstRelasi;
        while (r != nullptr) {
            if (r->buku == b) {
                cout << "- " << p->info.nama << " (ID: " << p->info.idPeminjam << ")\n";
                ada = true;
                break;
            }
            r = r->next;
        }
        p = p->next;
    }

    if (!ada) cout << "Belum ada peminjam untuk buku ini.\n";
}

void tampilkanBukuFavoritDanTidak(ListBuku LB) {
    if (LB.first == nullptr) {
        cout << "Tidak ada data buku.\n";
        return;
    }

    adrBuku p = LB.first;
    adrBuku favorit = p;
    adrBuku tidakFavorit = p;

    while (p != nullptr) {
        if (p->info.totalDipinjam > favorit->info.totalDipinjam) favorit = p;
        if (p->info.totalDipinjam < tidakFavorit->info.totalDipinjam) tidakFavorit = p;
        p = p->next;
    }

    cout << "\n=== BUKU PALING FAVORIT ===\n";
    cout << favorit->info.judul << " (TotalDipinjam: " << favorit->info.totalDipinjam << ")\n";

    cout << "\n=== BUKU PALING TIDAK FAVORIT ===\n";
    cout << tidakFavorit->info.judul << " (TotalDipinjam: " << tidakFavorit->info.totalDipinjam << ")\n";
}

void menuTambahBuku(ListBuku &LB) {
    Buku b;

    cout << "ID Buku       : ";
    getline(cin, b.idBuku);
    cout << "Judul         : ";
    getline(cin, b.judul);
    cout << "Penulis       : ";
    getline(cin, b.penulis);

    b.tahun = inputAngka("Tahun Terbit  : ", "Tahun harus berupa angka!");
    b.totalCopy = inputAngka("Jumlah Copy   : ", "Jumlah copy harus berupa angka!");

    b.copyTersedia = b.totalCopy;
    b.totalDipinjam = 0;

    if (findBukuByID(LB, b.idBuku) != nullptr) {
        cout << "ID Buku sudah ada, gagal menambah.\n";
        return;
    }

    adrBuku p = alokasiBuku(b);
    insertLastBuku(LB, p);
    cout << "Buku berhasil ditambahkan.\n";
}

void menuTambahPeminjam(ListPeminjam &LP) {
    Peminjam pem;
    cout << "ID Peminjam : ";
    getline(cin, pem.idPeminjam);
    cout << "Nama        : ";
    getline(cin, pem.nama);
    pem.jumlahPinjam = 0;

    if (findPeminjamByID(LP, pem.idPeminjam) != nullptr) {
        cout << "ID Peminjam sudah ada, gagal menambah.\n";
        return;
    }

    adrPeminjam p = alokasiPeminjam(pem);
    insertLastPeminjam(LP, p);
    cout << "Peminjam berhasil ditambahkan.\n";
}

void menuPinjamBuku(ListPeminjam &LP, ListBuku &LB) {
    string idP, idB;
    cout << "Masukkan ID Peminjam : ";
    getline(cin, idP);
    cout << "Masukkan ID Buku     : ";
    getline(cin, idB);
    pinjamBuku(LP, LB, idP, idB);
}

void menuHapusBuku(ListPeminjam &LP, ListBuku &LB) {
    string idB;
    cout << "Masukkan ID Buku yang akan dihapus: ";
    getline(cin, idB);

    adrBuku b = findBukuByID(LB, idB);
    if (b == nullptr) {
        cout << "Buku tidak ditemukan.\n";
        return;
    }

    hapusRelasiBuku(LP, b);
    adrBuku del = deleteBukuByID(LB, idB);

    if (del != nullptr) {
        cout << "Buku \"" << del->info.judul << "\" berhasil dihapus.\n";
        delete del;
    } else {
        cout << "Gagal menghapus buku.\n";
    }
}

void menuHapusPeminjam(ListPeminjam &LP) {
    string idP;
    cout << "Masukkan ID Peminjam yang akan dihapus: ";
    getline(cin, idP);

    adrPeminjam pem = findPeminjamByID(LP, idP);
    if (pem == nullptr) {
        cout << "Peminjam tidak ditemukan.\n";
        return;
    }

    hapusRelasiPeminjam(pem);
    adrPeminjam del = deletePeminjamByID(LP, idP);

    if (del != nullptr) {
        cout << "Peminjam \"" << del->info.nama << "\" berhasil dihapus.\n";
        delete del;
    } else {
        cout << "Gagal menghapus peminjam.\n";
    }
}

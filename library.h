#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct Buku {
    string idBuku;
    string judul;
    string penulis;
    int tahun;
    int totalCopy;
    int copyTersedia;
    int totalDipinjam;
};

struct Peminjam {
    string idPeminjam;
    string nama;
    int jumlahPinjam;
};

struct elmBuku;
struct elmPeminjam;
struct elmRelasi;

typedef elmBuku* adrBuku;
typedef elmPeminjam* adrPeminjam;
typedef elmRelasi* adrRelasi;

struct elmBuku {
    Buku info;
    adrBuku next;
};

struct ListBuku {
    adrBuku first;
};

struct elmRelasi {
    adrBuku buku;
    adrRelasi next;
};

struct elmPeminjam {
    Peminjam info;
    adrPeminjam next;
    adrPeminjam prev;
    adrRelasi firstRelasi;
};

struct ListPeminjam {
    adrPeminjam first;
    adrPeminjam last;
};

void createListBuku(ListBuku &LB);
void createListPeminjam(ListPeminjam &LP);
adrBuku alokasiBuku(const Buku &b);
adrPeminjam alokasiPeminjam(const Peminjam &p);
adrRelasi alokasiRelasi(adrBuku b);
void insertLastBuku(ListBuku &LB, adrBuku p);
adrBuku findBukuByID(ListBuku LB, const string &id);
adrBuku deleteBukuByID(ListBuku &LB, const string &id);
void insertLastPeminjam(ListPeminjam &LP, adrPeminjam p);
adrPeminjam findPeminjamByID(ListPeminjam LP, const string &id);
adrPeminjam deletePeminjamByID(ListPeminjam &LP, const string &id);
bool pinjamBuku(ListPeminjam &LP, ListBuku &LB, const string &idPeminjam, const string &idBuku);
void hapusRelasiPeminjam(adrPeminjam pem);
void hapusRelasiBuku(ListPeminjam &LP, adrBuku b);
void tampilkanSemuaBuku(ListBuku LB);
void tampilkanSemuaPeminjam(ListPeminjam LP);
void tampilkanPeminjamDanBukunya(ListPeminjam LP);
void tampilkanBukuDariPeminjam(ListPeminjam LP);
void tampilkanPeminjamDariBuku(ListPeminjam LP, ListBuku LB);
void tampilkanBukuFavoritDanTidak(ListBuku LB);
void menuTambahBuku(ListBuku &LB);
void menuTambahPeminjam(ListPeminjam &LP);
void menuPinjamBuku(ListPeminjam &LP, ListBuku &LB);
void menuHapusBuku(ListPeminjam &LP, ListBuku &LB);
void menuHapusPeminjam(ListPeminjam &LP);

#endif

// ============================================================
//  Program Kombinasi tanpa Perulangan
//  C(n,r) = n! / ((n-r)! x r!)
//  Kompilasi: g++ kombinasi.cpp -o kombinasi -std=c++11
//  (butuh conio.h dan system("cls") -> Windows / MinGW)
// ============================================================
#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <iomanip>
#include <climits>
#include <cstdlib>
#include <conio.h>
using namespace std;
typedef long long ll;

bool meluap = false; // true jika ada operasi yang melewati batas long long

// ------------------------------------------------------------
//  Operasi dasar (dengan deteksi overflow)
// ------------------------------------------------------------
ll gcdLL(ll a, ll b){
    while(b){ ll t = a % b; a = b; b = t; }
    return a;
}

ll kali(ll a, ll b){
    if(a == 0 || b == 0) return 0;
    if(a > LLONG_MAX / b){ meluap = true; return 0; }
    return a * b;
}

ll tambah(ll a, ll b){
    if(a > LLONG_MAX - b){ meluap = true; return 0; }
    return a + b;
}

ll faktorial(ll a){
    ll h = 1;
    for(ll i = 2; i <= a; i++) h = kali(h, i);
    return h;
}

// C(n,r) dengan rumus multiplikatif: hsl = hsl * (n-r+i) / i
// FPB dipakai supaya hasil antara tidak cepat meluap.
ll kombin(ll n, ll r){
    if(n < 0 || r < 0 || r > n) return 0;
    if(r > n - r) r = n - r;          // sifat simetri C(n,r) = C(n,n-r)
    ll hsl = 1;
    for(ll i = 1; i <= r; i++){
        ll g = gcdLL(hsl, i);
        ll a = hsl / g;
        ll b = (n - r + i) / (i / g);
        hsl = kali(a, b);
    }
    return hsl;
}

string bentukFaktorial(ll n, ll r){
    return to_string(n) + "! / (" + to_string(n - r) + "! x " + to_string(r) + "!)";
}

// ------------------------------------------------------------
//  Struktur ekspresi
//  Ekspresi = (jumlah dari suku) / m!  [/ C(N,R) jika peluang]
//  Suku     = koefisien x C(n1,r1) x C(n2,r2) x ...
// ------------------------------------------------------------
struct Faktor { ll n, r; };

struct Suku {
    ll koef;
    vector<Faktor> f;
};

struct Ekspresi {
    vector<Suku> suku;
    ll pembagiM;     // dibagi m! (0 = tidak dipakai)
    bool peluang;    // true -> dibagi ruang sampel C(N,R)
    ll N, R;
    Ekspresi() : pembagiM(0), peluang(false), N(0), R(0) {}
};

Suku buatSuku(ll koef, const vector<Faktor>& f){
    Suku s; s.koef = koef; s.f = f;
    return s;
}

Ekspresi buatEkspresi(const vector<Suku>& s, ll m = 0, bool pel = false, ll N = 0, ll R = 0){
    Ekspresi e;
    e.suku = s; e.pembagiM = m; e.peluang = pel; e.N = N; e.R = R;
    return e;
}

ll nilaiSuku(const Suku& s){
    ll h = s.koef;
    for(size_t i = 0; i < s.f.size(); i++) h = kali(h, kombin(s.f[i].n, s.f[i].r));
    return h;
}

string teksSuku(const Suku& s){
    string t;
    if(s.koef != 1) t = to_string(s.koef);
    for(size_t i = 0; i < s.f.size(); i++){
        if(!t.empty()) t += " x ";
        t += "C(" + to_string(s.f[i].n) + "," + to_string(s.f[i].r) + ")";
    }
    if(t.empty()) t = "1";
    return t;
}

// ------------------------------------------------------------
//  Utilitas tampilan dan input
// ------------------------------------------------------------
void garis(){
    cout << "======================================================\n";
}

void tunggu(){
    cout << "\nTekan sembarang tombol untuk lanjut...";
    _getch();
}

ll baca(const string& pesan, ll mn, ll mx){
    ll x;
    while(true){
        cout << pesan;
        if(cin >> x){
            if(x >= mn && x <= mx) return x;
        } else {
            if(cin.eof()) exit(0);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Input tidak valid (harus " << mn << " sampai " << mx << "). Ulangi.\n";
    }
}

bool bacaYT(const string& pesan){
    char c;
    while(true){
        cout << pesan << " (y/n): ";
        if(!(cin >> c)){
            if(cin.eof()) exit(0);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if(c == 'y' || c == 'Y') return true;
        if(c == 'n' || c == 'N') return false;
    }
}

void tanyaPeluang(Ekspresi& e){
    if(bacaYT("Hitung sebagai peluang (dibagi ruang sampel C(N,R))?")){
        e.peluang = true;
        e.N = baca("  N (banyak seluruh objek): ", 1, 1000);
        e.R = baca("  R (banyak objek yang diambil): ", 0, e.N);
    }
}

// ------------------------------------------------------------
//  Penyelesaian dan tampilan langkah
// ------------------------------------------------------------
void tampilkanHasil(const Ekspresi& e){
    meluap = false;
    int langkah = 1;

    cout << "\n";
    garis();
    cout << "Rumus: C(n,r) = n! / ((n-r)! x r!)\n\n";

    // Langkah 1: nilai tiap kombinasi (tanpa duplikat)
    cout << langkah++ << ". Nilai tiap kombinasi\n";
    vector<Faktor> sudah;
    for(size_t i = 0; i < e.suku.size(); i++){
        for(size_t j = 0; j < e.suku[i].f.size(); j++){
            Faktor f = e.suku[i].f[j];
            bool ada = false;
            for(size_t k = 0; k < sudah.size(); k++)
                if(sudah[k].n == f.n && sudah[k].r == f.r) ada = true;
            if(ada) continue;
            sudah.push_back(f);
            cout << "   C(" << f.n << "," << f.r << ") = " << bentukFaktorial(f.n, f.r)
                << " = " << kombin(f.n, f.r) << "\n";
        }
    }
    if(sudah.empty()) cout << "   (tidak ada kombinasi)\n";

    // Langkah 2: nilai tiap suku (aturan perkalian)
    cout << "\n" << langkah++ << ". Nilai tiap suku (aturan perkalian)\n";
    vector<ll> nilai;
    ll total = 0;
    for(size_t i = 0; i < e.suku.size(); i++){
        ll v = nilaiSuku(e.suku[i]);
        nilai.push_back(v);
        total = tambah(total, v);
        cout << "   " << teksSuku(e.suku[i]) << " = " << v << "\n";
    }

    // Langkah 3: jumlah semua kasus (aturan penjumlahan)
    if(e.suku.size() > 1){
        cout << "\n" << langkah++ << ". Jumlah semua kasus (aturan penjumlahan)\n   ";
        for(size_t i = 0; i < nilai.size(); i++){
            if(i) cout << " + ";
            cout << nilai[i];
        }
        cout << " = " << total << "\n";
    }

    // Langkah 4: pembagi m! (kelompok yang tidak berlabel)
    if(e.pembagiM >= 2){
        ll mf = faktorial(e.pembagiM);
        cout << "\n" << langkah++ << ". Kelompok tidak berlabel -> bagi dengan " << e.pembagiM << "!\n";
        cout << "   " << total << " / " << e.pembagiM << "! = " << total << " / " << mf;
        if(mf != 0) total /= mf;
        cout << " = " << total << "\n";
    }

    // Langkah 5: peluang
    if(e.peluang){
        ll pen = kombin(e.N, e.R);
        cout << "\n" << langkah++ << ". Ruang sampel\n";
        cout << "   C(" << e.N << "," << e.R << ") = " << bentukFaktorial(e.N, e.R)
            << " = " << pen << "\n";
        cout << "\n" << langkah++ << ". Peluang = kejadian / ruang sampel\n";
        if(pen == 0){
            cout << "   Ruang sampel = 0, periksa kembali N dan R.\n";
        } else {
            ll g = gcdLL(total, pen);
            if(g == 0) g = 1;
            cout << "   P = " << total << " / " << pen
                << " = " << total / g << " / " << pen / g << "\n";
            double p = (double)total / (double)pen;
            cout << fixed << setprecision(6);
            cout << "   P = " << p << "  (" << setprecision(2) << p * 100 << "%)\n";
            cout.unsetf(ios::fixed);
            cout << setprecision(6);
            if(total > pen) cout << "   Catatan: P > 1, kemungkinan ada input yang salah.\n";
        }
    } else {
        cout << "\nJawaban: " << total << " cara\n";
    }

    if(meluap)
        cout << "\nPERINGATAN: ada hasil yang melebihi batas long long, nilainya tidak akurat.\n";
    garis();
}

// ------------------------------------------------------------
//  Template input soal
// ------------------------------------------------------------

// 1. C(n,r) tunggal
void soalTunggal(){
    Ekspresi e;
    cout << "Memilih r objek dari n objek berbeda (urutan tidak diperhatikan).\n";
    ll n = baca("n: ", 0, 1000);
    ll r = baca("r: ", 0, n);
    vector<Faktor> f; Faktor x = {n, r}; f.push_back(x);
    e.suku.push_back(buatSuku(1, f));
    tanyaPeluang(e);
    tampilkanHasil(e);
}

// 2. Banyak himpunan bagian berukuran k_min .. k_max
void soalHimpunanBagian(){
    Ekspresi e;
    cout << "Himpunan bagian dengan ukuran k_min sampai k_max dari himpunan n elemen.\n"
        << "(Semua ukuran: k_min=0, k_max=n -> hasilnya 2^n)\n";
    ll n    = baca("n (banyak elemen himpunan): ", 0, 1000);
    ll kmin = baca("Ukuran bagian minimum: ", 0, n);
    ll kmax = baca("Ukuran bagian maksimum: ", kmin, n);
    for(ll k = kmin; k <= kmax; k++){
        vector<Faktor> f; Faktor x = {n, k}; f.push_back(x);
        e.suku.push_back(buatSuku(1, f));
    }
    tanyaPeluang(e);
    tampilkanHasil(e);
}

// 3. Panitia dari dua kelompok (laki-laki/perempuan, dsb)
void soalDuaKelompok(){
    Ekspresi e;
    cout << "Memilih panitia r orang dari kelompok A (nA orang) dan kelompok B (nB orang).\n"
        << "Tiap kasus: a orang dari A, (r-a) orang dari B -> C(nA,a) x C(nB,r-a).\n";
    ll nA = baca("nA (banyak anggota kelompok A): ", 0, 1000);
    ll nB = baca("nB (banyak anggota kelompok B): ", 0, 1000);
    ll r  = baca("r  (ukuran panitia): ", 1, nA + nB);
    cout << "Cara menentukan kasus:\n  1. Rentang jumlah anggota A (a_min sampai a_max)\n"
        << "  2. Daftar kasus satu per satu\n";
    ll mode = baca("Pilihan: ", 1, 2);

    vector<ll> kasus;
    if(mode == 1){
        ll a1 = baca("a_min: ", 0, r);
        ll a2 = baca("a_max: ", a1, r);
        for(ll a = a1; a <= a2; a++) kasus.push_back(a);
    } else {
        ll k = baca("Banyak kasus: ", 1, 50);
        for(ll i = 1; i <= k; i++){
            kasus.push_back(baca("  Kasus ke-" + to_string(i) + ", jumlah anggota A: ", 0, r));
        }
    }
    for(size_t i = 0; i < kasus.size(); i++){
        vector<Faktor> f;
        Faktor x = {nA, kasus[i]};         f.push_back(x);
        Faktor y = {nB, r - kasus[i]};     f.push_back(y);
        e.suku.push_back(buatSuku(1, f));
    }
    tanyaPeluang(e);
    tampilkanHasil(e);
}

// 4. Membagi N objek ke beberapa kelompok
void soalBagiKelompok(){
    Ekspresi e;
    cout << "Membagi N objek ke beberapa kelompok dengan ukuran tertentu:\n"
        << "C(N,s1) x C(N-s1,s2) x C(N-s1-s2,s3) x ...\n";
    ll N = baca("N (banyak objek): ", 1, 1000);
    ll g = baca("Banyak kelompok: ", 1, 20);
    ll sisa = N;
    vector<Faktor> f;
    for(ll i = 1; i <= g; i++){
        ll s = baca("  Ukuran kelompok ke-" + to_string(i) + ": ", 0, sisa);
        Faktor x = {sisa, s}; f.push_back(x);
        sisa -= s;
    }
    e.suku.push_back(buatSuku(1, f));
    cout << "Jika sebagian kelompok berukuran sama dan TIDAK berlabel (mis. pasangan),\n"
        << "hasil dibagi m! dengan m = banyak kelompok tak berlabel itu.\n";
    ll m = baca("m (0 jika semua kelompok berlabel): ", 0, g);
    if(m >= 2) e.pembagiM = m;
    tanyaPeluang(e);
    tampilkanHasil(e);
}

// 5. Ekspresi bebas: jumlah dari (koefisien x C x C x ...)
void soalBebas(){
    Ekspresi e;
    cout << "Ekspresi bebas: jumlah dari beberapa suku, tiap suku = koefisien x C(n,r) x C(n,r) x ...\n";
    ll banyak = baca("Banyak suku: ", 1, 20);
    for(ll i = 1; i <= banyak; i++){
        cout << "Suku ke-" << i << ":\n";
        ll koef = baca("  Koefisien (1 jika tidak ada, mis. 2^8 = 256): ", 0, 1000000000LL);
        ll jf   = baca("  Banyak faktor C(n,r): ", 0, 10);
        vector<Faktor> f;
        for(ll j = 1; j <= jf; j++){
            ll n = baca("    n faktor ke-" + to_string(j) + ": ", 0, 1000);
            ll r = baca("    r faktor ke-" + to_string(j) + ": ", 0, n);
            Faktor x = {n, r}; f.push_back(x);
        }
        e.suku.push_back(buatSuku(koef, f));
    }
    ll m = baca("Dibagi m! (0 jika tidak ada), m: ", 0, 20);
    if(m >= 2) e.pembagiM = m;
    tanyaPeluang(e);
    tampilkanHasil(e);
}

// ------------------------------------------------------------
//  Contoh soal (diambil dari soal-soal buku Matematika Diskrit)
// ------------------------------------------------------------
struct Contoh {
    string judul;
    string teks;
    Ekspresi e;
};

Faktor F(ll n, ll r){ Faktor x = {n, r}; return x; }

vector<Contoh> daftarContoh(){
    vector<Contoh> v;
    Contoh c;

    // Soal Latihan no. 8
    c.judul = "Soal Latihan no. 8(a)";
    c.teks  = "Di dalam sebuah kelas terdapat 100 mahasiswa, 40 orang di antaranya laki-laki.\n"
            "Berapa banyak cara dapat dibentuk sebuah panitia 10-orang?";
    c.e = buatEkspresi({ buatSuku(1, {F(100,10)}) });
    v.push_back(c);

    c.judul = "Soal Latihan no. 8(b)";
    c.teks  = "Ulangi (a) jika banyaknya laki-laki harus sama dengan banyaknya perempuan.";
    c.e = buatEkspresi({ buatSuku(1, {F(40,5), F(60,5)}) });
    v.push_back(c);

    c.judul = "Soal Latihan no. 8(c)";
    c.teks  = "Ulangi (a) jika panitia terdiri dari enam laki-laki dan empat perempuan\n"
            "atau empat laki-laki dan enam perempuan.";
    c.e = buatEkspresi({ buatSuku(1, {F(40,6), F(60,4)}),
                        buatSuku(1, {F(40,4), F(60,6)}) });
    v.push_back(c);

    // no. 9, 10, 11
    {
        vector<Suku> s;
        for(ll k = 0; k <= 5; k++) s.push_back(buatSuku(1, {F(5,k)}));
        c.judul = "Soal Latihan no. 9";
        c.teks  = "Carilah jumlah himpunan bagian dari A = {a, b, c, d, e}.\n"
                "(Hasilnya harus sama dengan 2^5)";
        c.e = buatEkspresi(s);
        v.push_back(c);
    }
    {
        vector<Suku> s;
        for(ll k = 6; k <= 10; k++) s.push_back(buatSuku(1, {F(10,k)}));
        c.judul = "Soal Latihan no. 10";
        c.teks  = "Berapakah jumlah himpunan bagian dari B = {1, 2, ..., 10}\n"
                "yang mempunyai anggota paling sedikit enam?";
        c.e = buatEkspresi(s);
        v.push_back(c);
    }
    {
        vector<Suku> s;
        for(ll k = 2; k <= 10; k++) s.push_back(buatSuku(1, {F(10,k)}));
        c.judul = "Soal Latihan no. 11";
        c.teks  = "Seseorang mempunyai 10 kawan. Berapa banyak cara ia dapat pergi\n"
                "makan ke restoran dengan dua atau lebih kawannya?";
        c.e = buatEkspresi(s);
        v.push_back(c);
    }

    // no. 16
    c.judul = "Soal Latihan no. 16";
    c.teks  = "Sebuah kelompok terdiri dari 7 orang pria dan 4 orang wanita. Berapa banyak\n"
            "perwakilan 4-orang yang dapat dibentuk jika paling sedikit harus ada 2 wanita?";
    c.e = buatEkspresi({ buatSuku(1, {F(7,2), F(4,2)}),
                        buatSuku(1, {F(7,1), F(4,3)}),
                        buatSuku(1, {F(7,0), F(4,4)}) });
    v.push_back(c);

    // no. 18
    c.judul = "Soal Latihan no. 18";
    c.teks  = "Klub mobil antik beranggotakan 6 pria dan 5 wanita. Berapa banyak jumlah\n"
            "panitia 5-orang jika paling sedikit satu pria dan satu wanita?\n"
            "(Kasus: 1..4 pria. Cek: C(11,5) - C(6,5) - C(5,5) = 455)";
    c.e = buatEkspresi({ buatSuku(1, {F(6,1), F(5,4)}),
                        buatSuku(1, {F(6,2), F(5,3)}),
                        buatSuku(1, {F(6,3), F(5,2)}),
                        buatSuku(1, {F(6,4), F(5,1)}) });
    v.push_back(c);

    // no. 21
    c.judul = "Soal Latihan no. 21";
    c.teks  = "Dalam berapa banyak cara 22 buku yang berbeda dapat diberikan kepada 5 mahasiswa\n"
            "sehingga 2 di antara mereka memperoleh 5 buku dan 3 lainnya memperoleh 4 buku?\n"
            "(C(5,2) untuk memilih siapa yang mendapat 5 buku)";
    c.e = buatEkspresi({ buatSuku(10, {F(22,5), F(17,5), F(12,4), F(8,4), F(4,4)}) });
    v.push_back(c);

    // Bab 6 no. 29
    c.judul = "Bab 6 no. 29";
    c.teks  = "Lima belas pemain basket akan direkrut oleh tiga tim profesional di Bandung,\n"
            "Jakarta, dan Surabaya, sedemikian sehingga setiap tim merekrut lima pemain.\n"
            "Dalam berapa banyak cara ini dapat dilakukan?";
    c.e = buatEkspresi({ buatSuku(1, {F(15,5), F(10,5), F(5,5)}) });
    v.push_back(c);

    // Bab 6 no. 31 (contoh n = 4)
    c.judul = "Bab 6 no. 31 (contoh n = 4)";
    c.teks  = "Berapa banyak cara 2n orang dapat dibagi menjadi n pasangan? (n = 4, 8 orang)\n"
            "Pasangan tidak berlabel -> dibagi 4!";
    c.e = buatEkspresi({ buatSuku(1, {F(8,2), F(6,2), F(4,2), F(2,2)}) }, 4);
    v.push_back(c);

    // Bab 6 no. 40
    c.judul = "Bab 6 no. 40";
    c.teks  = "Berapa peluang dari 5 buah kartu remi yang dibagi tidak mengandung ratu satu pun?";
    c.e = buatEkspresi({ buatSuku(1, {F(48,5)}) }, 0, true, 52, 5);
    v.push_back(c);

    // Bab 6 no. 44
    c.judul = "Bab 6 no. 44";
    c.teks  = "Ada sepuluh pasang sepatu di dalam lemari. Jika delapan sepatu diambil secara acak,\n"
            "berapa peluang tidak ada sepasang sepatu yang terambil?\n"
            "(pilih 8 dari 10 pasang, lalu 1 sepatu dari tiap pasang: C(10,8) x 2^8)";
    c.e = buatEkspresi({ buatSuku(256, {F(10,8)}) }, 0, true, 20, 8);
    v.push_back(c);

    return v;
}

void menuContoh(){
    vector<Contoh> v = daftarContoh();
    while(true){
        system("cls");
        garis();
        cout << setw(14) << "" << "Contoh Soal\n";
        garis();
        for(size_t i = 0; i < v.size(); i++)
            cout << setw(2) << i + 1 << ". " << v[i].judul << "\n";
        cout << " 0. Kembali\n";
        ll p = baca("Soal yang anda pilih: ", 0, (ll)v.size());
        if(p == 0) return;
        system("cls");
        garis();
        cout << v[p - 1].judul << "\n";
        garis();
        cout << v[p - 1].teks << "\n";
        tampilkanHasil(v[p - 1].e);
        tunggu();
    }
}

void menuInput(){
    while(true){
        system("cls");
        garis();
        cout << setw(14) << "" << "Input Soal Sendiri\n";
        garis();
        cout << "Jenis soal:\n"
            << "1. C(n,r) tunggal\n"
            << "2. Himpunan bagian berukuran k_min..k_max\n"
            << "3. Panitia dari dua kelompok (laki-laki/perempuan, dll.)\n"
            << "4. Membagi objek ke beberapa kelompok\n"
            << "5. Ekspresi bebas (jumlah dari hasil kali kombinasi)\n"
            << "6. Kembali\n";
        ll p = baca("Pilihan anda: ", 1, 6);
        if(p == 6) return;
        system("cls");
        garis();
        switch(p){
            case 1: soalTunggal();        break;
            case 2: soalHimpunanBagian(); break;
            case 3: soalDuaKelompok();    break;
            case 4: soalBagiKelompok();   break;
            case 5: soalBebas();          break;
        }
        tunggu();
    }
}

int main(){
    bool jalan = true;
    while(jalan){
        system("cls");
        garis();
        cout << setw(12) << "" << "Kombinasi tanpa Perulangan\n";
        garis();
        cout << "Apa yang anda mau lakukan?\n"
            << "1. Contoh soal\n"
            << "2. Input soal\n"
            << "3. Keluar\n";
        ll pil = baca("Pilihan anda: ", 1, 3);
        switch(pil){
            case 1: menuContoh(); break;
            case 2: menuInput();  break;
            case 3: jalan = false; break;
        }
    }
    return 0;
}

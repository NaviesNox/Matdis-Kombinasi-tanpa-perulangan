#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <conio.h>
#define ll long long
using namespace std;


//C(n,r)=n!/(n-r)!r!
ll faktorial(ll a){
    if(a <= 1) return 1;
    return a*(faktorial(a-1));
}

ll kombin(ll a, ll b){
    if(b < 0 || b > a) return 0;
    if(b == 0 || b == a) return 1;

    if(b > a-b) b = a-b;
    ll hsl=1;
    for(int i=1; i<=b; i++){
        hsl *= (a-b+i);
        hsl /= i;
    }
    return hsl;
}

void kombinbentuk(ll a, ll b){
    cout << "(" << a << "! / ( " << a-b << "! * " << b <<"!))";
    
}

void kombin2(){ //C(mhs,panit), C(n1,r1)*C(n2,r2)
    ll n1, n2, r1, r2, cara;
    char opr;
    cout << "Masukkan n1: "; cin >> n1;
    cout << "Masukkan n2: "; cin >> n2;
    cout << "Masukkan r1: "; cin >> r1;
    cout << "Masukkan r2: "; cin >> r2;
    cout << "Masukkan operator: "; cin >> opr;
    switch(opr){
        case '+':
        cara = kombin(n1, r1) + kombin(n2, r2);
        break;
        case '-':
        cara = kombin(n1, r1) - kombin(n2, r2);
        break;
        case '*':
        cara = kombin(n1, r1) * kombin(n2, r2);
        break;
        case '/':
        cara = kombin(n1, r1) / kombin(n2, r2);
        break;
    }

    cout << "C("<<n1<<"," << r1 << ") " << opr << " C(" << n2 << "," << r2 << ")" << " = "; 
    kombinbentuk(n1,r1);
    cout << " " << opr << " "; kombinbentuk(n2,r2);
    cout << " = " << cara << endl;
    _getch();
}


int main() {
    system("cls");
    int pilatas, pilcontoh;
    bool keluar1 = true ,keluar2 = true;

    do{
        system("cls");
        cout << "======================================================\n"
        << setw(12) << "" << "Kombinasi tanpa Perulangan\n"
        << "======================================================\n";
        cout << "Apa yang anda mau lakukan?\n"
        << "1. Contoh soal\n" << "2. Input soal\n" << "3. Keluar\n";
        cout << "Pilihan anda: "; cin >> pilatas;
        
        switch(pilatas){
            case 1:
            cout << "======================================================\n";
            do{
                cout << "Pilihan Soal:\n"
                << "1. Soal 1:\n2. Soal 2:\n3. Soal 3:\n4. Keluar\n";
                cout << "Soal yang anda pilih: "; cin >> pilcontoh;
                switch(pilcontoh){
                    case 1:{
                    ll mhs = 100, panit = 10, cara;
                    cout << "======================================================\n"
                    << "1. Didalam sebuah kelas terdapat 100 mahasiswa, 40 diantara orangnya adalah laki-laki."
                    << "Berapa banyak cara dapat dibentuk sebuah panitia 10-orang\n";
                    cara = kombin(mhs, panit);
                    cout << "Jawab:\n"; 
                    kombinbentuk(mhs, panit);
                    cout << " = " << cara << endl;
                    _getch();
                    break;
                    }
                    case 2:{
                    ll mhs = 100, panit = 10;
                    cout << "======================================================\n"
                    << "2. Didalam sebuah kelas terdapat 100 mahasiswa, 40 diantara orangnya adalah laki-laki. "
                    << "Berapa banyak cara dapat dibentuk sebuah panitia 10-orang jika banyaknya laki-laki yang dipilih"
                    << " sama banyak dengan yang perempuan.\n";
                    cout << "Jawab:\n";
                    kombin2();
                    break;
                    }
                    case 3:{
                    ll mhs = 100, panit = 10;
                    cout << "======================================================\n"
                    << "3. Didalam sebuah kelas terdapat 100 mahasiswa, 40 diantara orangnya adalah laki-laki. "
                    << "Berapa banyak cara dapat dibentuk sebuah panitia 10-orang jika laki-laki yang dipilih 6"
                    << " dan perempuannya 4 atau laki laki yang dipilih 4 dan perempuannya 6.\n";
                    cout << "Jawab:\n";
                    kombin2();
                    break;
                    }
                    case 4:{
                        keluar2 = false;
                        break;
                    }
                    default:{
                        cout << "Input yang anda masukkan salah. Ulangi\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }
            } while(keluar2);
            system("pause");
            break;
            case 2:

            break;
            // case 3:
            // default:
        }
    } while(keluar1);
    return 0;
}


#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <vector>
#include <queue>
using namespace std;

using namespace std::chrono;

ifstream fin ("teste.in");

vector<int> bubble_sort (vector<int> v, int n){
    /* Algoritmul de sortare bubble sort cu o mica optimizare. Functia returneaza vectorul sortat
       v - vectorul initial / n - numar elemente
       k - numar de iteratii prin v
       ord = 1 -> vectorul este sortat / ord = 0 -> vectorul nu e sortat
       La fiecare iterare prin vector al k-lea cel mai mare element ajunge pe pozitia corecta
       Complexitate O(n^2) */

    bool ord = 0;
    int i, k = 0;

    while(!ord){
        ord = 1;   ///presupunem ca vectorul e sortat
        ++k;
        for(i=0;i<n-k;++i)
            if(v[i] > v[i+1]){
                swap(v[i], v[i+1]);
                ord = 0;
            }

    }

    return v;
}

vector<int> count_sort (vector<int> v, int n, int vmax){
    /* Funcția returnează vectorul sortat folosind count sort
       f - vector de frecventa => foloseste multa memorie suplimentara
       Complexitate: timp - O(vmax + n), memorie O(vmax)
       Foarte rapid cand vmax e mic*/

        int f[vmax + 1] = {0};
        vector<int> aux;
        aux.reserve(n);
        for(int i=0;i<n;++i)
            f[v[i]] ++;

        for(int i = 0; i <= vmax; ++i)  /// parcurgem vectorul de frecventa si extragem elementele in ordinea buna
            while(f[i]--)
                aux.push_back(i);

    return aux;

}

void insertion_sort(vector<int> &v, int st, int dr){
    /* Folosim aceasta functie pentru Tim Sort deoarece
        insertion sort merge repede pe vectori mici.
       Functia sorteaza vectorul v de la indicele din stanga st pana in dreapta dr
       Complexitate: O(n^2)*/

       int el, j;
       for(int i = st + 1;i<=dr;i++){
            el = v[i];  /// el - elementul curent
            j = i - 1;
            while(j >= st && v[j] > el)
            {
                v[j+1] = v[j];  /// mutam toate numerele mai mari decat el o pozitie la dreapta
                j--;
            }
            v[j+1] = el;  /// inseram el pe pozitia eliberata (pozitia din vectorul sortat)
       }
}

inline void flip(vector<int> &v, int dr){

    int st = 0;

    while(st < dr){
        swap(v[st], v[dr]);
        st++;
        dr--;
    }



}

inline int find_max(vector<int> v, int n){

    int poz = 0;
    for(int i=0;i<n;++i)
        if(v[poz] < v[i])
        poz = i;
    return poz;
}

void pancake_sort(vector<int> &v, int n){
    /*Pentru a sorta vectorul avem la dispozitie functia flip care primeste ca parametru
    o pozitie i inverseaza elementele vectorului aflate intre pozitia 0 si cea data;
    scopul e sa sortam vectorul folosind flip de cat mai putine ori
       Plecam de la vectorul initial de lungime n si punem la fiecare pas cel mai mare
    element pe pozitia potrivita, reducem problema la sortarea a n-1 elemente
    Complexitate: O(n^2)*/

    for(int length = n; length > 1; --length){  /// lungimea initiala este n -> parcurgem pana la lungimea de 1
        int maxi = find_max(v, length); /// cautam maximul si il aducem pe pozitia potrivita
        if(maxi != length){
            flip(v, maxi);
            flip(v, length - 1);
        }

    }

}

inline void interclasare(vector<int> &v, int st, int m, int dr){
    /* Functia preia doi vectori sortati si furnizeaza un vector sortat
    format din elementele vectorilor furnizati ca parametrii*/

    vector<int> aux;
    int i, j;

    i = st;
    j = m + 1;

    while(i <= m && j <= dr)
        if(v[i] < v[j])
            aux.push_back(v[i++]);
        else
            aux.push_back(v[j++]);

    while (i <= m)
        aux.push_back(v[i++]);
    while (j <= dr)
        aux.push_back(v[j++]);

    for(i=st, j=0;i<=dr;++i,++j)
        v[i] = aux[j];

}

void merge_sort(vector<int> &v, int st, int dr){
    /* v - vector initial / st - pozitie stanga / dr - pozitie dreapta
       m - mijloc
       Complexitate: O(n log n)*/

    if(st < dr){
        int m = (st + dr) / 2;
        merge_sort(v, st, m);
        merge_sort(v, m+1, dr);   /// Divide
        interclasare(v, st, m , dr);    /// Impera
    }
}

void quick_sort(vector<int> &v, int st, int dr){
    /* Quick sort cu pivot ales in mod aleator.
       v - vector initial / st - stanga / dr - dreapta
       m - middle
       p - pivot
       Complexitate: O(n log n) -> O(n^2) pe cel mai nefavorabil caz (putin probabil)*/

    int i = st, j = dr;
    int x = v[rand() % (dr - st + 1) + st];

    while(i < j){
        while(v[i] < x)
            ++i;
        while(v[j] > x)
            --j;
        if(i <= j)
            swap(v[i++], v[j--]);
    }
    if(j > st) quick_sort(v, st, j);
    if(i < dr) quick_sort(v, i, dr);
}

void radix_sort_10(vector<int> &v, int n){
    /* LSD Radix sort cu baza 10*/

    queue<int> buckets[10];
    int  pow = 1, i, k = 1;

    do{
        for(i=0;i<n;++i)
            buckets[(v[i] / pow) % 10].push(v[i]);


        pow *= 10;   /// mutam pe urmatoarea cifra a numarului
        k += 1;
        i = 0;

        for(int j=0;j<10 && i<n; ++j)  /// extragem numerele din buckets
            while(!buckets[j].empty()){
                v[i++] = buckets[j].front();
                buckets[j].pop();

            }

    }while(k<10);  /// un int are maxim 10 cifre


}

void radix_sort_256(vector<int> &v, int n){
    /* LSD Radix sort cu baza 256 si operatii pe biti */

    queue<int> buckets[256];
    int  k = 0, i;

    do{
        for(i=0;i<n;++i)
            buckets[(v[i] >> k) & (255)].push(v[i]); /// x >> b = x / 2^b -> urmatoarea cifra a lui x / x & (b-1) = x % b


        k += 8;   /// mutam pe urmatoarea cifra a numarului
        i = 0;

        for(int j=0;j<256 && i<n; ++j)  /// extragem numerele din buckets
            while(!buckets[j].empty()){
                v[i++] = buckets[j].front();
                buckets[j].pop();

            }

    }while(k <= 24);  /// un int are maxim 32 de cifre in baza 2, deci daca impartim la 2^24 vom obtine doar numere de o cifra


}

inline vector<int> generate_random(int n, int vmax){
    /* returneaza un vector de n numere din intervalul [0, vmax]*/

    vector<int> v;
    //v.reserve(n);
    for(int i=0;i<n;++i)
        v.push_back(rand() % vmax + 1);
    return v;
}

inline vector<int> generate_sorted_asc(int n, int vmax){
    /* returneaza un vector de n numere sortat crescator unde v[n - 1] = vmax*/

    vector<int> v;
    //v.reserve(n);
    for(int i=0;i<n;i++)
        if(i<=vmax) v.push_back(i);
        else v.push_back(vmax);

    v[n - 1] = vmax;
    return v;
}

inline vector<int> generate_sorted_desc(int n, int vmax){
    /* returneaza un vector de n numere sortat descrescator unde v[0] = vmax*/

    vector<int> v;
    int i = 1;
    //v.reserve(n);
    v.push_back(vmax);
    while(i < n){
        if(v[i - 1] > 0)
            v.push_back(v[i - 1] - 1);
        else v.push_back(0);
        i++;
    }
    return v;
}

inline vector<int> generate_mountain(int n, int vmax){
    /* returneaza un vector de n numere unde prima jumatate a vectorului este
    sortata crescator si a doua jumatate este sortata descrescator*/

    vector<int> v;
    //v.reserve(n);
    for(int i=0;i<n/2;i++)
        if(i<=vmax)
            v.push_back(i);
        else
            v.push_back(vmax);


    for(int i=n/2;i<n;i++)
        if(vmax >= 1)
            v.push_back(vmax --);
         else
            v.push_back(0);

    return v;
}

inline bool verify(vector<int> v, int n){
    /* verifica daca vectorul este sortat */

    for(int i=0;i<n-1;++i)
        if(v[i] > v[i + 1])
        return false;

    return true;

}

int main()
{
    int n, vmax, T;
    vector<int> v, aux;

    fin>>T;  /// numarul de teste

    for(int i=1;i<=T;++i){
        fin>>n>>vmax;

        v.clear();  /// stergem elementele din vectorul anterior
        //v = generate_random(n, vmax);  /// generam un nou vector
        v = generate_sorted_asc(n, vmax);
        //v = generate_sorted_desc(n, vmax);
        //v = generate_mountain(n, vmax);

        cout<<"TESTUL "<<i<<" (N = "<<n<<", MaxValue = "<<vmax<<" )\n";

        /// Bubble Sort
        if(n > 10000)  /// 10^8 operatii = 1 secunda
            cout<<"Bubble Sort: FAILED ... N is too large!\n";
        else{
            auto start = high_resolution_clock::now();
            aux = bubble_sort(v, n);
            auto stop = high_resolution_clock::now();

            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Bubble Sort: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Bubble Sort: FAILED ... did not sort properly!\n";

        }

        /// Count Sort
        if(vmax >= 100000)  /// prea multa memorie suplimentara
            cout<<"Count Sort: FAILED ... MaxValue is too large!\n";
        else
            if(n > 100000000)
                cout<<"Count Sort: FAILED ... N is too large!\n";
            else{
                auto start = high_resolution_clock::now();
                aux = count_sort(v, n, vmax);
                auto stop = high_resolution_clock::now();

                if(verify(aux, n)){
                    auto duration = duration_cast<milliseconds>(stop-start);
                    cout<<"Count Sort: SUCCESS ... "<< duration.count() << " ms\n";
                }
                else
                cout<<"Count Sort: FAILED ... did not sort properly!\n";
            }

        /// Pancake Sort
        if(n > 10000)
            cout<<"Pancake Sort: FAILED ... N is too large!\n";
        else{
            aux = v;
            auto start = high_resolution_clock::now();
            pancake_sort(aux, n);
            auto stop = high_resolution_clock::now();

            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Pancake Sort: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Pancake Sort: FAILED ... did not sort properly!\n";

        }

        /// Insertion Sort
        if(n > 10000)
            cout <<"Insertion Sort: FAILED ... N is too large!\n";
        else{
            aux = v;
            auto start = high_resolution_clock::now();
            insertion_sort(aux, 0, n - 1);  /// capatul din dreapta e inclus
            auto stop = high_resolution_clock::now();

            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Insertion Sort: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Insertion Sort: FAILED ... did not sort properly!\n";
        }

        /// Merge Sort
        if(n >= 100000000)
            cout <<"Merge Sort: FAILED ... N is too large!\n";
        else{
            aux = v;
            auto start = high_resolution_clock::now();
            merge_sort(aux, 0, n - 1);  /// capatul din dreapta e inclus
            auto stop = high_resolution_clock::now();

            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Merge Sort: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Merge Sort: FAILED ... did not sort properly!\n";
        }

        /// Quick Sort
        if(n >= 100000000)
            cout <<"Quick Sort: FAILED ... N is too large!\n";
        else{
            aux = v;
            auto start = high_resolution_clock::now();
            quick_sort(aux, 0, n-1);  /// capatul din dreapta e inclus
            auto stop = high_resolution_clock::now();

            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Quick Sort: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Quick Sort: FAILED ... did not sort properly!\n";
        }

        /// Radix Sort
        if(n >= 100000000)
            cout<<"Radix Sort: FAILED ... N is too large!\n";
        else{
            /// Baza 10
            aux = v;
            auto start = high_resolution_clock::now();
            radix_sort_10(aux, n);
            auto stop = high_resolution_clock::now();
            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Radix Sort with base 10: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Radix Sort with base 10: FAILED ... did not sort properly!\n";

            /// Baza 256
            aux = v;
            start = high_resolution_clock::now();
            radix_sort_256(aux, n);
            stop = high_resolution_clock::now();
            if(verify(aux, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout<<"Radix Sort with base 256: SUCCESS ... "<< duration.count() << " ms\n";
            }
            else
                cout<<"Radix Sort with base 256: FAILED ... did not sort properly!\n";
        }

        /// STL Sort
        if(n >= 100000000)
            cout << "STL Sort: FAILED ... n is too large!\n";
        else{
            auto start = high_resolution_clock::now();
            sort(v.begin(),v.end());
            auto stop = high_resolution_clock::now();

            if(verify(v, n)){
                auto duration = duration_cast<milliseconds>(stop-start);
                cout << "STL Sort: SUCCESS ... " << duration.count() << " ms\n";
            }
            else
                cout << "STL Sort: FAILED ... did not sort properly\n";
        }
         cout << "--------------------------------\n";
    }
    return 0;
}


#include <bits/stdc++.h>
using namespace std;

long long operations = 0;
int currentDepth = 0;
int maxDepth = 0;

void complexRec(int n) {
    operations++;              
    currentDepth++;
    maxDepth = max(maxDepth, currentDepth);

    if (n <= 2) {
        currentDepth--;
        return;
    }

    int p = n;
    while (p > 0) {
        operations++;          
        vector<int> temp(n);
        for (int i = 0; i < n; i++) {
            temp[i] = i ^ p;
            operations++;
        }
        p >>= 1;
    }

    vector<int> small(n);
    for (int i = 0; i < n; i++) {
        small[i] = i * i;
        operations++;
    }

    reverse(small.begin(), small.end());
    operations += n;

    complexRec(n / 2);
    complexRec(n / 2);
    complexRec(n / 2);

    currentDepth--;
}

int main() {
    vector<int> sizes = {64, 128, 256, 512};

    for (int n : sizes) {
        operations = 0;
        currentDepth = 0;
        maxDepth = 0;

        auto start = chrono::high_resolution_clock::now();
        complexRec(n);
        auto end = chrono::high_resolution_clock::now();

        auto timeTaken =
            chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "n = " << n << endl;
        cout << "Operations = " << operations << endl;
        cout << "Max Recursion Depth = " << maxDepth << endl;
        cout << "Execution Time (ms) = " << timeTaken << endl;
        cout << "-----" << endl;
        
    }
    cout<<"Recurrance relation =3T(n/2)+n logn"<<endl;
    cout<<"Time complexity = O(n^log2^3)";
    return 0;
}

// Recurrance relation =3T(n/2)+n logn

// Time complexity = O(n^log2^3)

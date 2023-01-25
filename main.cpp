#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;


int sieving_primes[1229];

// simple serial sieve of Eratosthenes
void sequencial_eratosthenes(int n)
{

    bool prime[n + 1];
    memset(prime, true, sizeof(prime));
 
    for (int p = 2; p * p <= n; p++) {
        if (prime[p] == true) {

            for (int i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }
 

    int sieving_index = 0;
    // Print all prime numbers
    for (int p = 2; p <= n; p++) {
        if (prime[p]) {
            sieving_primes[sieving_index] = p;
            sieving_index++;
        }
    }
}


int main() {
    auto start = std::chrono::steady_clock::now();    
    sequencial_eratosthenes(10000);
    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    cout << elapsed_seconds << '\n';
}
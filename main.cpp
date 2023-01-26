#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
#include <mutex>
#include <thread>
using namespace std;

#define MAX_NUM 100000001

bitset<MAX_NUM> primes;

int sieving_primes[1229];
int sieving_prime_index = 0;

std::mutex retrieving_index;
std::mutex writingTo_bitset;

void concurrent_eratosthenes(int thread_num) {
    bool running = true;
    while(running) {

        retrieving_index.lock();
        if(sieving_prime_index > 1228) {
            running = false;
            retrieving_index.unlock();
            break;
        } 
        cout << "Thread " << thread_num << " retrieved index: " << sieving_prime_index <<": " << sieving_primes[sieving_prime_index] << '\n';
        int index = sieving_prime_index;
        sieving_prime_index++;
        retrieving_index.unlock();

/*
        int sieving_prime = sieving_primes[index];

        for(unsigned long long int i = sieving_prime*sieving_prime; i < MAX_NUM; i += sieving_prime) {

            //writingTo_bitset.lock();
            //cout << "Thread" << thread_num << "writing to index: " << i << '\n';
            primes[i] == 1;
            //writingTo_bitset.unlock();
        }
*/
    }
}

// Simple Sequential Eratosthenes sieve
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
    for (int p = 2; p <= n; p++) {
        if (prime[p]) {
            sieving_primes[sieving_index] = p;
            sieving_index++;
        }
    }
}

void print_count(){
    int count = 0;
    
    for(int i = 2; i < 50; i++) {
        //if(primes[i] == 0) {
            count++;
            cout << primes[i] << '\n';
        //}
    }

    count = 0;

    for(int i = 2; i < MAX_NUM; i++) {
        if(primes[i] == 0) {
            count++;
            //cout << i << '\n';
        }
    }

    cout << count << '\n';

}


int main() {
    auto start = std::chrono::steady_clock::now();    
    
    sequencial_eratosthenes(10000);
    
    std::thread t1(concurrent_eratosthenes, 1);
    std::thread t2(concurrent_eratosthenes, 2);
    std::thread t3(concurrent_eratosthenes, 3);
    std::thread t4(concurrent_eratosthenes, 4);
    std::thread t5(concurrent_eratosthenes, 5);
    std::thread t6(concurrent_eratosthenes, 6);
    std::thread t7(concurrent_eratosthenes, 7);
    std::thread t8(concurrent_eratosthenes, 8); 

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    //print_count();

    auto finish = std::chrono::steady_clock::now();
    
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    cout << elapsed_seconds << '\n';
}
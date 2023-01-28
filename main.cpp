#include <iostream>
#include <cstring>
#include <chrono>
#include <mutex>
#include <thread>
#include <fstream>
using namespace std;

#define MAX_NUM 100000001

bool primes[MAX_NUM];

int sieving_primes[1229];
int sieving_prime_index = 0;

std::mutex retrieving_index;

//Function to sieve primes concurrently according to Eratosthenes method.
void concurrent_eratosthenes() {
    bool running = true;
    
    while(running) {

        //Use a mutex to make sure accessing the global index doesn't cause data race.
        retrieving_index.lock();
        //If the index is past the max, stop checking.
        if(sieving_prime_index > 1228) {
            running = false;
            retrieving_index.unlock();
            break;
        } 
        //Save the index and increment it for the next thread that requests it.
        int index = sieving_prime_index;
        sieving_prime_index++;
        retrieving_index.unlock();

        //Save the prime number who's multiples we are sieving.
        int sieving_prime = sieving_primes[index];

        //Mark any multiples of the sieving prime assigned, as these are all composite.
        for(unsigned long long int i = sieving_prime*sieving_prime; i < MAX_NUM; i += sieving_prime) {
            primes[i] = false;
        }

    }
}

// Simple sequential Eratosthenes sieve. This function is used to find the sieving primes.
void sequencial_eratosthenes(int n)
{

    //Allocate space for the numbers.
    bool prime[n + 1];
    memset(prime, true, sizeof(prime));
 
    //Find next number that hasn't been marked as composite.
    for (int p = 2; p * p <= n; p++) {
        if (prime[p] == true) {
            //Mark all the multiples of the prime we found as composite.
            for (int i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }
 
    //Fill sieving primes array with primes found on this function.
    int sieving_index = 0;
    for (int p = 2; p <= n; p++) {
        if (prime[p]) {
            sieving_primes[sieving_index] = p;
            sieving_index++;
        }
    }
}

//Function to obtain the count and sum and last ten primes found.
void obtain_info(int *count_out, unsigned long long int *sum_out, int (&last_ten_out)[10]){
    int amount = 5761455;
    int last_ten_index = 0;

    //Single loop that does everything... nice.   
    for(int i = 2; i < MAX_NUM; i++) {
        if(primes[i] == true) {
            amount--;
            *sum_out += i;
            *count_out = *count_out + 1;
            if(amount < 10) {
                last_ten_out[last_ten_index] = i;
                last_ten_index++;
            }
        }
    }
}


int main() {

    //Initialize some variables and arrays.
    memset(sieving_primes, 0, sizeof(sieving_primes));
    memset(primes, true, sizeof(primes));
    int count_in = 0;
    unsigned long long int sum_in = 0;
    int last_ten_in[10] = {0};

    //Start timer.
    auto start = std::chrono::steady_clock::now();  

    //Obtain sieving primes.
    sequencial_eratosthenes(10000);
    //Sieve concurrently with 8 threads.
    std::thread t1(concurrent_eratosthenes);
    std::thread t2(concurrent_eratosthenes);
    std::thread t3(concurrent_eratosthenes);
    std::thread t4(concurrent_eratosthenes);
    std::thread t5(concurrent_eratosthenes);
    std::thread t6(concurrent_eratosthenes);
    std::thread t7(concurrent_eratosthenes);
    std::thread t8(concurrent_eratosthenes); 

    //Wait until all threads end.
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    //Calculate count, sum and last ten primes found.
    obtain_info(&count_in, &sum_in, last_ten_in);

    //End timer.
    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();

    //Print to file.
    ofstream file;
    file.open("primes.txt");
    file << elapsed_seconds << ' ' << count_in << ' ' << sum_in <<'\n';
    for(int i = 0; i < 10; i++) {
        file << last_ten_in[i] << ' ';
    }
    file.close();
}
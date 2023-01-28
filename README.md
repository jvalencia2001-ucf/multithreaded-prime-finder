# Multithreaded Prime Finder


## __Statments__

As the title states, this program is designed to be a multithreaded prime number finder which calculates the count and sum of the primes up to 100 million.

To use the program all you need is the main.cpp file and a computer which has g++ (preferably) or any other modern c++ compiler installed, then follow this simple steps:

 - Use a terminal and navigate to the folder where main.cpp is saved.

 - Compile the cpp file. If you are using g++, you can do this by executing:

    ```
    g++ main.cpp
    ```

 - Once the file is compiled, you are ready to execute the program. If you are using g++ you can do this by executing:

    ```
    ./a.out main
    ```

Once the program is executed, it will take a short time to calculate the primes from 0 to 100 million. The count and sum for the primes found withing this range, as well as the last ten primes found, will be written to a text file called _primes.txt_.

On my personal laptop (Intel® Core™ i7-1065G7 CPU @ 1.30GHz × 8). Calculating these values for the primes from 0 to 100 million takes in average 0.60 seconds using this program.

You can find a sample output for the program in _primes.txt_. The format is the following.

```
<Execution time in seconds> <Count> <Sum>
<Last then primes found in ascending order>.
```

## __Efficiency__

Like I just mentioned, the program will take only about 0.60 seconds to run on my computer, sometimes yeilding times as low as half a second. In addition, if the program does not calculate the sum and count at the end, but rather just sieves the composite numbers, it will run in almost half that time.

This fast times can be attributed to the algorithm the program uses to find the primes, as well as the fact that the majority of the algorith is running concurrently.

The algorithm used in this case is the Sieve of Eratosthenes. This is a pretty well known algorithm to find primes which runs with time complexity _O(n * log(log n))_. As for how I parellized the sieve, the program first find the primes up to the square root of 100 million sequentially, I called these the sieving primes. Then, the program concurrently uses 8 threads to mark all the indexes that are multiples of each of the sieving primes, 8 sieving primes at a time (one per thread). This process results on all the indexes that represent a composite number being marked in the array.

### How efficient is it compared to other methods?

I attempted to compare this program to two other methods to find the primes.

 - First, I wrote a simple program that found the primes using the classic method of divding each number _n_ by any number less than _n/2_ and checking if the reminder is 0. Attempting to use this program to find the primes up to 100 million resulted in a unacceptable undetermined wait time. My program is infinitely faster than this one lol.

 - My second attempt was to compare this program to a normal sequential Sieve of Eratosthenes. The sequential sieve found all the primes in an average time of 1.30 seconds. Therefore my program is at least a little more than two times faster than it's sequential counterpart.

### Is there any more speed-ups that could be implmented?

Yes, here is a list of some of the changes I think could generate a speed up in the program, altough I am sure the complete list is not limited to this.

 - The first and easiest way is to calculate the count and the sum concurently within the threads instead of sequentially after the threads. This can be done by saving the partial sum and count in each thread and adding it up at the end. 

  - The second method would be to implement a Sieve of Eratosthenes that only checks odd numbers, since every even number greater than 2 is obviously composite. Implementing this change for both the sequential and the concurrent sieve in this program should result in the program being faster by some constant factor.

  - The third and hardest way is to implement a way of finding the sieving primes concurrently. You can do this using a Sieve of Eratosthenes which finds the primes on a specific range, and then use that sieve so that each thread finds a range of sieving primes of its own. My best guess is that this would result in a big speed up because now the whole program is concurrent.

## __Proof of Correctness__

I tried to verify the correctness of the program in multiple different ways. To make sure that the program had the correct count, sum and last ten primes.
 
  - The first way I verified the answer the program provides is with other programs to find primes. Both the simple sequential Sieve of Eratosthenes and the square root method confirmed the output.

  - The second way I tried to verify the output is by looking for outside sources which also calculated the primes up to at least 100 million. All the sources I used seemed to confirm both the count and the last ten primes my program outputs. Here are some of the links to those outisde sources:

    - https://primes.utm.edu/howmany.html
    - http://compoasso.free.fr/primelistweb/page/prime/liste_online_en.php  

 - The third way to verify that my output was to simple check if the answer from the programs and websites I used in the first and second way to confirm were close to the purely mathematical approximation that was used to find the amount of primes before computers. Using the _x/lnx_ approximation confirms my output.

 ## __Experimental Evaluation__

 In addition to verifying the output of the program through the methods described above. The program was also tested on various other numbers as a limit, mostly smaller numbers which I could verify the output for. These includes but is not limited to: 

  - Using the smaller inputs 10, 50 and 100 as limits. To verify correctess manually.

  - Testing the output stayed correct for bigger numbers by testing with a limit of a billion.

In addition I tested the method using only one thread, to verify that the output did not change between using one or multiple threads, and also tested limits that were a prime number to know the program would work in this case.

The program stayed faithful throughout all tests and consistantly provided the correct output.

If you intend to test the program with other limits, keep in mind the programs purpose is to find the primes with a limit of 100 million, so there is a couple of steps to take to make it work with other limits: 

 - Change the globally defined constant _MAX_NUM_ to the limit you wish to test.

  - Change the size of the _sieving_primes_ array to the square root of what you set for _MAX_NUM_ rounding down to the nearest integer.

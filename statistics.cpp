//#include <iostream>: This header is used for input/output operations.
//
//#include <cstdlib>: It includes functions for memory allocation and generating random numbers.
//
//#include <chrono>: For measuring time intervals, especially for timing sequential and parallel computation.
//
//#include <omp.h>: For OpenMP, used to parallelize the computation.
//
//#include <limits>: Provides the INT_MAX and INT_MIN constants for setting initial extreme values.

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include <limits>

using namespace std;
//sequentialStats:
//
//Takes an integer array arr of size n, and computes the minimum, maximum, sum, and average.
//
//Uses simple loops to find the min, max, and sum, then calculates the average.
// Sequential computation
void sequentialStats(int* arr, int n, int &minVal, int &maxVal, long long &sum, double &avg) {
    minVal = INT_MAX;
    maxVal = INT_MIN;
    sum = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
        sum += arr[i];
    }
    avg = static_cast<double>(sum) / n;
}

//// Parallel computation
//Similar to sequentialStats, but this function leverages OpenMP parallelism to speed up the calculations.
//
//The array is divided across threads, and each thread computes the minimum, maximum, and sum for a subset of the array. The results are then merged using the critical section to ensure thread-safety.
//
//It uses #pragma omp parallel to start a parallel region and #pragma omp for to split the loop iterations among threads.
//
//The #pragma omp critical ensures that shared variables (minVal, maxVal, and sum) are updated safely.
void parallelStats(int* arr, int n, int &minVal, int &maxVal, long long &sum, double &avg) {
    minVal = INT_MAX;
    maxVal = INT_MIN;
    sum = 0;

    #pragma omp parallel
    {
        int localMin = INT_MAX;
        int localMax = INT_MIN;
        long long localSum = 0;

        #pragma omp for
        for (int i = 0; i < n; i++) {
            if (arr[i] < localMin) localMin = arr[i];
            if (arr[i] > localMax) localMax = arr[i];
            localSum += arr[i];
        }

        #pragma omp critical
        {
            if (localMin < minVal) minVal = localMin;
            if (localMax > maxVal) maxVal = localMax;
            sum += localSum;
        }
    }

    avg = static_cast<double>(sum) / n;
}

//User Input: The user is prompted to enter the length of the array (n) and the maximum random value (rand_max).
//
//Random Array Generation: The array is populated with random integers from 0 to rand_max.
//
//Sequential and Parallel Execution: Both sequentialStats and parallelStats functions are called, and their execution times are measured using chrono::high_resolution_clock.
//
//Output: The results (min, max, sum, and average) for both sequential and parallel computation are displayed, along with the respective times for each computation.
//
//Memory Cleanup: The dynamically allocated memory is freed using delete[].

int main() {
    int n, rand_max;

    // Input
    cout << "Enter length of array: ";
    cin >> n;
    cout << "Enter max random value: ";
    cin >> rand_max;

    // Generate random array
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % rand_max;
    }

    int minSeq, maxSeq, minPar, maxPar;
    long long sumSeq, sumPar;
    double avgSeq, avgPar;

    // Sequential stats
    auto start_seq = chrono::high_resolution_clock::now();
    sequentialStats(arr, n, minSeq, maxSeq, sumSeq, avgSeq);
    auto end_seq = chrono::high_resolution_clock::now();
    auto duration_seq = chrono::duration_cast<chrono::microseconds>(end_seq - start_seq).count();

    // Parallel stats
    auto start_par = chrono::high_resolution_clock::now();
    parallelStats(arr, n, minPar, maxPar, sumPar, avgPar);
    auto end_par = chrono::high_resolution_clock::now();
    auto duration_par = chrono::duration_cast<chrono::microseconds>(end_par - start_par).count();

    // Output
    cout << "\n--- Sequential ---\n";
    cout << "Min: " << minSeq << "\nMax: " << maxSeq << "\nSum: " << sumSeq << "\nAverage: " << avgSeq << endl;
    cout << "Time: " << duration_seq << " microseconds\n";

    cout << "\n--- Parallel ---\n";
    cout << "Min: " << minPar << "\nMax: " << maxPar << "\nSum: " << sumPar << "\nAverage: " << avgPar << endl;
    cout << "Time: " << duration_par << " microseconds\n";

    delete[] arr;
    return 0;
}
//Key Points:
//Sequential Computation: This is the standard, single-threaded approach where the array is processed element by element.
//
//Parallel Computation: Here, the computation is done in parallel across multiple threads. OpenMP divides the work among available processors and merges the results safely.
//
//OpenMP Pragmas:
//
//#pragma omp parallel: Starts a parallel block of code.
//
//#pragma omp for: Divides the loop iterations among threads.
//
//#pragma omp critical: Ensures that updates to shared variables are done safely by one thread at a time.
//
//Advantages of Parallelization:
//Speed: For large arrays, the parallel computation can be significantly faster, as multiple threads work on different parts of the array simultaneously.
//
//Efficient Use of Multiple Cores: OpenMP allows using multiple processor cores, leading to faster execution.
//
//Disadvantages of Parallelization:
//Overhead: Managing threads and synchronizing data introduces overhead, which may make parallelization inefficient for small arrays or operations that do not benefit from parallelism.
//
//Complexity: Writing parallel code requires careful management of shared resources, which can lead to bugs if not handled properly.
//
//Viva Questions and Answers:
//What is the purpose of #pragma omp parallel in parallelStats?
//
//#pragma omp parallel begins a parallel region where multiple threads will be created to execute the code block in parallel.
//
//How does OpenMP help in parallel computing?
//
//OpenMP provides a set of compiler directives and runtime functions to parallelize code. It automatically manages thread creation, synchronization, and workload distribution.
//
//What is the role of #pragma omp for in parallelStats?
//
//#pragma omp for divides the loop iterations among the threads in the parallel region, allowing each thread to work on a portion of the array.
//
//Why is #pragma omp critical used in parallelStats?
//
//#pragma omp critical ensures that only one thread can update the shared variables (minVal, maxVal, and sum) at a time, preventing data races.
//
//What is the time complexity of the sequential and parallel versions of this program?
//
//Both the sequential and parallel versions have a time complexity of O(n), where n is the size of the array. However, parallelization may reduce the constant factors, leading to faster execution on large arrays.


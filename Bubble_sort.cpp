//#include <iostream>: Used to handle input and output in C++ (e.g., printing data to the console).
//
//#include <cstdlib>: Provides functions for random number generation (rand()).
//
//#include <chrono>: For measuring time, used to calculate the time taken by the sorting algorithms.
//
//#include <algorithm>: Provides the copy function used to copy the original array into others.
//
//#include <omp.h>: Required for using OpenMP, which is used to implement parallelism (multi-threading) in the parallel bubble sort.

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <omp.h>

using namespace std;

// Swap function
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
//What it does: This is a variant of the bubble sort where elements are compared and swapped in an odd-even fashion. It works by performing two passes over the array:
//
//Odd indexed elements are compared in the first pass.
//
//Even indexed elements are compared in the second pass.
//
//Why it’s used: Bubble sort is a simple but inefficient sorting algorithm. This particular implementation is a "sequential" version of bubble sort where it processes one element at a time in a single thread.
// Sequential Bubble Sort (Odd-Even Transposition Sort)
void sequentialBubbleSort(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
//What it does: This is a parallel version of the bubble sort using OpenMP.
//
//The key part here is the #pragma omp parallel for directive, which tells OpenMP to execute the for loop in parallel, meaning multiple iterations of the loop can be handled by different processors at the same time.
//
//Why it’s used: Parallel bubble sort allows the sorting to be done much faster by dividing the work among multiple CPU cores. However, it is still based on the inefficient bubble sort algorithm, so the speed-up might not be as significant as other more efficient algorithms (like quicksort or mergesort).
// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
//What the main function does:
//Input Handling: Asks the user to input the size of the array and the maximum random value for the array elements.
//
//Array Allocation: It dynamically allocates memory for an array and fills it with random values between 0 and the specified maximum.
//
//Sorting & Timing:
//
//It then creates copies of the original array (seqArr and parArr) to perform sequential and parallel sorting.
//
//Sequential Sort: Measures how long it takes to sort the array using the sequentialBubbleSort function.
//
//Parallel Sort: Measures how long it takes to sort the array using the parallelBubbleSort function.
//
//Output: After both sorts, it prints the time taken for each sort and, for smaller arrays, prints the sorted arrays.
//
//Cleanup: It frees the allocated memory for the arrays to prevent memory leaks.
int main() {
    int n, rand_max;

    // Input
    cout << "Enter length of array: ";
    cin >> n;
    cout << "Enter max random value: ";
    cin >> rand_max;

    // Allocate and generate array
    int *original = new int[n];
    for (int i = 0; i < n; i++) {
        original[i] = rand() % rand_max;
    }

    // Copy array for both sorts
    int *seqArr = new int[n];
    int *parArr = new int[n];
    copy(original, original + n, seqArr);
    copy(original, original + n, parArr);

    // Sequential Sort Timing
    auto start_seq = chrono::high_resolution_clock::now();
    sequentialBubbleSort(seqArr, n);
    auto end_seq = chrono::high_resolution_clock::now();
    auto duration_seq = chrono::duration_cast<chrono::milliseconds>(end_seq - start_seq).count();

    // Parallel Sort Timing
    auto start_par = chrono::high_resolution_clock::now();
    parallelBubbleSort(parArr, n);
    auto end_par = chrono::high_resolution_clock::now();
    auto duration_par = chrono::duration_cast<chrono::milliseconds>(end_par - start_par).count();

    // Output time
    cout << "\nSequential Bubble Sort Time: " << duration_seq << " ms";
    cout << "\nParallel Bubble Sort Time:   " << duration_par << " ms\n";

    // Optional: Show sorted arrays for small sizes
    if (n <= 100) {
        cout << "\nSequential Sorted Array:\n";
        for (int i = 0; i < n; i++) cout << seqArr[i] << " ";
        cout << "\n\nParallel Sorted Array:\n";
        for (int i = 0; i < n; i++) cout << parArr[i] << " ";
        cout << "\n";
    }

    // Cleanup
    delete[] original;
    delete[] seqArr;
    delete[] parArr;

    return 0;
}
//Key Points in Code:
//OpenMP Directives:
//
//#pragma omp parallel for: This tells the compiler to execute the following for loop in parallel.
//
//#pragma omp critical: Ensures that only one thread accesses a particular section of code at a time.
//
//Timing: The chrono library is used to measure the time taken by both the sequential and parallel sorting functions.
//
//Memory Management: Dynamic memory allocation (new[]) is used to create the arrays, and delete[] is used to clean up.
//
//7. Advantages and Disadvantages of Parallelism in Sorting:
//Advantages:
//Speed: Using multiple processors allows the program to handle larger datasets faster, especially for time-consuming tasks like sorting.
//
//Scalability: As the number of cores in the machine increases, the parallel version of the algorithm can perform better.
//
//Disadvantages:
//Overhead: Parallelism comes with overhead for managing multiple threads. If the dataset is too small or the work too simple, the parallel version might be slower than the sequential version.
//
//Inefficiency of Bubble Sort: Bubble sort is inefficient for large datasets, even with parallelism. The parallel version doesn’t drastically improve performance compared to more efficient algorithms like quicksort or mergesort.
//
//8. Viva Questions:
//What is the purpose of using #pragma omp parallel for?
//
//Answer: It instructs the compiler to parallelize the following for loop so that multiple iterations of the loop can be executed simultaneously on different processor cores.
//
//Why is bubble sort not efficient for large datasets?
//
//Answer: Bubble sort has a time complexity of O(n^2), which makes it inefficient for sorting large arrays because it requires many comparisons and swaps.
//
//What does #pragma omp critical do in the parallel version of the algorithm?
//
//Answer: It ensures that only one thread at a time can access the section of code within the critical block, preventing race conditions when multiple threads try to access shared resources like the array.
//
//What is the difference between sequential and parallel sorting in this program?
//
//Answer: The sequential sorting algorithm processes one element at a time in a single thread, while the parallel sorting algorithm divides the work among multiple threads, potentially speeding up the process.

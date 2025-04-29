#include <iostream>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <omp.h>

using namespace std;

// Merge function
//This function is used to merge two halves of an array in sorted order.
//
//It splits the array into two sub-arrays L and R, and then merges them back in sorted order into the original array arr.


void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}
//What It Does:
//
//This is the standard merge sort function. It recursively divides the array into two halves until each part contains only one element.
//
//After dividing, it calls the merge() function to merge the divided parts back together in sorted order.
// Sequential merge sort
void sequentialMergeSort(int* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel merge sort with OpenMP tasks
//This is the parallelized version of merge sort. It uses OpenMP tasks to parallelize the recursive calls.
//
//The #pragma omp task directive indicates that the recursive calls to parallelMergeSort should be handled as separate tasks. These tasks can run in parallel on different threads.
//
//#pragma omp taskwait ensures that the program waits for the parallel tasks to finish before continuing with the merge operation.
//
//To avoid overloading the processor with too many parallel tasks, a depth limit (depth <= 4) is imposed on the recursion level. If the recursion goes beyond this depth, it switches back to sequential merge sort.
void parallelMergeSort(int* arr, int l, int r, int depth = 0) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth <= 4) { // limit parallel depth to avoid oversubscription
            #pragma omp task shared(arr)
            parallelMergeSort(arr, l, m, depth + 1);
            #pragma omp task shared(arr)
            parallelMergeSort(arr, m + 1, r, depth + 1);
            #pragma omp taskwait
        } else {
            sequentialMergeSort(arr, l, m);
            sequentialMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

int main() {
    int n, rand_max;

    // Input
    cout << "Enter length of array: ";
    cin >> n;
    cout << "Enter max random value: ";
    cin >> rand_max;

    // Generate random array
    int* original = new int[n];
    for (int i = 0; i < n; i++) {
        original[i] = rand() % rand_max;
    }

    // Create two copies
    int* seqArr = new int[n];
    int* parArr = new int[n];
    copy(original, original + n, seqArr);
    copy(original, original + n, parArr);

    // Time sequential merge sort
    auto start_seq = chrono::high_resolution_clock::now();
    sequentialMergeSort(seqArr, 0, n - 1);
    auto end_seq = chrono::high_resolution_clock::now();
    auto duration_seq = chrono::duration_cast<chrono::milliseconds>(end_seq - start_seq).count();

    // Time parallel merge sort
    auto start_par = chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(parArr, 0, n - 1);
    }
    auto end_par = chrono::high_resolution_clock::now();
    auto duration_par = chrono::duration_cast<chrono::milliseconds>(end_par - start_par).count();

    // Output timing results
    cout << "\nSequential Merge Sort Time: " << duration_seq << " ms";
    cout << "\nParallel Merge Sort Time:   " << duration_par << " ms\n";

    // Show sorted arrays for small sizes
    if (n <= 100) {
        cout << "\nSequentially sorted array:\n";
        for (int i = 0; i < n; i++) cout << seqArr[i] << " ";
        cout << "\n\nParallel sorted array:\n";
        for (int i = 0; i < n; i++) cout << parArr[i] << " ";
        cout << "\n";
    }

    delete[] original;
    delete[] seqArr;
    delete[] parArr;

    return 0;
}
//What It Does:
//
//The main function handles input, generates a random array, and creates copies of the array for both sequential and parallel sorting.
//
//It measures the execution time for both the sequential and parallel merge sort algorithms using chrono::high_resolution_clock.
//
//It displays the sorted arrays (if n <= 100), so you can visually verify that both sorting methods yield the same result.
//
//Finally, the allocated memory for the arrays is freed using delete[].
//
//Advantages of Parallel Merge Sort:
//Faster Execution: Parallel merge sort can significantly reduce the time complexity by dividing the task across multiple cores of the CPU.
//
//Scalability: It works efficiently for larger arrays by leveraging multiple threads for sorting.
//
//Better Resource Utilization: Makes better use of multicore processors.
//
//Disadvantages of Parallel Merge Sort:
//Overhead for Small Arrays: For small arrays, the overhead of creating threads might outweigh the benefits of parallelism.
//
//Complexity: Implementing parallelism introduces complexity in the code, which requires careful management of tasks and synchronization.
//
//Memory Usage: More memory is required for storing temporary arrays during merging.
//
//Possible Viva Questions and Answers:
//Q: What is the time complexity of merge sort?
//
//A: The time complexity of merge sort is 
//??
//(
//??
//log
//?
//??
//)
//O(nlogn), where 
//??
//n is the number of elements in the array. This is because the array is repeatedly divided in half, and each division requires linear time for merging.
//
//Q: What is the advantage of using OpenMP in parallel merge sort?
//
//A: OpenMP allows us to parallelize recursive calls and merge operations, reducing the overall time complexity for large arrays by utilizing multiple CPU cores. This results in faster execution, especially on multicore processors.
//
//Q: How do you ensure that parallel merge sort does not create too many threads?
//
//A: We limit the depth of recursion by using a depth parameter and switching to sequential merge sort beyond a certain depth (in this case, when depth <= 4). This prevents oversubscription and ensures efficient thread management.

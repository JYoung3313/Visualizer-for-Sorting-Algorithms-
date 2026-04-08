// Copyright [2026] <Justin J. Young>
#include "linearSort.h"
#include <raylib.h>
#include <vector>
#include <algorithm>

extern SortStatus global_status;

// helper for init_numbers
// Just swaps numbers 
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

SortStatus linear_sort(int* arr, int n, void (*draw_callback)()) {
    for (int i = 0; i < n; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {

            global_status.i = i;
            global_status.j = j;
            global_status.swapped = false;
            if (draw_callback) draw_callback();

            if (arr[j] < arr[min]) {
                min = j;
            }
        }

        if (min != i) {
            swap(&arr[min], &arr[i]);
            global_status.swapped = true; // Highlight the swap
            if (draw_callback) draw_callback();
        }
    }
    global_status.sort_done = true;
    return global_status;
}

SortStatus bubble_sort(int* arr, int n, void (*draw_callback)()) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped_this_pass = false;        
        for (int j = 0; j < n - i - 1; j++) {

            global_status.i = j;
            global_status.j = j + 1;
            global_status.swapped = false;

            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                global_status.swapped = true; 
                swapped_this_pass = true;
            }
            if (draw_callback) draw_callback();
        }
        if (!swapped_this_pass) break;
    }
    global_status.sort_done = true;
    return global_status;
}


// To heapify a subtree rooted with node i which is an index in arr[].
// n is the size of the heap
static void heapify(int* arr, int n, int i, void (*draw_callback)()) {
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // left child = 2*i + 1
    int right = 2 * i + 2; // right child = 2*i + 2

    global_status.i = i;
    global_status.j = largest;

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        global_status.swapped = true; // Set red highlight

        if (draw_callback) draw_callback();

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest, draw_callback);
    }
}

// Main function to do heap sort
SortStatus heapSort(int* arr, int n, void (*draw_callback)()) {
    // Build heap (rearrange array)
    // We start from the last non-leaf node
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, draw_callback);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(&arr[0], &arr[i]);

        if (draw_callback) draw_callback();

        // call max heapify on the reduced heap
        heapify(arr, i, 0, draw_callback);
    }
    return (SortStatus){ .swapped = true, .i = n, .j = n, .sort_done = true };
}

// Helper function to merge to halves
static void merge(int* arr, int left, int mid, int right, void (*draw_callback)()) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // creates temp arrays;
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n1; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        global_status.i = k;
        global_status.j = left + i;

        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        global_status.swapped = true;
        if (draw_callback) draw_callback();
        k++;
    }
    
    // Coping remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        global_status.i = k;
        if (draw_callback) draw_callback();
        i++;
        k++;
    }

    // Copy remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        global_status.i = k;
        if (draw_callback) draw_callback();
        j++;
        k++;
    }
}

// Main function for mergeSort
SortStatus mergeSort(int* arr, int n, int left, int right, void (*draw_callback)()) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sorting first and second halves
        mergeSort(arr, n, left, mid, draw_callback);
        mergeSort(arr, n, mid + 1, right, draw_callback);

        merge(arr, left, mid, right, draw_callback);        
    }

    if (left == 0 && right == n - 1) {
        global_status.sort_done = true;
    }
    return global_status;
}

SortStatus bucketSort(int* arr, int n, void (*draw_callback)()) {
    // 1. Create buckets (using vectors for simplicity inside buckets)
    const int bucket_count = 10;
    std::vector<int> buckets[bucket_count];

    // Find max value to normalize distribution
    int max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }

    // 2. Put elements into buckets
    for (int i = 0; i < n; i++) {
        int bucket_idx = (bucket_count - 1) * arr[i] / max_val;
        buckets[bucket_idx].push_back(arr[i]);

        global_status.i = i;
        if (draw_callback) draw_callback();
    }

    // 3. Sort individual buckets and put them back in the array
    int index = 0;
    for (int i = 0; i < bucket_count; i++) {
        std::sort(buckets[i].begin(), buckets[i].end()); // sorts each bucket

        for (size_t j = 0; j < buckets[i].size(); j++) {
            arr[index] = buckets[i][j];

            global_status.i = index;
            global_status.j = i; // Using j to represent the bucket number
            global_status.swapped = true;
            
            if (draw_callback) draw_callback();
            index++;
        }
    }
    global_status.sort_done = true;
    return global_status;
}
// Copyright [2026] <Justin J. Young>
#include "linearSort.h"
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <iostream>

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
    global_status.sort_done = true;
    return global_status;
}

// Helper function to merge to halves
static void merge(int* arr, int left, int mid, int right, void (*draw_callback)()) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Use std::vector to be safer with stack memory on large arrays
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j]; // Fixed: use n2 and j

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        global_status.i = k;
        global_status.j = mid + 1 + j; // Highlight what we are comparing against

        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        if (draw_callback) draw_callback();
        k++;
    }
    
    // Copy remaining elements
    while (i < n1) {
        arr[k] = L[i];
        global_status.i = k;
        if (draw_callback) draw_callback();
        i++; k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        global_status.i = k;
        if (draw_callback) draw_callback();
        j++; k++;
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
    global_status.type = "Sort";
    global_status.sort_done = false;
    
    // 1. Create buckets (using vectors for simplicity inside buckets)
    //const int bucket_count = n;
    const int bucket_count = n;

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
        global_status.swapped = false;
        if (draw_callback) draw_callback();
    }

    // 3. Sort individual buckets and put them back in the array
    int index = 0;
    // 2 & 3. GATHER AND SORT Phase
    for (int i = 0; i < bucket_count; i++) {
        int start_index = index;

        // Step A: Dump the bucket contents back into the main array (unsorted!)
        for (size_t j = 0; j < buckets[i].size(); j++) {
            arr[index] = buckets[i][j];
            
            // Highlight the section we are currently dumping
            global_status.i = index;
            global_status.swapped = true;
            if (draw_callback) draw_callback();
            
            index++;
        }
        int end_index = index;

        // Step B: Animate an Insertion Sort on this specific bucket's chunk of bars
        for (int k = start_index + 1; k < end_index; k++) {
            int key = arr[k];
            int m = k - 1;

            // Shift elements that are greater than the key to the right
            while (m >= start_index && arr[m] > key) {
                arr[m + 1] = arr[m];
                
                // Animate the shifting process
                global_status.i = m + 1;
                global_status.j = m; 
                global_status.swapped = true;
                if (draw_callback) draw_callback();
                
                m--;
            }
            // Place the key in its correct spot
            arr[m + 1] = key;
            
            // Animate the final placement
            global_status.i = m + 1;
            global_status.swapped = true;
            if (draw_callback) draw_callback();
        }
    }

    global_status.sort_done = true;
    return global_status;
}

// helper for search
// SortStatus linSearchReturn() {
//     global_status.sort_done = true;
//     return global_status;
// }

// partition helper function for quickSort
static int partition(int* arr, int low, int high, void (*draw_callback)()) {
    // Choose the rightmost element as the pivot
    int pivot = arr[high];
    // index of the smaller element
    int i = low - 1;

    for (int j = low; j < high; j++) {  // Standard quicksort loop goes to high-1
        global_status.i = j;     // The element being scanned
        global_status.j = high;  // The pivot element
        global_status.swapped = false;

        // Draw the current state of comparison
        if (draw_callback) draw_callback();

        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            // Highlight the swap
            global_status.swapped = true;
            if (draw_callback) draw_callback();
        }
    }
    
    // Place the pivot in its correct sorted Position
    swap(&arr[i + 1], &arr[high]);
    global_status.i = i + 1;
    global_status.j = high;
    global_status.swapped = true;
    if (draw_callback) draw_callback();
    return i + 1;
}

// This is the quckSort function
SortStatus quickSort(int* arr, int low, int high, void (*draw_callback)()) {
    if (low < high) {
        // pi is the partitioning index
        int pi = partition(arr, low, high, draw_callback);

        // Separately sort elements before and after partition
        quickSort(arr, low, pi - 1, draw_callback);
        quickSort(arr, pi + 1, high, draw_callback);
    }

    // Mark as done when the top-level call finishes
    if (low == 0 && high >= 499) {
        global_status.sort_done = true;
    }
    return global_status;
}

static bool isBitSet(unsigned int swap_target, int bit_pos) {
    return (swap_target >> bit_pos) & 1;
}

// Radix sorting algorithm
SortStatus binaryRadixSort(int* arr, int left, int right, int bit_pos, void (*draw_callback)()) {
    // Base case: No more bits to check or range is invalid
    const int n = right;

    if (bit_pos < 0 || left >= right) {
        return global_status;
    }

    int i = left;
    int j = right;

    // Sets the global status struct to allow visualizer to work
    

    // In-place partition (Similar to QuickSort Hoare partition)
    while (i <= j) {
        // Sets the global status struct to allow visualizer to work
        global_status.i = i;
        global_status.j = j;
        global_status.swapped = false;
        if (draw_callback) draw_callback();

        // Find element with bit_pos set to 1
        while (i <= j && !isBitSet(arr[i], bit_pos)) {
            i++;
            global_status.i = i;
            if (draw_callback) draw_callback();
        }

        // find element with bit_pos set to 0
        while (i <= j && isBitSet(arr[j], bit_pos)) {
            j--;
            global_status.j = j;
            if (draw_callback) draw_callback();
        }

        if (i < j) {
            swap(&arr[i], &arr[j]);
            global_status.swapped = true; // Highlight the swap
            if (draw_callback) draw_callback();
            i++;
            j--;
        }
    }

    // Recurse on the 0-bit partition (left to j)
    binaryRadixSort(arr, left, j, bit_pos - 1, draw_callback);
    // Recurse on the 1-bit partition (i to right)
    binaryRadixSort(arr, i, right, bit_pos - 1, draw_callback);

    // Only set sort_done at the very top level of the recursion
    if (bit_pos == 31 || (left == 0 && right == n && bit_pos >= 8)) {
        global_status.sort_done = true;
    }

    return global_status;
}   


// testing for search algorithm
SortStatus linearSearch(int* arr, int n, int target, void (*draw_callback)()) {
    global_status.type = "Search";
    // global_status.search_done = false;

    for (int i = 0; i < n; i++) {
        global_status.i = i;
        if (draw_callback) draw_callback();

        if (arr[i] == target) {
            global_status.search_done = true;
            global_status.sort_done = true;
            return global_status;
        }
    }
    global_status.sort_done = true; // Finished looking, even if not found
    return global_status;
}

SortStatus binarySearch(int* arr, int n, int target, void (*draw_callback)()) {
    global_status.type = "Search";
    global_status.search_done = false;
    global_status.sort_done = false;
    
    int start = 0;
    int end = n - 1;

    while (start <= end) {
        int mid = start + (end - start) / 2;
        global_status.i = start;
        global_status.j = end;
        global_status.mid = mid;

        if (draw_callback) draw_callback();


        if (arr[mid] == target) {
            global_status.i = mid;
            global_status.search_done = true; // target found at middle index
            global_status.sort_done = true; // Finished looking, even if not found
            return global_status;
            
        } else if (arr[mid] < target) {
            start = mid + 1 ;    // searching right half;
        } else {
            end = mid - 1;   // searching left half
        }
    }
    global_status.sort_done = true; // Finished looking, even if not found
    return global_status;
}
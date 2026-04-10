// Copyright [2026] <Justin J. Young>
#pragma once
#include<string>

typedef struct {
    bool swapped;
    int i, j;
    int mid;       // Add this to track the middle
    bool sort_done;
    std::string type;
    bool search_done;
} SortStatus;

//swap
void swap(int* a, int* b);

// linear sort
SortStatus linear_sort(int* arr, int n, void (*draw_callback)());

SortStatus bubble_sort(int* arr, int n, void (*draw_callback)());

SortStatus heapSort(int* arr, int n, void (*draw_callback)());

SortStatus mergeSort(int* arr, int n, int left, int right, void (*draw_callback)());

SortStatus bucketSort(int* arr, int n, void (*draw_callback)());

SortStatus linearSearch(int* arr, int n, int target, void (*draw_callback)());

SortStatus binarySearch(int* arr, int n, int target, void (*draw_callback)());
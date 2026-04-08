// Copyright [2026] <Justin J. Young>
#pragma once

typedef struct {
    bool swapped;
    int i, j;
    bool sort_done;
} SortStatus;

//swap
void swap(int* a, int* b);

// linear sort
SortStatus linear_sort(int* arr, int n, void (*draw_callback)());

SortStatus bubble_sort(int* arr, int n, void (*draw_callback)());

SortStatus heapSort(int* arr, int n, void (*draw_callback)());

SortStatus mergeSort(int* arr, int n, int left, int right, void (*draw_callback)());

SortStatus bucketSort(int* arr, int n, void (*draw_callback)());
// Copyright [2026] <Justin J. Young>
#include "linearSort.h"
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <random>

// Defines WIDTH and HIGHT of window
#define WIDTH 900 * 2
#define HEIGHT 600 * 2

// Defines count of bars
#define COUNT 500

// Global for array of number of bars
int numbers[COUNT];


SortStatus global_status = {false, 0, 0, false};


// Draws shape of choice
void drawShapes(std::string shape, int n, int valueOfNums, Color color) {
    if (shape == "bar") {
        // Color color_i = highlight ? RED : WHITE;
        // Color color_j = status.swapped ? RED : WHITE;

        // uses n as the index positon in respect to the x-pos of
        // the bar and y-pos as half the screen and
        // valueOfNums as pixal val or height when doing this 
        // Make sure to scale it by half the screen or by whatever you want
        // Then normalize it by COUNT or length of array
        int barHeight = valueOfNums * HEIGHT * 0.75 / COUNT;
        DrawRectangle(((float)n / COUNT) * WIDTH,
            HEIGHT * 0.75 - barHeight,
            WIDTH / COUNT - 2,
            barHeight, color);
        return;
    }
}
// testing for github
// draws the numbers as bars
void draw_bars() {
    

    for (int i = 0; i < COUNT; i++) {
        int val = numbers[i];
        Color color = WHITE;
        if (i == global_status.i || i == global_status.j) {
            drawShapes("bar", i, val, RED);
        } else
        drawShapes("bar", i, val, color);
    }
}

// helper for init_numbers
// Just swaps numbers 
// void swap(int* a, int* b) {
//     int temp = *a;
//     *a = *b;
//     *b = temp;
// }

// Initilizes a random array
void init_numbers() {
    static std::mt19937 mt(static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < COUNT; i++) {
        numbers[i] = i;
    }

    // Fish Yates Shuffer
    for (int i = COUNT-1; i >= 0; i--) {
        // int j = rand() % (i + 1);   
        
        // Generates a random number between 0 and i
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(mt);

        // Swaps the array entries
        swap(&numbers[i], &numbers[j]);
    }
}

// void sort_steps() {
//     static int i = 0;
    
//     int current_value = numbers[i];
//     if (i < COUNT - 1) {
//         int next_val = numbers[i + 1];

//         // Is curr > next?
//         // swap
//     }
// }

// Create a wrapper that handles a single frame update
void update_frame() {
    if (WindowShouldClose()) { CloseWindow(); exit(0); }
    
    BeginDrawing();
        ClearBackground(BLACK);
        draw_bars(); 
    EndDrawing();
}

int main() {

    init_numbers();
    
    // Initalizing window
    InitWindow(WIDTH, HEIGHT, "Sorting Visualizer");
    SetTargetFPS(60);

    // Calls from linearSort.cpp & linearSort.h
    // global_status = linear_sort(numbers, COUNT, update_frame);
    // global_status = bubble_sort(numbers, COUNT, update_frame);
    // global_status = heapSort(numbers, COUNT, update_frame);
    global_status = mergeSort(numbers, COUNT, 0, COUNT - 1, update_frame);
    // global_status = bucketSort(numbers, COUNT, update_frame);


    // Window main loop
    while (!WindowShouldClose()) {
        
        // drawShapes("bar", 1, 1);

        // Calls from linearSort.cpp & linearSort.h
        // linear_sort(&numbers[0], COUNT);
        // draws the "bars"
        BeginDrawing();
            ClearBackground(BLACK); 
            draw_bars();
            DrawText("SORT COMPLETE", 10, 10, 20, GREEN);
        EndDrawing();
    }

    // closes window
    CloseWindow();

    return 0;
}
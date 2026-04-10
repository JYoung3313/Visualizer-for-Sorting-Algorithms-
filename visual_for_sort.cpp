// Copyright [2026] <Justin J. Young>
#include "linearSort.h"
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <random>

// Defines WIDTH and HIGHT of window
#define WIDTH 900 * 2
#define HEIGHT 600 * 2

// Defines Max input for chars
#define MAX_INPUT_CHARS 9

// Defines count of bars
#define COUNT 500

// Global for array of number of bars
int numbers[COUNT];

// global for is textbox visable
bool isTextBoxVisible = true;

// Global for selection and target and picking target and currAlg
std::string currentAlg = ""; // This "remembers" the choice
std::string selection = "";
bool isPickingTarget = false; // Add this global at the top of visual_for_sort.cpp
int target = 0;

const std::vector<std::string> alg_choices = {"linear_sort", "bubble_sort", "heapSort",
            "mergeSort", "bucketSort", "linearSearch", "binarySearch"};


SortStatus global_status = {false, 0, 0, 0, false, "Sort", false};


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

        if (global_status.type == "Search") {
            if (i < global_status.i || i > global_status.j) {
                color = DARKGRAY; // This bar is eliminated from the search
            }
        }

        if (i == global_status.mid) {
            drawShapes("bar", i, numbers[i], RED);
        } else if (i == global_status.i || i == global_status.j) {
            drawShapes("bar", i, val, ORANGE);
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


// Logic to trigger the correct algorithm
void runSelectedSort(int target) {
    if (selection == "linear_sort") {
        global_status = linear_sort(numbers, COUNT, update_frame);
    } else if (selection == "bubble_sort") {
        global_status = bubble_sort(numbers, COUNT, update_frame);
    } else if (selection == "heapSort") {
        global_status = heapSort(numbers, COUNT, update_frame);
    } else if (selection == "mergeSort") {
        global_status = mergeSort(numbers, COUNT, 0, COUNT - 1, update_frame);
    } else if (selection == "bucketSort") {
        global_status = bucketSort(numbers, COUNT, update_frame);
    } else if (selection == "linearSearch") {
        global_status = linearSearch(numbers, COUNT, target, update_frame);
    } else if (selection == "binarySearch") {
        global_status = bucketSort(numbers, COUNT, update_frame);
        
        global_status.sort_done = false; 
        global_status.search_done = false;
        global_status = binarySearch(numbers, COUNT, target, update_frame);
    }
}

void HandleInput(std::string &text, int maxChars) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (text.length() < maxChars)) {
            text += (char)key;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty()) {
        text.pop_back();
    }
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
    // global_status = mergeSort(numbers, COUNT, 0, COUNT - 1, update_frame);
    // global_status = bucketSort(numbers, COUNT, update_frame);
    //global_status = linearSearch(numbers, COUNT, 399, update_frame);

    // std::string selected; 

    // Window main loop
    while (!WindowShouldClose()) {

        // Logic phase (Outside BeginDrawing)
        if (isTextBoxVisible) {
            HandleInput(selection, 20);

            if (IsKeyPressed(KEY_ENTER)) {
                bool isSearching = (selection == "linearSearch" || selection == "binarySearch");
                if (isSearching && !isPickingTarget) {
                    currentAlg = selection;
                    // Move to the target Pick Phase
                    isPickingTarget = true;
                    selection = ""; // Clear "linearSearch" so user can type a number
                } else {    // If we are picking a target save it
                    if (isPickingTarget) {
                        target = std::atoi(selection.c_str());
                        selection = currentAlg;; // Set it back so runSelectedSort knows what to do
                    }
                    isTextBoxVisible = false;
                    isPickingTarget = false;
                    runSelectedSort(target);
                }                
            }
        }
        
        // drawShapes("bar", 1, 1);

        // Calls from linearSort.cpp & linearSort.h
        // linear_sort(&numbers[0], COUNT);
        // draws the "bars"
        BeginDrawing();
            ClearBackground(BLACK);
            if (isTextBoxVisible) {
                if (!isPickingTarget) {
                    DrawText("Select algorithm", WIDTH/2 - 150, HEIGHT/2 - 100, 30, GREEN);
                    //DrawText("Select algorithm", 10, 10, 20, GREEN);
                    // Helper list
                    DrawText("Options: linear_sort, bubble_sort, heapSort, mergeSort, bucketSort, linearSearch, binarySearch", 
                            WIDTH/2 - 350, HEIGHT/2 + 20, 15, GRAY);
                } else {
                    DrawText("Select Target (0 - max: 499)", WIDTH/2 - 200, HEIGHT/2 - 100, 20, ORANGE);
                }
                // Draws input box
                DrawRectangle(WIDTH/2 - 200, HEIGHT/2 - 60, 400, 50, DARKGRAY);
                DrawRectangleLines(WIDTH/2 - 200, HEIGHT/2 - 60, 400, 50, WHITE);
                DrawText(selection.c_str(), WIDTH/2 - 190, HEIGHT/2 - 45, 30, WHITE);

            } else {
                // Display status
                draw_bars();
                if (global_status.type == "Search") {
                    if (global_status.search_done == true) {
                        DrawText("Search COMPLETE", 10, 10, 20, GREEN);
                    } else {
                        DrawText("Target Not Found", 10, 10, 20, GREEN);
                    }
                } else {
                    DrawText("SORT COMPLETE", 10, 10, 20, GREEN);
                }
                DrawText("Press R to Reset", 10, 40, 20, GRAY);

                // resets the program
                if (IsKeyPressed(KEY_R)) {
                    init_numbers();
                    isPickingTarget = false;
                    isTextBoxVisible = true;
                    selection = "";
                    global_status.sort_done = false;
                    global_status.search_done = false;
                    global_status.type = "Sort";
                }
            }
        EndDrawing();
    }
    // closes window
    CloseWindow();
    return 0;
}

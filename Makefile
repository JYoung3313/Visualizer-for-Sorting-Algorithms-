# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable name
TARGET = sortVisualizer

# Source files (Include all .cpp files here)
SRCS = visual_for_sort.cpp linearSort.cpp

# Raylib Linker Flags (for Linux)
# Includes math library (-lm) and system dependencies for X11/OpenGL
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Build rule: This is the default command run when you type 'make'
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Clean rule: Run 'make clean' to delete the executable and start fresh
clean:
	rm -f $(TARGET)

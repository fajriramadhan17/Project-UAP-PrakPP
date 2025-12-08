#include <ncurses/ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>

// Struktur untuk player
struct GameObject {
    int x, y;
    char symbol;
};

// Struktur untuk obstacle 3x3
struct Obstacle3x3 {
    int x, y; // Posisi tengah obstacle
    char symbol;
    char shape[3][3]; // Matriks 3x3
};

#include <ncurses/ncurses.h>
#include <cstring>
#include <string>
#include "asciiArt.h"
#include "highscore.h"
#include "game.h"

using namespace std;

int main() {
    // Inisialisasi ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

     // Inisialisasi warna
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK); // Untuk input nama
    }
    
    
    // Cleanup ncurses
    endwin();
    return 0;
}

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
    curs_set(0); // Sembunyikan kursor
    keypad(stdscr, TRUE); // Aktifkan special keys (seperti arrow keys)
    
    
    // Cleanup ncurses
    endwin();
    return 0;
}

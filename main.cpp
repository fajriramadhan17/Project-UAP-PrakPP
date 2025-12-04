#include <ncurses/ncurses.h>
#include <cstring>
#include <string>
#include "asciiArt.h"
#include "highscore.h"
#include "game.h"

using namespace std;  // TAMBAHKAN INI

int main() {
    // Inisialisasi ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0); // Sembunyikan kursor
    keypad(stdscr, TRUE); // Aktifkan special keys (seperti arrow keys)
    
    // Inisialisasi warna
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
    }
    
    // Data menu
    const char* menuItems[] = {
        "Play Game",
        "High Score", 
        "Quit Game"
    };
    const int menuCount = 3; // Jumlah item menu
    
    int selected = 0;
    bool quit = false;
    
    while (!quit) {
        clear();
        
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        // posisi untuk ASCII Art agar di tengah
        string asciiArt = tampilanMenu();
        
        // Hitung jumlah baris dalam ASCII Art
        int lineCount = 0;
        int maxLineWidth = 0;
        size_t pos = 0;
        string tempArt = asciiArt;
        
        while ((pos = tempArt.find('\n')) != string::npos) {
            string line = tempArt.substr(0, pos);
            int lineWidth = line.length();
            if (lineWidth > maxLineWidth) maxLineWidth = lineWidth;
            lineCount++;
            tempArt = tempArt.substr(pos + 1);
        }
        
        // Tampilkan ASCII Art di tengah secara vertikal dan horizontal
        int startY = (maxY - lineCount) / 3; // Posisi Y di 1/3 atas layar
        int startX = (maxX - maxLineWidth) / 2; // Posisi X di tengah
        
        tempArt = asciiArt;
        pos = 0;
        int currentLine = 0;
        while ((pos = tempArt.find('\n')) != string::npos) {
            string line = tempArt.substr(0, pos);
            mvprintw(startY + currentLine, startX, "%s", line.c_str());
            currentLine++;
            tempArt = tempArt.substr(pos + 1);
        }
        
        // Tampilkan judul di bawah ASCII Art
               // Tampilkan menu items di posisi yang disesuaikan
        int menuStartY = startY + lineCount + 2;
        int menuStartX = startX + 12;
        
        for (int i = 0; i < menuCount; i++) {
            int y = menuStartY + i;
            int x = menuStartX;
            
            if (i == selected) {
                // Item yang dipilih
                if (has_colors()) {
                    attron(COLOR_PAIR(2));
                } else {
                    attron(A_REVERSE);
                }
                mvprintw(y, x - 2, "> %s", menuItems[i]);
                if (has_colors()) {
                    attroff(COLOR_PAIR(2));
                } else {
                    attroff(A_REVERSE);
                }
            } else {
                // Item tidak dipilih
                if (has_colors()) {
                    attron(COLOR_PAIR(1));
                }
                mvprintw(y, x, "  %s", menuItems[i]);
                if (has_colors()) {
                    attroff(COLOR_PAIR(1));
                }
            }
        }

        
        // Input handling
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                selected--;
                if (selected < 0) selected = menuCount - 1;
                break;
            case KEY_DOWN:
                selected++;
                if (selected >= menuCount) selected = 0;
                break;
            case '\n': // Enter key
            case KEY_ENTER:
                // Handle pilihan menu
                switch (selected) {
                    case 0: // Play Game
                        clear();
                        mvprintw(maxY / 2, maxX / 2 - 5, "PLAY GAME!");
                        refresh();
                        getch(); // Tunggu input
                        break;
                    case 1: // High Score
                        clear();
                        mvprintw(maxY / 2, maxX / 2 - 6, "HIGH SCORE!");
                        refresh();
                        getch(); // Tunggu input
                        break;
                    case 2: // Quit Game
                        quit = true;
                        break;
                }
                break;
            case 'q': // Tombol 'q' untuk quit
            case 'Q':
                quit = true;
                break;
        }
    }
    
    // Cleanup ncurses
    endwin();
    return 0;
}

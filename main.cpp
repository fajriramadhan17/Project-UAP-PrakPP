#include <ncurses/ncurses.h>
#include <cstring>
#include <string>
#include "asciiArt.h"
#include "game.h"
#include "highscore.h"

using namespace std;

void playGame();
void showHighScores();
string getPlayerName();

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
    
    // Data menu
    const char* menuItems[] = {
        "Play Game",
        "High Score", 
        "Quit Game"
    };
    const int menuCount = 3;
    
    int selected = 0;
    bool quit = false;
    
    while (!quit) {
        clear();
        
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        // Tampilkan ASCII Art
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
        
        // Tampilkan ASCII Art di tengah
        int startY = (maxY - lineCount) / 3;
        int startX = (maxX - maxLineWidth) / 2;
        
        tempArt = asciiArt;
        pos = 0;
        int currentLine = 0;
        while ((pos = tempArt.find('\n')) != string::npos) {
            string line = tempArt.substr(0, pos);
            mvprintw(startY + currentLine, startX, "%s", line.c_str());
            currentLine++;
            tempArt = tempArt.substr(pos + 1);
        }
        
        // Tampilkan menu items
        int menuStartY = startY + lineCount + 2;
        int menuStartX = startX + 12;
        
        for (int i = 0; i < menuCount; i++) {
            int y = menuStartY + i;
            int x = menuStartX;
            
            if (i == selected) {
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
        
        // Tombol panah atas
        if (ch == KEY_UP) {
            selected--;
            if (selected < 0) selected = menuCount - 1;
        }
        // Tombol panah bawah
        else if (ch == KEY_DOWN) {
            selected++;
            if (selected >= menuCount) selected = 0;
        }
        // Tombol enter atau newline
        else if (ch == '\n' || ch == KEY_ENTER) {
            // Play Game
            if (selected == 0) {
                playGame();
            }
            // High Score
            else if (selected == 1) {
                showHighScores();
            }
            // Quit Game
            else if (selected == 2) {
                quit = true;
            }
        }
        // Tombol 'q' atau 'Q'
        else if (ch == 'q' || ch == 'Q') {
            quit = true;
        }
        // Tombol '1' langsung ke Play Game
        else if (ch == '1') {
            playGame();
        }
        // Tombol '2' langsung ke High Score
        else if (ch == '2') {
            showHighScores();
        }
        // Tombol '3' langsung ke Quit Game
        else if (ch == '3') {
            quit = true;
        }
    }
    
    endwin();
    return 0;
}

void playGame() {
    bool playAgain = true;
    
    while (playAgain) {
        // Inisialisasi game baru setiap kali memulai
        Game game;
        HighScoreManager hsManager;
        
        bool inGame = true;
        bool gameRunning = true;
        
        // Setup game
        timeout(50);
        
        while (inGame && gameRunning) {
            clear();
            
            // Update dan gambar game
            game.updateObstacles();
            game.drawBorder();
            game.drawPlayer();
            game.drawObstacles();
            
            // Cek tabrakan
            if (game.checkCollision()) {
                game.gameOver();
            }
            
            // Refresh layar
            refresh();
            
            // Input handling dalam game
            int ch = getch();
            
            // Tombol untuk bergerak ke atas
            if (ch == 'w' || ch == 'W' || ch == KEY_UP) {
                game.movePlayer(-1); // Naik
            }
            // Tombol untuk bergerak ke bawah
            else if (ch == 's' || ch == 'S' || ch == KEY_DOWN) {
                game.movePlayer(1);  // Turun
            }
            // Tombol untuk pause game
            else if (ch == 'p' || ch == 'P') {
                game.pauseGame();
                // Tampilkan layar pause
                clear();
                game.drawBorder();
                refresh();
                // Tunggu sampai 'p' ditekan lagi
                while (game.getIsPaused()) {
                    int pauseCh = getch();
                    if (pauseCh == 'p' || pauseCh == 'P') {
                        game.pauseGame();
                    } else if (pauseCh == 'q' || pauseCh == 'Q') {
                        inGame = false;
                        gameRunning = false;
                        break;
                    }
                }
            }
            // Tombol untuk quit game
            else if (ch == 'q' || ch == 'Q') {
                inGame = false;
                gameRunning = false;
            }
            
            // Jika game over
            if (game.getIsGameOver()) {
                clear();
                game.drawBorder();
                refresh();
                
                // Tampilkan skor akhir
                int maxY, maxX;
                getmaxyx(stdscr, maxY, maxX);
                mvprintw(maxY / 2 - 2, maxX / 2 - 10, 
                        "Your Score: %d", game.getScore());
                mvprintw(maxY / 2 - 1, maxX / 2 - 10, 
                        "Speed Level: %d", game.getSpeedLevel());
                mvprintw(maxY / 2, maxX / 2 - 15, 
                        "Enter your name for high score:");
                
                // Dapatkan nama pemain (di tengah layar)
                string playerName = getPlayerName();
                
                // Simpan high score
                if (!playerName.empty()) {
                    hsManager.addScore(playerName, game.getScore());
                }
                
                gameRunning = false;
                inGame = false;
            }
        }
        
        // Tanya apakah ingin main lagi
        timeout(-1);
        clear();
        
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        mvprintw(maxY / 2 - 2, maxX / 2 - 15, "Game Over!");
        mvprintw(maxY / 2, maxX / 2 - 15, "1. Play Again");
        mvprintw(maxY / 2 + 1, maxX / 2 - 15, "2. Return to Menu");
        mvprintw(maxY / 2 + 2, maxX / 2 - 15, "Choose option (1-2): ");
        
        refresh();
        
        int choice = getch();
        if (choice == '1') {
            playAgain = true;
        } else {
            playAgain = false;
        }
    }
}

void showHighScores() {
    HighScoreManager hsManager;
    hsManager.displayScores();
}

string getPlayerName() {
    echo();
    curs_set(1);
    
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    // Buat window untuk input di tengah layar
    int winHeight = 7;  // Sedikit lebih tinggi untuk tampilan yang lebih baik
    int winWidth = 50;  // Lebih lebar untuk instruksi
    int winY = maxY / 2 - winHeight / 2;  // Tengah vertikal
    int winX = maxX / 2 - winWidth / 2;   // Tengah horizontal
    
    WINDOW* inputWin = newwin(winHeight, winWidth, winY, winX);
    box(inputWin, 0, 0);
    
    // Judul window
    wattron(inputWin, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(inputWin, 0, winWidth/2 - 8, " ENTER YOUR NAME ");
    wattroff(inputWin, COLOR_PAIR(5) | A_BOLD);
    
    // Instruksi
    mvwprintw(inputWin, 1, 2, "Press ENTER to confirm, BACKSPACE to delete");
    mvwprintw(inputWin, 2, 2, "Press ESC to cancel");
    
    // Garis pemisah
    for (int i = 2; i < winWidth - 2; i++) {
        mvwaddch(inputWin, 3, i, '-');
    }
    
    // Label input
    mvwprintw(inputWin, 4, 2, "Name: ");
    
    // Tampilkan window
    wrefresh(inputWin);
// Variabel untuk input nama
    char name[20] = "";
    int nameLength = 0;
    int cursorPos = 8; // Posisi awal cursor (setelah "Name: ")
    
    // Loop input
    bool done = false;
    while (!done) {
        // Tampilkan nama saat ini
        mvwprintw(inputWin, 4, 8, "%-19s", name); // Clear dan tampilkan
        wmove(inputWin, 4, cursorPos);
        wrefresh(inputWin);
        
        // Dapatkan input
        int ch = wgetch(inputWin);
        
        if (ch == '\n' || ch == KEY_ENTER) {
            // Tekan Enter untuk selesai
            done = true;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8 || ch == KEY_DC) {
            // Backspace atau Delete untuk menghapus karakter
            if (nameLength > 0) {
                nameLength--;
                name[nameLength] = '\0';
                cursorPos--;
                
                // Hapus karakter terakhir dari layar
                mvwaddch(inputWin, 4, cursorPos, ' ');
                wmove(inputWin, 4, cursorPos);
                wrefresh(inputWin);
            }
        } else if (ch == 27) { // ESC key
            // Batal input
            name[0] = '\0';
            nameLength = 0;
            done = true;
        } else if (nameLength < 19 && ch >= 32 && ch <= 126) {
            // Karakter printable dan masih ada ruang
            name[nameLength] = ch;
            nameLength++;
            name[nameLength] = '\0';
            cursorPos++;
            
            // Tampilkan karakter yang baru ditambahkan
            mvwaddch(inputWin, 4, cursorPos - 1, ch);
            wrefresh(inputWin);
        } else if (ch == KEY_LEFT) {
            // Panah kiri untuk pindah kursor
            if (cursorPos > 8) {
                cursorPos--;
                wmove(inputWin, 4, cursorPos);
                wrefresh(inputWin);
            }
        } else if (ch == KEY_RIGHT) {
            // Panah kanan untuk pindah kursor
            if (cursorPos < 8 + nameLength) {
                cursorPos++;
                wmove(inputWin, 4, cursorPos);
                wrefresh(inputWin);
            }
        } else if (ch == KEY_HOME) {
            // Home untuk ke awal
            cursorPos = 8;
            wmove(inputWin, 4, cursorPos);
            wrefresh(inputWin);
        } else if (ch == KEY_END) {
            // End untuk ke akhir
            cursorPos = 8 + nameLength;
            wmove(inputWin, 4, cursorPos);
            wrefresh(inputWin);
        }
    }
    
    // Bersihkan window
    werase(inputWin);
    wrefresh(inputWin);
    delwin(inputWin);
    
    // Tampilkan pesan konfirmasi (hanya jika nama tidak kosong)
    if (strlen(name) > 0) {
        // Bersihkan layar dan tampilkan pesan konfirmasi di tengah
        clear();
        mvprintw(maxY / 2 - 1, maxX / 2 - 15, "High score saved successfully!");
        mvprintw(maxY / 2, maxX / 2 - 10, "Name: %s", name);
        mvprintw(maxY / 2 + 1, maxX / 2 - 10, "Score saved!");
        mvprintw(maxY / 2 + 3, maxX / 2 - 15, "Press any key to continue...");
        refresh();
        getch();
    } else {
        // Jika nama kosong (ESC ditekan), tampilkan pesan
        clear();
        mvprintw(maxY / 2, maxX / 2 - 15, "High score not saved (cancelled)");
        mvprintw(maxY / 2 + 2, maxX / 2 - 15, "Press any key to continue...");
        refresh();
        getch();
    }
    
    noecho();
    curs_set(0);
    
    return string(name);
}

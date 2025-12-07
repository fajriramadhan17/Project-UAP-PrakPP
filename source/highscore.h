#include <ncurses/ncurses.h>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>

struct HighScoreEntry {
    char name[20];
    int score;
};

class HighScoreManager {
private:
    HighScoreEntry highScores[10]; // Array fixed size untuk 10 high score
    int scoreCount;
    std::string filename;
    const int MAX_SCORES = 10;

public:
    HighScoreManager(const std::string& file = "highscores.txt") : filename(file), scoreCount(0) {
        loadScores();
    }
    
    void loadScores() {
        scoreCount = 0;

std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }

HighScoreEntry entry;
        while (scoreCount < MAX_SCORES && file >> entry.name >> entry.score) {
            highScores[scoreCount] = entry;
            scoreCount++;
        }

file.close();
        
        // Sort scores menggunakan bubble sort
        for (int i = 0; i < scoreCount - 1; i++) {
            for (int j = 0; j < scoreCount - i - 1; j++) {
                if (highScores[j].score < highScores[j + 1].score) {
                    HighScoreEntry temp = highScores[j];
                    highScores[j] = highScores[j + 1];
                    highScores[j + 1] = temp;
                }
            }
        }
    }

void saveScores() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return;
        }
        
        for (int i = 0; i < scoreCount; i++) {
            file << highScores[i].name << " " << highScores[i].score << "\n";
        }
        
        file.close();
    }
    
    void addScore(const std::string& name, int score) {
        // Jika array penuh dan skor lebih rendah dari yang terendah, tidak tambahkan
        if (scoreCount == MAX_SCORES && score <= highScores[MAX_SCORES - 1].score) {
            return;
        }
        
        // Buat entry baru
        HighScoreEntry newEntry;
        strncpy(newEntry.name, name.c_str(), 19);
        newEntry.name[19] = '\0';
        newEntry.score = score;
        
        // Jika array belum penuh, tambahkan di akhir
        if (scoreCount < MAX_SCORES) {
            highScores[scoreCount] = newEntry;
            scoreCount++;
        } 
        // Jika array penuh, ganti yang terendah
        else {
            highScores[MAX_SCORES - 1] = newEntry;
        }
        
        // Sort scores menggunakan bubble sort
        for (int i = 0; i < scoreCount - 1; i++) {
            for (int j = 0; j < scoreCount - i - 1; j++) {
                if (highScores[j].score < highScores[j + 1].score) {
                    HighScoreEntry temp = highScores[j];
                    highScores[j] = highScores[j + 1];
                    highScores[j + 1] = temp;
                }
            }
        }
        
        saveScores();
}

    void displayScores() {
        clear();
        
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        mvprintw(2, maxX / 2 - 6, "HIGH SCORES");
        mvprintw(3, maxX / 2 - 10, "====================");
        
        int startY = maxY / 2 - 5;
        
        if (scoreCount == 0) {
            mvprintw(startY, maxX / 2 - 10, "No high scores yet!");
        } else {
            mvprintw(startY - 2, maxX / 2 - 15, "RANK  NAME               SCORE");
            mvprintw(startY - 1, maxX / 2 - 15, "--------------------------------");
            
            for (int i = 0; i < scoreCount && i < MAX_SCORES; i++) {
                mvprintw(startY + i, maxX / 2 - 15, 
                        "%2d.   %-15s   %5d", 
                        i + 1, 
                        highScores[i].name, 
                        highScores[i].score);
            }
        }
        
        mvprintw(startY + 12, maxX / 2 - 10, "Press any key to continue...");
        
        refresh();
        getch();
    }
    
    int getScoreCount() const { return scoreCount; }
    
    HighScoreEntry getScoreAt(int index) const {
        if (index >= 0 && index < scoreCount) {
            return highScores[index];
        }
        HighScoreEntry empty;
        strcpy(empty.name, "");
        empty.score = 0;
        return empty;
    }
    
    void clearScores() {
        scoreCount = 0;
        saveScores();
    }
};

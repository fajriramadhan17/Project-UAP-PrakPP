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

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

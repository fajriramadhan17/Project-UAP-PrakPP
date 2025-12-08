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

class Game {
private:
    int width, height;
    int gameAreaWidth, gameAreaHeight;
    int gameAreaStartX, gameAreaStartY;
    
    GameObject player;
    Obstacle3x3* obstacles; // Ubah ke Obstacle3x3
    int obstacleCount;
    int maxObstacles;
    
    int score;
    bool isPaused;
    bool isGameOver;
    int speed;
    int frameCount;
    
    // Tambahan untuk game horizontal
    int cameraX;  // Posisi kamera untuk scrolling
    int playerBaseX;  // Posisi dasar pemain (untuk perhitungan relatif)
    
    // Variabel untuk melacak skor terakhir saat kecepatan ditingkatkan
    int lastSpeedIncreaseScore;
    int speedIncreaseCounter; // Menghitung berapa kali kecepatan sudah ditingkatkan
    
    // Variabel baru untuk kontrol kecepatan
    int playerSpeed;  // Kecepatan pergerakan player (otomatis maju)
    int obstacleSpawnRate;  // Frekuensi spawn obstacle
    int obstacleDensity;  // Kepadatan obstacle (berapa banyak yang muncul)

public:
    Game(int w = 80, int h = 24) {
        srand(time(0));
        
        width = w;
        height = h;
        gameAreaWidth = 60;  // Lebar lebih besar untuk horizontal scrolling
        gameAreaHeight = 18; // Tinggi lebih kecil
        gameAreaStartX = 10; // Area game dimulai dari kiri
        gameAreaStartY = (h - gameAreaHeight) / 2;
        
        maxObstacles = 20;   // Lebih banyak obstacle untuk level tinggi
        obstacles = new Obstacle3x3[maxObstacles];
        obstacleCount = 0;
        
        cameraX = 0;
        lastSpeedIncreaseScore = 0;
        speedIncreaseCounter = 0;

 // Inisialisasi variabel kecepatan baru
        playerSpeed = 1;  // Kecepatan awal player
        obstacleSpawnRate = 15;  // Frekuensi spawn awal
        obstacleDensity = 1;  // Kepadatan obstacle awal
        
        initGame();
    }
    
    ~Game() {
        delete[] obstacles;
    }
    
    void initGame() {
        // Posisi player di tengah kiri area game
        player.x = gameAreaStartX + 2;
        player.y = gameAreaStartY + gameAreaHeight / 2; // Tengah vertikal
        player.symbol = '>';
        
        playerBaseX = player.x;
        
        score = 0;
        isPaused = false;
        isGameOver = false;
        speed = 15; // Kecepatan obstacle bergerak
        frameCount = 0;
        obstacleCount = 0;
        cameraX = 0;
        lastSpeedIncreaseScore = 0;
        speedIncreaseCounter = 0;
        
        // Reset variabel kecepatan
        playerSpeed = 1;
        obstacleSpawnRate = 15;
        obstacleDensity = 1;
        
        // Spawn beberapa obstacle awal
        for (int i = 0; i < 5; i++) {
            spawnObstacle();
        }
    }
    
    void drawBorder() {
        if (has_colors()) {
            attron(COLOR_PAIR(3));
        }
        
        // Border atas
        for (int x = gameAreaStartX; x < gameAreaStartX + gameAreaWidth; x++) {
            mvaddch(gameAreaStartY, x, '-');
        }
        
        // Border bawah
        for (int x = gameAreaStartX; x < gameAreaStartX + gameAreaWidth; x++) {
            mvaddch(gameAreaStartY + gameAreaHeight - 1, x, '-');
        }
        
        // Border kiri dan kanan
        for (int y = gameAreaStartY; y < gameAreaStartY + gameAreaHeight; y++) {
            mvaddch(y, gameAreaStartX, '|');
            mvaddch(y, gameAreaStartX + gameAreaWidth - 1, '|');
        }
        
        // Sudut border
        mvaddch(gameAreaStartY, gameAreaStartX, '+');
        mvaddch(gameAreaStartY, gameAreaStartX + gameAreaWidth - 1, '+');
        mvaddch(gameAreaStartY + gameAreaHeight - 1, gameAreaStartX, '+');
        mvaddch(gameAreaStartY + gameAreaHeight - 1, gameAreaStartX + gameAreaWidth - 1, '+');
        
        if (has_colors()) {
            attroff(COLOR_PAIR(3));
        }
        
        // Info game dengan informasi kecepatan baru
        mvprintw(gameAreaStartY - 2, gameAreaStartX, "SCORE: %d", score);
        mvprintw(gameAreaStartY - 2, gameAreaStartX + 20, "SPEED LEVEL: %d", speedIncreaseCounter);
        mvprintw(gameAreaStartY - 2, gameAreaStartX + 40, "PLAYER SPEED: %d", playerSpeed);
        
        if (isPaused) {
            mvprintw(gameAreaStartY + gameAreaHeight / 2, 
                     gameAreaStartX + gameAreaWidth / 2 - 5, 
                     "[PAUSED]");
        }
        
        if (isGameOver) {
            mvprintw(gameAreaStartY + gameAreaHeight / 2 - 1, 
                     gameAreaStartX + gameAreaWidth / 2 - 10, 
                     "GAME OVER!");
            mvprintw(gameAreaStartY + gameAreaHeight / 2, 
                     gameAreaStartX + gameAreaWidth / 2 - 10, 
                     "Final Score: %d", score);
            mvprintw(gameAreaStartY + gameAreaHeight / 2 + 1, 
                     gameAreaStartX  + gameAreaWidth / 2 - 10, 
                     "Speed Level Reached: %d", speedIncreaseCounter);
        }
    }
void drawPlayer() {
        if (has_colors()) {
            attron(COLOR_PAIR(2));
        }
        
        // Gambar player sebagai ">"
        mvaddch(player.y, player.x - cameraX, '>');
        
        // Tambahkan efek visual untuk player
        if (!isGameOver) {
            mvaddch(player.y, player.x - cameraX - 1, '-'); // Ekor
            // Tambahkan efek kecepatan
            if (playerSpeed > 1) {
                mvaddch(player.y, player.x - cameraX - 2, '-');
            }
            if (playerSpeed > 2) {
                mvaddch(player.y, player.x - cameraX - 3, '-');
            }
        }
        
        if (has_colors()) {
            attroff(COLOR_PAIR(2));
        }
    }
    
    void drawObstacles() {
        if (has_colors()) {
            attron(COLOR_PAIR(1));
        }
        
        for (int i = 0; i < obstacleCount; i++) {
            // Gambar obstacle 3x3
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int screenX = (obstacles[i].x + dx) - cameraX;
                    int screenY = obstacles[i].y + dy;
                    
                    // Hanya gambar bagian yang terlihat di layar
                    if (screenX >= gameAreaStartX && 
                        screenX < gameAreaStartX + gameAreaWidth &&
                        screenY >= gameAreaStartY && 
                        screenY < gameAreaStartY + gameAreaHeight) {
                        
                        mvaddch(screenY, screenX, obstacles[i].shape[dy + 1][dx + 1]);
                    }
                }
            }
        }
        
        if (has_colors()) {
            attroff(COLOR_PAIR(1));
        }
    }
    
    void updateObstacles() {
        if (isPaused || isGameOver) return;
        
        frameCount++;
        
        // Cek apakah skor mencapai kelipatan 100 untuk meningkatkan kecepatan
        checkAndIncreaseSpeed();
        
        // Gerakkan obstacle ke kiri (mendekati player) dengan kecepatan yang disesuaikan
        for (int i = 0; i < obstacleCount; i++) {
            obstacles[i].x -= 2 + (speedIncreaseCounter / 2); // Semakin cepat saat level naik
            
            // Jika obstacle keluar dari layar di sisi kiri (periksa bagian kanan obstacle)
            if ((obstacles[i].x + 1) < cameraX) {
                score += 10 + (speedIncreaseCounter * 2); // Bonus skor lebih tinggi saat level naik
                // Hapus obstacle yang sudah lewat
                for (int j = i; j < obstacleCount - 1; j++) {
                    obstacles[j] = obstacles[j + 1];
                }
                obstacleCount--;
                i--; // Periksa ulang index ini
            }
        }
        
        // Spawn obstacle baru dengan rate dan density yang disesuaikan
        if (frameCount % obstacleSpawnRate == 0) {
            // Spawn multiple obstacles berdasarkan density
            for (int i = 0; i < obstacleDensity; i++) {
                spawnObstacle();
            }
        }

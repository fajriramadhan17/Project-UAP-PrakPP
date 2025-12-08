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

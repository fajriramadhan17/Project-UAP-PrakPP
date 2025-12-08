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

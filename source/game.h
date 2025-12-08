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

    updateScore();
        
        // Update kamera (scrolling)
        if (player.x - cameraX > gameAreaStartX + gameAreaWidth * 2 / 3) {
            cameraX = player.x - (gameAreaStartX + gameAreaWidth * 2 / 3);
        }
    }
    
    void checkAndIncreaseSpeed() {
        // Cek apakah skor mencapai kelipatan 100 dan belum ditingkatkan
        if (score >= lastSpeedIncreaseScore + 100) {
            // Hitung level peningkatan berdasarkan berapa kali sudah ditingkatkan
            int level = speedIncreaseCounter + 1;
            
            // 1. Tingkatkan kecepatan obstacle (bergerak lebih cepat ke kiri)
            speed -= 2; // Kurangi delay frame (semakin kecil = semakin cepat)
            if (speed < 3) speed = 3; // Minimum speed
            
            // 2. Tingkatkan kecepatan player (bergerak maju lebih cepat)
            playerSpeed += 1; // Tambah kecepatan player
            
            // 3. Kurangi spawn rate (obstacle muncul lebih sering)
            obstacleSpawnRate -= 2;
            if (obstacleSpawnRate < 3) obstacleSpawnRate = 3;
            
            // 4. Tingkatkan density (lebih banyak obstacle yang muncul)
            obstacleDensity += 1;
            if (obstacleDensity > 3) obstacleDensity = 3; // Maksimal 3 obstacle sekaligus
            
            speedIncreaseCounter++;
            lastSpeedIncreaseScore = (score / 100) * 100; // Bulatkan ke kelipatan 100 terdekat
            
            // Tampilkan pesan kecepatan meningkat di atas area game
            if (has_colors()) {
                attron(COLOR_PAIR(4) | A_BOLD); // Warna hijau tebal
            }
            
            // Simpan posisi skor untuk nanti dikembalikan
            int savedY = gameAreaStartY - 2;
            int savedX = gameAreaStartX;
            
            // Tampilkan pesan speed boost di atas border area game
            mvprintw(gameAreaStartY - 3, gameAreaStartX, 
                    "SPEED BOOST! Level: %d | Player Speed: %d | Spawn Rate: %d", 
                    speedIncreaseCounter, playerSpeed, obstacleSpawnRate);
            
            if (has_colors()) {
                attroff(COLOR_PAIR(4) | A_BOLD);
            }
            
            refresh();
            
            // Tunggu sebentar agar pesan terlihat
            napms(800); // 0.8 detik
            
            // Hapus pesan speed boost dengan menimpa dengan spasi
            mvprintw(gameAreaStartY - 3, gameAreaStartX, 
                    "                                                                   ");
            
            // Refresh lagi untuk memperbarui layar
            refresh();
        }
    }
    
    void spawnObstacle() {
        if (obstacleCount < maxObstacles) {
            obstacles[obstacleCount].x = cameraX + gameAreaStartX + gameAreaWidth + rand() % 20;
            
            // Sesuaikan distribusi obstacle berdasarkan level kecepatan
            int minY = gameAreaStartY + 1;
            int maxY = gameAreaStartY + gameAreaHeight - 2;
            int range = maxY - minY;
            
            // Pada level tinggi, obstacle bisa muncul di mana saja
            if (speedIncreaseCounter > 0) {
                obstacles[obstacleCount].y = minY + rand() % range;
            } else {
                // Awal game, obstacle hanya di tengah
                obstacles[obstacleCount].y = minY + (range / 3) + rand() % (range / 3);
            }
            
            // Pilih simbol obstacle berdasarkan level
            char symbols[] = {'#', '@', '*', '%', '&', '+', 'X', 'O'};
            char chosenSymbol = symbols[rand() % (4 + speedIncreaseCounter)];
            if (chosenSymbol > 'O') chosenSymbol = 'O';
            obstacles[obstacleCount].symbol = chosenSymbol;
            
            // Buat obstacle berdasarkan level kecepatan
            for (int dy = 0; dy < 3; dy++) {
                for (int dx = 0; dx < 3; dx++) {
                    if (dx == 1 && dy == 1) {
                        // Tengah: simbol utama
                        obstacles[obstacleCount].shape[dy][dx] = chosenSymbol;
                    } else {
                        // Pada level tinggi, buat obstacle lebih padat
                        if (speedIncreaseCounter > 0 && rand() % (4 - speedIncreaseCounter) == 0) {
                            obstacles[obstacleCount].shape[dy][dx] = chosenSymbol;
                        } else {
                            char borderChars[] = {chosenSymbol, '.', '+', 'o', '~', '-'};
                            obstacles[obstacleCount].shape[dy][dx] = borderChars[rand() % 6];
                        }
                    }
                }
            }
            
            // Pada level tinggi, kadang buat obstacle lebih besar
            if (speedIncreaseCounter > 2 && rand() % 5 == 0) {
                for (int dy = 0; dy < 3; dy++) {
                    for (int dx = 0; dx < 3; dx++) {
                        obstacles[obstacleCount].shape[dy][dx] = chosenSymbol;
                    }
                }
            }
            
            obstacleCount++;
        }
    }
    
    void movePlayer(int dy) {  // Sekarang dy untuk naik/turun
        if (isPaused || isGameOver) return;
        
        int newY = player.y + dy;
        
        // Batasi pergerakan dalam area game
        if (newY > gameAreaStartY + 1 && newY < gameAreaStartY + gameAreaHeight - 2) {
            player.y = newY;
        }
        
        // Gerakkan player maju dengan kecepatan yang disesuaikan
        if (!isPaused && !isGameOver) {
            player.x += playerSpeed; // Gunakan playerSpeed
            
            // Tambah skor berdasarkan kecepatan player
            if (frameCount % 5 == 0) {
                score += playerSpeed; // Skor lebih tinggi jika playerSpeed lebih besar
            }
        }
    }
    
    bool checkCollision() {
        if (isGameOver) return false;
        
        for (int i = 0; i < obstacleCount; i++) {
            // Cek tabrakan dengan seluruh area obstacle 3x3
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int obsX = obstacles[i].x + dx;
                    int obsY = obstacles[i].y + dy;
                    
                    // Tabrakan di posisi player
                    if (obsX == player.x && obsY == player.y) {
                        return true;
                    }
                    
                    // Tabrakan jika obstacle tepat di depan player
                    if (obsX == player.x + 1 && obsY == player.y) {
                        return true;
                    }
                    
                    // Tabrakan jika obstacle menyentuh ekor player
                    if (obsX == player.x - 1 && obsY == player.y) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    void updateScore() {
        // Skor sudah ditambah di movePlayer() dan updateObstacles()
        // Tambahkan bonus untuk bertahan hidup
        if (frameCount % 100 == 0 && !isPaused && !isGameOver) {
            score += 5 * playerSpeed; // Bonus berdasarkan kecepatan player
        }
    }
    
    void pauseGame() {
        isPaused = !isPaused;
    }
    
    void resumeGame() {
        isPaused = false;
    }
    
    void gameOver() {
        isGameOver = true;
    }
    
    bool getIsPaused() const { return isPaused; }
    bool getIsGameOver() const { return isGameOver; }
    int getScore() const { return score; }
    GameObject getPlayer() const { return player; }
    int getSpeedLevel() const { return speedIncreaseCounter; }
    
    std::string playerName;
};

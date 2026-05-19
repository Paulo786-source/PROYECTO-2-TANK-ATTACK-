#pragma once
#include "raylib.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void initialize();
    void close();
    void update(); // llamar cada frame para el stream de música

    // Combate
    void playShot();
    void playExplosion();
    void playBounce();
    void playHit();

    // Movimiento
    void playTankMove();
    void playTankSelect();

    // Power-ups
    void playPowerUpPickup();
    void playPowerUpActivate();

    // UI / Juego
    void playTurnChange();
    void playResults();
    void playButtonClick();
    void startBackgroundMusic();
    void stopBackgroundMusic();

private:
    Sound shotSound;
    Sound explosionSound;
    Sound bounceSound;
    Sound hitSound;
    Sound tankMoveSound;
    Sound tankSelectSound;
    Sound powerUpPickupSound;
    Sound powerUpActivateSound;
    Sound turnChangeSound;
    Sound resultsSound;
    Sound buttonClickSound;

    Music backgroundMusic;

    bool initialized = false;
};
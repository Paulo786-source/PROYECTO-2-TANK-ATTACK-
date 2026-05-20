#pragma once
#include "raylib.h"
#include "GameEventListener.h"

class AudioManager : public GameEventListener {
public:
    AudioManager();
    ~AudioManager();

    // implementaci?n del Observer
    void onGameEvent(GameEvent event) override;

    void initialize();
    void close();
    void update(); // llamar cada frame para el stream de musica

    // combate
    void playShot();
    void playExplosion();
    void playBounce();
    void playHit();

    // movimiento
    void playTankMove();
    void playTankSelect();

    // power-ups
    void playPowerUpPickup();
    void playPowerUpActivate();

    // UI / Juego
    void playTurnChange();
    void playResults();
    void playButtonClick();
    void startBackgroundMusic();
    void stopBackgroundMusic();

private:
    Sound shotSound = {};
    Sound explosionSound = {};
    Sound bounceSound = {};
    Sound hitSound = {};
    Sound tankMoveSound = {};
    Sound tankSelectSound = {};
    Sound powerUpPickupSound = {};
    Sound powerUpActivateSound = {};
    Sound turnChangeSound = {};
    Sound resultsSound = {};
    Sound buttonClickSound = {};

    Music backgroundMusic = {};

    bool initialized = false;
};
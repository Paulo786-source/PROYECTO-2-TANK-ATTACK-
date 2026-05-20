#include "AudioManager.h"

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    close();
}

void AudioManager::initialize() {
    InitAudioDevice();

    shotSound = LoadSound("Sonidos/disparo.ogg");
    explosionSound = LoadSound("Sonidos/muerte.ogg");
    bounceSound = LoadSound("Sonidos/boing.ogg");
    hitSound = LoadSound("Sonidos/herir.ogg");
    tankMoveSound = LoadSound("Sonidos/caminando.ogg");
    tankSelectSound = LoadSound("Sonidos/click.ogg");
    powerUpPickupSound = LoadSound("Sonidos/recoger_powerup.ogg");
    powerUpActivateSound = LoadSound("Sonidos/powerup_activado.ogg");
    turnChangeSound = LoadSound("Sonidos/cambio_turno.ogg");
    resultsSound = LoadSound("Sonidos/resultados.ogg");
    buttonClickSound = LoadSound("Sonidos/click.ogg");

    backgroundMusic = LoadMusicStream("Sonidos/musica_fondo.mp3");
    SetMusicVolume(backgroundMusic, 0.5f);

    initialized = true;
}

void AudioManager::close() {
    if (!initialized) return;

    UnloadSound(shotSound);
    UnloadSound(explosionSound);
    UnloadSound(bounceSound);
    UnloadSound(hitSound);
    UnloadSound(tankMoveSound);
    UnloadSound(tankSelectSound);
    UnloadSound(powerUpPickupSound);
    UnloadSound(powerUpActivateSound);
    UnloadSound(turnChangeSound);
    UnloadSound(resultsSound);
    UnloadSound(buttonClickSound);

    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();

    initialized = false;
}

void AudioManager::update() {
    if (!initialized) return;
    UpdateMusicStream(backgroundMusic);
}

void AudioManager::startBackgroundMusic() {
    if (!initialized) return;
    PlayMusicStream(backgroundMusic);
}

// combate
void AudioManager::playShot() { if (initialized) PlaySound(shotSound); }
void AudioManager::playExplosion() { if (initialized) PlaySound(explosionSound); }
void AudioManager::playBounce() { if (initialized) PlaySound(bounceSound); }
void AudioManager::playHit() { if (initialized) PlaySound(hitSound); }

// movimiento
void AudioManager::playTankMove() { if (initialized) PlaySound(tankMoveSound); }
void AudioManager::playTankSelect() { if (initialized) PlaySound(tankSelectSound); }

// power-ups
void AudioManager::playPowerUpPickup() { if (initialized) PlaySound(powerUpPickupSound); }
void AudioManager::playPowerUpActivate() { if (initialized) PlaySound(powerUpActivateSound); }

// UI 
void AudioManager::playTurnChange() { if (initialized) PlaySound(turnChangeSound); }
void AudioManager::playResults() { if (initialized) PlaySound(resultsSound); }
void AudioManager::playButtonClick() { if (initialized) PlaySound(buttonClickSound); }

void AudioManager::stopBackgroundMusic() {
    if (!initialized) return;
    StopMusicStream(backgroundMusic);
}

// observer
void AudioManager::onGameEvent(GameEvent event) {
    switch (event) {
    case GameEvent::Shot:                playShot();              break;
    case GameEvent::Explosion:           playExplosion();         break;
    case GameEvent::Bounce:              playBounce();            break;
    case GameEvent::Hit:                 playHit();               break;
    case GameEvent::TankMove:            playTankMove();          break;
    case GameEvent::TankSelect:          playTankSelect();        break;
    case GameEvent::PowerUpPickup:       playPowerUpPickup();     break;
    case GameEvent::PowerUpActivate:     playPowerUpActivate();   break;
    case GameEvent::TurnChange:          playTurnChange();        break;
    case GameEvent::Results:             playResults();           break;
    case GameEvent::ButtonClick:         playButtonClick();       break;
    case GameEvent::BackgroundMusicStart: startBackgroundMusic(); break;
    case GameEvent::BackgroundMusicStop:  stopBackgroundMusic();  break;
    }
}
#pragma once

enum class GameEvent {
    Shot,
    Explosion,
    Bounce,
    Hit,
    TankMove,
    TankSelect,
    PowerUpPickup,
    PowerUpActivate,
    TurnChange,
    Results,
    ButtonClick,
    BackgroundMusicStart,
    BackgroundMusicStop
};

// interfaz base del patrón Observer
// cualquier clase que quiera escuchar eventos de juego debe heredar 
class GameEventListener {
public:
    virtual ~GameEventListener() = default;
    virtual void onGameEvent(GameEvent event) = 0;
};
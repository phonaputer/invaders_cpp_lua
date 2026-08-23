export type AnimationStrip = number

declare AnimationStrips: {
    create: () -> (AnimationStrip),
    addFrame: (AnimationStrip, number, number) -> (),
}

export type Callback = number

declare Callbacks: {
    register: (package: string, func: string) -> (Callback),
}

declare DamageType: {
    ALIEN: number,
    PLAYER: number,
    FORTRESS: number,
    ALIEN_PROJECTILE: number,
    PLAYER_PROJECTILE: number,
}

declare Game: {
    WINDOW_WIDTH: number,
    WINDOW_HEIGHT: number,
    decrementLives: () -> (boolean),
    setLives: (number) -> (),
    resetScore: () -> (),
    incrementScore: (number) -> (),
    pause: () -> (),
    unpause: () -> (),
    displayGameOver: () -> (),
    playSound: (string) -> (),
    stopSound: (string) -> (),
}
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

export type Entity = number

declare HUD: {
    incrementScore: (number) -> (),
}

declare Game: {
    WINDOW_WIDTH: number,
    WINDOW_HEIGHT: number,
}

export type Animation = {
    curFrame: number,
    tickCounter: number,
    ticksPerFrame: number,
    stripID: AnimationStrip,
    playing: boolean,
    playReversed: boolean,
}

export type AnimationStepped = {
    curFrame: number,
    stripID: AnimationStrip,
}

export type CollisionActive = {
    hitboxOffsetX: number,
    hitboxOffsetY: number,
    hitboxW: number,
    hitboxH: number,
}

export type CollisionCallback = {
    callback: Callback,
}

export type CollisionPassive = {
    hitboxOffsetX: number,
    hitboxOffsetY: number,
    hitboxW: number,
    hitboxH: number,
}

export type Damage = {
    type: number,
    amount: number,
}

export type DeletionCallback = {
    callback: Callback,
}

export type Hitpoints = {
    susceptibleTo: number,
    curHitpoints: number,
}

export type InvaderOrchestrationState = {
    noInvadersCallback: number,
    baseTicksPerMove: number,
    tickCounter: number,
    xSpeed: number,
    ySpeed: number,
    lastInvaderXSpeed: number,
    lastInvaderYSpeed: number,
    movingLeft: boolean,
}

export type OrchestratedInvader = {}

export type PlayerAttack = {
    ticksPerAttack: number,
    tickCounter: number,
    callback: Callback,
}

export type PlayerMovement = {
    xSpeed: number,
}

export type Position = {
    x: number,
    y: number,
    w: number,
    h: number,
    z: number,
}

export type PositionFollowing = {
    leader: number,
    xOffset: number,
    yOffset: number,
}

export type Sprite = {
    srcID: string,
    srcX: number,
    srcY: number,
    srcW: number,
    srcH: number,
    dstW: number,
    dstH: number,
}

export type ToBeDeleted = {}

export type TTL = {
    ticksToLive: number,
    tickCounter: number,
}

export type Velocity = {
    x: number,
    y: number,
}

declare Components: {
    Animation: (() -> Animation),
    AnimationStepped: (() -> AnimationStepped),
    CollisionActive: (() -> CollisionActive),
    CollisionCallback: (() -> CollisionCallback),
    CollisionPassive: (() -> CollisionPassive),
    Damage: (() -> Damage),
    DeletionCallback: (() -> DeletionCallback),
    Hitpoints: (() -> Hitpoints),
    InvaderOrchestrationState: (() -> InvaderOrchestrationState),
    OrchestratedInvader: (() -> OrchestratedInvader),
    PlayerAttack: (() -> PlayerAttack),
    PlayerMovement: (() -> PlayerMovement),
    Position: (() -> Position),
    PositionFollowing: (() -> PositionFollowing),
    Sprite: (() -> Sprite),
    ToBeDeleted: (() -> ToBeDeleted),
    TTL: (() -> TTL),
    Velocity: (() -> Velocity),
}

declare ECS: {
    create: () -> (Entity),

    hasAnimation: (Entity) -> (boolean),
    getAnimation: (Entity) -> Animation,
    setAnimation: (Entity, Animation) -> (),
    removeAnimation: (Entity) -> (),

    hasAnimationStepped: (Entity) -> (boolean),
    getAnimationStepped: (Entity) -> AnimationStepped,
    setAnimationStepped: (Entity, AnimationStepped) -> (),
    removeAnimationStepped: (Entity) -> (),

    hasCollisionActive: (Entity) -> (boolean),
    getCollisionActive: (Entity) -> CollisionActive,
    setCollisionActive: (Entity, CollisionActive) -> (),
    removeCollisionActive: (Entity) -> (),

    hasCollisionCallback: (Entity) -> (boolean),
    getCollisionCallback: (Entity) -> CollisionCallback,
    setCollisionCallback: (Entity, CollisionCallback) -> (),
    removeCollisionCallback: (Entity) -> (),

    hasCollisionPassive: (Entity) -> (boolean),
    getCollisionPassive: (Entity) -> CollisionPassive,
    setCollisionPassive: (Entity, CollisionPassive) -> (),
    removeCollisionPassive: (Entity) -> (),

    hasDamage: (Entity) -> (boolean),
    getDamage: (Entity) -> Damage,
    setDamage: (Entity, Damage) -> (),
    removeDamage: (Entity) -> (),

    hasDeletionCallback: (Entity) -> (boolean),
    getDeletionCallback: (Entity) -> DeletionCallback,
    setDeletionCallback: (Entity, DeletionCallback) -> (),
    removeDeletionCallback: (Entity) -> (),

    hasHitpoints: (Entity) -> (boolean),
    getHitpoints: (Entity) -> Hitpoints,
    setHitpoints: (Entity, Hitpoints) -> (),
    removeHitpoints: (Entity) -> (),

    hasInvaderOrchestrationState: () -> (boolean),
    getInvaderOrchestrationState: () -> InvaderOrchestrationState,
    setInvaderOrchestrationState: (InvaderOrchestrationState) -> (),
    removeInvaderOrchestrationState: () -> (),

    hasOrchestratedInvader: (Entity) -> (boolean),
    getOrchestratedInvader: (Entity) -> OrchestratedInvader,
    setOrchestratedInvader: (Entity, OrchestratedInvader) -> (),
    removeOrchestratedInvader: (Entity) -> (),

    hasPlayerAttack: (Entity) -> (boolean),
    getPlayerAttack: (Entity) -> PlayerAttack,
    setPlayerAttack: (Entity, PlayerAttack) -> (),
    removePlayerAttack: (Entity) -> (),

    hasPlayerMovement: (Entity) -> (boolean),
    getPlayerMovement: (Entity) -> PlayerMovement,
    setPlayerMovement: (Entity, PlayerMovement) -> (),
    removePlayerMovement: (Entity) -> (),

    hasPosition: (Entity) -> (boolean),
    getPosition: (Entity) -> Position,
    setPosition: (Entity, Position) -> (),
    removePosition: (Entity) -> (),

    hasPositionFollowing: (Entity) -> (boolean),
    getPositionFollowing: (Entity) -> PositionFollowing,
    setPositionFollowing: (Entity, PositionFollowing) -> (),
    removePositionFollowing: (Entity) -> (),

    hasSprite: (Entity) -> (boolean),
    getSprite: (Entity) -> Sprite,
    setSprite: (Entity, Sprite) -> (),
    removeSprite: (Entity) -> (),

    hasToBeDeleted: (Entity) -> (boolean),
    getToBeDeleted: (Entity) -> ToBeDeleted,
    setToBeDeleted: (Entity, ToBeDeleted) -> (),
    removeToBeDeleted: (Entity) -> (),

    hasTTL: (Entity) -> (boolean),
    getTTL: (Entity) -> TTL,
    setTTL: (Entity, TTL) -> (),
    removeTTL: (Entity) -> (),

    hasVelocity: (Entity) -> (boolean),
    getVelocity: (Entity) -> Velocity,
    setVelocity: (Entity, Velocity) -> (),
    removeVelocity: (Entity) -> (),
}
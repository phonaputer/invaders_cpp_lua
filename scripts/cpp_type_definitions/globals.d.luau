export type Animation = {
    curFrame: number,
    tickCounter: number,
    ticksPerFrame: number,
    stripID: number,
    playing: boolean,
    playReversed: boolean,
}

export type Collision = {
    hitboxOffsetX: number,
    hitboxOffsetY: number,
    hitboxW: number,
    hitboxH: number,
    isPassive: boolean,
}

export type DeletionCallback = {
    callback: string,
}

export type PlayerAttack = {
    ticksPerAttack: number,
    tickCounter: number,
    callback: string,
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

export type Velocity = {
    x: number,
    y: number,
}

declare Components: {
    Animation: (() -> Animation),
    Collision: (() -> Collision),
    DeletionCallback: (() -> DeletionCallback),
    PlayerAttack: (() -> PlayerAttack),
    PlayerMovement: (() -> PlayerMovement),
    Position: (() -> Position),
    PositionFollowing: (() -> PositionFollowing),
    Sprite: (() -> Sprite),
    ToBeDeleted: (() -> ToBeDeleted),
    Velocity: (() -> Velocity),
}

type Entity = number

declare ECS: {
    create: () -> (Entity),

    hasAnimation: (Entity) -> (boolean),
    getAnimation: (Entity) -> Animation,
    setAnimation: (Entity, Animation) -> (),
    removeAnimation: (Entity) -> (),

    hasCollision: (Entity) -> (boolean),
    getCollision: (Entity) -> Collision,
    setCollision: (Entity, Collision) -> (),
    removeCollision: (Entity) -> (),

    hasDeletionCallback: (Entity) -> (boolean),
    getDeletionCallback: (Entity) -> DeletionCallback,
    setDeletionCallback: (Entity, DeletionCallback) -> (),
    removeDeletionCallback: (Entity) -> (),

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

    hasVelocity: (Entity) -> (boolean),
    getVelocity: (Entity) -> Velocity,
    setVelocity: (Entity, Velocity) -> (),
    removeVelocity: (Entity) -> (),
}

type AnimationStrip = number

declare AnimationStrips: {
    create: () -> (AnimationStrip),
    addFrame: (AnimationStrip, number, number) -> (),
}

declare Game: {
    WINDOW_WIDTH: number,
    WINDOW_HEIGHT: number,
}
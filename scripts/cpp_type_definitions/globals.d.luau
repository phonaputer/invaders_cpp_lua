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

export type Position = {
    x: number,
    y: number,
    w: number,
    h: number,
    z: number,
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
    Position: (() -> Position),
    Sprite: (() -> Sprite),
    ToBeDeleted: (() -> ToBeDeleted),
    Velocity: (() -> Velocity)
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

    hasPosition: (Entity) -> (boolean),
    getPosition: (Entity) -> Position,
    setPosition: (Entity, Position) -> (),
    removePosition: (Entity) -> (),

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
    removeVelocity: (Entity) -> ()
}

type AnimationStrip = number

declare AnimationStrips: {
    create: () -> (AnimationStrip),
    addFrame: (AnimationStrip, number, number) -> ()
}
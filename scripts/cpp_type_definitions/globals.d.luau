export type PositionInstance = {
    x: number,
    y: number,
    z: number,
}

export type VelocityInstance = {
    x: number,
    y: number,
}

declare Components: {
    Position: (() -> PositionInstance),
    Velocity: (() -> VelocityInstance)
}

declare ECS: {
    create: () -> (number),

    hasPosition: (number) -> (boolean),
    getPosition: (number) -> PositionInstance,
    setPosition: (number, PositionInstance) -> (),
    removePosition: (number) -> (),

    hasVelocity: (number) -> (boolean),
    getVelocity: (number) -> VelocityInstance,
    setVelocity: (number, VelocityInstance) -> (),
    removeVelocity: (number) -> ()
}

declare Host: {
    printPosition: (PositionInstance) -> (),
    runCallback: ((string)->()) -> (),
    printAll: ({string}) -> (),
    printAllRAW: (...string) -> (),
}
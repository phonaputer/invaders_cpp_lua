export type PositionInstance = {
    x: number,
    y: number,
    z: number,
}

declare Components: {
    Position: (() -> PositionInstance) & {
        GetTypeID: () -> string,
    }
}

declare test: {
    printPosition: (PositionInstance) -> (),
    runCallback: ((string)->()) -> (),
    printAll: ({string}) -> (),
    printAllRAW: (...string) -> (),
}

declare ECS: {
    create: () -> (number),
    hasPosition: (number) -> (boolean),
    getPosition: (number) -> PositionInstance,
    setPosition: (number, PositionInstance) -> (),
    removePosition: (number) -> (),
}
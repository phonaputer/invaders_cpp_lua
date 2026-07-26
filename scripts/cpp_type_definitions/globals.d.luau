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
    print_from_cpp: (PositionInstance) -> (),
    type_check: (id: string) -> (),
}
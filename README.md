# Invaders C++ Lua

_No LLMs were used in the generation of the source code, documentation, or assets (images, audio) in this repository. However, during implementation LLMs (specifically, Google search "AI Mode") were used to search through C++ and library docs, as well as find relevant information for specific error messages._

_This means any crap code is entirely my own fault._ 😉

---

This is a knockoff of the classic Taito game _Space Invaders_ written in C++ and [Luau](https://luau.org/) using [SDL3](https://wiki.libsdl.org/SDL3/FrontPage), [SDL3 Mixer](https://wiki.libsdl.org/SDL3_mixer/FrontPage), and [EnTT](https://github.com/skypjack/entt).
Unit tests use [GoogleTest](https://google.github.io/googletest/).

It was written to learn about embedding a scripting language in a C++ application.
_Space Invaders_ was chosen as it is complex enough to try some patterns but simple enough to write in a short time.

### Motivation

I wrote this because I was intrigued by the concept of embedding a sandboxed scripting language in a host application to make the host extensible by untrusted 3rd parties.

I was familiar with Lua being used in this role by [Redis](https://redis.io/docs/latest/develop/programmability/eval-intro/) (written in C) and Project Zomboid (no idea what the host language is for them).
When reading further on this topic I found that Garry's Mod and Roblox also use Lua.
I haven't played either title, but I was aware that both aim to provide a smooth end user modding experience.
So this seemed to demonstrate the effectiveness of this architecture and Lua's place in it.

Finding that Roblox's "Luau" Lua variant provides both type checking & prioritizes sandboxing, I decided to write a C++ application embedding this dialect.

As a random sidenote, I found [an interesting blog article](https://www.birjob.com/blog/lua-5-5-luajit-luau-dialect-split-2026) about different Lua dialects on an Azerbaijani jobs website of all places.

### Impressions

I wouldn't say I've made something super moddable - but I'd say I can see the seeds of something like that.

The current setup makes the ECS and all components available to Luau & expects a 0-arg function named `main` in the `./scripts/invasion/init.luau` file, which it invokes on entry into the invasion scene.
Some core game logic is "hardcoded" in C++ - primarily systems for synchronized movement of the invaders & player tank movement.
But apart from that, everything is written in Luau and is therefore "moddable" in some sense.

I would say that this setup does indeed provide faster development since Luau is quite easy to write.
However since I was developing both the Luau scripts & C++ host at the same time I found myself needing to dip into C++ regularly to create missing components which slowed things down.

Notably, it may be that Space Invaders is not a great project for taking advantage of this setup.
For example, Space Invaders does not have multiple levels or a large cast of characters to create opportunities to reuse shared components.
If I did have a project with lots of content I suspect I would have got much more mileage out of each C++ component I wrote.
And therefore I would have spent much more time with Luau than C++ - maximizing the benefits of the split architecture.
For example, if I wrote a dialog component in C++ and the game was an RPG with 10s or 100s of characters the ratio of C++ dev time to Luau dev time would be very different.

One positive for this particular project is that the "tweakability" of Luau on top of C++ is quite nice.
Since you can modify anything in Luau on-demand with no waiting (i.e. compiling) experimentation is fast and painless - which encourages a lot more of it.
The simple and dynamic syntax of Luau also adds to this effect.

If I were to build further on this project I would add components to C++ which provide more experimentation options.
For example, hooks to add new kinds of invaders, powerups, etc.
I think this would maximize the "experimentability" benefits of the split architecture.

Another thing I'm curious about is code generation.
C++ templates are nice but it would be great to be able to generate both LuaBridge bindings & Luau type definitions from my C++ component files.

### Build & Run

This project uses [CMake](https://cmake.org/).
Presets for debug & release builds may be found in `CMakePresets.json`.

After building with CMake, unit tests may be run with the following command:

```
ctest --output-on-failure
```

And (on Linux) the game may be run with the following command:

```
./game
```

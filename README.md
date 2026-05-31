# Battleship

> [Русская версия](README_RU.md)

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.12%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)


Console bot for [Battleship](https://en.wikipedia.org/wiki/Battleship_(game)) over stdin/stdout. Supports master/slave roles, configurable grid size and ship counts, and two shot strategies: ordered sweep and hunt-and-target. Written as part of a C++ course at ITMO University.

Two roles: **master** sets game parameters, **slave** receives them. All interaction is via a line-based text protocol.

## Build

```bash
git clone https://github.com/notakeith/battleship-bot.git
cd battleship-bot
cmake -B build && cmake --build build
ctest --test-dir build
```

Requirements: CMake ≥ 3.12, GCC 13+ or Clang 17+.

## Protocol

All commands and responses are lowercase strings separated by `\n`.

| Command | Response | Description |
|---|---|---|
| `ping` | `pong` | connection check |
| `exit` | `ok` | terminate program |
| `create master` | `ok` | master mode (fixed 5×4 grid, 3 four-cell ships) |
| `create slave` | `ok` | slave mode (accepts parameters from master) |
| `start` | `ok` | start game, place ships |
| `stop` | `ok` | stop current game |
| `set width N` | `ok` | set grid width |
| `get width` | `N` | get grid width |
| `set height N` | `ok` | set grid height |
| `get height` | `N` | get grid height |
| `set count [1-4] N` | `ok` | set ship count for type 1–4 |
| `get count [1-4]` | `N` | get ship count for type 1–4 |
| `set strategy ordered` | `ok` | strategy: sequential sweep |
| `set strategy custom` | `ok` | strategy: hunt-and-target |
| `shot X Y` | `hit` / `miss` / `kill` | receive shot on own grid |
| `shot` | `X Y` | request next shot coordinates |
| `set result [miss,hit,kill]` | — | report result of last shot |
| `finished` | `yes` / `no` | is the game over? |
| `win` | `yes` / `no` | did we win? |
| `lose` | `yes` / `no` | did we lose? |
| `dump PATH` | `ok` | save grid to file |
| `load PATH` | `ok` | load grid from file |

> After each `shot` (request), `set result` must be sent before the next `shot`.

## File format (dump/load)

```
5 20
4 v 0 0
4 v 2 0
4 v 4 0
```

First line: `width height`. Each following line: `size direction x y`, where direction is `h` (horizontal) or `v` (vertical).

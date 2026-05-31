# Battleship

> [English version](README.md)

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.12%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)


Консольный бот для игры в [морской бой](https://ru.wikipedia.org/wiki/%D0%9C%D0%BE%D1%80%D1%81%D0%BA%D0%BE%D0%B9_%D0%B1%D0%BE%D0%B9_(%D0%B8%D0%B3%D1%80%D0%B0)) через stdin/stdout. Поддерживает режимы master/slave, произвольный размер поля и количество кораблей, две стратегии стрельбы: последовательный обход и охота с добиванием. Написан в рамках курса по C++ в ИТМО.

Два режима: **master** задаёт параметры игры, **slave** их принимает. Взаимодействие — текстовый протокол через строки.

## Сборка

```bash
git clone https://github.com/notakeith/battleship-bot.git
cd battleship-bot
cmake -B build && cmake --build build
ctest --test-dir build
```

Требования: CMake ≥ 3.12, GCC 13+ или Clang 17+.

## Протокол

Все команды и ответы — строки в нижнем регистре, разделены `\n`.

| Команда | Ответ | Описание |
|---|---|---|
| `ping` | `pong` | проверка связи |
| `exit` | `ok` | завершить программу |
| `create master` | `ok` | режим master (фиксированное поле 5×4, 3 четырёхпалубных) |
| `create slave` | `ok` | режим slave (принимает параметры извне) |
| `start` | `ok` | начать партию, расставить корабли |
| `stop` | `ok` | остановить партию |
| `set width N` | `ok` | задать ширину поля |
| `get width` | `N` | получить ширину поля |
| `set height N` | `ok` | задать высоту поля |
| `get height` | `N` | получить высоту поля |
| `set count [1-4] N` | `ok` | задать количество кораблей типа 1–4 |
| `get count [1-4]` | `N` | получить количество кораблей типа 1–4 |
| `set strategy ordered` | `ok` | стратегия: последовательный обход |
| `set strategy custom` | `ok` | стратегия: охота с добиванием |
| `shot X Y` | `hit` / `miss` / `kill` | принять выстрел по своему полю |
| `shot` | `X Y` | запросить координаты следующего выстрела |
| `set result [miss,hit,kill]` | — | сообщить результат последнего выстрела |
| `finished` | `yes` / `no` | партия завершена? |
| `win` | `yes` / `no` | победа? |
| `lose` | `yes` / `no` | поражение? |
| `dump PATH` | `ok` | сохранить поле в файл |
| `load PATH` | `ok` | загрузить поле из файла |

> После каждой команды `shot` (запрос выстрела) необходимо передать `set result` перед следующим `shot`.

## Формат файла (dump/load)

```
5 20
4 v 0 0
4 v 2 0
4 v 4 0
```

Первая строка — `width height`. Далее строки вида `size direction x y`, где `direction` — `h` (горизонталь) или `v` (вертикаль).

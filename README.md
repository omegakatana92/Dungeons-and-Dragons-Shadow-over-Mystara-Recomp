# Dungeons & Dragons: Shadow over Mystara — Recomp

A native PC port of **Dungeons & Dragons: Shadow over Mystara**, the 1996 Capcom arcade
beat 'em up (the second D&D game on the CP System II board). Its 68000 and Z80 programs are
translated ahead of time into C++ by [cps2recomp](https://github.com/tmcleroy/cps2recomp),
and run on a small machine written for it — no emulator, and no game data in this repository.

This is a work in progress.

The port reads your own **Dungeons & Dragons: Shadow over Mystara** ROM set (the MAME set
`ddsom`, Europe 960619) — nothing else is needed, and the set's key is used on the fly, as the
board does.

Not affiliated with, endorsed by or sponsored by Capcom or Wizards of the Coast.
*Dungeons & Dragons* is a trademark of Wizards of the Coast; the arcade game is a Capcom work.

## Features

With every feature off the port runs exactly as the board does. From the toolkit (no game-specific
work):

- The **launcher**: choose the ROM set (checked file by file by CRC-32), set the controls, Play.
- The **in-game menu** (Esc or `F1`; a stick's click, or Start+Select): save states (nine slots),
  Reset, Insert coin, Player 2 joins, Quit game.
- **Stop, step, slow** (`P`, `O`, `-`/`=`).
- **Run-ahead** and the **Display** options (scaling, picture shape, window size, full screen).
- Every key and button **remappable**, for each player.
- The board's 384×224 picture, its QSound music and effects, its settings EEPROM and its cipher.

The game's own features (`games/ddsom/src/`), from addresses found on the running board and recorded
in [games/ddsom/PROGRESS.md](games/ddsom/PROGRESS.md):

| Feature | Category | What it does |
| --- | --- | --- |
| Free play | Game | A coin goes in by itself, so Start works without one. |
| No damage | Cheats | Every player's health is put back to its maximum. |
| Infinite items | Cheats | Every player's six item slots stay full. |
| Infinite keys | Cheats | The party's keys never run out. |

`--list-features` prints the whole list. More is planned; see [ROADMAP.md](ROADMAP.md).
Ready-made builds for Windows, Linux and the Steam Deck go on the
[Releases](../../releases) page.

**A note on players:** the arcade board is a **4-player** game, but cps2recomp's frontend maps
the first two players (as it does for every CPS-2 port). Players 3 and 4 are not available yet;
that is the largest piece of work left (see ROADMAP.md).

## Layout

| Path | What |
| --- | --- |
| `cps2recomp/` | A copy of the toolkit (`v0.3.0`), made by its `tools/sync-toolkit.sh`. Never change it here. |
| `games/ddsom/game.cfg` | The ROM set's files and CRC-32s, the launcher's words, and the game's settings. |
| `games/ddsom/src/` | The game's own C++: the optional features on top of the board. |
| `games/ddsom/PROGRESS.md` | The game's memory as it is found, and how each address was found. |
| `games/ddsom/movies/` | Small input movies used to drive the board while finding things out. No game data. |
| `ROADMAP.md` | The features still to build and what each needs. |
| `rom/` | Your ROM set: a folder of its files or the set's zip. Not committed. |
| `build/`, `work/` | Build output and generated code. Not committed. |

## Prerequisites (MSYS2, MinGW64)

Run all commands from an **MSYS2 MinGW64** shell (so `gcc`/`g++`, SDL3 and zlib are found). Install
what is missing once:

```sh
pacman -S --needed base-devel make git diffutils \
  mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-SDL3 mingw-w64-x86_64-zlib
```

Python 3 comes with MSYS2. CMake must be on `PATH` (`which cmake` must succeed); the MinGW64 package
above puts it in `/mingw64/bin`. `make check` also needs `git` (the repository must be a git working
tree, since `check-common` asks git for the top level) and `diffutils` (for `cmp`).

On Linux (for the AppImage and the Steam Deck build), install a compiler, cmake, ninja, python3 and
SDL 3's build dependencies; `tools/release/linux.sh` lists the packages for Ubuntu.

### A space-free build path (Windows)

The toolkit passes `$(CURDIR)` to CMake unquoted in `cps2recomp/build.mk`, so a checkout under a
path with spaces (such as `Documents\Default Project`) does not configure. Use a drive letter:

```bat
subst G: "C:\path\to\Dungeons-and-Dragons-Shadow-over-Mystara-Recomp"
```

then build from `G:\` in the MinGW64 shell. (If another project is already using `G:`, pick a free
letter and use that; the point is the space-free path.)

## The ROM set

`ROM` points at a folder holding the set's files or at the set's zip. The default is `rom`, so put the
set's zip at `rom/ddsom.zip` (or the loose files in `rom/`). Files inside a zip are found by CRC-32,
whatever they are named, so they may sit in a subfolder. The set is only read, never changed.

The set this port is made from is MAME's `ddsom` (Europe 960619); `games/ddsom/game.cfg` lists its
files and checksums, and `games/ddsom/PROGRESS.md` says how it was told apart from the other
revisions. **Only this revision is accepted**: the port is translated from its program, and the
game's features use addresses found in it. The clones (`ddsoma`, `ddsomb`, `ddsomj`, `ddsomjr1`,
`ddsomr1`, `ddsomr2`, `ddsomr3`, `ddsomu`, `ddsomur1`) are built from other revisions and are not
accepted here.

## Building

```sh
make            # translate the game's code to C++ and link build/ddsom/ddsom(.exe)
make launcher   # build the launcher: build/ddsom/ddsom-launcher(.exe)
```

`make` reads the board's program out of the ROM set (above) and translates it, so the set must be in
place first. A fresh tree also needs `make explore` once, because it starts out knowing almost none of
the game's code:

```sh
make explore    # find the game's code by made-up play, in rounds (once, about ten minutes)
make            # translate again, now that the code is known
```

`make explore` plays the game headless with made-up input across several runs, records every place the
code was reached by a computed jump, and translates what it found before the next round. Its progress
is in the final run's log line, `instructions had no translation` — as low as possible is what you
want. For this port the final translated build leaves **0** 68000 instructions to the interpreter
(and 1 in the sound driver).

## Running it

The build makes two programs in `build/ddsom/`: the game, `ddsom.exe`, and the launcher,
`ddsom-launcher.exe`. Keep them beside each other.

- **The launcher** is the way in for a player: choose the ROM set (every file checked by CRC-32), set
  the controls, choose features, Play.
- **Double-clicking `ddsom.exe`** starts the launcher beside it — straight into the game if a ROM set
  was chosen before, otherwise at the launcher's first page.
- **From a shell:** `make run` runs the port windowed, with `rom/` and the settings file the Makefile
  names; pass options through with `ARGS=`, e.g. `make run ARGS=--fullscreen`. A game is started with
  a coin (Insert coin in the menu) or with Free play.

Feature choices come from `work/ddsom/settings.ini`; the EEPROM and the save states (`state0.bin` …)
sit beside it. `--help` lists every option (`--headless`, `--frames`, `--movie`, `--poke`, `--trace`,
`--dump-ram`, `--list-features`, `--cpu`, `--sound`, …).

**On Windows the two programs need the MinGW runtime beside them** — `SDL3.dll`, `zlib1.dll`,
`libiconv-2.dll`, `libstdc++-6.dll`, `libgcc_s_seh-1.dll`, `libwinpthread-1.dll`. `make` copies them
into `build/ddsom/` (the `deps` target), so the folder can be double-clicked or run from any shell.
Starting the programs from an **MSYS2 Mingw64** shell works as well, because that shell has
`/mingw64/bin` on `PATH`.

## Troubleshooting

- **"SDL3.dll was not found"**, or **"The procedure entry point … could not be located in the dynamic
  link library"** — the runtime DLLs are missing from beside the programs, or an older one from another
  toolchain is being picked up. Run `make deps`, which copies the ones this build needs out of
  `/mingw64/bin`; or launch from an MSYS2 MinGW64 shell.
- **`cmake: command not found`** — install `mingw-w64-x86_64-cmake` (Prerequisites); it lands in
  `/mingw64/bin`, which the MinGW64 shell has on `PATH`.
- **CMake says `Ignoring extra path from command line`** — the checkout path has spaces. Build from a
  `subst` drive as above.
- **The launcher refuses the ROM set** — it checks every file by CRC-32. This port wants MAME's
  `ddsom` (Europe 960619); other regions and revisions are refused on purpose.
- **The title still says "INSERT COIN"** — insert a coin from the in-game menu, or switch on
  **Free play**; the port does not change the board's coin settings.
- **Slow, or the log reports instructions left to the interpreter** — run `make explore` once, then
  `make` again.
- **No sound** — the sound program runs like the rest of the game; check the launcher's Sound row and
  that the set has its `sound_program` files (`dd2.01`, `dd2.02`).

## Checking

```sh
make check          # translated code against the interpreter, by state hash; two runs alike
make check-blocks   # every translated block beside the interpreter (registers, cycles, writes)
make check-toolkit  # cps2recomp/ is what its tools/sync-toolkit.sh recorded
make check-common   # recomp-common/ is what tools/sync-common.sh recorded
```

See `cps2recomp/build.mk` for the full list of targets.

## Releases

The release archives are made **by hand**, on a machine that has the ROM set: they hold the game's
translated code, so CI (which has no ROM set) never builds one. `tools/release/README.md` has the
steps. The Linux side is built on Ubuntu 22.04 (glibc 2.35) so that it runs on SteamOS 3.5 and later.

## Licence

GPL-3.0-only; see [LICENSE](LICENSE) and [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

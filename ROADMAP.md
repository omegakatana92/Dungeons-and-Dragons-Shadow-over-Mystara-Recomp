# Dungeons & Dragons: Shadow over Mystara — features and roadmap

Where this port stands against a finished one, and what each missing piece needs. The reference is the
toolkit's own complete example, [vampire-savior-recomp](https://github.com/tmcleroy/vampire-savior-recomp)
(`games/vampire_savior/src/`), and the toolkit's documents `docs/FEATURES.md` and `docs/FIGHT.md`.

Everything below is an **optional feature**: with every feature off, the port must run exactly as the
board does. Nothing from the game (no ROM, no RAM dump, no ripped art/text) may ever be committed — each
thing found is recorded in `PROGRESS.md` as *how* it was found (a RAM diff, a write watch, a trace),
never as the data itself.

## What works today

From the toolkit, with no game work:

- The **launcher**: chooses the ROM set, checks every file by CRC-32, sets the controls, Play.
- The **in-game menu** (Esc, `F1`, a stick's click, or Start+Select): save states (9 slots), Reset,
  Insert coin, Player 2 joins, Quit game.
- **Stop, step, slow** (P, O, `-`/`=`).
- **Run-ahead** and the **Display** options (scaling, picture shape, window size, full screen).
- Every key and button **remappable**, for each player.
- The board's QSound sound, settings EEPROM, cipher and 384×224 picture.

Built on the game's own memory (`games/ddsom/src/`):

- `free_play` (Enhanced) — a coin pulse goes in by itself, so Start works with no coin.
- `no_damage` — every active player's health is put back to its maximum each frame.
- `infinite_items` — every active player's six item slots are held at 9.
- `infinite_keys` — the party's key count is held at 9.

`--list-features` shows these with the toolkit's.

## What a finished port adds (this is the work)

### 0. Foundation — know the game's memory (started)

The four players' records (`FF8640`, `FF8840`, `FF8A40`, `FF8C40`), their health and maximum, their
item slots, the credits and the keys are found and named in `games/ddsom/src/ddsom.h`, with how each
was found in `PROGRESS.md`. Still missing:

- **the state word** (standing, walking, attacking, in the air, reeling) — the frame data, the dummy
  and the hit boxes all want it;
- **the attack boxes** themselves;
- **the stage/camera scroll** for a widescreen picture.

How the toolkit finds these (all in `docs/FEATURES.md`):

- `--dump-ram F:FILE` + `tools/ramfind.py DUMP=VALUE ...` — addresses that held a known value across
  several dumps.
- `--trace LOW-HIGH:FILE` with a run that does known things at known frames.
- `--watch LOW[-HIGH]` — every write to an address and the instruction that made it, read against
  `work/ddsom/program.lst`.
- `--poke ADDR=BYTE,...` to try a guess; `--menu FRAME:KEYS` to press the menu's own rows;
  `--shot-every N:PFX` to see a run.

### 1. Players 3 and 4 — the largest missing piece

The arcade board is a **4-player** game (`cps2_4p4b`). cps2recomp's frontend maps the first two
players: its `Machine` has `players[2]`, its bindings name players 1 and 2, and the launcher's
controllers page has two columns. To play the game as the board did, the toolkit would have to grow
two more players (input bits, bindings, launcher columns, HUD), then the game's sources would say how
the third and fourth records are used. This is the one thing that cannot be done inside
`games/ddsom/` alone, because it is a change to `cps2recomp/`.

### 2. A beat 'em up's training aids

The toolkit's `fight/` parts were written for fighting games, but the pieces that do not depend on a
round are reusable:

| Aid | Needs |
| --- | --- |
| Hit boxes | the state word and the attack boxes, in the game's own coordinates, plus `fight::DrawBoxes` |
| Frame data | the state word, the move id and the frame counter, as `fight::StepView` |
| Input display | the players' panel bits (already in the port) and each player's facing (the record's x and the other player's x) |
| Combo/readout | the state word and the hit counter |

`fight/colours.cpp` and the colour editor work already; a game only has to say where its palettes are.

### 3. Widescreen

The picture is 384×224, drawn from the board's own tilemaps and sprites. A wider picture needs the
camera's x and the two background scrollers found (a trace over a stage, as the fighting ports did),
then a `widescreen.cpp` using `Video::wide`. Nothing about it is started.

### 4. More of the game's own switches

The public cheat collection names the stage byte (`FF8034`, a list of stage ids in `PROGRESS.md`'s
source list), the spell/SP counters and the equipment bytes. Each becomes a feature only after it is
checked on the board and the right moment to write it is known; a wrong value in the character record
can leave the game in a bad state.

### 5. Readable rewrites

`docs/READABLE.md` describes replacing a game routine with ordinary C++ and checking it against the
original on every call. A rewrite of the damage rule would make `no_damage` cleaner than restoring the
health every frame, and a rewrite of the coin/credit check could give a true free-play switch.

## Order of work

1. The state word and the attack boxes (they unlock most of the training aids).
2. Players 3 and 4 (a toolkit change, then the game's sources).
3. Widescreen and the remaining switches.

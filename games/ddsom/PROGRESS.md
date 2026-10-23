# Dungeons & Dragons: Shadow over Mystara — progress

What is known of *Dungeons & Dragons: Shadow over Mystara* (Europe 960619)'s memory, and what has
been built on it. Every address here was confirmed by experiment on the running board with the
toolkit's own tools (`cps2recomp/docs/FEATURES.md`); several were first reached for from FinalBurn
Neo's public cheat collection for this same ROM set, which is said where it is, and then checked on
the board itself. Nothing is a dump of the game's data, and no game data is written down: only
addresses, values and what they were seen to do.

## The ROM set

The set is MAME's **`ddsom`** (Europe 960619), the parent of `ddsoma`, `ddsomb`, `ddsomj`,
`ddsomjr1`, `ddsomr1`, `ddsomr2`, `ddsomr3`, `ddsomu` and `ddsomur1`. Its files and CRC-32s are in
`game.cfg`; the board is `cps2_4p4b` (four players, four buttons), 384×224, and runs at the usual
CPS-2 speed.

- the program is eight 512 KiB ROMs — `dd2e.03e`, `dd2e.04e`, `dd2e.05e`, `dd2e.06e`, `dd2e.07`,
  `dd2e.08`, `dd2e.09`, `dd2e.10` — 4 MiB in all, so `code_end = 400000`;
- the graphics are two rows of four 8-bit files: `dd2.13m`, `dd2.15m`, `dd2.17m`, `dd2.19m`
  (4 MiB each), then `dd2.14m`, `dd2.16m`, `dd2.18m`, `dd2.20m` (2 MiB each) — 24 MiB in all;
- the sound program is `dd2.01`, `dd2.02`; the QSound samples are `dd2.11m`, `dd2.12m` (4 MiB);
- the key is `ddsom.key`.

Every file is checked by CRC-32, so only this revision is accepted.

## Finding the game's code

The tree started out knowing almost none of the game's code. The steps were the toolkit's:

1. `make` — the first build runs in the interpreter, dumps the deciphered program and translates the
   reset path.
2. `make explore` — made-up play in rounds, each round's computed jumps added to `work/ddsom/entries.txt`
   and translated before the next. The rounds found 4 835 entries on the first pass and 5 326 after
   the long one-player run and the test-switch run.
3. `make` — the final translation: **124 839 instructions in 30 217 blocks and 8 120 routines**,
   with 77 bytes left to the interpreter (45 of them data a guessed entry took for code).
4. A headless run of the final build (12 000 frames, `--stats`) reports **0 instructions had no
   translation** (about 0.000 % of the run) and 1 in the sound driver. That is the line to watch:
   it is as low as it goes without rewriting the last few bytes by hand.

The auto-play reaches the first stage on its own; the movies in `games/ddsom/movies/` were written
for the runs where a known place was wanted (a coin, then Start).

## How the game's own memory is found

The tools used, in the order they are reached for (all in `cps2recomp/docs/FEATURES.md`):

- `--auto SEED[:FROM[:N]]` drives the board with made-up input, including coins and Start.
- `--movie FILE` replays a hand-written input movie (a press and release of Coin, then Start).
- `--dump-ram F:FILE` writes the 64 KiB of work RAM after a frame; `--trace LOW-HIGH:FILE` appends a
  stretch of work RAM (or all of it) after every frame.
- `--poke ADDR=BYTE` holds a guess at a value every frame; `--shot-every N:PFX` writes pictures.
- `cps2recomp/tools/ramfind.py` and `tracechanges.py` compare dumps and traces.
- The public FinalBurn Neo cheat collection for `ddsom` (`ddsom.ini`) was used as a list of
  **candidates** only; each address below that came from it was checked on the running port.

**Getting into a game.** The title sequence starts at about frame 3000; a coin pressed at frame 3800
and a Start at 4200–5500 reaches the character select and then Stage 1 by about frame 6000. The
movie `movies/coins-start.txt` does exactly that. (A coin pressed during the boot, before the title,
is ignored; the coin input needs an edge after the title is up.)

**The credits.** A run with a coin at 3800 and 3900 was traced over the whole of work RAM (0–FFFF)
and the trace compared at frames 3700 (before), 3850 and 4200. `FF8454` was the byte that went
0 → 1 → 1 and then 1 → 2 as the second coin was processed. It is the counter the title screen
displays. Holding it at 9 in memory did **not** make the title accept Start, though: what the title
waits for is a coin **input** edge, not the counter alone. Free play therefore puts a real coin pulse
on player 1's Coin bit every two seconds; the counter follows, and Start works.

**The players' records.** Four records 0x200 bytes apart: `FF8640`, `FF8840`, `FF8A40`, `FF8C40`.
Each begins with a big-endian word of current health at +0 and the character's maximum at +2. In the
first stage player 1 read `0070`/`0070`; after a fight it read `0058`/`0070` — the current health
had fallen while the maximum stayed. Poking the low byte of the current health to 1
(`FF8641=01`) shortened player 1's bar, which fixed the address. The cheat collection names the same
byte for "Infinite Energy PL1"; the +2 word is the maximum and is what "No damage" restores.

**The item slots.** Six bytes at +0xE6 (`FF8726`–`FF872B` for player 1); the cheat collection holds
them at 9. With the feature on, all six read 9 at frame 8000 in a run.

**The keys.** A byte at `FF8045`; the cheat collection holds it at 9. With the feature on it reads 9
after the run.

**The invincibility byte.** The cheat collection holds the byte at +0x1C (`FF865C` for player 1) at 3
and calls it invincibility. Holding it there did **not** stop player 1 losing health in our runs
(0x58 at frame 8000, the same as with no feature), so it is recorded as a partial finding and no
feature uses it. The game may use it only for a particular kind of hit, or that cheat may be for a
different revision.

## The memory so far

| Address | Size | What | How it was found |
| --- | --- | --- | --- |
| `FF8454` | byte | **Credits**: the counter the title screen reads (0 with no coin, 1 after one, 2 after two) | Traced all of work RAM around a coin at frame 3800: the only byte that went 0 → 1 and stayed 1; a second coin took it to 2. Holding it alone did not let Start work. |
| `FF8640` (+`FF8840`, `FF8A40`, `FF8C40`) | four records, 0x200 apart | **The four players' records** | Health and items below. |
| `+0x00` (`FF8640`…) | word | **Current health** (big-endian) | `0070` at the start of Stage 1, `0058` after damage; poking the low byte to 1 shortened the bar. The cheat collection names the low byte. |
| `+0x02` (`FF8642`…) | word | **Maximum health** for this character | Stayed `0070` while current health fell to `0058`; what "No damage" restores. |
| `+0xE6` (`FF8726`…) | 6 bytes | **The six item slots** | The cheat collection's "Infinite Item" addresses; with the feature on all six read 9 at frame 8000. |
| `+0x1C` (`FF865C`…) | byte | **The cheat collection's "invincibility" byte** (partial finding) | Holding it at 3 did not stop damage in our runs; not used. |
| `FF8045` | byte | **Keys the party carries** | The cheat collection's address; with the feature on it reads 9 after a run. |

## Features built on it (`src/`)

| Feature | Category | What it does | Check |
| --- | --- | --- | --- |
| `free_play` | Game (Enhanced) | Puts a coin pulse on player 1 every two seconds, so the title accepts Start with no coin. | `movies/start-only.txt` + the feature reaches Stage 1 (screenshots at frames 6000 and 8000); without it the title stays on "INSERT COIN". |
| `no_damage` | Cheats | Every active player's current health is put back to its maximum word every frame. | At frame 8000 in an auto run with the feature, player 1 read `0070`/`0070` and player 2 `0060`/`0060`; without it they read `0058`/`0070` and `0060`/`0060`. |
| `infinite_items` | Cheats | The six item slots of every active player are held at 9. | All six read 9 at frame 8000 with the feature; 0 in the unused seats. |
| `infinite_keys` | Cheats | `FF8045` is held at 9. | Read 9 at frame 8000 with the feature. |

With every feature off, the game runs exactly as the board does; the features are off by default
(except free play, which the Enhanced preset switches on with the rest of the toolkit's enhanced set).

## Next

- **Players 3 and 4.** The board is a four-player game, but cps2recomp's frontend maps two players.
  Adding two more would need the toolkit's input, bindings and HUD to grow, not just the game's
  sources; it is the largest remaining piece.
- **The state word** (standing, walking, attacking, in the air, reeling) and the **attack boxes**,
  for hit boxes and frame data like the fighting ports' training aids. The toolkit's `fight/` parts
  want a `StepView`; this game's record layout has not been mapped far enough yet.
- **Widescreen**: the picture is drawn from the board's own 384 columns; a wider picture needs the
  camera and the stage scrollers found and a `widescreen.cpp` written.
- **Stage select** (the cheat collection's `FF8034`) as an in-game choice, once the timing at which
  the game reads it is known.
- A **rewrite of the damage rule** (`docs/READABLE.md`) would make "No damage" a cleaner feature than
  restoring the health every frame.

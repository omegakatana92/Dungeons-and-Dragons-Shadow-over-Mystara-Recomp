# Dungeons & Dragons: Shadow over Mystara Recomp — changelog

## 0.1.0-beta.1

First build. The 68000 and Z80 programs of *Dungeons & Dragons: Shadow over Mystara*
(Europe 960619) are translated to C++ ahead of time and run on cps2recomp's own machine. The
launcher, the in-game menu, save states, run-ahead, the display options and every control are
remappable.

The game's own features are in, from addresses found on the running board (each recorded in
[games/ddsom/PROGRESS.md](games/ddsom/PROGRESS.md) with how it was found):

- **Free play**, so Start works without a coin (a coin pulse goes in by itself);
- **No damage** for every active player;
- **Infinite items** and **infinite keys**.

The 4-player board is played with players 1 and 2 for now; players 3 and 4 need the toolkit's
frontend to grow two more players. See [ROADMAP.md](ROADMAP.md).

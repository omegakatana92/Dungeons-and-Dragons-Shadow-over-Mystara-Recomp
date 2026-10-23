Dungeons & Dragons: Shadow over Mystara - recomp port (Windows x64)
==================================================================

A native PC port of Dungeons & Dragons: Shadow over Mystara (Capcom, 1996, CPS-2 board),
made with tmcleroy's cps2recomp (https://github.com/tmcleroy/cps2recomp) by static
recompilation. NOT affiliated with, endorsed by or sponsored by Capcom or Wizards of the Coast.
Dungeons & Dragons is a trademark of Wizards of the Coast.

NO GAME FILES ARE INCLUDED. You need your own copy of the game: the arcade ROM set "ddsom"
(Europe, 960619), from a board you own. This archive cannot help you get one. Other regions and
revisions (ddsoma, ddsomb, ddsomj, ddsomjr1, ddsomr1, ddsomr2, ddsomr3, ddsomu, ddsomur1) do not
work and are refused on purpose.


How to play
-----------
1. Double-click ddsom-launcher.exe.
2. Choose your ROM set: point the launcher at the folder that holds the set's files, or at
   the set's .zip. It checks every file by CRC-32 before it will run.
3. Set the controls and any features you want, then Play.

In game: Esc or F1 opens the menu (save states, Insert coin, Player 2 joins, Reset, Quit).
P, O and -/= stop, step and slow the game. Everything is remappable in the launcher.

The arcade board is a 4-player game, but this port plays the first two players (the toolkit's
frontend has two). Players 3 and 4 are not available yet.

The game is a beat 'em up with a party of characters: one credit starts a one-player game; a
second player can join from the in-game menu's "Insert coin" and "Player 2 joins".


What is in here
---------------
ddsom.exe                the game
ddsom-launcher.exe       the launcher (keep it beside the game)
SDL3.dll                 \
zlib1.dll                 >  the runtime libraries the two programs need
libiconv-2.dll           /   (keep them all in this folder, beside the .exe files)
libgcc_s_seh-1.dll      /
libstdc++-6.dll        /
libwinpthread-1.dll   /
licenses/                the licences of the port and of everything shipped with it

Keep the .exe and .dll files together in one folder. Save states, settings and the board's
EEPROM are kept beside them (or wherever you run the game from).

The port is GPL-3.0; its source, and how it was made, are at
https://github.com/omegakatana92/Dungeons-and-Dragons-Shadow-over-Mystara-Recomp
You can also build it yourself from that source. To play, you still need your own ROM set.

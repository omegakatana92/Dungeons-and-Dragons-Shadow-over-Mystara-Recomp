// SPDX-License-Identifier: GPL-3.0-only
//
// Dungeons & Dragons: Shadow over Mystara: what the game's own sources add to the port (see PROGRESS.md).
// The features are put together here from the game's own memory (ddsom.h) and hooks into the game's code
// (runtime/lift.h).
#include "game.h"

namespace cps2recomp {

// (the game's own parts; each registers its features and state and is called from around a frame)
void RegisterCheats(); // cheats.cpp
void CheatsBeforeFrame(Machine &machine);
void CheatsFrame(Machine &machine);

namespace {
    void BeforeFrame(Machine &machine) {
        CheatsBeforeFrame(machine);
    }

    void Frame(Machine &machine) {
        CheatsFrame(machine);
    }
} // namespace

void RegisterGame() {
    RegisterCheats();
    TheGame().beforeFrame = BeforeFrame;
    TheGame().frame = Frame;
}

} // namespace cps2recomp

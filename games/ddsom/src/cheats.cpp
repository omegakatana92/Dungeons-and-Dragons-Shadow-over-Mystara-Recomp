// SPDX-License-Identifier: GPL-3.0-only
//
// Dungeons & Dragons: Shadow over Mystara: the optional features built on the game's own memory (ddsom.h).
// They are switched in the launcher and the in-game menu; with every feature off the game runs exactly
// as the board does. Each address was checked on the running board (PROGRESS.md).
#include "ddsom.h"

#include "game.h"
#include "machine.h"

namespace cps2recomp {

namespace {
    using namespace ddsom;

    // A player is in the game when its record holds health. An empty seat, the title screen and the
    // character select keep 0 there.
    bool Playing(uint32_t base) {
        return Ram16(base + kHealth) != 0;
    }

    // Free play: with no credit in, a coin is put in (a player's Coin button, as the board sees it), so
    // the title screen offers Start. A credit held in memory is not enough: the game's own coin input is
    // what it waits for, which is why the byte at FF8454 alone left "INSERT COIN" up.
    void BeforeFrame(Machine &machine) {
        // (a coin is a press and then a release: a bit held down forever is one edge at boot, which the
        // board's coin mechanism is not listening for yet. A pulse every two seconds is what the title
        // screen sees as a coin going in.)
        static uint32_t tick = 0;
        tick++;
        if (FeatureOn("free_play") && tick % 120 < 4) {
            machine.players[0] |= kCoin;
        }
    }

    void Frame(Machine &) {
        const bool noDamage = FeatureOn("no_damage");
        const bool infiniteItems = FeatureOn("infinite_items");
        for (int n = 0; n < 4; n++) {
            const uint32_t base = kPlayerBase[n];
            if (!Playing(base)) {
                continue;
            }
            if (noDamage) {
                // Put the health back to the most this character can have (the word at +2), so the
                // hit is undone before the next frame is drawn. A beaten player (0) is left alone.
                const uint16_t most = Ram16(base + kHealthMax);
                if (most != 0) {
                    SetRam16(base + kHealth, most);
                }
            }
            if (infiniteItems) {
                for (uint32_t i = 0; i < kItemCount; i++) {
                    SetRam8(base + kItems + i, 9);
                }
            }
        }
        // (the keys are the party's, not a player's)
        if (FeatureOn("infinite_keys")) {
            SetRam8(kKeys, 9);
        }
    }
} // namespace

void CheatsBeforeFrame(Machine &machine) {
    BeforeFrame(machine);
}

void CheatsFrame(Machine &machine) {
    Frame(machine);
}

void RegisterCheats() {
    Game &game = TheGame();
    game.features.push_back({"free_play", "Game", "Free play", "Start works without a coin.", kFeatureEnhanced});
    game.features.push_back({"no_damage", "Cheats", "No damage", "Every player's health is put back to its maximum."});
    game.features.push_back({"infinite_items", "Cheats", "Infinite items", "Every player's six item slots stay full."});
    game.features.push_back({"infinite_keys", "Cheats", "Infinite keys", "The party's keys never run out."});
}

} // namespace cps2recomp

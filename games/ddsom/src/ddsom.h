// SPDX-License-Identifier: GPL-3.0-only
//
// Dungeons & Dragons: Shadow over Mystara: the game's own memory, as it was found on the running
// board (see PROGRESS.md). Nothing here is a dump of the game's data: each is an address and what it
// was seen to do.
#pragma once

#include <cstdint>

namespace cps2recomp::ddsom {

// The four players' records, 0x200 bytes apart. A character's record starts at FF8640 (player 1),
// FF8840 (player 2), FF8A40 (player 3) and FF8C40 (player 4).
inline constexpr uint32_t kPlayerBase[4] = {0xFF8640, 0xFF8840, 0xFF8A40, 0xFF8C40};

// The record's health: a big-endian word at +0, and the most the character can have at +2. Player 1's
// are FF8640/FF8642. (In the running game the current health was 0x70 at the start of the first stage,
// 0x58 after hits, while the maximum stayed 0x70.)
inline constexpr uint32_t kHealth = 0x00;
inline constexpr uint32_t kHealthMax = 0x02;

// A byte at +0x1C (FF865C for player 1) that the cheat collections hold at 3 and call invincibility.
// Holding it there did not stop player 1 losing health in our runs (0x58 at frame 8000, the same as with
// the feature off), so no feature uses it; it is recorded here as a partial finding, not used.
inline constexpr uint32_t kInvincible = 0x1C;

// The item counts at +0xE6 .. +0xEB (FF8726..FF872B for player 1), six bytes, one per item slot.
inline constexpr uint32_t kItems = 0xE6;
inline constexpr uint32_t kItemCount = 6;

// The credits, counted by the title screen: 0 with no coin in, 1 after a coin. (A run that pressed Coin
// at frame 3800 was traced over the whole of work RAM; FF8454 was the only byte that went 0 -> 1 and
// stayed 1, and it is the byte MAME's collection calls the credits for this board.)
inline constexpr uint32_t kCredits = 0xFF8454;

// The keys the party carries: a byte the cheat collections hold at 9.
inline constexpr uint32_t kKeys = 0xFF8045;

} // namespace cps2recomp::ddsom

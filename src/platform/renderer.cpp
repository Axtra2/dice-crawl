#include <ftxui/dom/elements.hpp>

#include <platform/renderer.hpp>
#include <game/character/player.hpp>
#include <game/direction.hpp>
#include <game/item.hpp>
#include <game/map.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>

Renderer::Renderer(void* data)
  : data_(data)
{ }

static void drawRoom(
    int32_t topLeftX,
    int32_t topLeftY,
    const Map& map,
    ftxui::Canvas& canvas
);

static ftxui::Element drawHUD(
    const Room& room,
    const Player& player,
    int32_t selectedInventorySlot
);

void Renderer::render(
    const Map& map,
    const Player& player,
    int32_t selectedInventorySlot
) {
    using namespace ftxui;

    auto s = Terminal::Size();
    auto c = Canvas((s.dimx * 2) / 2 /* make space for HUD */, s.dimy * 4);
    int32_t centerX = (c.width() / 2) / 2;
    int32_t centerY = (c.height() / 4) / 2;
    const auto& room = map.currentRoom();
    drawRoom(
        centerX - room.getPlayerX(),
        centerY - room.getPlayerY(),
        map,
        c
    );
    c.DrawText(c.width() / 2, c.height() / 2, "x");
    auto hud = drawHUD(room, player, selectedInventorySlot);
    new(data_) Element(hbox({canvas(std::move(c)), separator(), hud}));
}


struct Rectangle {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

Rectangle intersectRectangles(
    int32_t left1,
    int32_t top1,
    int32_t right1,
    int32_t bottom1,

    int32_t left2,
    int32_t top2,
    int32_t right2,
    int32_t bottom2
) {
    assert(left1 <= right1);
    assert(top1 <= bottom1);
    assert(left2 <= right2);
    assert(top2 <= bottom2);

    Rectangle res = {
        .left = std::max(left1, left2),
        .top = std::max(top1, top2),
        .right = std::min(right1, right2),
        .bottom = std::min(bottom1, bottom2),
    };

    if (res.right - res.left < 0 ||
        res.bottom - res.top < 0
    ) {
        return Rectangle { 0, 0, 0, 0 };
    }
    return res;
}


static void drawAt(ftxui::Canvas& canvas, int32_t x, int32_t y, ftxui::Color color) {
    x *= 2;
    y *= 4;
    canvas.DrawBlock(x + 0, y + 0, true, color);
    canvas.DrawBlock(x + 1, y + 0, true, color);
    canvas.DrawBlock(x + 0, y + 1, true, color);
    canvas.DrawBlock(x + 1, y + 1, true, color);
    canvas.DrawBlock(x + 0, y + 2, true, color);
    canvas.DrawBlock(x + 1, y + 2, true, color);
    canvas.DrawBlock(x + 0, y + 3, true, color);
    canvas.DrawBlock(x + 1, y + 3, true, color);
}

static void drawAt(
    ftxui::Canvas& canvas,
    int32_t x,
    int32_t y,
    const std::string& text
) {
    canvas.DrawText(x * 2, y * 4, text);
}

static void drawAt(
    ftxui::Canvas& canvas,
    int32_t x,
    int32_t y,
    const std::string& text,
    ftxui::Color color
) {
    canvas.DrawText(x * 2, y * 4, text, color);
}

static void drawRoom(
    int32_t topLeftX,
    int32_t topLeftY,
    const Map& map,
    ftxui::Canvas& canvas
) {
    const auto& room = map.currentRoom();
    auto rect = intersectRectangles(
        topLeftX - 1, // -1 for border
        topLeftY - 1,
        topLeftX + room.getWidth() - 1 + 1, // +1 for border
        topLeftY + room.getHeight() - 1 + 1,

        0,
        0,
        (canvas.width() / 2) - 1,
        (canvas.height() / 4) - 1
    );

    // tiles
    for (const auto& [k, v] : room.getTiles()) {
        int32_t x = k.first + topLeftX;
        int32_t y = k.second + topLeftY;
        if (x >= rect.left && x <= rect.right &&
            y >= rect.top  && y <= rect.bottom
        ) {
            switch (v) {
            case '#':
                drawAt(canvas, x, y, ftxui::Color::White);
                break;
            default:
                break;
            }
        }
    }

    // items
    for (const auto& [k, v] : room.getItems()) {
        int32_t x = k.first + topLeftX;
        int32_t y = k.second + topLeftY;
        if (x >= rect.left && x <= rect.right &&
            y >= rect.top  && y <= rect.bottom
        ) {
            switch (v) {
            case 1: // sword
                drawAt(canvas, x, y, ftxui::Color::Blue);
                break;
            case 2: // helmet
                drawAt(canvas, x, y, ftxui::Color::Purple);
                break;
            default:
                break;
            }
        }
    }

    // mobs
    for (auto& [mobID, mob] : room.getMobs()) {
        int32_t x = mob.getX() + topLeftX;
        int32_t y = mob.getY() + topLeftY;
        if (x >= rect.left && x <= rect.right &&
            y >= rect.top  && y <= rect.bottom
        ) {
            using namespace std::string_literals;
            if (mob.isDead()) {
                drawAt(canvas, x, y, "X"s);
                continue;
            }
            auto icon = mob.getStrategyName().substr(0, 1);
            using ftxui::Color;
            Color color = Color::White;
            switch (icon.front()) {
            case 'H': color = Color::Red; break;
            case 'P': color = Color::White; break;
            case 'C': color = Color::Yellow; break;
            }
            drawAt(canvas, x, y, icon, color);
        }
    }

    using enum Direction;
    // left border
    if (topLeftX - 1 >= rect.left && topLeftX - 1 <= rect.right) {
        for (
            int32_t y = std::max(rect.top, topLeftY - 1);
            y <= std::min(rect.bottom, topLeftY + room.getHeight());
            ++y
        ) {
            if (
                !map.currentRoomHasNeighbour(WEST) ||
                !room.isDoor(-1, y - topLeftY)
            ) {
                drawAt(canvas, topLeftX - 1, y, ftxui::Color::Red);
            }
        }
    }

    // top border
    if (topLeftY - 1 >= rect.top && topLeftY - 1 <= rect.bottom) {
        for (
            int32_t x = std::max(rect.left, topLeftX - 1);
            x <= std::min(rect.right, topLeftX + room.getWidth());
            ++x
        ) {
            if (
                !map.currentRoomHasNeighbour(NORTH) ||
                !room.isDoor(x - topLeftX, -1)
            ) {
                drawAt(canvas, x, topLeftY - 1, ftxui::Color::Red);
            }
        }
    }

    // right border
    if (
        topLeftX + room.getWidth() >= rect.left &&
        topLeftX + room.getWidth() <= rect.right
    ) {
        for (
            int32_t y = std::max(rect.top, topLeftY - 1);
            y <= std::min(rect.bottom, topLeftY + room.getHeight());
            ++y
        ) {
            if (
                !map.currentRoomHasNeighbour(EAST) ||
                !room.isDoor(room.getWidth(), y - topLeftY)
            ) {
                drawAt(canvas, topLeftX + room.getWidth(), y, ftxui::Color::Red);
            }
        }
    }

    // bottom border
    if (
        topLeftY + room.getHeight() >= rect.top &&
        topLeftY + room.getHeight() <= rect.bottom
    ) {
        for (
            int32_t x = std::max(rect.left, topLeftX - 1);
            x <= std::min(rect.right, topLeftX + room.getWidth());
            ++x
        ) {
            if (
                !map.currentRoomHasNeighbour(SOUTH) ||
                !room.isDoor(x - topLeftX, room.getHeight())
            ) {
                drawAt(canvas, x, topLeftY + room.getHeight(), ftxui::Color::Red);
            }
        }
    }
}

static ftxui::Element drawHUD(
    const Room& room,
    const Player& player,
    int32_t selectedInventorySlot
) {
    // health / maxHealth | attack
    // standing on (item)
    // Equipped:
    // Hand: Head: Torso: Legs: Feet:
    // inventory

    using namespace std::string_literals;
    using namespace ftxui;

    auto& itemsDict = getItemsDict();

    std::stringstream ss;
    ss << "Health: " << player.getHealth() << " / " << player.getMaxHealth()
       << " | Base Attack Dice:";
    for (const auto& v : player.getBaseAttackDice()) {
        ss << " " << v.second;
    }



    auto getItemStr = [&](int32_t itemID) {
        const auto& item = itemsDict.at(itemID);
        std::string res = item.getName();
        res += " |";
        if (item.hasAttackDice()) {
            for (const auto& [_, desc] : item.getAttackDice()) {
                res += " " + desc;
            }
        }
        if (item.hasDefenseDice()) {
            res += " |";
            for (const auto& [_, desc] : item.getDefenseDice()) {
                res += " " + desc;
            }
        }
        return res;
    };

    std::string onGroundStr = "Nothing";
    auto it = room.getItems().find({room.getPlayerX(), room.getPlayerY()});
    if (it != room.getItems().end()) {
        onGroundStr = getItemStr(it->second); // itemsDict.at(it->second).getName();
    }

    using enum Player::EquipmentSlot;
    static const std::unordered_map<Player::EquipmentSlot, std::string> eqSlotToName = {
        { HAND, "Hand" },
        { HEAD, "Head" },
        { TORSO, "Torso" },
        { LEGS, "Legs" },
        { FEET, "Feet" }
    };

    auto vb = vbox({
        text(ss.str()),
        separator(),
        text("On ground: " + onGroundStr),
        separator(),
        text("Equipped:")
    });

    for (int32_t i = 0; auto& eqSlot : player.getEquipment()) {
        vb = vbox({
            vb,
            text(
                (i == selectedInventorySlot ?  " -> "s : "    "s) +
                eqSlotToName.at(static_cast<Player::EquipmentSlot>(i)) +
                ": "s +
                (eqSlot.has_value() ? getItemStr(eqSlot.value()) : ""s)
            )
        });
        ++i;
    }

    vb = vbox({
        vb,
        separator(),
        text("Inventory:")
    });
    for (
        int32_t i = static_cast<int32_t>(N_WEAR_TYPES);
        auto slot : player.getInventory()
    ) {
        std::string prefix = i == selectedInventorySlot ? " -> "s : "    "s;
        if (!slot) {
            vb = vbox({vb, text(prefix)});
        } else {
            auto itemStr = getItemStr(slot.value());
            if (i == selectedInventorySlot) {
                vb = vbox({vb, text(prefix + itemStr)});
            } else {
                vb = vbox({vb, text(prefix + itemStr)});
            }
        }
        ++i;
    }
    return vbox({vb, separator()});
}


void Renderer::render(
    const std::string& title,
    int32_t selectedOption,
    const std::span<std::string>& options
) {
    using namespace ftxui;

    Element e = vbox({text(title), separator()});
    for (int32_t i = 0; const auto& op : options) {
        if (i == selectedOption) {
            e = vbox({e, text(" -> " + op)});
        } else {
            e = vbox({e, text("    " + op)});
        }
        ++i;
    }
    new(data_) Element(std::move(e));
}

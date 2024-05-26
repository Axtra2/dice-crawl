#include <ftxui/dom/elements.hpp>

#include <renderer.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>

Renderer::Renderer(void* data)
  : data_(data)
{ }

static void drawRoom(
    int32_t topLeftX,
    int32_t topLeftY,
    const Room& room,
    ftxui::Canvas& canvas
);

static ftxui::Element drawHUD(
    const Room& room,
    const Player& player,
    const std::unordered_map<int32_t, ItemInfo>& itemsDict
);

bool Renderer::render(
    const Room& room,
    const Player& player,
    const std::unordered_map<int32_t, TileInfo>& tilesDict,
    const std::unordered_map<int32_t, ItemInfo>& itemsDict
) {
    using namespace ftxui;

    auto c = Canvas(100, 100);
    int32_t centerX = (c.width() / 2) / 2;
    int32_t centerY = (c.height() / 4) / 2;
    drawRoom(
        centerX - player.x,
        centerY - player.y,
        room,
        c
    );
    c.DrawText(c.width() / 2, c.height() / 2, "x");
    auto hud = drawHUD(room, player, itemsDict);
    new(data_) Element(hbox({canvas(std::move(c)), separator(), hud}));
    return true;
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

static void drawRoom(
    int32_t topLeftX,
    int32_t topLeftY,
    const Room& room,
    ftxui::Canvas& canvas
) {
    auto rect = intersectRectangles(
        topLeftX - 1, // -1 for border
        topLeftY - 1,
        topLeftX + room.width - 1 + 1, // +1 for border
        topLeftY + room.height - 1 + 1,

        0,
        0,
        (canvas.width() / 2) - 1,
        (canvas.height() / 4) - 1
    );

    // tiles
    for (auto& [k, v] : room.tiles) {
        int32_t x = k.first + topLeftX;
        int32_t y = k.second + topLeftY;
        if (x >= rect.left && x <= rect.right &&
            y >= rect.top  && y <= rect.bottom
        ) {
            switch (v)
            {
            case '#':
                drawAt(canvas, x, y, ftxui::Color::White);
                break;
            default:
                break;
            }
        }
    }

    // items
    for (auto& [k, v] : room.items) {
        int32_t x = k.first + topLeftX;
        int32_t y = k.second + topLeftY;
        if (x >= rect.left && x <= rect.right &&
            y >= rect.top  && y <= rect.bottom
        ) {
            switch (v)
            {
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

    // left border
    if (topLeftX - 1 >= rect.left && topLeftX - 1 <= rect.right) {
        for (
            int32_t y = std::max(rect.top, topLeftY - 1);
            y <= std::min(rect.bottom, topLeftY + room.height);
            ++y
        ) {
            if (!room.w || y != topLeftY + room.height / 2) {
                drawAt(canvas, topLeftX - 1, y, ftxui::Color::Red);
            }
        }
    }

    // top border
    if (topLeftY - 1 >= rect.top && topLeftY - 1 <= rect.bottom) {
        for (
            int32_t x = std::max(rect.left, topLeftX - 1);
            x <= std::min(rect.right, topLeftX + room.width);
            ++x
        ) {
            if (!room.n || x != topLeftX + room.width / 2) {
                drawAt(canvas, x, topLeftY - 1, ftxui::Color::Red);
            }
        }
    }

    // right border
    if (topLeftX + room.width >= rect.left && topLeftX + room.width <= rect.right) {
        for (
            int32_t y = std::max(rect.top, topLeftY - 1);
            y <= std::min(rect.bottom, topLeftY + room.height);
            ++y
        ) {
            if (!room.e || y != topLeftY + room.height / 2) {
                drawAt(canvas, topLeftX + room.width, y, ftxui::Color::Red);
            }
        }
    }

    // bottom border
    if (topLeftY + room.height >= rect.top && topLeftY + room.height <= rect.bottom) {
        for (
            int32_t x = std::max(rect.left, topLeftX - 1);
            x <= std::min(rect.right, topLeftX + room.width);
            ++x
        ) {
            if (!room.s || x != topLeftX + room.width / 2) {
                drawAt(canvas, x, topLeftY + room.height, ftxui::Color::Red);
            }
        }
    }
}

static ftxui::Element drawHUD(
    const Room& room,
    const Player& player,
    const std::unordered_map<int32_t, ItemInfo>& itemsDict
) {
    // health / maxHealth | attack
    // standing on (item)
    // Equipped:
    // Hand: Head: Torso: Legs: Feet:
    // inventory

    using namespace std::string_literals;
    using namespace ftxui;

    std::stringstream ss;
    ss << "Health: " << player.health << " / " << player.calcMaxHealth()
       << " | Attack: " << player.calcAttack();


    std::string onGroundStr = "Nothing";
    auto it = room.items.find({player.x, player.y});
    if (it != room.items.end()) {
        onGroundStr = itemsDict.at(it->second).name;
    }

    auto nameOrNothing = [&](const auto& v){
        return v ? itemsDict.at(v.value()).name : "Nothing"s;
    };

    std::string handStr = nameOrNothing(player.hand);
    std::string headStr = nameOrNothing(player.head);
    std::string torsoStr = nameOrNothing(player.torso);
    std::string legsStr = nameOrNothing(player.legs);
    std::string feetStr = nameOrNothing(player.feet);

    auto vb = vbox({
        text(ss.str()),
        separator(),
        text("On ground: " + onGroundStr),
        separator(),
        text("Equipped:"),
        text((player.selectedInventorySlot == 0 ? " ->"s : "   "s) + " Hand:  " + handStr),
        text((player.selectedInventorySlot == 1 ? " ->"s : "   "s) + " Head:  " + headStr),
        text((player.selectedInventorySlot == 2 ? " ->"s : "   "s) + " Torso: " + torsoStr),
        text((player.selectedInventorySlot == 3 ? " ->"s : "   "s) + " Legs:  " + legsStr),
        text((player.selectedInventorySlot == 4 ? " ->"s : "   "s) + " Feet:  " + feetStr),
        separator(),
        text("Inventory:")
    });
    for (int32_t i = 5; auto item : player.inventory) {
        if (i == player.selectedInventorySlot) {
            vb = vbox({vb, text(" -> " + itemsDict.at(item).name)});
        } else {
            vb = vbox({vb, text("    " + itemsDict.at(item).name)});
        }
        ++i;
    }
    return vb;
}


bool Renderer::render(
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
    return true;
}
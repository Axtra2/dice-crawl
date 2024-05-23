#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <game.h>

#include <cstdint>
#include <random>

std::mt19937 rng(std::random_device{}());

using namespace ftxui;

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


void drawAt(Canvas& canvas, int32_t x, int32_t y, Color color) {
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

void drawRoom(
    int32_t topLeftX,
    int32_t topLeftY,
    Canvas& canvas,
    const Room& room
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
                drawAt(canvas, x, y, Color::White);
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
            case 1:
                drawAt(canvas, x, y, Color::Blue);
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
                drawAt(canvas, topLeftX - 1, y, Color::Red);
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
                drawAt(canvas, x, topLeftY - 1, Color::Red);
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
                drawAt(canvas, topLeftX + room.width, y, Color::Red);
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
            if (!room.w || x != topLeftX + room.width / 2) {
                drawAt(canvas, x, topLeftY + room.height, Color::Red);
            }
        }
    }

}

int main(void) {
    using namespace std::string_literals;

    auto room = generateRoom(21, 21, rng);
    Player player;
    player.x = room.width / 2;
    player.y = room.height - 1;

    int mouse_x = 0;
    int mouse_y = 0;

    auto renderer = Renderer([&] {
        auto c = Canvas(100, 100);
        int32_t centerX = (c.width() / 2) / 2;
        int32_t centerY = (c.height() / 4) / 2;
        drawRoom(centerX - player.x, centerY - player.y, c, room);
        c.DrawText(c.width() / 2, c.height() / 2, "x");
        auto attack = player.attack;
        if (player.hand) {
            auto m = getItemsDict()
                .at(player.hand.value())
                .wearInfo.value()
                .attackModifier;
            if (m) {
                attack = m.value()(attack);
            }
        }
        auto atck_str = std::to_string(attack);
        auto vb = vbox({text("attack: " + atck_str)});
        for (auto item : player.inventory) {
            vb = vbox({vb, text(getItemsDict().at(item).name)});
        }
        return hbox({canvas(std::move(c)), separator(), vb});
    });

    auto renderer_with_events = CatchEvent(renderer, [&](Event e){
        if (e.is_mouse()) {
            mouse_x = e.mouse().x * 2;
            mouse_y = e.mouse().y * 4;
        }
        std::string input = e.character();
        if (input.size() != 0) {
            int32_t oldPx = player.x;
            int32_t oldPy = player.y;
            switch (input.back()) {
            case 'w':
                player.y -= 1;
                break;
            case 'a':
                player.x -= 1;
                break;
            case 's':
                player.y += 1;
                break;
            case 'd':
                player.x += 1;
                break;
            case 'p': {
                // item pickup
                auto it = room.items.find({player.x, player.y});
                if (it != room.items.end()) {
                    player.inventory.push_back(it->second);
                    room.items.erase(it);
                }
                break;
            }
            case 'e':
                // item equip
                if (!player.inventory.empty()) {
                    ItemID itemID = player.inventory.front();
                    const ItemInfo& itemInfo = getItemsDict().at(itemID);
                    if (itemInfo.wearInfo) {
                        using enum ItemInfo::WearInfo::WearType;
                        switch (itemInfo.wearInfo.value().wearType) {
                        case HAND:
                            if (!player.hand) {
                                player.inventory.erase(player.inventory.begin());
                                player.hand = itemID;
                            }
                            break;
                        case HEAD:
                            break;
                        case TORSO:
                            break;
                        case LEGS:
                            break;
                        case FEET:
                            break;
                        }
                    }
                }
            }
            player.x = std::clamp(player.x, 0, room.width - 1);
            player.y = std::clamp(player.y, 0, room.height - 1);
            auto it = room.tiles.find({player.x, player.y});
            if (it != room.tiles.end() && it->second == '#') {
                player.x = oldPx;
                player.y = oldPy;
            }
        }
        return false;
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer_with_events);

    return EXIT_SUCCESS;
}

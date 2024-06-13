#include <game/program_mode/game_command.hpp>
#include <game/character/mob/confused.hpp>
#include <game/character/player.hpp>
#include <game/program.hpp>
#include <game/map.hpp>

#include <utils/inrange.hpp>

MoveCommand::MoveCommand(Map& map, Player& player, Direction dir)
  : map_(map),
    player_(player),
    dir_(dir)
{ }

bool MoveCommand::operator()() {
    return player_.move(map_, dir_);
}

UseItemCommand::UseItemCommand(
    Map& map,
    Player& player,
    int32_t selectedInventorySlot
) : map_(map),
    player_(player),
    selectedInventorySlot_(selectedInventorySlot)
{ }

bool UseItemCommand::operator()() {
    using enum ItemInfo::WearInfo::WearType;
    int32_t nEqSlots = static_cast<int32_t>(N_WEAR_TYPES);
    if (selectedInventorySlot_ < nEqSlots) {
        return player_.unequip(
            static_cast<Player::EquipmentSlot>(selectedInventorySlot_)
        );
    }

    int32_t i = selectedInventorySlot_ - nEqSlots;
    auto item = player_.getInventory()[i];
    if (item.has_value() && item.value() == 3) {
        // TODO: remove bomb from inventory
        // TODO: move this code into Player::use(Room&, slotID)
        for (auto& mob : map_.currentRoom().mobs()) {
            // TODO: maybe add dynamic type check to not add multiple
            // layers of confusion
            mob.reset(new Confused(std::move(mob)));
        }
        return true;
    } else {
        return player_.equip(i);
    }
}

PickUpCommand::PickUpCommand(Map& map, Player& player)
  : map_(map), player_(player)
{ }

bool PickUpCommand::operator()() {
    return player_.pickUp(map_.currentRoom());
}

EscCommand::EscCommand(Program& program)
  : program_(program)
{ }

bool EscCommand::operator()() {
    program_.toEscMenu();
    program_.init();
    return true;
}

MoveInventorySelectionCommand::MoveInventorySelectionCommand(int32_t& selectedInventorySlot, int32_t delta)
  : selectedInventorySlot_(selectedInventorySlot),
    delta_(delta)
{ }

bool MoveInventorySelectionCommand::operator()() {
    return addInRange(
        selectedInventorySlot_,
        delta_,
        0,
        Player::N_EQ_SLOTS + Player::N_INV_SLOTS
    );
}

UpdateOnSuccessCommand::UpdateOnSuccessCommand(
    Map& map,
    Player& player,
    Program& program,
    std::unique_ptr<GameCommand> before
) : map_(map),
    player_(player),
    program_(program),
    before_(std::move(before))
{ }

bool UpdateOnSuccessCommand::operator()() {
    if (!before_ || !before_->operator()()) {
        return false;
    }
    map_.currentRoom().updateMobs();
    if (player_.isDead()) {
        program_.toGameOver();
        program_.init();
    }
    assert(player_.getLevel() >= 0);
    if (player_.getXP() >= (1ull << player_.getLevel())) {
        program_.toLevelUp(player_);
        program_.init();
    }
    return true;
}

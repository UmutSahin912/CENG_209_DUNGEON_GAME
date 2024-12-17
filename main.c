#include "game.h"

int main() {
    srand(time(NULL));

    Player player;
    player.health = 100;
    player.maxHealth = 100;
    player.strength = 20;
    player.defense = 5;
    player.attackSpeed = 5;
    player.inventoryCount = 0;
    player.gameFinished = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        player.inventory[i] = NULL;
    }

    player.baseStrength = player.strength;
    player.baseDefense = player.defense;
    player.flameWeaponActive = 0;
    player.attackPotionCounter = 0;
    player.defensePotionCounter = 0;

    initializeRooms(rooms);
    player.currentRoom = rooms[0];
    player.checkPointRoom = rooms[0];

    gameLoop(&player);

    // Bellek temizliği
    for (int i = 0; i < NUM_ROOMS; i++) {
        if (rooms[i]) {
            for (int j = 0; j < rooms[i]->itemCount; j++) {
                if (rooms[i]->items[j]) {
                    free(rooms[i]->items[j]);
                }
            }
            if (rooms[i]->creature) free(rooms[i]->creature);
            free(rooms[i]);
        }
    }

    for (int i = 0; i < player.inventoryCount; i++) {
        free(player.inventory[i]);
    }

    return 0;
}

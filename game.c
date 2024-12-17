#include "game.h"

// Global rooms array
Room *rooms[NUM_ROOMS];

// Sabit oda açıklamaları, eşyalar, yaratıklar
static const char *hardcodedRoomDescs[NUM_ROOMS] = {
    "A dimly lit room with moss on the walls.",
    "A narrow corridor with dripping water.",
    "A large chamber decorated with strange symbols.",
    "A bright room with a high ceiling and old frescoes.",
    "A cavernous hall with echoes of distant screams.",
    "A musty storage room filled with broken crates.",
    "A small dusty library filled with cobwebs.",
    "A flooded room where you step in ankle-deep water.",
    "A chamber with walls covered in ancient runes.",
    "A dark room with an eerie silence.",
    "A cramped space that smells of sulfur.",
    "A grand hall with a colossal statue at its center."
};

static Item hardcodedItems[NUM_ITEM_TYPES] = {
    {"Health Potion", "Restores 50 HP.", 1},
    {"Attack Potion", "Increases your strength temporarily (2 rooms).", 2},
    {"Defense Potion", "Reduces incoming damage temporarily (2 rooms).", 3},
    {"Revive Stone", "Revives you if you die, restoring 30% health.", 4},
    {"Flame Weapon", "Greatly increases your attack until you leave the room.", 5},
    {"Zensu Beans", "Permanently increases your max health by 40.", 6}
};

static Creature hardcodedNormalCreatures[NUM_NORMAL_CREATURES] = {
    {"Goblin", 60, 10, 0, 0},
    {"Spider", 40, 20, 0, 0}
};

static Creature hardcodedBossCreatures[NUM_BOSS_CREATURES] = {
    {"Master Mantis", 120, 30, 1, 3},
    {"Giant Serpent", 140, 35, 1, 6},
    {"Kraken", 160, 40, 1, 9},
    {"Dragonoid", 180, 45, 1, 12}
};

// Rastgele sayı üretir [min,max]
int randomBetween(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Yeni item oluşturma
Item *createItem(const char *name, const char *description, int effectType) {
    Item *item = (Item *)malloc(sizeof(Item));
    strncpy(item->name, name, MAX_NAME_LENGTH);
    strncpy(item->description, description, MAX_DESC_LENGTH);
    item->effectType = effectType;
    return item;
}

// Boss yaratma
Creature *createBoss(int roomID) {
    Creature *c = (Creature *)malloc(sizeof(Creature));
    c->isBoss = 1;
    c->bossRoomID = roomID; 

    for (int i = 0; i < NUM_BOSS_CREATURES; i++) {
        if (hardcodedBossCreatures[i].isBoss == 1 && hardcodedBossCreatures[i].bossRoomID == roomID) {
            strncpy(c->name, hardcodedBossCreatures[i].name, MAX_NAME_LENGTH);
            c->health = hardcodedBossCreatures[i].health;
            c->attackPower = hardcodedBossCreatures[i].attackPower;
            c->isBoss = hardcodedBossCreatures[i].isBoss;
            c->bossRoomID = hardcodedBossCreatures[i].bossRoomID;
            return c;
        }
    }

    strncpy(c->name, "Unknown Boss", MAX_NAME_LENGTH);
    c->health = 100;
    c->attackPower = 20;
    c->isBoss = 1;
    c->bossRoomID = roomID;
    return c;
}

// Normal yaratık oluşturma
Creature *createNormalCreature() {
    Creature *c = (Creature *)malloc(sizeof(Creature));
    c->isBoss = 0;
    int idx = rand() % NUM_NORMAL_CREATURES;
    strncpy(c->name, hardcodedNormalCreatures[idx].name, MAX_NAME_LENGTH);
    c->health = hardcodedNormalCreatures[idx].health;
    c->attackPower = hardcodedNormalCreatures[idx].attackPower;
    c->bossRoomID = 0; 
    return c;
}

// Oda oluşturma
Room *createRoom(int roomID) {
    Room *r = (Room *)malloc(sizeof(Room));
    r->roomID = roomID;
    r->itemCount = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        r->items[i] = NULL;
    }

    r->isBossRoom = (roomID == 3 || roomID == 6 || roomID == 9 || roomID == 12);
    r->trapCount = randomBetween(0,3);

    if (r->isBossRoom) {
        r->creature = createBoss(roomID);
    } else {
        r->creature = createNormalCreature();
    }

    int itemCount = randomBetween(1,3);
    for (int i = 0; i < itemCount; i++) {
        int idx = randomBetween(0,NUM_ITEM_TYPES-1);
        r->items[i] = createItem(hardcodedItems[idx].name, hardcodedItems[idx].description, hardcodedItems[idx].effectType);
        r->itemCount++;
    }

    r->up = r->down = r->left = r->right = NULL;

    char tempDesc[MAX_DESC_LENGTH];
    snprintf(tempDesc, MAX_DESC_LENGTH, "You are in Room %d. %s", roomID, hardcodedRoomDescs[roomID-1]);
    strncpy(r->description, tempDesc, MAX_DESC_LENGTH);
    r->description[MAX_DESC_LENGTH - 1] = '\0';

    return r;
}

// Odaları bağla
void connectRooms(Room **rooms) {
    rooms[0]->right = rooms[1]; rooms[1]->left = rooms[0]; rooms[1]->right = rooms[2];
    rooms[2]->left = rooms[1]; rooms[2]->right = rooms[3]; rooms[3]->left = rooms[2];

    rooms[7]->right = rooms[6]; rooms[6]->left = rooms[7]; rooms[6]->right = rooms[5];
    rooms[5]->left = rooms[6]; rooms[5]->right = rooms[4]; rooms[4]->left = rooms[5];

    rooms[8]->right = rooms[9]; rooms[9]->left = rooms[8]; rooms[9]->right = rooms[10];
    rooms[10]->left = rooms[9]; rooms[10]->right = rooms[11]; rooms[11]->left = rooms[10];

    rooms[0]->down = rooms[7]; rooms[7]->up = rooms[0];
    rooms[1]->down = rooms[6]; rooms[6]->up = rooms[1];
    rooms[2]->down = rooms[5]; rooms[5]->up = rooms[2];
    rooms[3]->down = rooms[4]; rooms[4]->up = rooms[3];

    rooms[7]->down = rooms[8]; rooms[8]->up = rooms[7];
    rooms[6]->down = rooms[9]; rooms[9]->up = rooms[6];
    rooms[5]->down = rooms[10]; rooms[10]->up = rooms[5];
    rooms[4]->down = rooms[11]; rooms[11]->up = rooms[4];
}

// Odaları başlat
void initializeRooms(Room **rooms) {
    for (int i = 0; i < NUM_ROOMS; i++) {
        rooms[i] = createRoom(i+1);
    }
    connectRooms(rooms);
}

// Flame weapon sıfırla
void resetFlameWeapon(Player *player) {
    if (player->flameWeaponActive) {
        player->strength = player->baseStrength;
        player->flameWeaponActive = 0;
    }
}

// İksir sürelerini güncelle
void updatePotionsOnRoomChange(Player *player) {
    if (player->attackPotionCounter > 0) {
        player->attackPotionCounter--;
        if (player->attackPotionCounter == 0) {
            player->strength = player->baseStrength;
        }
    }
    if (player->defensePotionCounter > 0) {
        player->defensePotionCounter--;
        if (player->defensePotionCounter == 0) {
            player->defense = player->baseDefense;
        }
    }
}

// Revive stone var mı?
int hasReviveStone(Player *player) {
    for (int i = 0; i < player->inventoryCount; i++) {
        if (player->inventory[i]->effectType == 4) return i;
    }
    return -1;
}

// Oyuncuyu dirilt
void revivePlayer(Player *player) {
    int idx = hasReviveStone(player);
    if (idx != -1) {
        printf("Your Revive Stone activates!\n");
        player->health = (int)(player->maxHealth * 0.3);
        if (player->health < 0) player->health = 0; 
        free(player->inventory[idx]);
        for (int i = idx; i < player->inventoryCount - 1; i++) {
            player->inventory[i] = player->inventory[i+1];
        }
        player->inventoryCount--;
        printf("You are revived with %d health.\n", player->health);
    } else {
        player->health = player->maxHealth;
        player->currentRoom = player->checkPointRoom; 
        printf("You died. Respawning at checkpoint with full health.\n");
    }
}

// Look sonrası tuzak tetikle
void triggerTrapsAfterLook(Player *player) {
    Room *r = player->currentRoom;
    for (int i = 0; i < r->trapCount; i++) {
        if (rand() % 2 == 0) {
            player->health -= 10;
            if (player->health < 0) player->health = 0;
            printf("A trap hit you for 10 damage! Health: %d\n", player->health);
            if (player->health <= 0) {
                printf("You died from a trap!\n");
                revivePlayer(player);
                if (player->health <= 0) return;
            }
        }
    }
}

// Oyuncuyu hareket ettir
void movePlayer(Player *player, char *direction) {
    Room *curr = player->currentRoom;
    Room *next = NULL;

    if (strcmp(direction, "up") == 0) {
        next = curr->up;
    } else if (strcmp(direction, "down") == 0) {
        next = curr->down;
    } else if (strcmp(direction, "left") == 0) {
        next = curr->left;
    } else if (strcmp(direction, "right") == 0) {
        next = curr->right;
    } else {
        printf("Invalid direction.\n");
        return;
    }

    if (next) {
        resetFlameWeapon(player);
        updatePotionsOnRoomChange(player);
        player->currentRoom = next;
        printf("You move to room %d.\n", next->roomID);
    } else {
        printf("You cannot move in that direction.\n");
    }
}

// look komutu
void look(Player *player) {
    Room *r = player->currentRoom;
    printf("%s\n", r->description);

    printf("Exits: ");
    int exitsCount = 0;
    if (r->up) { printf("[Up] "); exitsCount++; }
    if (r->down) { printf("[Down] "); exitsCount++; }
    if (r->left) { printf("[Left] "); exitsCount++; }
    if (r->right) { printf("[Right] "); exitsCount++; }
    if (exitsCount == 0) printf("None");
    printf("\n");

    if (r->itemCount > 0) {
        printf("Items in this room:\n");
        for (int i = 0; i < r->itemCount; i++) {
            if (r->items[i]) {
                printf("- %s: %s\n", r->items[i]->name, r->items[i]->description);
            }
        }
    } else {
        printf("No items in this room.\n");
    }

    if (r->creature) {
        printf("There is a creature: %s (HP:%d, ATK:%d)%s\n", 
            r->creature->name, r->creature->health, r->creature->attackPower,
            r->isBossRoom?" [BOSS]":"");
    } else {
        printf("No creatures here.\n");
    }

    triggerTrapsAfterLook(player);
}

// Envanteri göster
void showInventory(Player *player) {
    printf("Your inventory:\n");
    for (int i = 0; i < player->inventoryCount; i++) {
        printf("- %s: %s\n", player->inventory[i]->name, player->inventory[i]->description);
    }
    if (player->inventoryCount == 0) {
        printf("Empty.\n");
    }
}

// Eşya al
void pickupItem(Player *player, char *itemName) {
    Room *r = player->currentRoom;
    for (int i = 0; i < r->itemCount; i++) {
        if (r->items[i] && strcasecmp(r->items[i]->name, itemName) == 0) {
            if (player->inventoryCount < MAX_INVENTORY) {
                player->inventory[player->inventoryCount++] = r->items[i];
                printf("You picked up %s.\n", r->items[i]->name);
                for (int j = i; j < r->itemCount - 1; j++) {
                    r->items[j] = r->items[j+1];
                }
                r->items[r->itemCount - 1] = NULL;
                r->itemCount--;
                return;
            } else {
                printf("Your inventory is full.\n");
                return;
            }
        }
    }
    printf("No such item in this room.\n");
}

// Eşya kullan
void useItem(Player *player, char *itemName) {
    int found = -1;
    for (int i = 0; i < player->inventoryCount; i++) {
        if (strcasecmp(player->inventory[i]->name, itemName) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("You don't have that item.\n");
        return;
    }

    Item *it = player->inventory[found];
    switch (it->effectType) {
        case 1:
            player->health += 50;
            if (player->health > player->maxHealth) player->health = player->maxHealth;
            printf("You used a Health Potion. Health: %d\n", player->health);
            break;
        case 2:
            player->strength += 10;
            player->attackPotionCounter = 2;
            printf("You used an Attack Potion. Strength: %d (2 rooms)\n", player->strength);
            break;
        case 3:
            player->defense += 5;
            player->defensePotionCounter = 2;
            printf("You used a Defense Potion. Defense: %d (2 rooms)\n", player->defense);
            break;
        case 4:
            printf("Revive Stone is passive.\n");
            return; 
        case 5:
            player->strength += 20;
            player->flameWeaponActive = 1;
            printf("You used a Flame Weapon. Strength: %d (until leave this room)\n", player->strength);
            break;
        case 6:
            player->maxHealth += 40;
            player->health += 40;
            if (player->health > player->maxHealth) player->health = player->maxHealth;
            printf("You used Zensu Beans! Max health: %d, Health: %d\n", player->maxHealth, player->health);
            break;
    }

    if (it->effectType != 4) {
        free(it);
        for (int i = found; i < player->inventoryCount - 1; i++) {
            player->inventory[i] = player->inventory[i+1];
        }
        player->inventoryCount--;
    }
}

// Oyunu bitir
void finishGame(Player *player) {
    player->gameFinished = 1;
    printf("YOU FINISHED THIS GAME, CONGRATS!!!!\n");
    showFinalStats(player);
}

// Final statları göster
void showFinalStats(Player *player) {
    printf("Final Health: %d\n", player->health);
    printf("Final Strength: %d\n", player->strength);
    printf("Final Inventory:\n");
    for (int i = 0; i < player->inventoryCount; i++) {
        printf("- %s\n", player->inventory[i]->name);
    }
    if (player->inventoryCount == 0) {
        printf("Empty.\n");
    }
}

// Yaratığa saldır
void attackCreature(Player *player) {
    Room *r = player->currentRoom;
    if (!r->creature) {
        printf("No creature to attack.\n");
        return;
    }

    Creature *c = r->creature;
    printf("You attack the %s!\n", c->name);

    c->health -= player->strength;
    if (c->health < 0) c->health = 0; 
    printf("%s's health: %d\n", c->name, c->health);

    if (c->health <= 0) {
        printf("You defeated the %s!\n", c->name);
        if (c->isBoss && r->roomID == 12) {
            finishGame(player);
            return;
        } else if (c->isBoss) {
            if (player->inventoryCount < MAX_INVENTORY) {
                Item *bossDrop = createItem("Boss Relic", "A powerful artifact from the defeated boss.", 2);
                player->inventory[player->inventoryCount++] = bossDrop;
                printf("Boss dropped a Boss Relic! You picked it up.\n");
            } else {
                printf("Boss dropped an item but your inventory is full.\n");
            }
        }
        free(c);
        r->creature = NULL;
        return;
    }

    int damage = c->attackPower - player->defense;
    if (damage < 1) damage = 1; 
    player->health -= damage;
    if (player->health < 0) player->health = 0;
    printf("%s attacks you for %d damage. Health: %d\n", c->name, damage, player->health);

    if (player->health <= 0) {
        printf("You died!\n");
        revivePlayer(player);
    }
}

// Saldırı komutu
void attack(Player *player) {
    attackCreature(player);
}

// Oyunu kaydet
void saveGame(Player *player, const char *filepath) {
    // Kaydetmeden önce checkpoint'i currentRoom'a set et
    player->checkPointRoom = player->currentRoom;

    FILE *f = fopen(filepath, "w");
    if (!f) {
        printf("Failed to save the game.\n");
        return;
    }

    Room *r = player->currentRoom;
    fprintf(f, "ENVIRONMENT SNAPSHOT:\n");
    fprintf(f, "Current Room: %d\n", r->roomID);
    fprintf(f, "Description: %s\n", r->description);
    fprintf(f, "Traps: %d\n", r->trapCount);

    if (r->itemCount > 0) {
        fprintf(f, "Items in room:\n");
        for (int i = 0; i < r->itemCount; i++) {
            if (r->items[i]) {
                fprintf(f, "- %s: %s (EffectType %d)\n", r->items[i]->name, r->items[i]->description, r->items[i]->effectType);
            }
        }
    } else {
        fprintf(f, "No items in this room.\n");
    }

    if (r->creature) {
        fprintf(f, "Creature: %s (HP:%d, ATK:%d)%s\n",
            r->creature->name, r->creature->health, r->creature->attackPower,
            r->isBossRoom?" [BOSS]":"");
    } else {
        fprintf(f, "No creature here.\n");
    }

    fprintf(f, "PLAYER STATS:\n");
    fprintf(f, "Health: %d\n", player->health);
    fprintf(f, "Strength: %d\n", player->strength);

    if (player->inventoryCount > 0) {
        fprintf(f, "Player Inventory:\n");
        for (int i = 0; i < player->inventoryCount; i++) {
            fprintf(f, "- %s: %s (EffectType %d)\n", player->inventory[i]->name, player->inventory[i]->description, player->inventory[i]->effectType);
        }
    } else {
        fprintf(f, "Inventory is empty.\n");
    }

    fprintf(f, "\nSAVEGAME DATA (used for load):\n");
    fprintf(f, "%d %d %d %d %d\n", player->health, player->maxHealth, player->strength, player->defense, player->attackSpeed);
    fprintf(f, "%d\n", player->currentRoom->roomID);
    fprintf(f, "%d\n", player->inventoryCount);
    for (int i = 0; i < player->inventoryCount; i++) {
        fprintf(f, "\"%s\" %d\n", player->inventory[i]->name, player->inventory[i]->effectType);
    }
    fprintf(f, "%d\n", player->checkPointRoom->roomID);
    fprintf(f, "%d %d %d %d %d\n", player->baseStrength, player->baseDefense, player->flameWeaponActive, player->attackPotionCounter, player->defensePotionCounter);

    fclose(f);
    printf("Game saved to %s.\n", filepath);
}

// Oyunu yükle
void loadGame(Player *player, const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) {
        printf("Failed to load the game from %s.\n", filepath);
        return;
    }

    char line[512];
    int foundSave = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "SAVEGAME")) {
            foundSave = 1;
            break;
        }
    }

    if (!foundSave) {
        printf("No save data found in %s.\n", filepath);
        fclose(f);
        return;
    }

    fscanf(f, "%d %d %d %d %d\n", &player->health, &player->maxHealth, &player->strength, &player->defense, &player->attackSpeed);
    int currentRoomID;
    fscanf(f, "%d\n", &currentRoomID);
    player->currentRoom = rooms[currentRoomID - 1];

    int invCount;
    fscanf(f, "%d\n", &invCount);
    player->inventoryCount = invCount;
    for (int i = 0; i < invCount; i++) {
        char itemNameWithQuotes[MAX_NAME_LENGTH];
        int effType;
        fscanf(f, " \"%[^\"]\" %d\n", itemNameWithQuotes, &effType);
        player->inventory[i] = createItem(itemNameWithQuotes, "Loaded item from save.", effType);
    }

    int checkID;
    fscanf(f, "%d\n", &checkID);
    player->checkPointRoom = rooms[checkID - 1];

    fscanf(f, "%d %d %d %d %d\n", &player->baseStrength, &player->baseDefense, &player->flameWeaponActive, &player->attackPotionCounter, &player->defensePotionCounter);

    fclose(f);
    printf("Game loaded from %s.\n", filepath);
}

// help komutu
void showHelp() {
    printf("Commands:\n");
    printf("- move <direction>: Move (up, down, left, right).\n");
    printf("- look: Look around the current room.\n");
    printf("- inventory: Show your inventory.\n");
    printf("- pickup <item>: Pick up an item.\n");
    printf("- use <item>: Use an item.\n");
    printf("- discard <item>: Discard an item.\n");
    printf("- attack: Attack the creature.\n");
    printf("- info: Show your stats.\n");
    printf("- save: Will ask for a filename (e.g. Umut1.txt) and save.\n");
    printf("- load: Will ask for a filename (e.g. Umut1.txt) and load.\n");
    printf("- quit: Quit.\n");
}

// info komutu
void info(Player *player) {
    printf("Your health: %d\n", player->health);
    printf("Your strength: %d\n", player->strength);
}

// discard komutu
void discardItem(Player *player, char *itemName) {
    int found = -1;
    for (int i = 0; i < player->inventoryCount; i++) {
        if (strcasecmp(player->inventory[i]->name, itemName) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("You don't have that item.\n");
        return;
    }

    free(player->inventory[found]);
    for (int i = found; i < player->inventoryCount - 1; i++) {
        player->inventory[i] = player->inventory[i+1];
    }
    player->inventoryCount--;
    printf("You discarded %s.\n", itemName);
}

// Oyun döngüsü
void gameLoop(Player *player) {
    char command[MAX_COMMAND_LENGTH];
    printf("Welcome to the Dungeon Adventure Game!\n");
    printf("Type 'help' for commands.\n");

    while (1) {
        printf("\n> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "help") == 0) {
            showHelp();
        } else if (strncmp(command, "move", 4) == 0) {
            movePlayer(player, command+5);
        } else if (strcmp(command, "look") == 0) {
            look(player);
        } else if (strcmp(command, "inventory") == 0) {
            showInventory(player);
        } else if (strncmp(command, "pickup", 6) == 0) {
            pickupItem(player, command+7);
        } else if (strncmp(command, "use", 3) == 0) {
            useItem(player, command+4);
        } else if (strncmp(command, "discard", 7) == 0) {
            discardItem(player, command+8);
        } else if (strcmp(command, "attack") == 0) {
            attack(player);
        } else if (strcmp(command, "info") == 0) {
            info(player);
        } else if (strcmp(command, "save") == 0) {
            // Save alırken checkpoint room = currentRoom
            player->checkPointRoom = player->currentRoom;
            printf("Enter filename to save (e.g. Umut1.txt): ");
            char filename[64];
            fgets(filename, 64, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            saveGame(player, filename);
        } else if (strcmp(command, "load") == 0) {
            printf("Enter filename to load (e.g. Umut1.txt): ");
            char filename[64];
            fgets(filename, 64, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            loadGame(player, filename);
        } else if (strcmp(command, "quit") == 0) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid command.\n");
        }

        if (player->gameFinished) {
            break;
        }

        if (player->health <= 0) {
            printf("Game Over!\n");
            break;
        }
    }
}

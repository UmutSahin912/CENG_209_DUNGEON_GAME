#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Sabitler
#define MAX_INVENTORY 10
#define MAX_DESC_LENGTH 256
#define MAX_NAME_LENGTH 32
#define MAX_ITEMS 3
#define NUM_ROOMS 12
#define MAX_COMMAND_LENGTH 64
#define NUM_ITEM_TYPES 6
#define NUM_NORMAL_CREATURES 2
#define NUM_BOSS_CREATURES 4

typedef struct Item {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    int effectType;
} Item;

typedef struct Creature {
    char name[MAX_NAME_LENGTH];
    int health;
    int attackPower;
    int isBoss; 
    int bossRoomID;
} Creature;

typedef struct Room {
    char description[MAX_DESC_LENGTH];
    int roomID;
    Item *items[MAX_ITEMS];
    int itemCount;
    Creature *creature;
    int isBossRoom;
    int trapCount;
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
} Room;

typedef struct Player {
    int health;
    int maxHealth;
    int strength;
    int defense;
    int attackSpeed;
    int inventoryCount;
    Item *inventory[MAX_INVENTORY];
    Room *currentRoom;
    Room *checkPointRoom;

    int baseStrength;
    int baseDefense;
    int flameWeaponActive; 
    int attackPotionCounter; 
    int defensePotionCounter;

    int gameFinished; 
} Player;

// Global rooms array
extern Room *rooms[NUM_ROOMS];

// Fonksiyon prototipleri
int randomBetween(int min, int max);
void initializeRooms(Room **rooms);
void connectRooms(Room **rooms);
Item *createItem(const char *name, const char *description, int effectType);
Creature *createNormalCreature();
Creature *createBoss(int roomID);
Room *createRoom(int roomID);

void movePlayer(Player *player, char *direction);
void look(Player *player);
void showInventory(Player *player);
void pickupItem(Player *player, char *itemName);
void useItem(Player *player, char *itemName);
void attackCreature(Player *player);
void attack(Player *player);
void saveGame(Player *player, const char *filepath); 
void loadGame(Player *player, const char *filepath);
void showHelp();
void gameLoop(Player *player);
int hasReviveStone(Player *player);
void revivePlayer(Player *player);
void resetFlameWeapon(Player *player);
void updatePotionsOnRoomChange(Player *player);
void info(Player *player);
void discardItem(Player *player, char *itemName);
void triggerTrapsAfterLook(Player *player);
void finishGame(Player *player);
void showFinalStats(Player *player);

#endif // GAME_H

##README
##Dungeon Adventure Game

##Introduction
This project is a text-based adventure game set in a fictional dungeon environment. The player starts in a single room and explores a network of interconnected rooms, each containing items, creatures, and traps. The player can navigate, fight enemies (including bosses), pick up and use various items, and ultimately aims to reach and defeat the final boss to finish the game. This README provides a comprehensive guide on how the game works, how to compile and run it, and details about its design and implementation.

##Key Features
Exploration & Navigation:
Move through a network of NUM_ROOMS (12 by default), arranged both horizontally and vertically.
Use directional commands (move up/down/left/right) to travel and discover new areas.
Each room comes with unique descriptions and possible traps.

##Room Contents:
Rooms can contain items, creatures, and traps.
Boss rooms exist at specific room IDs (3, 6, 9, and 12), each with a unique boss creature.
Items are randomly distributed, providing replay value.

##Combat Mechanics:
Engage with creatures found in the rooms.
Attack them with the attack command. If the creature survives, it will retaliate.
Defeating boss creatures is necessary to progress toward the final goal.

##Items and Inventory Management:
Collect and carry up to MAX_INVENTORY (10) items.
Items offer various effects: healing potions, temporary stat boosts, revival stones, and permanent upgrades.
Use (use <item>) items at any time to gain their benefits.
Discard (discard <item>) items to free inventory space.

##Checkpoints & Revivals:
The player's current room is set as a checkpoint upon saving.
Dying without a revive item returns the player to the last checkpoint with full health.
If carrying a Revive Stone, the player is automatically revived at 30% max health upon death, without losing progress.

##Saving & Loading Progress:
Save the current game state (player stats, inventory, room states) to a file.
Reload the saved state to continue your adventure later.

##Traps & Hidden Dangers:
Some rooms contain traps that may damage the player after using the look command.
Traps trigger with a certain probability, adding risk to exploration.

##Repository Structure and Files

##main.c:
The entry point of the program.
Initializes the player’s stats.
Calls initializeRooms() to set up the dungeon.
Assigns the starting room and checkpoint room.
Initiates the main game loop (gameLoop()).

##game.h:
The header file that includes all the necessary constants, structure definitions, and function prototypes.
Defines constants for inventory size, room counts, item counts, etc.
Declares structures for Player, Room, Creature, and Item.
Declares all functions used to manage the game state and logic.

##game.c:
Implements the core game logic defined in game.h.
Initialization functions: initializeRooms(), createRoom(), and connectRooms()
Gameplay mechanics: Movement (movePlayer()), looking around (look()), picking up items (pickupItem()), using items (useItem()), attacking (attack()), and discarding items (discardItem()).
Status and effects: Updating potion durations (updatePotionsOnRoomChange()), resetting temporary buffs (resetFlameWeapon()), and handling trap triggers (triggerTrapsAfterLook()).
Creatures and Bosses: Functions createBoss(), createNormalCreature() and combat resolution in attackCreature().
Saving & Loading: saveGame() and loadGame() handle serialization and deserialization of game state.
Game flow: gameLoop() processes player input and coordinates game actions until completion or death.

##Data Structures and Their Roles
##Player Structure:
c
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
Fields:
health, maxHealth: Manage player's survivability.
strength, defense: Combat stats affecting damage dealt and taken.
attackSpeed: Currently unused in the provided logic, but could influence future combat mechanics.
inventoryCount, inventory[]: Store items the player carries.
currentRoom, checkPointRoom: Track player's current location and the room to respawn after death.
baseStrength, baseDefense: Original stats used to reset after temporary potion buffs expire.
flameWeaponActive, attackPotionCounter, defensePotionCounter: Flags and counters for managing temporary effects.
gameFinished: A flag indicating that the player has defeated the final boss and completed the game.


##Room Structure:
c
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
Fields:
description: Text describing the room.
roomID: Unique ID (1 to 12).
items[], itemCount: Items present in the room.
creature: A pointer to a creature (boss or normal) occupying the room.
isBossRoom: Indicates if the room contains a boss.
trapCount: How many traps are in the room.
up, down, left, right: Pointers to adjacent rooms for navigation.


##Creature Structure:
c
typedef struct Creature {
    char name[MAX_NAME_LENGTH];
    int health;
    int attackPower;
    int isBoss;
    int bossRoomID;
} Creature;
Fields:
name, health, attackPower: Defines the creature's stats.
isBoss: Flag to determine if the creature is a boss.
bossRoomID: Specifies which room is assigned as that boss’s territory.


##Item Structure:
c
typedef struct Item {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    int effectType;
} Item;
Fields:
name, description: Identifying information.
effectType: An integer specifying what the item does (e.g., 1 for health potion, 2 for attack potion, etc.).

##Items and Their Effects
Health Potion (effectType = 1): Restores 50 HP, capped at max health.
Attack Potion (effectType = 2): Increases the player's strength by +10 for the next 2 room movements.
Defense Potion (effectType = 3): Increases the player's defense by +5 for the next 2 room movements.
Revive Stone (effectType = 4): Passive item. If the player dies, it automatically revives them to 30% of max health and is then consumed.
Flame Weapon (effectType = 5): Increases the player's strength by +20 until they leave the current room.
Zensu Beans (effectType = 6): Permanently increase max health by 40 and also heal the player by 40 HP immediately.

##How to Play
##Starting the Game:
Run the compiled executable.
You start in room 1 with default stats and no items.

##Commands:
move <direction>: Move to an adjacent room if it exists. Directions: up, down, left, right.
look: Inspect the current room. Reveals exits, items, and creatures. Also triggers traps.
inventory: List your items.
pickup <item_name>: Collect an item found in the room and add it to your inventory.
use <item_name>: Activate an item's effect from your inventory.
discard <item_name>: Remove an item from your inventory without using it.
attack: Attack the creature in the room (if any).
info: Display your current health and strength.
save: Prompts for a filename and saves the current game state.
load: Prompts for a filename and loads a previously saved state.
quit: Exits the game.
help: Prints a list of available commands.

##Progression:
Explore the dungeon, defeating bosses at rooms 3, 6, 9, and finally 12.
The game ends successfully when you defeat the boss in the final room (room 12).

##Death and Checkpoints:
If your health drops to 0 and you have a Revive Stone, you are revived automatically.
Without a Revive Stone, you return to the checkpoint room (last saved room) at full health.
Saving and Loading the Game

##Saving:
Use the save command and provide a filename (e.g., Umut1.txt).
The current room becomes your checkpoint room.
The file will store your stats, current room, inventory, and other relevant details.

##Loading:
Use the load command and specify the filename used previously.
Your player state, inventory, and room position are restored to the saved snapshot.
Traps and Risks
After looking around (using look):
The game checks if any traps are triggered.
Each trap has a 50% chance to deal 10 damage.
If you die from traps and have no Revive Stone, you respawn at the checkpoint.

##Compilation and Execution
##Requirements:
A C compiler like gcc.
A UNIX-like environment is recommended for convenience.

##Compilation Command:
bash
gcc -o dungeon main.c game.c

##Running the Game:
bash
./dungeon


##Code Design and Implementation Details

##Separation of Concerns:
game.h declares the interfaces and data structures.
game.c implements all game logic, ensuring a clean separation between declarations and implementations.
main.c sets up initial conditions and starts the gameplay loop.

##Memory Management:
Dynamically allocated memory for rooms, items, and creatures is freed at the end of main() to prevent memory leaks.
Items and creatures picked up or defeated are properly freed when no longer needed.

##Randomization:
The dungeon layout is consistent but items and traps in rooms vary.
Random number generation (rand()) is seeded at program start (srand(time(NULL))) to add unpredictability.

##Adaptability:
Changing constants (e.g., NUM_ROOMS, NUM_ITEM_TYPES) in game.h allows for easy scaling of the dungeon size or variety of items.
The code can be extended to support additional features such as more item types, more complex puzzles, multiple floors, or persistent effects.

##Extending the Game
New Items:
Add new entries to hardcodedItems in game.c and implement their effects in useItem().

##New Creatures and Bosses:
Modify or extend hardcodedNormalCreatures and hardcodedBossCreatures.
Update createBoss() and createNormalCreature() logic as needed.

##Additional Rooms and Layouts:
Increase NUM_ROOMS and add room descriptions and connections in connectRooms().
Adjust the logic in initializeRooms() to create more diverse or larger dungeons.

##Complex Mechanics:
Introduce status effects, new traps, puzzles, locked doors that require keys, or special commands for more interactive gameplay.

##Disclaimer and License
This code is provided as-is for educational and entertainment purposes. Feel free to modify, extend, and distribute it. No warranty is provided, and the authors are not liable for any damages arising from its use.

##Conclusion
The Dungeon Adventure Game is a foundational text-based exploration and combat game. It demonstrates basic game loops, data management, and logic structuring in C. With a modular design and clear data structures, it is straightforward to enhance and personalize the experience. Enjoy exploring the dungeon, collecting items, and overcoming formidable foes on your path to victory!







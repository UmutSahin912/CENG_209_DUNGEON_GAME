# CENG_209_DUNGEON_GAME
 Textbased Dungeon Game with C
Dungeon Adventure Game
Overview
Dungeon Adventure Game is a text-based adventure game written in C. Players navigate through a dungeon composed of interconnected rooms, each filled with items, creatures, and traps. The objective is to explore the dungeon, collect valuable items, battle creatures (including formidable bosses), and ultimately conquer the final challenge to finish the game. The game emphasizes strategic movement, inventory management, and combat tactics.

Gameplay
Starting the Game
Initialization:
When the game starts, the player is placed in the first room of the dungeon. The initial room's description is displayed, along with available exits.

Player Stats:
The player's stats, including health, strength, defense, and inventory, are initialized.

Basic Commands
Players interact with the game using text commands. Below are the primary commands available:

Movement:

move up: Move to the room above the current room.
move down: Move to the room below the current room.
move left: Move to the room to the left of the current room.
move right: Move to the room to the right of the current room.
Examination:

look: Examine the current room to see its description, available exits, items present, and any creatures inhabiting the room.
Inventory Management:

inventory: Display all items you're carrying.
pickup <item>: Pick up a specified item from the current room and add it to your inventory (if you have space).
use <item>: Use a specified item from the inventory to gain its effects.
discard <item>: Remove a specified item from the inventory permanently.
Combat:

attack: Engage in combat with the creature present in the current room.
Player Information:

info: Display the player's current health and strength stats.
Game Control:

save: Save the current game state to a file. The player will be prompted to enter a filename.
load: Load a previously saved game state from a file. The player will be prompted to enter a filename.
help: Display a list of all available commands.
quit: Exit the game.
Detailed Command Descriptions
move <direction>:

Description: Moves the player in the specified direction if an exit exists.
Directions: up, down, left, right.
Example: move up
look:

Description: Provides a detailed description of the current room.
Effects: Lists all available exits, displays any items present, shows information about any creature present, and triggers traps.
inventory:

Description: Lists all items currently carried by the player.
Note: If the inventory is empty, it displays "Empty."
pickup <item>:

Description: Allows the player to pick up an item from the room and add it to their inventory.
Limit: The inventory has a maximum capacity of 10 items.
Example: pickup Health Potion
use <item>:

Description: Uses an item from the inventory, applying its effects to the player.
Example: use Health Potion
discard <item>:

Description: Removes an item from the inventory permanently.
Example: discard Zensu Beans
attack:

Description: Engages in combat with the creature present in the room.
Mechanics: The player's strength reduces the creature's health. If the creature survives, it retaliates.
info:

Description: Displays the player's current health and strength stats.
save:

Description: Saves the current game state to a specified file.
Process: Prompts the player to enter a filename (e.g., save1.txt).
Checkpoint: Sets the current room as a checkpoint before saving.
load:

Description: Loads a previously saved game state from a specified file.
Process: Prompts the player to enter a filename (e.g., save1.txt).
help:

Description: Displays a list of all available commands with brief descriptions.
quit:

Description: Exits the game gracefully, performing necessary memory clean-up.
Code Structure
The project is organized into three primary files:

1. game.h
Purpose:
Defines the core data structures (Item, Creature, Room, Player) used throughout the game. Declares constants, global variables, and function prototypes.

Key Components:

Structures:
Item: Represents items with a name, description, and effect type.
Creature: Represents entities that the player can encounter, with attributes like health and attack power.
Room: Represents each room in the dungeon, containing descriptions, items, creatures, traps, and connections to other rooms.
Player: Represents the player's state, including stats and inventory.
Constants:
MAX_INVENTORY, MAX_DESC_LENGTH, MAX_NAME_LENGTH, etc., define limits for various game elements.
Function Prototypes:
Includes all game-related functions for room initialization, player actions, combat, saving/loading, and utilities.
2. game.c
Purpose:
Implements all functions declared in game.h. Manages game logic, including room setup, player interactions, combat mechanics, and game state management.

Key Components:

Global Variables:
Room *rooms[NUM_ROOMS];: An array holding pointers to all rooms in the dungeon.
Static Data:
hardcodedRoomDescs: Descriptions for each room.
hardcodedItems: Predefined items available in the game.
hardcodedNormalCreatures: Predefined normal creatures with their stats.
hardcodedBossCreatures: Predefined boss creatures, each linked to specific rooms.
Functions:
Utility Functions:
randomBetween: Generates a random number within a specified range.
createItem, createNormalCreature, createBoss: Functions to instantiate items and creatures.
createRoom: Initializes a room with items and a creature based on its type.
connectRooms: Establishes connections between rooms to allow navigation.
initializeRooms: Sets up all rooms at the start of the game.
Player Interaction Functions:
movePlayer: Handles player movement between rooms, updates stats, and manages effects.
look: Provides detailed information about the current room and triggers traps.
showInventory: Displays the player's inventory.
pickupItem, useItem, discardItem: Manage inventory actions.
attackCreature, attack: Handle combat mechanics.
saveGame, loadGame: Manage game state persistence.
showHelp: Displays available commands.
gameLoop: The main game loop that processes player commands and updates the game state.
Special Mechanics Functions:
hasReviveStone: Checks if the player possesses a Revive Stone.
revivePlayer: Handles player revival mechanics upon death.
resetFlameWeapon: Resets flame weapon effects when leaving a room.
updatePotionsOnRoomChange: Manages temporary potion effects based on room changes.
triggerTrapsAfterLook: Activates traps after the player uses the look command.
finishGame, showFinalStats: Handle game completion and display final player stats.
3. main.c
Purpose:
Serves as the entry point of the game. Initializes the player and rooms, starts the game loop, and handles memory cleanup upon game termination.

Key Components:

main Function:
Seeds the random number generator.
Initializes the player's stats and inventory.
Calls initializeRooms to set up the dungeon.
Sets the player's starting room and checkpoint room.
Starts the game loop by calling gameLoop.
Upon exiting the game loop, performs memory cleanup by freeing all dynamically allocated memory for rooms, items, and creatures to prevent memory leaks.
Game Logic
Rooms and Navigation
Dungeon Layout:

The dungeon consists of 12 rooms, each uniquely identified by a roomID (1 to 12).
Rooms are interconnected through up, down, left, and right pointers, allowing players to navigate between them.
Some rooms are designated as boss rooms (rooms 3, 6, 9, and 12), containing powerful boss creatures.
Room Initialization:

Each room is created using the createRoom function, which assigns a description, populates it with items, and adds a creature (normal or boss) based on the room type.
Items within rooms are randomly selected from predefined types, ensuring variety in each playthrough.
Connections between rooms are established using the connectRooms function, defining the dungeon's layout.
Items and Inventory Management
Item Types and Effects:

Health Potion (EffectType 1): Restores 50 HP when used.
Attack Potion (EffectType 2): Increases player strength by 10 for 2 rooms.
Defense Potion (EffectType 3): Increases player defense by 5 for 2 rooms.
Revive Stone (EffectType 4): Passive item that revives the player upon death, restoring 30% health.
Flame Weapon (EffectType 5): Greatly increases player strength by 20 until they leave the current room.
Zensu Beans (EffectType 6): Permanently increases the player's maximum health by 40.
Inventory Mechanics:

The player's inventory can hold up to 10 items.
Items can be picked up from rooms using the pickupItem function, which adds the item to the inventory if space permits.
Items can be used via the useItem function, applying their effects to the player.
Items can be discarded using the discardItem function, freeing up inventory space.
Passive items like the Revive Stone cannot be actively used but provide automatic benefits.
Creatures and Combat Mechanics
Creature Types:

Normal Creatures:
Goblin: 60 HP, 10 ATK.
Spider: 40 HP, 20 ATK.
Boss Creatures:
Master Mantis: 120 HP, 30 ATK (Room 3).
Giant Serpent: 140 HP, 35 ATK (Room 6).
Kraken: 160 HP, 40 ATK (Room 9).
Dragonoid: 180 HP, 45 ATK (Room 12).
Combat Flow:

Initiated by the attack command.
The player's strength reduces the creature's health.
If the creature survives, it retaliates by dealing damage based on its attack power and the player's defense.
Defeating a creature removes it from the room.
Defeating the boss in Room 12 triggers the finishGame function, ending the game successfully.
Bosses may drop special items like the Boss Relic upon defeat.
Traps
Trap Mechanics:

Each room may contain a random number of traps (0 to 3).
Traps are triggered after the player uses the look command.
Each trap has a 50% chance to activate, dealing 10 damage to the player.
Trap Consequences:

If a trap deals damage that reduces the player's health to 0 or below, the player dies.
If the player has a Revive Stone in their inventory, it automatically revives them with 30% of their maximum health.
Otherwise, the player respawns at the last checkpoint room with full health.
Checkpoints and Save/Load Functionality
Checkpoints:

The player's current room is set as a checkpoint before saving the game.
This ensures that if the player dies without a Revive Stone, they respawn at this checkpoint room.
Saving the Game:

Executed via the save command.
Prompts the player to enter a filename to save the game state.
Saves the player's stats, current room, inventory, and other relevant data to the specified file.
Loading the Game:

Executed via the load command.
Prompts the player to enter a filename to load the game state from.
Restores the player's stats, current room, inventory, and other relevant data from the specified file.
Game Completion
Objective:

The main goal is to navigate through all rooms, defeat creatures and bosses, and ultimately conquer the final boss in Room 12.
Ending the Game:

Defeating the Dragonoid in Room 12 triggers the finishGame function.
Displays a congratulatory message along with the player's final stats and inventory.
The game then terminates gracefully.
How to Compile and Run
Prerequisites
C Compiler: Ensure that you have a C compiler installed (e.g., gcc).
Compilation Steps
Organize Files:

Ensure that the following files are in the same directory:
game.h
game.c
main.c
Open Terminal:

Navigate to the directory containing the game files using the terminal.
Compile the Code:

Run the following command to compile the game:
bash
Kodu kopyala
gcc -o dungeon_game main.c game.c
This command compiles main.c and game.c into an executable named dungeon_game.
Run the Game:

Execute the compiled program by running:
bash
Kodu kopyala
./dungeon_game
The game will start, displaying a welcome message and prompting for commands.
Example Session
plaintext
Kodu kopyala
Welcome to the Dungeon Adventure Game!
Type 'help' for commands.

> help
Commands:
- move <direction>: Move (up, down, left, right).
- look: Look around the current room.
- inventory: Show your inventory.
- pickup <item>: Pick up an item.
- use <item>: Use an item.
- discard <item>: Discard an item.
- attack: Attack the creature.
- info: Show your stats.
- save: Will ask for a filename (e.g. save1.txt) and save.
- load: Will ask for a filename (e.g. save1.txt) and load.
- quit: Quit.

> look
You are in Room 1. A dimly lit room with moss on the walls.
Exits: [Right] [Down] 
Items in this room:
- Health Potion: Restores 50 HP.
- Zensu Beans: Permanently increases your max health by 40.
There is a creature: Goblin (HP:60, ATK:10)

> attack
You attack the Goblin!
Goblin's health: 50
Goblin attacks you for 10 damage. Health: 90

> use Health Potion
You used a Health Potion. Health: 100

> move right
You move to room 2.

> save
Enter filename to save (e.g. save1.txt): my_save.txt
Game saved to my_save.txt.

> quit
Goodbye!
Summary of Commands
Movement:

move up: Move to the room above.
move down: Move to the room below.
move left: Move to the room to the left.
move right: Move to the room to the right.
Examination:

look: Examine the current room.
Inventory Management:

inventory: View current inventory.
pickup <item>: Pick up an item from the room.
use <item>: Use an item from the inventory.
discard <item>: Remove an item from the inventory.
Combat:

attack: Attack the creature in the room.
Player Information:

info: View current health and strength.
Game Control:

save: Save the current game state.
load: Load a previously saved game state.
help: Display available commands.
quit: Exit the game.
Memory Management
Dynamic Allocation:
The game dynamically allocates memory for items and creatures within rooms.

Memory Cleanup:
Upon quitting the game or completing it, the main function ensures that all allocated memory is freed to prevent memory leaks. This includes freeing all items in rooms, creatures in rooms, and items in the player's inventory.

Contributions and Enhancements
Players and developers can contribute to enhancing the game by:

Adding New Items:

Introduce new item types with unique effects.
Expanding the Dungeon:

Increase the number of rooms or add new areas with different themes.
Introducing More Creatures:

Add variety by introducing new normal and boss creatures with distinct abilities.
Enhancing Combat Mechanics:

Implement features like critical hits, special abilities, or weapon types.
Improving Save/Load Functionality:

Optimize the save file structure or add multiple save slots.
Adding Story Elements:

Introduce a narrative or quests to provide more depth to the gameplay.
License
This project is open-source and available under the MIT License.

This README.md provides a comprehensive guide to understanding and playing the Dungeon Adventure Game, as well as insight into its underlying code structure and mechanics. Whether you're a player eager to dive into the adventure or a developer looking to contribute, this README serves as a valuable resource.







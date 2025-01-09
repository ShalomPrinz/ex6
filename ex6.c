#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128

// TODO define all my new funcs in h file and copy func documentation

// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================

// --------------------------------------------------------------
// 1) Safe integer reading
// --------------------------------------------------------------

void trimWhitespace(char *str) {
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0) {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int) strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

char *myStrdup(const char *src) {
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest) {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt) {
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success) {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0) {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int) strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0') {
            printf("Invalid input.\n");
        }
        else {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// TODO why not set enum values to string? ...
// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type) {
    switch (type) {
        case GRASS:
            return "GRASS";
        case FIRE:
            return "FIRE";
        case WATER:
            return "WATER";
        case BUG:
            return "BUG";
        case NORMAL:
            return "NORMAL";
        case POISON:
            return "POISON";
        case ELECTRIC:
            return "ELECTRIC";
        case GROUND:
            return "GROUND";
        case FAIRY:
            return "FAIRY";
        case FIGHTING:
            return "FIGHTING";
        case PSYCHIC:
            return "PSYCHIC";
        case ROCK:
            return "ROCK";
        case GHOST:
            return "GHOST";
        case DRAGON:
            return "DRAGON";
        case ICE:
            return "ICE";
        default:
            return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput() {
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp) {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node) {
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

// --------------------------------------------------------------
// Display Menu
// --------------------------------------------------------------
void displayMenu(OwnerNode *owner) {
    if (!owner->pokedexRoot) {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice) {
        case 1:
            // displayBFS(owner->pokedexRoot);
            break;
        case 2:
            // preOrderTraversal(owner->pokedexRoot);
            break;
        case 3:
            // inOrderTraversal(owner->pokedexRoot);
            break;
        case 4:
            // postOrderTraversal(owner->pokedexRoot);
            break;
        case 5:
            // displayAlphabetical(owner->pokedexRoot);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// --------------------------------------------------------------
// void enterExistingPokedexMenu() {
//     // list owners
//     printf("\nExisting Pokedexes:\n");
//     // you need to implement a few things here :)
//
//     printf("\nEntering %s's Pokedex...\n", cur->ownerName);
//
//     int subChoice;
//     do {
//         printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
//         printf("1. Add Pokemon\n");
//         printf("2. Display Pokedex\n");
//         printf("3. Release Pokemon (by ID)\n");
//         printf("4. Pokemon Fight!\n");
//         printf("5. Evolve Pokemon\n");
//         printf("6. Back to Main\n");
//
//         subChoice = readIntSafe("Your choice: ");
//
//         switch (subChoice) {
//             case 1:
//                 addPokemon(cur);
//                 break;
//             case 2:
//                 displayMenu(cur);
//                 break;
//             case 3:
//                 freePokemon(cur);
//                 break;
//             case 4:
//                 pokemonFight(cur);
//                 break;
//             case 5:
//                 evolvePokemon(cur);
//                 break;
//             case 6:
//                 printf("Back to Main Menu.\n");
//                 break;
//             default:
//                 printf("Invalid choice.\n");
//         }
//     } while (subChoice != 6);
// }

/**
 * @param first first string
 * @param second second string
 * @return whether strings are equal, by using strcmp
 */
int areStringsEqual(char *first, char *second) {
    return strcmp(first, second) == 0;
}

/**
 * @param name owner name to validate
 * @return whether given name is a duplicate of any existing owner's name
 */
int isDuplicatedOwnerName(char *name) {
    // If no owners at all, duplication is not possible
    if (ownerHead == NULL)
        return 0;

    // Iterate over all owners
    OwnerNode *currentOwner = ownerHead;
    char *initialName = currentOwner->ownerName;
    do {
        // Validate ownerName isn't duplicate of any existing owner
        if (areStringsEqual(currentOwner->ownerName, name))
            return 1; // name is a duplication of existing owner's name

        // Move on to next owner in list
        currentOwner = currentOwner->next;
    } while (!areStringsEqual(currentOwner->ownerName, initialName));

    // No duplication found
    return 0;
}

OwnerNode *createOwner(char *ownerName, PokemonNode *starter) {
    // Create newOwner's node with given input data
    OwnerNode *newOwner = (OwnerNode *)malloc(sizeof(OwnerNode));
    if (newOwner == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Fill newOwner's data with given data
    newOwner->ownerName = ownerName;
    newOwner->pokedexRoot = starter;
    return newOwner;
}

void openPokedexMenu() {
    // Input owner name
    printf("Your name: ");
    char *name = getDynamicInput();

    // Validate name isn't duplicate of existing owner name
    if (isDuplicatedOwnerName(name)) {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        free(name);
        return;
    }

    // Input starter pokemon
    printf("Choose Starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    int choice = readIntSafe("Your choice: ");
    int pokemonIndex = (choice - 1) * 3; // TODO explain the formula

    // Init new owner's pokedex
    PokemonNode *ownerPokedex = (PokemonNode *)malloc(sizeof(PokemonNode));
    if (ownerPokedex == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    ownerPokedex->left = NULL;
    ownerPokedex->right = NULL;
    // TODO should I malloc new mem for this? currently i think not
    ownerPokedex->data = (PokemonData *) &pokedex[pokemonIndex];

    // Add owner to global owner linked list
    OwnerNode *newOwner = createOwner(name, ownerPokedex);
    if (newOwner == NULL)
        return;

    if (ownerHead == NULL) {
        // If no owners yet, ownerHead should be newOwner
        ownerHead = newOwner;
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
    } else {
        // Update circular list to match newly inserted newOwner
        OwnerNode *lastOwner = ownerHead->prev;
        lastOwner->next = newOwner;
        ownerHead->prev = newOwner;
        newOwner->next = ownerHead;
        newOwner->prev = lastOwner;
    }

    printf("New Pokedex created for %s with starter %s.\n", name, ownerPokedex->data->name);
}

// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu() {
    int choice;
    do {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice) {
            case 1:
                openPokedexMenu();
                break;
            case 2:
                // enterExistingPokedexMenu();
                break;
            case 3:
                // deletePokedex();
                break;
            case 4:
                // mergePokedexMenu();
                break;
            case 5:
                // sortOwners();
                break;
            case 6:
                // printOwnersCircular();
                break;
            case 7:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid.\n");
        }
    } while (choice != 7);
}

/**
 * @brief Removes owner from owners list and frees it from heap
 * @param owner owner to remove
 */
void removeOwner(OwnerNode* owner) {
    if (owner->next != owner) {
        // Connect prev of owner to its next
        owner->next->prev = owner->prev;
        owner->prev->next = owner->next;
    }

    // Reset and free owner properties
    owner->next = NULL;
    owner->prev = NULL;
    free(owner->ownerName);
    free(owner->pokedexRoot); // TODO implement
    free(owner);
    owner = NULL;
}

void freeAllOwners() {
    // No owners - nothing to free
    if (ownerHead == NULL)
        return;

    // Iterate over all owners and free them
    OwnerNode *current = ownerHead;
    while (current != NULL && current != current->next) {
        OwnerNode *next = current->next;
        removeOwner(current);
        current = next;
    }

    // Remove last remaining owner
    removeOwner(current);

    // Reset ownerHead to NULL as required
    ownerHead = NULL;
}

int main() {
    mainMenu();
    freeAllOwners();
    return 0;
}

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

/**
 * @brief Reads a single char from user's input and clears the buffer.
 * @param prompt message to user
 * @return first's user input char which is not a whitespace
 */
char readCharSafe(char* prompt) {
    printf("%s", prompt);
    // Clear all whitespaces and then read a single char
    char ch;
    scanf(" %c", &ch);

    // Clear the buffer from all chars until reached '\n'
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return ch;
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
            displayBFS(owner->pokedexRoot);
            break;
        case 2:
            preOrderTraversal(owner->pokedexRoot);
            break;
        case 3:
            inOrderTraversal(owner->pokedexRoot);
            break;
        case 4:
            postOrderTraversal(owner->pokedexRoot);
            break;
        case 5:
            // displayAlphabetical(owner->pokedexRoot);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void displayBFS(PokemonNode *root) {
    BFSGeneric(root, printPokemonNode);
}

void preOrderTraversal(PokemonNode *root) {
    preOrderGeneric(root, printPokemonNode);
}

void inOrderTraversal(PokemonNode *root) {
    inOrderGeneric(root, printPokemonNode);
}

void postOrderTraversal(PokemonNode *root) {
    postOrderGeneric(root, printPokemonNode);
}

// void displayAlphabetical(PokemonNode *root) {
//
// }

OwnerNode *choosePokedex(char *prompt) {
    OwnerNode *currentOwner = ownerHead;
    int ownerIndex = 1;
    do {
        // Print each pokedex in this format, then move on to next pokedex
        printf("%d. %s\n", ownerIndex++, currentOwner->ownerName);
        currentOwner = currentOwner->next;
    } while (currentOwner != ownerHead);

    // Input an owner index
    int choice = readIntSafe(prompt);

    // Reset values for another loop, in order to reach chosen owner
    currentOwner = ownerHead;
    ownerIndex = 0;

    // Loop until ownerIndex equals choice
    while (choice > ++ownerIndex)
        currentOwner = currentOwner->next;

    return currentOwner;
}

// TODO move. h? up?
#define LAST_POKEMON_ID 151

// --------------------------------------------------------------
// Queue Implementation
// --------------------------------------------------------------

typedef struct PokemonQueueNode {
    PokemonNode *node;
    struct PokemonQueueNode *next;
} PokemonQueueNode;

typedef struct PokemonQueue {
    PokemonQueueNode *front, *rear;
} PokemonQueue;

/**
 * @brief Create new queue node from a given pokemon node.
 * @param node pokemon node to create queue node from
 * @return newly allocated pokemon queue node
 */
PokemonQueueNode *createPokemonQueueNode(PokemonNode *node) {
    // Allocate new queue node
    PokemonQueueNode *queueNode = malloc(sizeof(PokemonQueueNode));
    if (!queueNode) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Init queue node with default values and return it
    queueNode->node = node;
    queueNode->next = NULL;
    return queueNode;
}

/**
 * @param queue queue to check if empty
 * @return whether given queue is empty
 */
int isEmptyQueue(PokemonQueue *queue) {
    return queue == NULL || queue->front == NULL;
}

/**
 * @brief Inserts new queue node that with given data into given queue.
 * @param queue queue to insert new node into
 * @param data data of new queue node
 */
void queueInsert(PokemonQueue* queue, PokemonNode* data) {
    // Allocate new queue node
    PokemonQueueNode* newQNode = createPokemonQueueNode(data);
    if (newQNode == NULL)
        return;

    // If queue is empty, new queue node is both front and rear of the queue
    if (queue->rear == NULL) {
        queue->front = newQNode;
        queue->rear = newQNode;
        return;
    }

    // If queue isn't empty, new queue node is now rear of the queue
    queue->rear->next = newQNode;
    queue->rear = newQNode;
}

/**
 * @brief Pops first queue node from given queue.
 * @param queue queue to remove node from
 * @return popped queue node or NULL if no nodes in queue
 */
PokemonNode *queuePop(PokemonQueue* queue) {
    // Return NULL if no nodes in queue
    if (queue->front == NULL)
        return NULL;

    // Pop first queue node and move front pointer one step forward
    PokemonQueueNode* queueNode = queue->front;
    PokemonNode* data = queueNode->node;
    queue->front = queue->front->next;

    // ? TODO
    if (queue->front == NULL)
        queue->rear = NULL;

    // Free popped queue node and return its data
    free(queueNode);
    return data;
}

void BFSGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // Allocate queue
    PokemonQueue *queue = malloc(sizeof(PokemonQueue));
    if (!queue) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Init queue with root pokemon node
    queue->front = NULL;
    queue->rear = NULL;
    queueInsert(queue, root);

    // Iterate BFS style on given root
    while (!isEmptyQueue(queue)) {
        // Save front QNode from queue
        PokemonNode *current = queuePop(queue);

        // If current node has left child, add it to queue and visit it later
        if (current->left)
            queueInsert(queue, current->left);

        // If current node has right child, add it to queue and visit it later
        if (current->right)
            queueInsert(queue, current->right);

        // Visit current node
        visit(current);
    }

    // Finish BFS, free queue
    free(queue);
}

void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If current node is NULL, don't visit it
    if (root == NULL) return;

    // Visit given root node
    visit(root);
    // Visit left child
    preOrderGeneric(root->left, visit);
    // Visit right child
    preOrderGeneric(root->right, visit);
}

void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If current node is NULL, don't visit it
    if (root == NULL) return;

    // Visit left child
    inOrderGeneric(root->left, visit);
    // Visit given root node
    visit(root);
    // Visit right child
    inOrderGeneric(root->right, visit);
}

void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If current node is NULL, don't visit it
    if (root == NULL) return;

    // Visit left child
    postOrderGeneric(root->left, visit);
    // Visit right child
    postOrderGeneric(root->right, visit);
    // Visit given root node
    visit(root);
}

PokemonNode *createPokemonNode(const PokemonData *data) {
    // Allocate memory for new pokemon node
    PokemonNode *newNode = malloc(sizeof(PokemonNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Init pokemon node with default values and return it
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->data = (PokemonData*) data; // TODO not allocating, think if it's right
    return newNode;
}

PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode) {
    // If pokedex is empty, start pokedex with given node
    if (root == NULL)
        return newNode;

    // DFS TODO document. maybe use generic dfs func
    PokemonNode *current = root;
    while (current != NULL) {
        if (current->data->id > newNode->data->id) {
            if (current->left == NULL) {
                current->left = newNode;
                return root;
            }
            current = current->left;
            continue;
        }

        if (current->data->id < newNode->data->id) {
            if (current->right == NULL) {
                current->right = newNode;
                return root;
            }
            current = current->right;
            continue;
        }

        // Duplicate! TODO implemented differently..
        return NULL;
    }

    // TODO redundant?
    return root;
}

void addPokemon(OwnerNode *owner) {
    int pokemonID = readIntSafe("Enter ID to add: ");
    if (pokemonID < 1 || pokemonID > LAST_POKEMON_ID) {
        printf("Invalid ID.\n");
        return;
    }

    // Allocate new pokemon node
    PokemonData *pokemon = (PokemonData *) &pokedex[pokemonID - 1];
    PokemonNode *newNode = createPokemonNode(pokemon);
    if (newNode == NULL)
        return;

    // Replace pokedex root with new root which includes new node
    PokemonNode *newRoot = insertPokemonNode(owner->pokedexRoot, newNode);
    if (newRoot == NULL) {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", pokemonID);
        free(newNode);
        return;
    }

    // Successfully inserted pokemon to pokedex
    owner->pokedexRoot = newRoot;
    printf("Pokemon %s (ID %d) added.\n", pokemon->name, pokemonID);
}

// TODO currently free node only because pokemon isn't allocated
void freePokemonNode(PokemonNode *node) {
    free(node);
}

void freePokemonTree(PokemonNode *root) {
    BFSGeneric(root, freePokemonNode);
}

// void freePokemon(OwnerNode *owner) {
//     int pokemonID = readIntSafe("Enter Pokemon ID to release: ");
//     removeNodeBST(owner->pokedexRoot, pokemonID);
// }

// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// --------------------------------------------------------------
void enterExistingPokedexMenu() {
    // If there are no owners, there is no owner to choose
    if (ownerHead == NULL) {
        printf("No existing Pokedexes.\n");
        return;
    }

    // Input a pokedex choice and enter its menu
    printf("\nExisting Pokedexes:\n");
    OwnerNode* pokedex = choosePokedex("Choose a Pokedex by number: ");
    printf("\nEntering %s's Pokedex...\n", pokedex->ownerName);

    int subChoice;
    do {
        printf("\n-- %s's Pokedex Menu --\n", pokedex->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");
        switch (subChoice) {
            case 1:
                addPokemon(pokedex);
                break;
            case 2:
                displayMenu(pokedex);
                break;
            case 3:
                // freePokemon(pokedex);
                break;
            case 4:
                // pokemonFight(pokedex);
                break;
            case 5:
                // evolvePokemon(pokedex);
                break;
            case 6:
                printf("Back to Main Menu.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

/**
 * @brief Swap data between given nodes.
 * @param first owner node 1
 * @param second owner node 2
 * @note Only data is swapped to keep original list structure, which is much more complicated to change
 */
void swap(OwnerNode* first, OwnerNode* second) {
    // Swap owners names
    char* temp = first->ownerName;
    first->ownerName = second->ownerName;
    second->ownerName = temp;

    // Swap owners pokedexes
    PokemonNode* pokedex = first->pokedexRoot;
    first->pokedexRoot = second->pokedexRoot;
    second->pokedexRoot = pokedex;
}

/**
 * @brief Implementation of insertion sort algorithm to sort owners list
 */
void insertionSortOwners() {
    // New head of list
    OwnerNode* start = ownerHead;
    OwnerNode* end = ownerHead;

    // Iterate all owners
    do {
        // Start at last owner and move 1 step backward each iteration
        end = end->prev;
        // Start at first owner each iteration
        OwnerNode* current = start;

        // Iterate from first owner to current end owner
        while (current != end) {
            // If next owner should be before current owner, swap them
            if (strcmp(current->ownerName, current->next->ownerName) > 0)
                swap(current, current->next);

            // Move to next owner - 1 step forward
            current = current->next;
        }
    } while (end != start);

    // Set ownerHead to point to the sorted owners list
    ownerHead = start;
}
void sortOwners() {
    // Don't sort if there are no owners at all
    if (ownerHead == NULL) {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }

    // Sort only if there is more then one node in list
    if (ownerHead->next != ownerHead)
        insertionSortOwners();
    printf("Owners sorted by name.\n");
}

/**
 * @brief Prints X owners names, starting at head and moving forward or backward.
 * Direction and X times are determined by user input.
 */
void printOwnersCircular() {
    // No owners to print
    if (ownerHead == NULL) {
        printf("No owners.\n");
        return;
    }

    // Input print count and movement direction
    char c = readCharSafe("Enter direction (F or B): ");
    int count = readIntSafe("How many prints? ");

    // Start at head, move 1 step in chosen direction until reached count times
    OwnerNode *current = ownerHead;
    for (int i = 1; i <= count; i++) {
        printf("[%d] %s\n", i, current->ownerName);
        /*
         If chosen direction isn't 'F' or 'f', default to backward direction
         (Assuming input is valid - only f / F / b / B)
        */
        current = c == 'F' || c == 'f' ? current->next : current->prev;
    }
}

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
    do {
        // Validate ownerName isn't duplicate of any existing owner
        if (areStringsEqual(currentOwner->ownerName, name))
            return 1; // name is a duplication of existing owner's name

        // Move on to next owner in list
        currentOwner = currentOwner->next;
    } while (currentOwner != ownerHead);

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
    newOwner->pokedexRoot = insertPokemonNode(NULL, starter);
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
                enterExistingPokedexMenu();
                break;
            case 3:
                deletePokedex();
                break;
            case 4:
                // mergePokedexMenu();
                break;
            case 5:
                sortOwners();
                break;
            case 6:
                printOwnersCircular();
                break;
            case 7:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid.\n");
        }
    } while (choice != 7);
}

void freeOwnerNode(OwnerNode *owner) {
    if (owner->next != owner) {
        // Connect prev of owner to its next
        owner->next->prev = owner->prev;
        owner->prev->next = owner->next;
    }

    // Reset and free owner properties
    owner->next = NULL;
    owner->prev = NULL;
    free(owner->ownerName);
    freePokemonTree(owner->pokedexRoot);
    free(owner);
}

void freeAllOwners() {
    // No owners - nothing to free
    if (ownerHead == NULL)
        return;

    // Iterate over all owners and free them
    OwnerNode *current = ownerHead;
    while (current != NULL && current != current->next) {
        OwnerNode *next = current->next;
        freeOwnerNode(current);
        current = next;
    }

    // Remove last remaining owner
    freeOwnerNode(current);

    // Reset ownerHead to NULL as required
    ownerHead = NULL;
}

void deletePokedex() {
    if (ownerHead == NULL) {
        printf("No existing Pokedexes to delete.\n");
        return;
    }

    // Print all pokedexes with indices by their order
    printf("\n=== Delete a Pokedex ===\n");

    OwnerNode *currentOwner = choosePokedex("Choose a Pokedex to delete by number: ");

    // Remove chosen owner from owners list
    printf("Deleting %s's entire Pokedex...\n", currentOwner->ownerName);
    /*
     If first owner is removed, save its next owner for after removal
     It should be NULL if there is only one owner, otherwise simply the next owner
    */
    OwnerNode* nextOwner = ownerHead == ownerHead->next ? NULL : ownerHead->next;
    freeOwnerNode(currentOwner);
    printf("Pokedex deleted.\n");

    // If first pokedex was removed, set head to the second (which now becomes first)
    if (currentOwner == ownerHead)
        ownerHead = nextOwner;
}

int main() {
    mainMenu();
    freeAllOwners();
    return 0;
}

#include "ex6.h"

// TODO define all my new funcs in h file and copy func documentation

// TODO malloc / realloc should be done with cast?

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

/**
 * @brief Prints given pokemon data in required format.
 * @param data pokemon data to print
 */
void printPokemonData(PokemonData *data) {
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           data->id,
           data->name,
           getTypeName(data->TYPE),
           data->hp,
           data->attack,
           data->CAN_EVOLVE == CAN_EVOLVE ? "Yes" : "No");
}

void printPokemonNode(PokemonNode *node) {
    // If given node is NULL, don't print it
    if (node == NULL)
        return;

    // Print pokemon node data
    printPokemonData(node->data);
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
            displayAlphabetical(owner->pokedexRoot);
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

/**
 * @brief Inserts all pokemons from given pokedex to a sorted pokemons array.
 * @param root root pokemon node
 * @param pokemons pointers array of pokemons
 * Sorts pokemons alphabetically during insertion to pokemons array.
 */
void preOrderSort(PokemonNode *root, PokemonData **pokemons) {
    // Base case: current BST node is NULL
    if (root == NULL) return;

    // Calculate insertion position and last pokemon index
    int position = 0;
    int lastIndex = 0;
    while (pokemons[lastIndex] != NULL) {
        // If current root pokemon's name should be after pokemon at position, increase pos by 1
        if (strcmp(root->data->name, pokemons[position]->name) > 0)
            position++;

        // Unconditionally increase last index
        lastIndex++;
    }

    // Move pokemons 1 step forward from insertion position till last pokemon
    if (pokemons[position] != NULL) {
        for (int i = lastIndex; i > position; i--)
            pokemons[i] = pokemons[i - 1];
    }

    // Insert root pokemon's data to current insertion position
    pokemons[position] = root->data;

    // Recursively visit left child of root
    preOrderSort(root->left, pokemons);
    // Recursively visit right child of root
    preOrderSort(root->right, pokemons);
}

void displayAlphabetical(PokemonNode *root) {
    // Allocate pokemons pointers array
    PokemonData **pokemons = malloc(sizeof(PokemonData*) * LAST_POKEMON_ID);
    if (pokemons == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Init pokemons pointers to NULL
    for (int i = 0; i < LAST_POKEMON_ID; i++)
        pokemons[i] = NULL;

    // Insert all pokemons from pokedex to pokemons array, and sort during insertion
    preOrderSort(root, pokemons);

    // Print alphabetically sorted pokemons
    for (int i = 0; pokemons[i] != NULL; i++)
        printPokemonData(pokemons[i]);

    // Free sorted pokemons array
    free(pokemons);
}

/**
 * @brief Inputs an owner from owners list.
 * @param prompt message for choosing pokedex
 * @return chosen pokedex
 */
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
    // Don't insert empty node into queue
    if (data == NULL)
        return;

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

    // If queue's front is NULL, set queue's rear to NULL to make sure insert works correctly
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

    return root;
}

PokemonNode *searchPokemonBFS(PokemonNode *root, int id) {
    // Allocate queue
    PokemonQueue *queue = malloc(sizeof(PokemonQueue));
    if (!queue) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Init queue with root pokemon node
    queue->front = NULL;
    queue->rear = NULL;
    queueInsert(queue, root);

    // Iterate BFS style on given root
    while (!isEmptyQueue(queue)) {
        // Save front QNode from queue
        PokemonNode *current = queuePop(queue);

        // Visit current node
        if (current->data->id == id) {
            // Empty queue
            while (!isEmptyQueue(queue))
                queuePop(queue);

            // Free queue and return pokemon with given id
            free(queue);
            return current;
        }

        // If current node has left child, add it to queue and visit it later
        if (current->left)
            queueInsert(queue, current->left);

        // If current node has right child, add it to queue and visit it later
        if (current->right)
            queueInsert(queue, current->right);
    }

    // Free queue and return NULL as no pokemon with given id was found in given pokedex
    free(queue);
    return NULL;
}

/**
 * @brief Returns minimum child of root, assuming BST is balanced.
 * @param root node to start search from
 * @return minimum value child of root
 */
PokemonNode *findMin(PokemonNode *root) {
    // Traverse to most-left child of root
    while (root->left != NULL)
        root = root->left;

    // Return most-left child (minimum child)
    return root;
}

PokemonNode *removeNodeBST(PokemonNode *root, int id) {
    // Base case: if root is NULL, the node to remove isn't on this branch
    if (root == NULL)
        return NULL;

    // Traverse the tree (post-order) to find the node to remove
    if (id < root->data->id) {
        // Traverse to root left child
        root->left = removeNodeBST(root->left, id);
    } else if (id > root->data->id) {
        // Traverse to root right child
        root->right = removeNodeBST(root->right, id);
    } else {
        // Found node with given id, and it has 1 or 0 children
        if (root->left == NULL || root->right == NULL) {
            // Removed node child becomes removed node's parent child / NULL if node has 0 children
            PokemonNode *newChild = root->left != NULL ? root->left : root->right;
            // Free removed node
            freePokemonNode(root);
            // Return new child and replace removed node with it
            return newChild;
        }

        // Found node has 2 children: find successor to replace him
        PokemonNode* successor = findMin(root->right);

        // Replace root's data with its successor's data
        root->data = successor->data;

        // Remove the successor node
        root->right = removeNodeBST(root->right, successor->data->id);
    }

    // Return root as new pokedex root
    return root;
}

void freePokemon(OwnerNode *owner) {
    // If no pokemons at pokedex, don't even input an ID
    if (owner->pokedexRoot == NULL) {
        printf("No Pokemon to release.\n");
        return;
    }

    // Input pokemon ID to release from pokedex
    int pokemonID = readIntSafe("Enter Pokemon ID to release: ");

    // Search for a pokemon with chosen ID in pokedex
    PokemonNode *pokemonNode = searchPokemonBFS(owner->pokedexRoot, pokemonID);
    if (pokemonNode == NULL) {
        printf("No Pokemon with ID %d found.\n", pokemonID);
        return;
    }

    // Remove chosen pokemon from pokedex
    printf("Removing Pokemon %s (ID %d).\n", pokemonNode->data->name, pokemonID);
    PokemonNode *newPokedexRoot = removeNodeBST(owner->pokedexRoot, pokemonID);
    owner->pokedexRoot = newPokedexRoot;
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

/**
 * @brief Calculate given pokemon's score based on the fight formula: (attack * 1.5 + hp * 1.2).
 * @param pokemon calculate its score
 * @return given pokemon's score
 */
float calcualatePokemonScore(PokemonData *pokemon) {
    return (float) pokemon->attack * FIGHT_ATTACK_FACTOR + (float) pokemon->hp * FIGHT_HP_FACTOR;
}

void pokemonFight(OwnerNode *owner) {
    // If pokedex is empty, don't even attempt to have a pokemon fight
    if (owner->pokedexRoot == NULL) {
        printf("Pokedex is empty.\n");
        return;
    }

    // Input two pokemon IDs
    int firstID = readIntSafe("Enter ID of the first Pokemon: ");
    int secondID = readIntSafe("Enter ID of the second Pokemon: ");

    // Search pokemons in pokedex
    PokemonNode *firstPokemon = searchPokemonBFS(owner->pokedexRoot, firstID);
    PokemonNode *secondPokemon = searchPokemonBFS(owner->pokedexRoot, secondID);

    // Validate both pokemons exist in owner's pokedex
    if (firstPokemon == NULL || secondPokemon == NULL) {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }

    // Calculate chosen pokemons score
    float firstScore = calcualatePokemonScore(firstPokemon->data);
    float secondScore = calcualatePokemonScore(secondPokemon->data);

    // Print fight statistics and result
    printf("Pokemon 1: %s (Score = %.2f)\n", firstPokemon->data->name, firstScore);
    printf("Pokemon 2: %s (Score = %.2f)\n", secondPokemon->data->name, secondScore);

    if (firstScore > secondScore)
        printf("%s wins!\n", firstPokemon->data->name);
    else if (firstScore < secondScore)
        printf("%s wins!\n", secondPokemon->data->name);
    else
        printf("It's a tie!\n");
}

void evolvePokemon(OwnerNode *owner) {
    if (owner->pokedexRoot == NULL) {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }

    int pokemonID = readIntSafe("Enter ID of Pokemon to evolve: ");

    PokemonNode *pokemon = searchPokemonBFS(owner->pokedexRoot, pokemonID);
    if (pokemon == NULL) {
        printf("No Pokemon with ID %d found.\n", pokemonID);
        return;
    }

    PokemonData *pokemonData = pokemon->data;
    if (!pokemon->data->CAN_EVOLVE) {
        printf("%s (ID %d) cannot evolve.\n", pokemonData->name, pokemonID);
        return;
    }

    // Check if evolved pokemon already exists in pokedex
    int evolvedID = pokemonID + 1;
    PokemonData *evolvedPokemon = (PokemonData *) &pokedex[evolvedID - 1];
    if (searchPokemonBFS(owner->pokedexRoot, evolvedID) != NULL) {
        printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).\n",
            evolvedID, evolvedPokemon->name, pokemonData->name, pokemonID);

        // Remove old pokemon and don't insert its evolved form
        printf("Removing Pokemon %s (ID %d).\n", pokemonData->name, pokemonID);
        owner->pokedexRoot = removeNodeBST(owner->pokedexRoot, pokemonID);
        return;
    }

    // Remove old pokemon before its evolved form is inserted into pokedex
    printf("Removing Pokemon %s (ID %d).\n", pokemonData->name, pokemonID);
    owner->pokedexRoot = removeNodeBST(owner->pokedexRoot, pokemonID);

    // Create pokemon node with evolved pokemon data
    PokemonNode *evolvedNode = createPokemonNode(evolvedPokemon);
    if (evolvedNode == NULL)
        return;

    // Insert evolved pokemon into pokedex
    owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, evolvedNode);
    printf("Pokemon evolved from %s (ID %d) to %s (ID %d).\n",
        pokemonData->name, pokemonID, evolvedPokemon->name, evolvedID);
}

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
                freePokemon(pokedex);
                break;
            case 4:
                pokemonFight(pokedex);
                break;
            case 5:
                evolvePokemon(pokedex);
                break;
            case 6:
                printf("Back to Main Menu.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

void swapOwnerData(OwnerNode* a, OwnerNode* b) {
    // Swap owners names
    char* temp = a->ownerName;
    a->ownerName = b->ownerName;
    b->ownerName = temp;

    // Swap owners pokedexes
    PokemonNode* pokedex = a->pokedexRoot;
    a->pokedexRoot = b->pokedexRoot;
    b->pokedexRoot = pokedex;
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
                swapOwnerData(current, current->next);

            // Move to next owner - 1 step forward
            current = current->next;
        }
    } while (end != start);

    // Set ownerHead to point to the sorted owners list
    ownerHead = start;
}

void sortOwners() {
    // Don't sort if there are no owners at all or 1 owner
    if (ownerHead == NULL || ownerHead->next == ownerHead) {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }

    // Sort owners
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

OwnerNode *findOwnerByName(const char *name) {
    // If no owners at all, there is no owner with given name
    if (ownerHead == NULL)
        return NULL;

    // Iterate over all owners
    OwnerNode *currentOwner = ownerHead;
    do {
        // Check whether given name is current owner's name
        if (strcmp(currentOwner->ownerName, name) == 0)
            return currentOwner;

        // Move on to next owner in list
        currentOwner = currentOwner->next;
    } while (currentOwner != ownerHead);

    // No owner found with given name;
    return NULL;
}

void linkOwnerInCircularList(OwnerNode *newOwner) {
    // If no owners yet, ownerHead should be newOwner, and point to itself from both directions
    if (ownerHead == NULL) {
        ownerHead = newOwner;
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
        return;
    }

    // Link newOwner to circular list by circular list logic
    OwnerNode *lastOwner = ownerHead->prev;
    lastOwner->next = newOwner;
    ownerHead->prev = newOwner;
    newOwner->next = ownerHead;
    newOwner->prev = lastOwner;
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

/**
 * @brief Derives pokemon's index from user's choice by a formula.
 * @param choice user's starter choice
 * @return chosen pokemon's index in pokemons array
 * Formula matches each possible choice correctly:
 * 1. Bulbasaur     -> index: 0
 * 2. Charmander    -> index: 3
 * 3. Squirtle      -> index: 6
 */
int getStarterPokemonID(int choice) {
    return (choice - 1) * 3;
}

void openPokedexMenu() {
    // Input owner name
    printf("Your name: ");
    char *name = getDynamicInput();

    // Validate name isn't duplicate of existing owner name
    if (findOwnerByName(name) != NULL) {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        free(name);
        return;
    }

    // Input starter pokemon and calculate its index in pokemons array
    printf("Choose Starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    int choice = readIntSafe("Your choice: ");
    int pokemonIndex = getStarterPokemonID(choice);

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

    // Link owner to circular list
    linkOwnerInCircularList(newOwner);
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
                mergePokedexMenu();
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

void removeOwnerFromCircularList(OwnerNode *target) {
    // Connect prev of target to its next
    target->next->prev = target->prev;
    target->prev->next = target->next;
}

void freeOwnerNode(OwnerNode *owner) {
    // Save new owner head to set it after freeing given owner
    OwnerNode* newOwnerHead = ownerHead;
    // If there is only 1 owner, set it to NULL after freeing it
    if (ownerHead == ownerHead->next)
        newOwnerHead = NULL;
    // If given owner is head and there are more owners, new owner head should be current head's next
    else if (ownerHead == owner)
        newOwnerHead = ownerHead->next;

    // Remove given owner from owners list
    removeOwnerFromCircularList(owner);

    // Free owner by its properties order
    free(owner->ownerName);
    freePokemonTree(owner->pokedexRoot);
    free(owner);

    // If first owner was removed, set head to the saved newOwnerHead
    ownerHead = newOwnerHead;
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

    // Input an owner index to delete
    OwnerNode *currentOwner = choosePokedex("Choose a Pokedex to delete by number: ");

    // Remove chosen owner from owners list
    printf("Deleting %s's entire Pokedex...\n", currentOwner->ownerName);
    freeOwnerNode(currentOwner);
    printf("Pokedex deleted.\n");
}

/**
 * @brief Merges given pokedexes and updates target pokedex to include all relevant pokemons from source.
 * @param target pokedex merge target
 * @param source pokedex merge source
 * @return new target pokedex root
 */
PokemonNode *mergePokedex(PokemonNode *target, PokemonNode *source) {
    // Allocate queue
    PokemonQueue *queue = malloc(sizeof(PokemonQueue));
    if (!queue) {
        printf("Memory allocation failed.\n");
        return target;
    }

    // Init queue with source pokedex root node
    queue->front = NULL;
    queue->rear = NULL;
    queueInsert(queue, source);

    // Iterate BFS style on given source pokedex
    while (!isEmptyQueue(queue)) {
        // Save front QNode from queue
        PokemonNode *current = queuePop(queue);

        // If current node has left child, add it to queue and visit it later
        if (current->left)
            queueInsert(queue, current->left);

        // If current node has right child, add it to queue and visit it later
        if (current->right)
            queueInsert(queue, current->right);

        // Create new node with current's data
        PokemonNode *insertedNode = createPokemonNode(current->data);
        // Safely continue to next node if new node is NULL for any reason
        if (insertedNode == NULL)
            continue;

        // Insert the new new node into target's pokedex
        PokemonNode *newTargetRoot = insertPokemonNode(target, insertedNode);

        // If current pokemon was already in target pokedex, free insertedNode to prevent data leak
        if (newTargetRoot == NULL)
            free(insertedNode);
        // Otherwise update target pokedex root to include inserted node
        else
            target = newTargetRoot;
    }

    // Finish BFS, free queue
    free(queue);
    // Return new target pokedex root
    return target;
}

void mergePokedexMenu() {
    // Don't try to merge if there are 0 or 1 owners
    if (ownerHead == NULL || ownerHead->next == ownerHead) {
        printf("Not enough owners to merge.\n");
        return;
    }

    // Display merge pokedexes menu
    printf("\n=== Merge Pokedexes ===\nEnter name of first owner: ");
    char *targetName = getDynamicInput();
    printf("Enter name of second owner: ");
    char *sourceName = getDynamicInput();

    // Find owners by given names
    OwnerNode *targetOwner = findOwnerByName(targetName);
    OwnerNode *sourceOwner = findOwnerByName(sourceName);

    // Merge given owners
    if (targetOwner == NULL || sourceOwner == NULL)
        // If no such owners with given names, don't merge any of them
        printf("One or both owners not found.\n");
    else if (targetOwner->pokedexRoot == NULL && sourceOwner->pokedexRoot == NULL) {
        // If both owners have empty pokedex, merge doesn't make sense
        printf("Both Pokedexes empty. Nothing to merge.\n");
    } else {
        // Valid owner names
        printf("Merging %s and %s...\n", targetName, sourceName);
        // Merge owners by required merge logic, update target owner pokedex root
        targetOwner->pokedexRoot = mergePokedex(targetOwner->pokedexRoot, sourceOwner->pokedexRoot);
        // Free source owner after all its pokemons were inserted into target owner pokedex
        freeOwnerNode(sourceOwner);
        printf("Merge completed.\nOwner '%s' has been removed after merging.\n", sourceName);
    }

    // Free given owner names
    free(targetName);
    free(sourceName);
}

int main() {
    mainMenu();
    freeAllOwners();
    return 0;
}

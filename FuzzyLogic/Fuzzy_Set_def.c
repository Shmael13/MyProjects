#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LOAD_FACTOR = 0.75

/*Node and HashSet Struct definitions*/
typedef struct Pairing {
    char * key;
    double val;
}Pairing;

typedef struct Node {
    char * key;
    double val;
    struct Node* next;
}Node;

typedef struct HashSet{
    Node** linkedList;
    int size;
    int capacity;
}HashSet;

/*
Hashfunction from Bernstien? 
Found online from http://www.cse.yorku.ca/~oz/hash.html 
Adapted to improve performance
*/
unsigned long hashFunction(unsigned char *str){
    unsigned long hash = 5387; //prime
    int c;

    while ( c =*(str++))
        hash = ((hash << 5) + hash) ^ c; //easy on cache - avoids clustering, collision avoidance - XOR adds "randomness", fast operations  
    return hash;
}

/*Creates new Hashset*/
HashSet* createHashSet(int init_capacity){
    HashSet* newHash = (HashSet*)malloc(sizeof(HashSet));
    if (newHash == NULL){
        fprintf(stderr, "Could not allocate new hashSet with capacity: %i\n", init_capacity);
        return NULL;
    }
    newHash->linkedList = (Node**)malloc(init_capacity * sizeof(Node*)); //could use Calloc here, but Malloc is faster
    if (newHash->linkedList == NULL){
        fprintf(stderr, "Could not allocate linked list of new hashSet with capacity: %i\n", init_capacity);
        free(newHash);                                                                                                      //CHECK
        return NULL;
    }
    for (int i = 0; i < init_capacity; i++){
        newHash->linkedList[i] = NULL;
    }
    newHash->capacity = init_capacity;
    newHash->size = 0;
    return newHash;
}

/*Checks if a Hashset Contains key*/
bool hashSetContainsKey(HashSet * set, char* keyToFind){
    int idx = hashFunction(keyToFind) % set->capacity;    
    Node* current = set->linkedList[idx]; //Start node of linkedList for key
    while (current){
        if (current->key == keyToFind){return true;}
        current = current->next;
    }
    return false;
}

/*Checks if a Hashset Contains Val
Returns a list of keys who correspond to that val
Number of keys is stored in the int pointer numKeys
*/
char** hashSetContainsVal(HashSet * set, double valToFind, int* numKeys){
    int setSize = set->size;
    char** keys = malloc(setSize * sizeof(char*));
    if (keys == NULL){
        fprintf(stderr, "Could not allocate key array of size %i in hashSetContainsVal\n", setSize);
        return NULL;
    }

    int currIdx = 0;
    for (int i =0; i < setSize; i++){
        Node* current = set->linkedList[i]; //Will have to search the whole set
        while (current){
            if (current->val == valToFind){
                keys[currIdx] = current->key;
                currIdx++;}
                
            current = current->next;

        }
    }
    *numKeys = currIdx;
    return keys;
}


/*Inserts a key, value pair into the hashset. */
int hashSetInsert(HashSet* set, char* insertKey, double insertVal, bool replace){
    if (insertVal < 0 || insertVal > 1){
        fprintf(stderr, "Invalid insertVal: %lf. It must be between 0 and 1\n", insertVal);
        return -1;
    }

    char * inKey = strdup(insertKey);
    if (inKey == NULL){
        fprintf(stderr, "Duplication of %s failed\n", insertKey);
        return -1;
    }

    int setSize = set->size;
    int setCap = set->capacity;
                                                                                                                    ///implement resizeSetstuff
    if (setSize+1>setCap){
        //resizeSet(set);
    }

    //create the node to insert
    Node * newNode = malloc(sizeof(Node));
    if (newNode == NULL){
        fprintf(stderr, "Could not allocate Node of size %lu in hashSetInsert\n", sizeof(Node));
        return -1;
    }
    newNode->key = inKey;
    newNode->val = insertVal;
    newNode->next = NULL;

    //figure out where to insert it
    unsigned long hashIdx = hashFunction(inKey)%set->capacity;
    Node* current = set->linkedList[hashIdx];

    //insert immediately if there is nothing in linked list
    if (current == NULL){
        set->linkedList[hashIdx] = newNode;
    }

    //find correct position in linked list and then insert(so that removal and lookup by value is faster for fails)
    else{
        // if we insert at the head
        if (current->val > insertVal){
            //error if equal and no replaces
            if (!strcmp(current->key,inKey) && !replace){
                free(inKey);
                free(newNode);
                fprintf(stderr, "%s already in set\n", inKey);
                return -1;}    
            set->linkedList[hashIdx] = newNode;
            newNode->next = current;
        }

        //otherwise, search through until newVal > currentVal or we reach the end of the list
        else{
            while ((current->next != NULL) && (current->val < insertVal)){
                current = current->next;
            }
            //replace current with newNode
            if (!strcmp(current->key,inKey) && !replace){
                free(inKey);
                free(newNode);
                fprintf(stderr, "%s is already in set\n", inKey);
                return -1;}    
            newNode->next = current->next;
            current->next = newNode;
        }
    }
    set->size++;
    return 1;
}

//bool resizeSet(HashSet* set){}


//for testing
int main(){
    // Assuming HashSet has been initialized
HashSet* set = createHashSet(3);

hashSetInsert(set, "this", 2, true);
hashSetInsert(set, "this", 1, false);
hashSetInsert(set, "this221", 1, true);

// Print the linked list to verify correct ordering
for (int i = 0; i < set->capacity; i++) {
    Node* current = set->linkedList[i];
    while (current != NULL) {
        printf("Key: %s, Value: %.2f\n", current->key, current->val);
        current = current->next;
    }
}

}



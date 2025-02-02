#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LOAD_FACTOR = 0.75


/*Node and HashSet Struct definitions*/
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
Adapted to improve
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
        fprintf(stderr, "Could not allocate new hashSet with capacity: %i", init_capacity);
        return NULL;
    }
    newHash->linkedList = (Node**)malloc(init_capacity * sizeof(Node*)); //could use Calloc here, but Malloc is faster
    if (newHash->linkedList == NULL){
        fprintf(stderr, "Could not allocate linked list of new hashSet with capacity: %i", init_capacity);
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
bool setContainsKey(HashSet * set, char* keyToFind){
    int idx = hashFunction(keyToFind);    
    Node* current = set->linkedList[idx]; //Start node of linkedList for key
    while (current){
        if (current->key == keyToFind){return true;}
        current = current->next;
    }
    return false;
}

/*Checks if a Hashset Contains Val*/
bool setContainsKey(HashSet * set, char* valToFind){
    int idx = hashFunction(valToFind);    
    Node* current = set->linkedList[idx]; //Start node of linkedList for key
    while (current){
        if (current->key == valToFind){return true;}
        current = current->next;
    }
    return false;
}



//for testing
int main(){
    createHashSet(10);

}



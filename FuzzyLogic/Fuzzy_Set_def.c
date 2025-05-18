#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LOAD_FACTOR 0.75
#define GROWTH_RATE 2

/*Node and HashSet Struct definitions*/
typedef struct Node {
    char * key;
    double val;
    struct Node* next;
    struct Node* 
}Node_list;

typedef struct HashSet{
    Node_list * linked_list;
    int size;
    int capacity;
}HashSet;
//All the functions I will use
HashSet* resizeSet(HashSet* oldSet);
unsigned long hashFunction(unsigned char *str);
HashSet* createHashSet(int init_capacity);
bool hashSetContainsKey(HashSet * set, char* keyToFind);
char** hashSetContainsVal(HashSet * set, double valToFind, int* numKeys);
int hashSetInsert(HashSet* set, char* insertKey, double insertVal, bool replace);

/*
Hashfunction from Bernstien? 
Found online from http://www.cse.yorku.ca/~oz/hash.html 
Adapted to improve performance
*/
unsigned long hashFunction(unsigned char *str){
    unsigned long hash = 5387; //prime
    int c;

    while ( (c = (*(str++)) ) )
        hash = ((hash << 5) + hash) ^ c; //easy on cache - avoids clustering, collision avoidance - XOR adds "randomness", fast operations  
    return hash;
}

/*Creates new Hashset*/
HashSet* createHashSet(int init_capacity){
    HashSet* newHash = (HashSet*)malloc(sizeof(HashSet));
    if (newHash == NULL){
        fprintf(stderr, "Could not allocate new hashSet\n");
        return NULL;
    }
    newHash->linked_list = (Node**)malloc(init_capacity * sizeof(Node*)); //could use Calloc here, but Malloc is faster
    if (newHash->linked_list == NULL){
        fprintf(stderr, "Could not allocate linked list of new hashSet with capacity: %i\n", init_capacity);
        free(newHash);
        return NULL;
    }
    for (int i = 0; i < init_capacity; i++){
        newHash->linked_list[i] = NULL;
    }
    newHash->capacity = init_capacity;
    newHash->size = 0;
    return newHash;
}

///*Checks if a Hashset Contains key*/
//bool hashSetContainsKey(HashSet * set, char* keyToFind){
//    int idx = hashFunction(keyToFind) % set->capacity;    
//    Node* current = set->linkedList[idx]; //Start node of linkedList for key
//    while (current){
//        if (!strcmp(current->key,keyToFind)){return true;}
//        current = current->next;
//    }
//    return false;
//}
//
///*Checks if a Hashset Contains Val
//Returns a list of keys who correspond to that val
//Number of keys is stored in the int pointer numKeys
//*/
//char** hashSetContainsVal(HashSet * set, double valToFind, int* numKeys){
//    int setSize = set->size;
//    char** keys = malloc(setSize * sizeof(char*));
//    if (keys == NULL){
//        fprintf(stderr, "Could not allocate key array of size %i in hashSetContainsVal\n", setSize);
//        return NULL;
//    }
//
//    int currIdx = 0;
//    for (int i =0; i < setSize; i++){
//        Node* current = set->linkedList[i]; //Will have to search the whole set
//        while (current){
//            if (current->val == valToFind){
//                keys[currIdx] = current->key;
//                currIdx++;}
//                
//            current = current->next;
//
//        }
//    }
//    *numKeys = currIdx;
//    return keys;
//}
//
//
///*Inserts a key, value pair into the hashset. */
//int hashSetInsert(HashSet* set, char* insertKey, double insertVal, bool replace){
//    if (insertVal < 0 || insertVal > 1){
//        fprintf(stderr, "Invalid insertVal: %lf. It must be between 0 and 1\n", insertVal);
//        return -1;
//    }
//
//    char * inKey = strdup(insertKey);
//    if (inKey == NULL){
//        fprintf(stderr, "Duplication of %s failed\n", insertKey);
//        return -1;
//    }
//
//    int setSize = set->size;
//
//    //resize if neccesary
//    if (((setSize+1)*LOAD_FACTOR)>=set->capacity){
//        puts("we got here?1");
//        HashSet* newSet = resizeSet(set);
//        if (newSet == NULL){
//            fprintf(stderr, "Could not resize when inserting");
//            return -1;
//        }
//        *set = *newSet;
//        free(newSet); //fix freeing
//    }
//    
//
//
//
//
//    int setCap = set->capacity;
//    
//
//    //create the node to insert
//    Node * newNode = malloc(sizeof(Node));
//    if (newNode == NULL){
//        free(inKey);
//        fprintf(stderr, "Could not allocate Node of size %lu in hashSetInsert\n", sizeof(Node));
//        return -1;
//    }
//    newNode->key = inKey;
//    newNode->val = insertVal;
//    newNode->next = NULL;
//
//    //figure out where to insert it
//    unsigned long hashIdx = hashFunction(inKey)%set->capacity;
//    Node* current = set->linkedList[hashIdx];
//
//    //insert immediately if there is nothing in linked list
//    if (current == NULL){
//        set->linkedList[hashIdx] = newNode;
//    }
//
//    //find correct position in linked list and then insert(so that removal and lookup by value is faster for fails)
//    else{
//        // if we insert at the head
//        if (current->val > insertVal){
//            //error if equal and no replaces
//            if (!strcmp(current->key,inKey) && !replace){
//                free(inKey);
//                //fix freeing
//                free(newNode);
//                fprintf(stderr, "%s already in set\n", inKey);
//                return -1;}    
//            set->linkedList[hashIdx] = newNode;
//            newNode->next = current;
//        }
//
//        //otherwise, search through until newVal > currentVal or we reach the end of the list
//        else{
//            while ((current->next != NULL) && (current->val < insertVal)){
//                current = current->next;
//            }
//            //replace current with newNode
//            if (!strcmp(current->key,inKey) && !replace){
//                free(inKey);
//                //fix freeing
//                free(newNode);
//                fprintf(stderr, "%s is already in set\n", inKey);
//                return -1;}    
//            newNode->next = current->next;
//            current->next = newNode;
//        }
//    }
//    set->size++;
//    return 1;
//}
//
//HashSet* resizeSet(HashSet* oldSet){
//    //init newSet
//    int newCapacity = oldSet->capacity * GROWTH_RATE;
//    HashSet* newSet = createHashSet(newCapacity);
//    if (newSet == NULL){
//        fprintf(stderr, "Could not resize the hashset\n");
//        return NULL;
//    }
//    int oldCapacity = oldSet->capacity;
//
//    for (int i = 0; i < oldCapacity; i++){
//        Node* current = oldSet->linkedList[i];
//        while(current != NULL){
//            char * oldKey = current->key;
//            double oldVal = current->val;
//            unsigned long hashIdx = hashFunction(oldKey) % newCapacity;
//            hashSetInsert(newSet, oldKey, oldVal, false);
//            current = current->next;
//        }
//    }
//    puts("we got here 2");
//
//    
//    //fix freeing
//    free(oldSet);
//    return newSet;
//}
//
//
//for testing
int main(){
    // Assuming HashSet has been initialized
  HashSet* set = createHashSet(1);

  //hashSetInsert(set, "this", 1, true);

  //// Print the linked list to verify correct ordering
  //for (int i = 0; i < set->capacity; i++) {
  //    Node* current = set->linkedList[i];
  //    while (current != NULL) {
  //        printf("Key: %s, Value: %.2f\n", current->key, current->val);
  //        current = current->next;
  //    }
  //}

  //hashSetInsert(set, "this1", 1, true);

  //puts("");
  //// Print the linked list to verify correct ordering
  //for (int i = 0; i < set->capacity; i++) {
  //    Node* current = set->linkedList[i];
  //    while (current != NULL) {
  //        printf("Key: %s, Value: %.2f\n", current->key, current->val);
  //        current = current->next;
  //    }
  //}

  //puts("Segfault from next instruction");
  //printf("Curr Set Capacity: %i\n\n", set->capacity);


  //hashSetInsert(set, "this221", 1, true);

  //// Print the linked list to verify correct ordering
  //for (int i = 0; i < set->capacity; i++) { 
  //    Node* current = set->linkedList[i];
  //    while (current != NULL) {
  //        printf("Key: %s, Value: %.2f\n", current->key, current->val);
  //        current = current->next;
  //    }
  //}

}



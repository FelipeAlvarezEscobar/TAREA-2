#ifndef HashMap_h
#define HashMap_h

// Definición de las estructuras
typedef struct HashMapEntry {
    char *key;
    void *value;
    struct HashMapEntry *next;
} HashMapEntry;

typedef struct {
    HashMapEntry **table;
    int capacity;
    int size;
} HashMap;

// Definición de ValuesList
typedef struct {
    void **values;
    int size;
    int capacity;
} ValuesList;

HashMap *createMap(int capacity);
void insertMap(HashMap *map, const char *key, void *value);
void *searchMap(HashMap *map, const char *key);
void eraseMap(HashMap *map, const char *key);
void freeMap(HashMap *map);
ValuesList *getHashMapValues(HashMap *map); // Cambio de nombre aquí
List *getValues(HashMap *map);

#endif /* HashMap_h */

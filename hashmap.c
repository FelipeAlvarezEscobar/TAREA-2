#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct {
    void **values;
    int size;
    int capacity;
} ValuesList;

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
    Pair **buckets;
    long size; // cantidad de datos/pairs en la tabla
    long capacity; // capacidad de la tabla
    long current; // índice del último dato accedido
};

// Función auxiliar para inicializar la lista de valores.
static ValuesList *createValuesList(int initialCapacity) {
    ValuesList *list = (ValuesList *)malloc(sizeof(ValuesList));
    if (list == NULL) {
        return NULL; // Fallo de asignación de memoria
    }

    list->values = (void **)malloc(initialCapacity * sizeof(void *));
    if (list->values == NULL) {
        free(list);
        return NULL; // Fallo de asignación de memoria
    }

    list->size = 0;
    list->capacity = initialCapacity;
    return list;
}

// Función auxiliar para agregar un valor a la lista de valores.
static void addToValuesList(ValuesList *list, void *value) {
    if (list->size >= list->capacity) {
        // Necesitamos redimensionar la lista.
        int newCapacity = list->capacity * 2;
        void **newValues = (void **)realloc(list->values, newCapacity * sizeof(void *));
        if (newValues == NULL) {
            // Manejo de error en caso de fallar la asignación de memoria.
            return;
        }
        list->values = newValues;
        list->capacity = newCapacity;
    }

    list->values[list->size] = value;
    list->size++;
}

// Función auxiliar para liberar la memoria de la lista de valores.
static void freeValuesList(ValuesList *list) {
    if (list != NULL) {
        free(list->values);
        free(list);
    }
}

// Función para obtener una lista de valores de un HashMap.
ValuesList *getHashMapValues(HashMap *map) {
    if (map == NULL) {
        return NULL;
    }

    ValuesList *valuesList = createValuesList(map->size);
    if (valuesList == NULL) {
        return NULL; // Fallo de asignación de memoria
    }

    for (int i = 0; i < map->capacity; i++) {
        HashMapEntry *entry = map->table[i];
        while (entry != NULL) {
            addToValuesList(valuesList, entry->value);
            entry = entry->next;
        }
    }

    return valuesList;
}

Pair *createPair(char *key, void *value) {
    Pair *new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char *key, long capacity) {
    unsigned long hash = 0;
    char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash * 32 + tolower(*ptr);
    }
    return hash % capacity;
}

int is_equal(void *key1, void *key2) {
    if (key1 == NULL || key2 == NULL)
        return 0;
    if (strcmp((char *)key1, (char *)key2) == 0)
        return 1;
    return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
    long codigo = hash(key, map->capacity);
    int count = 0;

    if (map->size > (map->capacity * 0.7)) {
        enlarge(map); // se agranda la capacidad del mapa si está llena hasta el 70%
    }
    while (map->buckets[codigo] != NULL && count < map->capacity) {

        if (is_equal(map->buckets[codigo]->key, key)) {
            return;
        }
        codigo = (codigo + 1) % map->capacity;
        count++;
    }
    if (count == map->capacity) {
        return;
    }

    map->buckets[codigo] = createPair(key, value);
    map->current = codigo;
    map->size++;
}

void enlarge(HashMap *map) {
    enlarge_called = 1; // no borrar (testing purposes)
    Pair **old_buckets = map->buckets;
    int old_capacity = map->capacity;

    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));

    map->size = 0;

    for (int i = 0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }

    free(old_buckets);
}

HashMap *createMap(long capacity) {
    HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
    mapa->buckets = (Pair **)malloc(capacity * sizeof(Pair **));
    mapa->size = 0;
    mapa->capacity = capacity;
    mapa->current = -1;
    return mapa;
}

void eraseMap(HashMap *map, char *key) {
    Pair *dato = searchMap(map, key);
    if (dato != NULL) {
        dato->key = NULL;
        map->size--;
    }
}

Pair *searchMap(HashMap *map, char *key) {
    long codigo = hash(key, map->capacity);
    int i = 0;
    while (i < map->capacity) {
        if (map->buckets[codigo] == NULL) {
            break;
        }
        if (is_equal(map->buckets[codigo]->key, key)) {
            map->current = codigo;
            return map->buckets[codigo];
        }
        codigo = (codigo + 1) % map->capacity;
        i++;
    }
    return NULL;
}

Pair *firstMap(HashMap *map) {
    map->current = 0;

    while (map->current < map->capacity) {
        if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL) {
            map->current++;
            return (map->buckets[map->current - 1]);
        }
        map->current++;
    }
    return NULL;
}

Pair *nextMap(HashMap *map) {

    while (map->current < map->capacity) {
        if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL) {
            map->current++;
            return map->buckets[(map->current) - 1];
        }
        map->current++;
    }
    return NULL;
}

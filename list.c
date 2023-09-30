#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"

struct Node {
    void * data;
    struct Node * next;
    struct Node * prev;
};

struct List {
    struct Node * head;
    struct Node * tail;
    struct Node * current;
};

Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
     List * new = (List *)malloc(sizeof(List));
     assert(new != NULL);
     new->head = new->tail = new->current = NULL;
     return new;
}

void * firstList(List * list) {
    if (list == NULL || list->head == NULL) return NULL;
    list->current = list->head;
    return (void *)list->current->data;
}

void * nextList(List * list) {
    if (list == NULL || list->head == NULL || list->current == NULL || list->current->next == NULL) return NULL;
    list->current = list->current->next;
    return (void *)list->current->data;
}

void * lastList(List * list) {
    if (list == NULL || list->head == NULL) return NULL;
    list->current = list->tail;
    return (void *)list->current->data;
}

void * prevList(List * list) {
    if (list == NULL || list->head == NULL || list->current == NULL || list->current->prev == NULL) return NULL;
    list->current = list->current->prev;
    return (void *)list->current->data;
}

void pushFront(List * list, void * data) {
    assert(list != NULL);
    
    Node * new = createNode(data);
    
    if (list->head == NULL) {
        list->tail = new;
    } else {
        new->next = list->head;
        list->head->prev = new;
    }
    
    list->head = new;
}

void pushBack(List * list, void * data) {
    list->current = list->tail;
    if(list->current==NULL) pushFront(list,data);
    else pushCurrent(list,data);
}

void pushCurrent(List * list, void * data) {
    assert(list != NULL && list->current !=NULL);
    Node * new = createNode(data);

    if(list->current->next)
        new->next = list->current->next;
    new->prev = list->current;

    if(list->current->next)
        list->current->next->prev = new;
    list->current->next = new;

    if(list->current==list->tail)
        list->tail=new;

}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) {
    assert(list != NULL || list->head != NULL);
    
    if (list->current == NULL) return NULL;
    
    Node * aux = list->current;
    
    if (aux->next != NULL) 
        aux->next->prev = aux->prev;
    
    
    if (aux->prev != NULL) 
        aux->prev->next = aux->next;
    
    
    void * data = (void *)aux->data;
    
    if(list->current == list->tail)
        list->tail = list->current->prev;

    if(list->current == list->head)
        list->head = list->current->next;
        
    list->current = aux->prev;



    
    free(aux);
    
    return data;
}

void cleanList(List * list) {
    assert(list != NULL);
    
    while (list->head != NULL) {
        popFront(list);
    }
}

void * searchList(List * list, const void * data, int (*cmp)(const void *, const void *)) {
    assert(list != NULL && cmp != NULL);

    list->current = list->head;
    while (list->current != NULL) {
        if (cmp(list->current->data, data) == 0) {
            return (void *)list->current->data;
        }
        list->current = list->current->next;
    }
    return NULL;
}

// Función para agregar un elemento al final de la lista
void appendList(List * list, const void * data) {
    assert(list != NULL);

    Node * newNode = createNode((void *)data);
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->current = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
}

List * getValues(List * list) {
    // Crea una nueva lista para almacenar los valores
    List * valuesList = createList();

    // Recorre la lista original y agrega los valores a la nueva lista
    list->current = list->head;
    while (list->current != NULL) {
        pushBack(valuesList, list->current->data);
        list->current = list->current->next;
    }

    // Restaura el puntero 'current' de la lista original
    list->current = list->head;

    return valuesList;
}

List * getValues(HashMap * map);


// Implementación de next para List
void *next(List *list) {
    if (list == NULL || list->head == NULL || list->current == NULL || list->current->next == NULL) return NULL;
    list->current = list->current->next;
    return (void *)list->current->data;
}

void insert(List *lp, const void *data){
  Node *new_node = malloc(sizeof(Node));
  new_node->data = data;
  new_node->next = NULL;
  if (lp->head == NULL) {
    lp->head = new_node;
    lp->tail = new_node;
  }
  else {
    lp->tail->next = new_node;
    lp->tail = new_node;
  }

}

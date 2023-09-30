#ifndef List_h
#define List_h

typedef struct List List;

List * createList(void);

void * firstList(List * list);

void *next(List *list);

void * lastList(List * list);

void * prevList(List * list);

void pushFront(List * list,  void * data);

void pushBack(List * list,  void * data);

void pushCurrent(List * list,  void * data);

void * popFront(List * list);

void * popBack(List * list);

void * popCurrent(List * list);

void cleanList(List * list);

void insert(List *lp, const void *data);

void *searchList(List *list, const void *data, int (*cmp)(const void *, const void *));

void appendList(List *list, const void *data);

#endif /* List_h */

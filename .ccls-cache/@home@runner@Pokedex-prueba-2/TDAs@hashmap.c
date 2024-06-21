#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if(map==NULL || key==NULL || value==NULL) return;
  long pos = hash(key,map->capacity);
  while(map->buckets[pos]!=NULL && map->buckets[pos]->key!=NULL){
    pos = (pos+1)%map->capacity;
  }
  map->buckets[pos] = createPair(key,value);
  map->size++;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  if(map==NULL) return;

  Pair ** oldBuckets = map->buckets;
  long oldCapacity = map->capacity;

  map->capacity = map->capacity*2;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0;

  for (long i = 0; i < oldCapacity; i++){
    if(oldBuckets[i]!=NULL && oldBuckets[i]->key!=NULL){
      insertMap(map,oldBuckets[i]->key,oldBuckets[i]->value);
      map->size += 1;
    }
  }
  free(oldBuckets);
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {
  if (map == NULL || key == NULL) return;

  long pos = hash(key,map->capacity);
  while(map->buckets[pos]!=NULL && map->buckets[pos]->key!=NULL){
    map->current = pos;
    if(strcmp(map->buckets[pos]->key, key) == 0) break;
    pos = (pos + 1) % map->capacity;
  }

  if (map->buckets[pos] == NULL) return;
  else{
    map->buckets[pos]->key = NULL;
    (map->size)--;
  }
}

Pair * searchMap(HashMap * map,  char * key){
  if(map==NULL || key==NULL) return NULL;
  long pos = hash(key,map->capacity);
  while(map->buckets[pos]!=NULL && map->buckets[pos]->key!=NULL && map->buckets[pos]->value != NULL){
    map->current = pos;
    if (strcmp(map->buckets[pos]->key, key)==0) return map->buckets[pos];
    //else if (map->buckets[pos]->key==NULL) return NULL;
    pos = (pos+1)%map->capacity;
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  for (int i = 0 ; i < map->capacity ; i++){
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for (long i = map->current + 1 ; i < map->capacity ; i++){
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}

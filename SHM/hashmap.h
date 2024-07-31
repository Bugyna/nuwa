#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define u64 uint64_t

#ifndef SHM_DEBUG
#define SHM_DEBUG(s, ...) printf(s __VA_ARGS__);
#endif

#define SHM_LOG(...) SHM_DEBUG("SHM LOG: ", __VA_ARGS__);
#define SHM_ERROR(...) SHM_DEBUG("SHM ERROR: ", __VA_ARGS__); exit(-1)

#define SHM_ASSERT(cond, err, ...) if (!(cond)) { printf(err __VA_ARGS__); exit(-1); }

#ifdef SHM_REPORTING
#else
#define SHM_WARN(...) 
#endif

int __hash(const char* key)
{
	SHM_ASSERT(key != NULL, "trying to hash an empty key");

	int mask = 1 << strlen(key);
	for (unsigned int i = 0; i < strlen(key); i++) {
		mask += (int)key[i];
	}
	return mask ^ (mask >> 4);
}
#ifndef hash
#define hash __hash
#endif

#define __ITERATE_HASHMAP(MAP_TYPE, MAP, VAL_TYPE, KEY)\
MAP_TYPE##_BUCKET* BUCKET = MAP_TYPE##_GET_BUCKET(MAP, KEY);\
VAL_TYPE* val = NULL;\
if (BUCKET != NULL) val = BUCKET->val;\
for (; BUCKET != NULL && (val = BUCKET->val) != NULL; BUCKET = BUCKET->next)


#define __ITERATE_HASHMAP_AT_INDEX(MAP_TYPE, MAP, VAL_TYPE, N)\
MAP_TYPE##_BUCKET* BUCKET = MAP_TYPE##_GET_BUCKET_AT_INDEX(MAP, N);\
VAL_TYPE* val = NULL;\
if (BUCKET != NULL) val = BUCKET->val;\
for (; BUCKET != NULL && (val = BUCKET->val) != NULL; BUCKET = BUCKET->next)


#define __ITERATE_HASHMAP_VN(MAP_TYPE, MAP, VAL_TYPE, VAL_NAME, KEY)\
MAP_TYPE##_BUCKET* BUCKET = MAP_TYPE##_GET_BUCKET(MAP, KEY);\
VAL_TYPE* VAL_NAME = NULL;\
if (BUCKET != NULL) VAL_NAME = BUCKET->val;\
for (; BUCKET != NULL && (VAL_NAME = BUCKET->val) != NULL; BUCKET = BUCKET->next)


#define __ITERATE_HASHMAP_AT_INDEX_VN(MAP_TYPE, MAP, VAL_TYPE, VAL_NAME, N)\
MAP_TYPE##_BUCKET* BUCKET = MAP_TYPE##_GET_BUCKET_AT_INDEX(MAP, N);\
VAL_TYPE* VAL_NAME = NULL;\
if (BUCKET != NULL) VAL_NAME = BUCKET->val;\
for (; BUCKET != NULL && (VAL_NAME = BUCKET->val) != NULL; BUCKET = BUCKET->next)

#define ITERATE_HASHMAP(MAP_TYPE, MAP, VAL_TYPE, KEY, EXPR)\
{\
	__ITERATE_HASHMAP(MAP_TYPE, MAP, VAL_TYPE, KEY) EXPR\
}

#define ITERATE_HASHMAP_AT_INDEX(MAP_TYPE, MAP, VAL_TYPE, N, EXPR)\
{\
	__ITERATE_HASHMAP_AT_INDEX(MAP_TYPE, MAP, VAL_TYPE, N) EXPR\
}


#define DEF_HASHMAP(NAME, VAL_TYPE, ...)\
typedef struct NAME NAME;\
typedef struct NAME##_BUCKET NAME##_BUCKET;\
struct NAME##_BUCKET\
{\
	char* key;\
	VAL_TYPE* val;\
\
	NAME##_BUCKET* next;\
};\
struct NAME \
{\
	NAME##_BUCKET* list;\
	size_t size, occupied;\
	__VA_ARGS__\
};\
void NAME##_INIT(NAME* hm, int size);\
NAME##_BUCKET* NAME##_GET_BUCKET_SPECIAL(NAME* hm, char* key);\
NAME##_BUCKET* NAME##_GET_BUCKET(NAME* hm, char* key);\
NAME##_BUCKET* NAME##_GET_BUCKET_AT_INDEX(NAME* hm, int n);\
VAL_TYPE* NAME##_GET(NAME* hm, char* key);\
VAL_TYPE* NAME##_ADD(NAME* hm, char* key, VAL_TYPE* val);\
void NAME##_ADD_AT_INDEX(NAME* hm, int n, VAL_TYPE* val);\
void NAME##_ADD_COPY(NAME* hm, char* key, VAL_TYPE* val);\
void NAME##_ADD_COPY_AT_INDEX(NAME* hm, int n, VAL_TYPE* val);\
void NAME##_ADD_DUPLICATE(NAME* hm, char* key, VAL_TYPE* val);\
void NAME##_REMOVE(NAME* hm, char* key);\


#define IMPL_HASHMAP(NAME, VAL_TYPE, ...)\
void NAME##_INIT(NAME* hm, int size)\
{\
	if (hm == NULL) { SHM_ERROR("trying to initialize null pointer %d %s\n\n", __LINE__, __FILE__); }\
	if (size < 0) size = 20;\
	SHM_LOG("Initializing hashmap with size of: %ld\n", size);\
	hm->list = calloc(size, sizeof(NAME##_BUCKET));\
	hm->size = size;\
	hm->occupied = 0;\
}\
\
NAME##_BUCKET* NAME##_GET_BUCKET_SPECIAL(NAME* hm, char* key)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
	check:\
	if (b != NULL && b->val && b->key != NULL && strcmp(b->key, key)) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		SHM_WARN("'%s' could not be found\n", key);\
		return NULL;\
	}\
\
	return b;\
}\
NAME##_BUCKET* NAME##_GET_BUCKET(NAME* hm, char* key)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
	check:\
	if (b != NULL && b->val && b->key != NULL && strcmp(b->key, key)) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		SHM_WARN("'%s' could not be found\n", key);\
		return NULL;\
	}\
\
	return b;\
}\
NAME##_BUCKET* NAME##_GET_BUCKET_AT_INDEX(NAME* hm, int n)\
{\
	char* key = calloc(20, sizeof(char));\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
	check:\
	if (b != NULL && b->key != NULL && strcmp(b->key, key) && b->val != NULL) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		SHM_WARN("'%s' could not be found\n", key);\
		free(key);\
		return NULL;\
	}\
\
	free(key);\
	return b;\
}\
VAL_TYPE* NAME##_GET(NAME* hm, char* key)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && strcmp(b->key, key)) { b = b->next; goto check;}\
	if (b == NULL || b->val == NULL || b->key == NULL) {\
		SHM_WARN("'%s' could not be found\n", key);\
		return NULL;\
	}\
\
	return b->val;\
}\
VAL_TYPE* NAME##_ADD(NAME* hm, char* key, VAL_TYPE* val)\
{\
	size_t h = hash(key);\
	NAME##_BUCKET* b = &hm->list[(h % hm->size)];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = calloc(strlen(key), 1);\
	strcpy(b->key, key);\
	b->val = val;\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
	return b->val;\
}\
\
void NAME##_ADD_AT_INDEX(NAME* hm, int n, VAL_TYPE* val)\
{\
	char* key = calloc(20, sizeof(char));\
	sprintf(key, "%d", n);\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = key;\
	b->val = val;\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
void NAME##_ADD_COPY(NAME* hm, char* key, VAL_TYPE* val)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = key;\
	b->val = malloc(sizeof(VAL_TYPE));\
	memcpy(b->val, val, sizeof(VAL_TYPE));\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
void NAME##_ADD_COPY_AT_INDEX(NAME* hm, int n, VAL_TYPE* val)\
{\
	char* key = calloc(20, sizeof(char));\
	sprintf(key, "%d", n);\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = key;\
	b->val = malloc(sizeof(VAL_TYPE));\
	memcpy(b->val, val, sizeof(VAL_TYPE));\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
void NAME##_ADD_DUPLICATE(NAME* hm, char* key, VAL_TYPE* val)\
{\
	int h = hash(key) % hm->size;\
	NAME##_BUCKET* b = &hm->list[h];\
\
	int i = 0;\
	check:\
	if (b != NULL && b->val != NULL && b->key != NULL && b->next != NULL) { b = b->next; goto check;}\
\
	b->key = key;\
	b->val = malloc(sizeof(VAL_TYPE));\
	memcpy(b->val, val, sizeof(VAL_TYPE));\
\
	if (b->next == NULL) {\
		b->next = calloc(1, sizeof(NAME##_BUCKET));\
		b->next->key = NULL;\
	}\
	hm->occupied++;\
}\
\
void NAME##_REMOVE(NAME* hm, char* key)\
{\
	NAME##_BUCKET* b = NAME##_GET_BUCKET(hm, key);\
	NAME##_BUCKET* tmp = b->next;\
	if (tmp != NULL && tmp->key != NULL) b = tmp;\
	else {\
		b->val = NULL;\
		b->key = NULL;\
	}\
\
	hm->occupied--;\
}



#define DEFINE_HASHMAP(NAME, VAL_TYPE, ...)\
DEF_HASHMAP(NAME, VAL_TYPE, __VA_ARGS__)\
IMPL_HASHMAP(NAME, VAL_TYPE, __VA_ARGS__)


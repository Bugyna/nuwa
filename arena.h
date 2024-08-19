#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifndef ARENA_H_
#define ARENA_H_


typedef struct ARENA ARENA;
struct ARENA {
	char* buf;
	size_t index, available;
};

void ARENA_REALLOC(ARENA* a, size_t new_size)
{
	printf("ARENA REALLOC\n");
	a->available = new_size;
	a->buf = realloc(a->buf, a->available);
	memset(a->buf+a->index, 0, a->available-a->index);
}

char* ARENA_ALLOC(ARENA* a, size_t size)
{
	int index = a->index;
	a->index += size;
	if (a->index >= a->available) {
		ARENA_REALLOC(a, a->available+size);
	}
	return &a->buf[index];
}

void ARENA_INIT(ARENA* a, size_t size)
{
	a->buf = malloc(size);
	memset(a->buf, '\0', size);
	a->available = size;
	a->index = 0;
}

void ARENA_FREE(ARENA* a)
{
	a->index = 0;
}




#define ITERATE_VECTOR(VEC, TYPE, VAL)\
TYPE* VAL = &VEC.items[0];\
for (int i = 1; i <= VEC.index; VAL = &VEC.items[i++])

#define DEFINE_VECTOR(NAME, TYPE)                                    \
typedef struct NAME NAME;                                            \
struct NAME {                                                        \
	TYPE* items;                                                        \
	size_t index, available;                                          \
};                                                                   \
                                                                                   \
void NAME##_REALLOC(NAME* vec, size_t new_size)                                    \
{                                                                                  \
	vec->available = new_size;                                                      \
	TYPE* tmp = vec->items;                                                         \
	vec->items = realloc(vec->items, vec->available*sizeof(TYPE));                               \
	printf("REALLOC FOR: " #NAME " | new size: %d | at index: %d | old ptr -> new ptr: %p -> %p\n", vec->available, vec->index, tmp, vec->items);                                             \
	if (vec->items == NULL) { NUWA_ERROR("realloc failed for: " #NAME "\n"); }         \
	memset(vec->items + vec->index, 0, vec->available - vec->index);                   \
}                                                                                  \
                                                                                   \
TYPE* NAME##_ADD(NAME* vec, TYPE p)                                  \
{                                                                    \
	printf(#NAME ": add %d\n", vec->index);                             \
	if (vec->index+1 >= vec->available) {                               \
		NAME##_REALLOC(vec, vec->available*2);                         \
	}                                                                 \
	vec->items[vec->index] = p;                                       \
	return &vec->items[vec->index++];                                   \
}                                                                    \
TYPE* NAME##_INSERT(NAME* vec, TYPE p, size_t index)                                  \
{                                                                    \
	if (index >= vec->index) return NAME##_ADD(vec, p);               \
	printf(#NAME ": insert\n");                                       \
	vec->index++;                                                     \
	if (vec->index+1 >= vec->available) {                               \
		NAME##_REALLOC(vec, vec->available*2);                         \
	}                                                                 \
	for (int i = vec->index; i > index+1; i--) {\
		printf("wahtat: %d\n", i);\
		vec->items[i] = vec->items[i-1];\
	}\
	vec->items[index] = p;                                            \
	return &vec->items[vec->index-1];                                   \
}                                                                    \
TYPE* NAME##_INSERT_FORCE(NAME* vec, TYPE p, size_t index)                                  \
{                                                                    \
	vec->index++;                                                     \
	if (vec->index+1 >= vec->available) {                               \
		NAME##_REALLOC(vec, vec->available*2);                         \
	}                                                                 \
	memcpy(vec->items + index + 1, vec->items + index, vec->index - index);  \
	vec->items[index] = p;                                            \
	return &vec->items[vec->index-1];                                   \
}                                                                    \
TYPE* NAME##_REPLACE(NAME* vec, TYPE p, size_t index)                                  \
{                                                                    \
	if (index >= vec->index) return NAME##_ADD(vec, p);               \
	printf(#NAME ": replace\n");                                       \
	vec->items[index] = p;                                            \
	return &vec->items[index];                                        \
}                                                                    \
void NAME##_REMOVE(NAME* vec, size_t index)                          \
{                                                                    \
	if (index >= vec->index || index < 0) return;                     \
	for (int i = index; i < vec->index - 1; i++) {                    \
        vec->items[i] = vec->items[i + 1];                               \
        vec->items[i + 1] = (TYPE){0};                              \
   }                                                                 \
	vec->index--;                                                     \
}                                                                    \
void NAME##_DELETE(NAME* vec, TYPE ptr)                              \
{                                                                    \
	int index = -1;                                                   \
	for (int i = 0; i <= vec->index; i++) {                            \
		TYPE val = vec->items[i];                                         \
		if (!memcmp(&val, &ptr, sizeof(TYPE))) { index = i; break; }                          \
	}                                                                 \
	NAME##_REMOVE(vec, index);                                        \
}                                                                    \
void NAME##_POP(NAME* vec)                                           \
{                                                                    \
	NAME##_REMOVE(vec, vec->index);                                   \
}                                                                    \
TYPE* NAME##_GET(NAME* vec, size_t index)                            \
{                                                                    \
	if (index+1 > vec->index) return NULL;               \
	return &vec->items[index];                                          \
}                                                                    \
void NAME##_SET(NAME* vec, TYPE* val, size_t index)                  \
{                                         \
	vec->available = index; vec->index = index-1;                     \
	vec->items = val;                                                   \
}                                                                    \
void NAME##_SET_VAL(NAME* vec, TYPE val, size_t index)               \
{                                                                    \
	vec->items[index] = val;                                              \
}                                                                    \
void NAME##_INIT(NAME* vec, size_t size)                             \
{                                                                    \
	vec->items = calloc(size, sizeof(TYPE));                            \
	vec->available = size;                                            \
	vec->index = 0;                                                   \
}                                                                    \
void NAME##_INIT_WITH_VAL(NAME* vec, TYPE* val, size_t size, size_t index)         \
{                                                                    \
	vec->items = val;                                                   \
	vec->available = size;                                            \
	vec->index = index;                                               \
}                                             


#endif
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




#define ITERATE_VECTOR(VEC, TYPE, VAL)\
TYPE* VAL = &VEC.vec[0];\
for (int i = 0; i <= VEC.index; VAL = &VEC.vec[i++])

#define DEFINE_VECTOR(NAME, TYPE)\
typedef struct NAME NAME;                    \
struct NAME {                                \
	TYPE* vec;                                \
	size_t index, available;                  \
};                                           \
TYPE* NAME##_ADD(NAME* vec, TYPE p)           \
{                                            \
	vec->vec[vec->index++] = p;               \
	if (vec->index >= vec->available) {       \
		vec->available *= 2;                   \
		vec->vec = realloc(vec->vec, vec->available);    \
	}                                         \
	return &vec->vec[vec->index-1];                \
}                                            \
void NAME##_REMOVE(NAME* vec, size_t index)  \
{                                            \
	                                          \
}                                            \
TYPE* NAME##_GET(NAME* vec, size_t index)     \
{                                             \
	return &vec->vec[index];                    \
}                                             \
void NAME##_INIT(NAME* vec, size_t size)      \
{                                             \
	vec->vec = calloc(size, sizeof(TYPE));     \
	vec->available = size;                     \
	vec->index = 0;                            \
}                                             


#endif
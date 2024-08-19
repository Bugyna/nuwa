#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifndef STR_VEC_H_
#define STR_VEC_H_

typedef struct STRING STRING;
struct STRING {
	char* str;
	size_t index, available;
	bool initialized;
};

void STRING_REALLOC(STRING* str, size_t new_size)
{
	str->available = new_size;
	char* tmp = str->str;
	str->str = realloc(str->str, str->available);
	// printf("REALLOC FOR: STRING | new size: %d | at index: %d | old ptr -> new ptr: %p -> %p\n", str->available, str->index, tmp, str->str);
	if (str->str == NULL) { NUWA_ERROR("realloc failed for: STRING\n"); }
	memset(str->str+str->index, 0, str->available-str->index);
}

char* STRING_ADD(STRING* str, char p)
{
	// printf("STRING: add %d\n", str->index);
	if (str->index+1 >= str->available) {
		STRING_REALLOC(str, str->available * 2);
	}
	str->str[str->index] = p;
	return &str->str[str->index++];
}
char* STRING_INSERT(STRING* str, char p, size_t index)
{
	if (index >= str->index) return STRING_ADD(str, p);
	printf("STRING: insert\n");
	str->index++;
	if (str->index >= str->available) {
		STRING_REALLOC(str, str->available * 2);
	}
	memcpy(str->str + index + 1, str->str + index, str->index - index);
	str->str[index] = p;
	return &str->str[str->index-1];
}
char* STRING_REPLACE(STRING* str, char p, size_t index)
{
	if (index >= str->index) return STRING_ADD(str, p);
	printf("STRING: replace\n");
	str->str[index] = p;
	return &str->str[index];
}
void STRING_REMOVE(STRING* str, size_t index)                          
{
	if (index >= str->index || index < 0) return;

	for (int i = index; i < str->index; i++) {
        str->str[i] = str->str[i + 1];
        str->str[i + 1] = '\0';
   }                                                                 
	str->index--;
}
void STRING_DELETE(STRING* str, char ptr)
{
	int index = -1;
	for (int i = 0; i <= str->index; i++) {
		char val = str->str[i];
		if (!memcmp(&val, &ptr, sizeof(char))) { index = i; break; }
	}
	STRING_REMOVE(str, index);
}
void STRING_POP(STRING* str)
{
	STRING_REMOVE(str, str->index-1);
}
char* STRING_GET(STRING* str, size_t index)
{
	return &str->str[index];
}
void STRING_SET(STRING* str, char* val, size_t index)
{
	str->available = index; str->index = index-1;
	str->str = val;
}
void STRING_INIT(STRING* str, size_t size)
{
	str->str = calloc(size, sizeof(int));
	str->available = size;
	str->index = 0;
}
void STRING_INIT_WITH_VAL(STRING* str, char* val, size_t size, size_t index)
{
	str->str = val;
	str->available = size;
	str->index = index;
}

void STRING_MERGE(STRING* dest, STRING* src)
{
	int index = dest->index+src->index;
	if (index > dest->available) {
		STRING_REALLOC(dest, index);
	}
	strcpy(dest->str+dest->index, src->str);
	dest->index = index;
}

void STRING_MERGE_F(STRING* dest, STRING* src, int offset)
{
	int index = dest->index+src->index-offset;
	if (index > dest->available) {
		STRING_REALLOC(dest, index);
	}
	strcpy(dest->str+dest->index, src->str+offset);
	dest->index = index;
}


#endif
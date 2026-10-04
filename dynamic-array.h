#ifndef DYNAMIC_ARRAY_H 
#define DYNAMIC_ARRAY_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <err.h>
#include <errno.h>

#define DYNAMIC_ARRAY_MIN_CAP 16

#define DYNAMIC_ARRAY_B_RED "\x1b[91m"
#define DYNAMIC_ARRAY_RESET "\x1b[0m"
#define DYNAMIC_ARRAY_STRINGIFY(x) #x

#define getLen(arrayStruct) (arrayStruct->len)
#define getCap(arrayStruct) (arrayStruct->cap)
#define getElem(arrayStruct, i) (arrayStruct->array[i])
#define getElemP(arrayStruct, i) (&arrayStruct->array[i])

#define ARRAY_OF_STRUCT_DECLARE(type, arrStruct)\
	typedef struct {\
		size_t len, cap;\
		type *array;\
	} arrStruct;\
\
arrStruct *init_##arrStruct(size_t cap);\
\
type *get_##arrStruct(arrStruct *self, size_t index);\
\
size_t getlen_##arrStruct(arrStruct *self);\
\
size_t getcap_##arrStruct(arrStruct *self);\
\
void append_##arrStruct(arrStruct *self, type *elem);\
\
bool maintain_##arrStruct(arrStruct *self);\
\
void pop_##arrStruct(arrStruct *self);\
\
arrStruct *free_##arrStruct(arrStruct *self);\


#define ARRAY_OF_STRUCT_IMPLEMENT(type, arrStruct, free_func)\
	arrStruct *init_##arrStruct(size_t cap) {\
		arrStruct *self = (arrStruct *)malloc(sizeof(arrStruct));\
		\
		if (self == NULL) {\
			fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array malloc failed for cap = %zu: %s\n" DYNAMIC_ARRAY_RESET, cap, strerror(errno));\
			exit(errno);\
		}\
		\
		if (cap < DYNAMIC_ARRAY_MIN_CAP) {\
			cap = DYNAMIC_ARRAY_MIN_CAP;\
		}\
		\
		self->cap = cap;\
		self->len = 0;\
		self->array = (type *)malloc(sizeof(type) * cap);\
		\
		if (self->array == NULL) {\
			fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array malloc failed for cap = %zu: %s\n" DYNAMIC_ARRAY_RESET, cap, strerror(errno));\
			exit(errno);\
		}\
		\
		/* printf(BRI_MAGENTA "> sizeof(" DYNAMIC_ARRAY_STRINGIFY(arrStruct) "): %zu\n" DYNAMIC_ARRAY_RESET, sizeof(*self)); */\
		/* printf(BRI_MAGENTA "> sizeof(" DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array) * cap: %zu\n" DYNAMIC_ARRAY_RESET, sizeof(*self->array) * self->cap); */\
		\
		return self;\
	}\
\
type *get_##arrStruct(arrStruct *self, size_t index) {\
	return &self->array[index];\
}\
\
size_t getlen_##arrStruct(arrStruct *self) {\
	return self->len;\
}\
\
size_t getcap_##arrStruct(arrStruct *self) {\
	return self->cap;\
}\
\
void append_##arrStruct(arrStruct *self, type *elem) {\
	maintain_##arrStruct(self);\
	\
	self->array[self->len++] = *elem;\
}\
\
bool maintain_##arrStruct(arrStruct *self) {\
	if (self->len < self->cap) {\
		return false;\
	}\
	printf("Len: %ld, Cap: %ld\n", self->len, self->cap);\
	\
	size_t old_cap = self->cap;\
	size_t new_cap = self->cap * 2;\
	printf("Reached cap: New cap: %zu\n", new_cap);\
	\
	type *tempArr = (type *)realloc(self->array, sizeof(*self->array) * new_cap);\
	\
	if (tempArr == NULL) {\
		fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array realloc failed when appending for cap: %zu: %s\n" DYNAMIC_ARRAY_RESET, self->cap, strerror(errno));\
		exit(errno);\
		\
	}\
	\
	self->cap = new_cap;\
	self->array = tempArr;\
	\
	return true;\
}\
\
void pop_##arrStruct(arrStruct *self) {\
	if (self->len <= 0) {\
		fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "Underflow\n" DYNAMIC_ARRAY_RESET);\
		return;\
	}\
	\
	type *elem = &self->array[--self->len];\
	free_func(elem);\
	\
	if (self->len * 4 < self->cap) {\
		self->cap /= 2;\
		\
		if (self->cap < DYNAMIC_ARRAY_MIN_CAP) {\
			self->cap = DYNAMIC_ARRAY_MIN_CAP;\
		}\
		\
		type *tempArr = (type *)realloc(self->array, sizeof(type) * self->cap);\
		\
		if (tempArr == NULL) {\
			fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array realloc failed when popping for cap: %zu\n" DYNAMIC_ARRAY_RESET, self->cap);\
			exit(errno);\
		}\
		\
		self->array = tempArr;\
	}\
}\
\
arrStruct *free_##arrStruct(arrStruct *self) {\
	for(size_t i = 0; i < self->len; i++) {\
		free_func(&self->array[i]);\
	}\
	\
	free(self->array);\
	free(self);\
	return NULL;\
}\





#define ARRAY_OF_POINTER_DECLARE(type, arrStruct)\
	typedef struct {\
		size_t len, cap;\
		type **array;\
	} arrStruct;\
\
arrStruct *init_##arrStruct(size_t cap);\
\
type *get_##arrStruct(arrStruct *self, size_t index);\
\
size_t getlen_##arrStruct(arrStruct *self);\
\
size_t getcap_##arrStruct(arrStruct *self);\
\
void append_##arrStruct(arrStruct *self, type *elem);\
\
bool maintain_##arrStruct(arrStruct *self);\
\
void pop_##arrStruct(arrStruct *self);\
\
arrStruct *free_##arrStruct(arrStruct *self);\


#define ARRAY_OF_POINTER_IMPLEMENT(type, arrStruct, free_func)\
	arrStruct *init_##arrStruct(size_t cap) {\
		arrStruct *self = (arrStruct *)malloc(sizeof(arrStruct));\
		\
		if (self == NULL) {\
			fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array malloc failed for cap = %zu: %s\n" DYNAMIC_ARRAY_RESET, cap, strerror(errno));\
			exit(errno);\
		}\
		\
		if (cap < DYNAMIC_ARRAY_MIN_CAP) {\
			cap = DYNAMIC_ARRAY_MIN_CAP;\
		}\
		\
		self->cap = cap;\
		self->len = 0;\
		self->array = (type **)malloc(sizeof(*self->array) * self->cap);\
		\
		if (self->array == NULL) {\
			fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array malloc failed for cap = %zu: %s\n" DYNAMIC_ARRAY_RESET, cap, strerror(errno));\
			exit(errno);\
		}\
		\
		for (size_t i = 0; i < self->cap; i++) {\
			self->array[i] = NULL;\
		}\
		\
		/* printf(BRI_MAGENTA "> sizeof(" DYNAMIC_ARRAY_STRINGIFY(arrStruct) "): %zu\n" DYNAMIC_ARRAY_RESET, sizeof(*self)); */\
		/* printf(BRI_MAGENTA "> sizeof(" DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array) * cap: %zu\n" DYNAMIC_ARRAY_RESET, sizeof(*self->array) * self->cap); */\
		\
		return self;\
	}\
\
type *get_##arrStruct(arrStruct *self, size_t index) {\
	return self->array[index];\
}\
\
size_t getlen_##arrStruct(arrStruct *self) {\
	return self->len;\
}\
\
size_t getcap_##arrStruct(arrStruct *self) {\
	return self->cap;\
}\
\
void append_##arrStruct(arrStruct *self, type *elem) {\
	maintain_##arrStruct(self);\
	\
	self->array[self->len++] = elem;\
}\
\
bool maintain_##arrStruct(arrStruct *self) {\
	if (self->len < self->cap) {\
		return false;\
	}\
	printf("Len: %ld, Cap: %ld\n", self->len, self->cap);\
	\
	size_t old_cap = self->cap;\
	size_t new_cap = self->cap * 2;\
	printf("Reached cap: New cap: %zu\n", new_cap);\
	\
	type **tempArr = (type **)realloc(self->array, sizeof(*self->array) * new_cap);\
	\
	if (tempArr == NULL) {\
		fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array realloc failed when appending for cap: %zu: %s\n" DYNAMIC_ARRAY_RESET, self->cap, strerror(errno));\
		exit(errno);\
		\
	}\
	\
	self->cap = new_cap;\
	self->array = tempArr;\
	\
	for (size_t i = old_cap; i < new_cap; i++) {\
		self->array[i] = NULL;\
	}\
	\
	return true;\
}\
\
void pop_##arrStruct(arrStruct *self) {\
	if (self->len <= 0) {\
		fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "Underflow\n" DYNAMIC_ARRAY_RESET);\
		return;\
	}\
	\
	type *elem = self->array[--self->len];\
	free_func(elem);\
	\
	if (self->len * 4 < self->cap) {\
		self->cap /= 2;\
		\
		if (self->cap < DYNAMIC_ARRAY_MIN_CAP) {\
			self->cap = DYNAMIC_ARRAY_MIN_CAP;\
		}\
		\
		type **tempArr = (type **)realloc(self->array, sizeof(*self->array) * self->cap);\
		\
		if (tempArr == NULL) {\
			fprintf(stderr, DYNAMIC_ARRAY_B_RED DYNAMIC_ARRAY_STRINGIFY(arrStruct) "->array realloc failed when popping for cap: %zu\n" DYNAMIC_ARRAY_RESET, self->cap);\
			exit(errno);\
		}\
		\
		self->array = tempArr;\
	}\
}\
\
arrStruct *free_##arrStruct(arrStruct *self) {\
	for(size_t i = 0; i < self->len; i++) {\
		free_func(self->array[i]);\
	}\
	\
	free(self->array);\
	free(self);\
	return NULL;\
}\

#endif

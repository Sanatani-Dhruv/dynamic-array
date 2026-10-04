#include "dynamic-array.h"

int freeint(int *i) {
	return 0;
}

ARRAY_OF_STRUCT_DECLARE(int, intArray)
ARRAY_OF_STRUCT_IMPLEMENT(int, intArray, freeint)

ARRAY_OF_POINTER_DECLARE(int, intPointerArray)
ARRAY_OF_POINTER_IMPLEMENT(int, intPointerArray, freeint)

int main() {

	intArray *array = init_intArray(256);

	free_intArray(array);
}

/* Next Feature, for_##array macro to interate every index and get key */

#include "dynamic-array.h"

int freeint(int *i) {
	return 0;
}

ARRAY_OF_STRUCT_DECLARE(int, intArray)
ARRAY_OF_STRUCT_IMPLEMENT(int, intArray, freeint)

ARRAY_OF_POINTER_DECLARE(int, intPointerArray)
ARRAY_OF_POINTER_IMPLEMENT(int, intPointerArray, freeint)

int main() {

}

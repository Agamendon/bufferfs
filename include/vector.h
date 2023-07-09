#pragma once

#include <stdlib.h>


/// @brief A simple C++-like vector. Should be initialized via vector_init.
/// All access operation should be conducted via library's functions
/// @param el_size              size of one element in bytes
/// @param length               vector length
/// @param space_allocated      space allocated for vector at ptr, bytes. Cannot be 0.
/// @param ptr                  pointer to first element of vector
typedef struct vector {
    size_t el_size;
    unsigned long length;
    unsigned long space_allocated;
    void *ptr;
} vector;



/// @brief Initialize vector of specific length.
/// @param v                    pointer to vector
/// @param el_size              size of vector element, bytes
/// @param length               desired length
/// @return Pointer to same vector on success, NULL of failure
vector *vector_init(vector *v, size_t el_size, unsigned long length);


/// @brief Push element to end of vector
/// @param v                    pointer to vector
/// @param el                   pointer to element to be inserted
/// @return Pointer to new vector element on success, NULL on failure
void *vector_push_back(vector *v, void *el);


/// @brief Pop element from the end of vector
/// @param v                    pointer to vector
/// @return Pointer to vector on success, NULL if vector failed to resize
vector *vector_pop_back(vector *v);


/// @brief Reads element from vector
/// @param v                    pointer to vector
/// @param pos                  position of element
/// @return Pointer to desired element from vector
void *vector_get(const vector *v, unsigned long pos);


/// @brief Modifies element at vector
/// @param v                    pointer to vector
/// @param pos                  position of element
/// @param el                   source element
/// @return Pointer to written element
void *vector_put(vector *v, unsigned long pos, void *el);


/// @brief Resizes vector
/// @param v                    pointer to vector
/// @param length               desired new length
/// @return Pointer to vector on sucesss, NULL on failure
vector *vector_resize(vector *v, unsigned long new_length);


/// @brief Clears vector
/// @param v                    pointer to vector
/// @return Pointer to vector on sucess, NULL on failure
vector* vector_clear(vector *v);


/// @brief Copies a number of elements from v1 to v2. el_size of vectors should be the same.
/// @param v1                   source vector
/// @param v2                   destination vector
/// @param offset1              offset in first vector, elements
/// @param offset2              offset in second vector, elements
/// @param copy_length          length of copy, elements 
void vector_copy(const vector *v1, vector *v2, 
    unsigned long offset1, unsigned long offset2, unsigned long copy_length);
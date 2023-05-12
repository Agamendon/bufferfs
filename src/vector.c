#include <vector.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//
// STATIC FUNCTIONS
//


/// @brief Return minimal positive power of 2 that is greater or equal to number
/// @param num                  Number to be assessed
/// @return Power of 2
static unsigned long upper_pow2(unsigned long num)
{
    if (__builtin_popcountl(num) == 1) return num;
    if (num == 0) return 0;
    return 1 << (sizeof(unsigned long) * 8 - __builtin_ffsl(num));
}


/// @brief Doubles allocated space of vector.
/// @param v                    pointer to vector
/// @return Pointer to reallocated space on success, NULL on failure
static void *vector_double_realloc(vector *v)
{
    void *realloc_ret = realloc(v->ptr, v->space_allocated * 2);
    if (realloc_ret != NULL) {
        v->ptr = realloc_ret;
        v->space_allocated *= 2;
    }
    return realloc_ret;
}


/// @brief Halfs allocated space of vector. If vector allocated space is is el_size, nothing changes.
/// @param v                    pointer to vector 
/// @return Pointer to reallocated space on success, NULL on failure
static void *vector_half_realloc(vector *v)
{
    if (v->space_allocated == v->el_size) return v->ptr;
    void *realloc_ret = realloc(v->ptr, v->space_allocated / 2);
    if (realloc_ret != NULL) {
        v->ptr = realloc_ret;
        v->space_allocated /= 2;
    }
    return realloc_ret;
}


//
// PUBLIC FUNCTIONS
//


vector *vector_init(vector *v, size_t el_size, unsigned long length)
{
    assert(el_size > 0);

    unsigned long length_to_allocate = length;
    if (length == 0) length_to_allocate = 1;

    unsigned long space_allocated = upper_pow2(length_to_allocate) * el_size;
    void *malloc_ret = malloc(space_allocated);
    if (malloc_ret == NULL) return NULL;

    v->el_size = el_size;
    v->length = length;
    v->space_allocated = space_allocated;
    v->ptr = malloc_ret;

    return v;
}


void *vector_push_back(vector *v, void *el)
{
    (v->length)++;
    if (v->length * v->el_size > v->space_allocated) {
        void *realloc_ret = vector_double_realloc(v);
        if (realloc_ret == NULL) {
            (v->length)--;
            return NULL;
        }
    }
    void* ret = vector_put(v, v->length - 1, el);
    return ret;
}


vector *vector_pop_back(vector *v)
{
    assert(v->length > 0);

    void *half_realloc_ret;
    if ((v->length - 1) * v->el_size < v->space_allocated / 2)
        half_realloc_ret = vector_half_realloc(v);
    if (half_realloc_ret != NULL) {
        v->length--;
        return v;
    }
    return NULL;
}

void *vector_get(const vector *v, unsigned long pos)
{
    assert(pos < v->length && pos >= 0);

    return (char *) v->ptr + pos * v->el_size;
}


void *vector_put(vector *v, unsigned long pos, void *el)
{
    assert(pos < v->length && pos >= 0);

    void *dest = (char *) v->ptr + pos * v->el_size;
    return memcpy(dest, el, v->el_size);
}


vector *vector_resize(vector *v, unsigned long new_length)
{
    unsigned long new_space_allocated = upper_pow2(new_length) * v->el_size;
    void *realloc_ret = realloc(v->ptr, new_space_allocated);
    if (realloc_ret != NULL) {
        v->ptr = realloc_ret;
        v->space_allocated = new_space_allocated;
        return v;
    }
    return NULL;
}

vector* vector_clear(vector *v)
{
    void *realloc_get = realloc(v->ptr, v->el_size);
    if (realloc_get != NULL) {
        v->length = 0;
        v->space_allocated = v->el_size;
        v->ptr = realloc_get;
    }
    return NULL;
}


void vector_copy(const vector *v1, vector *v2,
    unsigned long offset1, unsigned long offset2, unsigned long copy_length)
{
    assert(v1->el_size == v2->el_size);

    void *v1_copy_ptr = (char *) v1->ptr + offset1 * v1->el_size;
    void *v2_copy_ptr = (char *) v2->ptr + offset2 * v2->el_size;
    memcpy(v2_copy_ptr, v1_copy_ptr, copy_length * v1->el_size);
}
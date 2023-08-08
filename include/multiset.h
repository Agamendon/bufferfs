// This is implementation of simple multimultiset structure based on Splay tree
#include <splay_tree.h>
#include <stdint.h>


struct multiset {
    size_t el_size;
    struct splay_tree tree;
};


struct multiset *multiset_init();


// Returns 0 on success, non-0 on error.
// Value initialization should be done manually
int multiset_insert(struct multiset *tmultiset, uint64_t descriptor, void *value);


void *multiset_find(struct multiset *tmultiset, uint64_t  descriptor);


void multiset_lower_bound(struct multiset *tmultiset, uint64_t descriptor);


void multiset_delete(struct multiset *tmultiset, uint64_t descriptor);


//void *multiset_iterate(multiset *tmultiset, void (*foreach)(void *value, void *data), void *data);


void multiset_dealloc(struct multiset *tmultiset);

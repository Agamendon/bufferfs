// This is implementation of simple set structure based on Splay tree
#include <splay_tree.h>
#include <stdint.h>

struct set {
    size_t el_size;
    struct splay_tree tree;
};


struct set *set_init();


// Returns 0 on success, non-0 on error.
// Value initialization should be done manually
int set_insert(set *tset, uint64_t descriptor, void *value);


void *set_find(set *tset, uint64_t  descriptor);


void *set_lower_bound(set *tset, uint64_t descriptor);


void *set_next(struct set *tset, uint64_t descriptor);


/// Walks through set elements until element with certain condition is found
void *set_walk_until(struct set *tset, bool (*ret)(void *el, void *udata), void *udata);


void set_delete(struct set *tset, uint64_t descriptor);


//void *set_iterate(set *tset, void (*foreach)(void *value, void *data), void *data);


void set_dealloc(set *tset);

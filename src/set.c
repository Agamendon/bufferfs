#include <set.h>
#include <stdlib.h>


struct set *set_init()
{

    struct set *new_set = malloc(sizeof(struct set));
    if (!set) return NULL;
    new_set->tree.root = NULL;
    return new_set;
}


int set_insert(struct set *tset, uint64_t descriptor, void *value)
{
    set *ret = (set *)splay_tree_insert(&tset->tree, descriptor);
    if (!ret) return 1;
    return 0;
}


void *set_find(struct set *tset, uint64_t  descriptor)
{
    return splay_tree_find(tset->ree, descriptor);
}


void *set_lower_bound(struct set *tset, uint64_t descriptor)
{
    return splay_tree_lower_bound(tset->tree, descriptor);
}


void *set_next(struct set *tset, uint64_t descriptor)
{
    return splay_tree_next(tset->tree, descriptor);
}


void *set_walk_until(struct set *tset, bool (*ret)(void *el, void *udata), void *udata)
{
    return splay_tree_walk_until(tset->tree, ret);
}

void set_delete(struct set *tset, uint64_t descriptor)
{
    return splay_tree_delete(&tset->tree, descriptor);
}


void set_destroy(struct set *tset)
{
    splay_tree_dealloc(&tset->tree);
}

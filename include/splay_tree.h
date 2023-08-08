// Simple splay tree to be used underneath set structure
// Written using https://zhtluo.com/cp/splay-tree-one-tree-to-rule-them-all.html


//
struct splay_tree {
    node *root;
};


void *splay_tree_find(splay_tree *tree, uint64_t descriptor);


/// value initialization should be done manually
/// returns NULL if insertion failed for absense of memory
void *splay_tree_insert(splay_tree *tree, uint64_t descriptor);


/// finds element with greatest descriptor that's less than descriptor
void *splay_tree_lower_bound(splay_tree *tree, uint64_t descriptor)



void *splay_tree_next(struct splay_tree *tree, uint64_t descriptor);


void *splay_tree_walk_until(struct splay_tree *tree, bool (*ret)(void *el, void *udata), void *udata);


/// For this to work, node with descriptor should be in tree
/// Returns pointer to value
void splay_tree_delete(splay_tree *tree, uint64_t descriptor);


/// Deallocates all elements in the tree
void splay_tree_dealloc(splay_tree *tree);

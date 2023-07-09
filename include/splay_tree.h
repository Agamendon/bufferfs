#pragma once

#include <stdlib.h>


/// @brief Splay tree node.
/// Nodes should not be created as independent objects.
/// @param parent                   parent node
/// @param left                     left child
/// @param right                    right child
/// @param value                    pointer to node value
typedef struct splay_node {
    struct splay_node *parent;
    struct splay_node *left; 
    struct splay_node *right;
    void *value;                  
} splay_node;                             


/// @brief Splay tree 
/// @param root                     pointer to root node
/// @param value_width              size of node value in bytes
/// @param cmp                      strcmp-like value comparison function
/// @param value_destructor         destructor for node value, should properly deallocate value
typedef struct splay_tree {
    splay_node *root;
    size_t value_width;
    int (*cmp)(void *a, void *b);
    void (*value_destructor)(void *value);
} splay_tree;


/// @brief Initializes splay tree
/// @param tree                     pointer to tree
/// @param value_width              size of node value in bytes
/// @param cmp                      pointer to strcmp-like comparison function for values
void splay_tree_init(splay_tree *tree, size_t value_width, int (*cmp)(void *a, void *b),
    void (*value_destructor)(void *));


/// @brief Splay operation on splay tree
/// @param tree                     pointer to tree
/// @param node                     pointer to node
void splay_tree_splay(splay_tree *tree, splay_node *node);


/// @brief Join operation on splay tree. May modify initial trees.
/// @param tree1                    pointer to first tree
/// @param tree2                    pointer to second tree
/// @return Pointer to new tree
splay_tree *splay_tree_join(splay_tree *tree1, splay_tree *tree2);


/// @brief Split operation on splay tree.
/// This method does not preserve the original tree.
/// Divides tree into `less` and `greater or equal` trees.
/// @param tree1                    initial tree, eventually the lower half of tree
/// @param tree2                    upper half of initial tree
/// @param node                     pointer to node
void splay_tree_split(splay_tree *tree1, splay_tree *tree2, splay_node *node);


/// @brief Insert element into splay tree.
/// Value gets copied and allocated once more, consider that.
/// @param tree                     pointer to tree
/// @param value                    pointer to value
/// @return Pointer to new node on success, NULL on failure
splay_node *splay_tree_insert(splay_tree *tree, void *value);


/// @brief Delete element from tree
/// @param tree                     pointer to tree
/// @param node                     pointer to node
void splay_tree_delete(splay_tree *tree, splay_node *node);


/// @brief Find element in splay tree by value
/// @param tree                     pointer to tree
/// @param value                    pointer to value
/// @return Pointer to node on success, NULL on failure
splay_node *splay_tree_find(splay_tree *tree, void *value);


/// @brief Completely deallocate all splay tree nodes and values.
/// This will set `splay_tree->parent` to NULL
/// @param tree                     pointer to tree
void splay_tree_dealloc(splay_tree *tree);
#pragma once

#include <stdlib.h>


/// @brief Splay tree node
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
/// @param cmp                      strcmp-like @b value comparison function
typedef struct splay_tree {
    splay_node *root;
    size_t value_width;
    int (*cmp)(void *a, void *b);
} splay_tree;


/// @brief Splay tree node accessor.
/// @param node                     pointer to node
/// @param node_depth               depth of node
typedef struct splay_node_accessor {
    splay_node *node;
    unsigned node_depth;
} splay_node_accessor;


/// @brief Initializes splay tree
/// @param tree                     pointer to tree
/// @param value_width              size of node value in bytes
/// @param cmp                      pointer to strcmp-like comparison function for values
void splay_tree_init(splay_tree *tree, size_t value_width, int (*cmp)(void *a, void *b));


/// @brief Splay operation on splay tree
/// @param tree                     pointer to tree
/// @param node_accessor            node accessor
void splay_tree_splay(splay_tree *tree, splay_node_accessor node_accessor);


/// @brief Join operation on splay tree. May modify initial trees.
/// @param tree1                    pointer to first tree
/// @param tree2                    pointer to second tree
/// @return Pointer to new tree
splay_tree *splay_tree_join(splay_tree *tree1, splay_tree *tree2);


/// @brief Split operation on splay tree.
/// This method does not preserve the original tree.
/// Divides tree into `less` and `greater or equal` trees
/// @param tree1                    initial tree, eventually the lower half of tree
/// @param tree2                    upper half of initial tree
/// @param node_accessor            accessor to splitting node
void splay_tree_split(splay_tree *tree1, splay_tree *tree2, splay_node_accessor node_accessor);


/// @brief Insert element into splay tree.
/// @param tree                     pointer to tree
/// @param value                    pointer to value
/// @return Node accessor that points to new node. On failure node's address will be NULL
splay_node_accessor splay_tree_insert(splay_tree *tree, void *value);


/// @brief Delete element from tree
/// @param tree                     pointer to tree
/// @param node                     pointer to node
void splay_tree_delete(splay_tree *tree, splay_node_accessor node_accessor);


/// @brief Find element in splay tree by value
/// @param tree                     pointer to tree
/// @param value                    pointer to value
/// @return Pointer to node on success, NULL on failure
splay_node *splay_tree_find(splay_tree *tree, void *value);
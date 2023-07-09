#include <splay_tree.h>
#include <stdbool.h>
#include <string.h>


#define swap(T, a, b) T swp = b; b = a; a = swp;

enum child_pos {LEFT, RIGHT, NONE};


//
// Static functions
//


/// @brief Checks if node is left child of its parent.
/// Parent should be not NULL.
/// @param node                     pointer to node
/// @return `true` if node is left, `false` if right 
static bool is_left(splay_node *node)
{
    if (node->parent->left == node)
        return true;
    return false;
}


/// @brief Rotates tree on edge that links node to its parent.
/// Does nothing if node is root.
/// @param tree                 pointer to tree
/// @param node                 pointer to node
static void splay_tree_rotate(splay_tree *tree, splay_node *node)
{
    splay_node *parent = node->parent;

    if (node == tree->root) 
        return;

    if (tree->root == parent) // if parent is root, node will be the new root
        tree->root = node;

    if (is_left(node)) { // the "rehanging" itself
        if (node->right != NULL)
            node->right->parent = parent;
        parent->left = node->right;
        node->right = parent;
    } else {
        if (node->left != NULL)
            node->left->parent = parent;
        parent->right = node->left;
        node->left = parent;
    }

    node->parent = parent->parent; // setting new parents
    parent->parent = node;
}


/// @brief Performs "zig" splay step.
/// Should be performed when node parent is root.
/// @param tree                 pointer to tree
/// @param node                 pointer to node
static void splay_tree_splay_zig(splay_tree *tree, splay_node *node)
{
    splay_tree_rotate(tree, node);
}


/// @brief Performs "zig-zig" splay step.
/// Should be performed when node and its parent are both right or left children.
/// @param tree                 pointer to tree
/// @param node                 pointer to node
static void splay_tree_splay_zigzig(splay_tree *tree, splay_node *node)
{
    splay_tree_rotate(tree, node->parent);
    splay_tree_rotate(tree, node);
}


/// @brief Performs "zig-zag" splay step.
/// Should be performed when node and its parent are left-right or right-left children.
/// @param tree                 pointer to tree
/// @param node                 pointer to node
static void splay_tree_splay_zigzag(splay_tree *tree, splay_node *node)
{
    splay_tree_rotate(tree, node);
    splay_tree_rotate(tree, node);
}


/// @brief Allocates and creates new node with chosen value.
/// @param tree                 pointer to tree
/// @param parent               pointer to node's parent
/// @param value                value of new node
/// @param cp                   whether node will be left or right child
/// @return Newly created node on success, NULL on failure 
static splay_node *splay_node_new(splay_tree *tree, splay_node *parent, void *value, enum child_pos cp)
{
    splay_node *ret = malloc(sizeof(splay_node));
    if (ret == NULL)
        return NULL;

    ret->value = malloc(tree->value_width);
    if (value == NULL) { // if value malloc failed, no point to continue
        free(ret);
        return NULL;
    }

    ret->left = NULL;
    ret->right = NULL;

    if (cp != NONE) {
        ret->parent = parent;

        if (cp == LEFT)
            parent->left = ret;
        else if(cp == RIGHT)
            parent->right = ret;

    } else {
        ret->parent = NULL;
    }

    memcpy(ret->value, value, tree->value_width);
    return ret;
}


/// @brief Node deletion for two-children case.
/// @param tree                 pointer to tree
/// @param node                 pointer to node
static void splay_node_delete2(splay_tree *tree, splay_node *node)
{
    splay_node *current;

    if (rand() % 2) { // random for better balance
        current = node->left;
        while (current->right != NULL)
            current = current->right;
    } else {
        current = node->right;
        while (current->left != NULL)
            current = current->left;
    }
    swap(void *, current->value, node->value);
    tree->value_destructor(current->value);
    splay_node *parent = current->parent;
    free(current);
    splay_tree_splay(tree, parent);
}


//
// Library functions
//

void splay_tree_init(splay_tree *tree, size_t value_width, int (*cmp)(void *a, void *b),
    void (*value_destructor)(void *))
{
    tree->root = NULL;
    tree->value_width = value_width;
    tree->cmp = cmp;
    tree->value_destructor = value_destructor;
}


void splay_tree_splay(splay_tree *tree, splay_node *node)
{
    while (tree->root != node) {
        if (node->parent == tree->root)
            splay_tree_splay_zig(tree, node);
        else if (is_left(node) == is_left(node->parent))
            splay_tree_splay_zigzig(tree, node);
        else
            splay_tree_splay_zigzag(tree, node);
    }
}


void splay_tree_split(splay_tree *tree1, splay_tree *tree2, splay_node *node)
{
    splay_tree_splay(tree1, node);
    splay_tree_init(tree2, tree1->value_width, tree1->cmp, tree1->value_destructor);

    if (node->left != NULL) {
        tree1->root = node->left;
        node->left->parent = NULL;
    }

    tree2->root = node;
    node->left = NULL;
}


splay_node *splay_tree_insert(splay_tree *tree, void *value)
{
    splay_node *current = tree->root;
    splay_node *prev = NULL;
    enum child_pos cp = NONE;

    if (current == NULL) {
        tree->root = splay_node_new(tree, prev, value, cp);
        splay_tree_splay(tree, tree->root);
        return tree->root;
    }

    for(int i = 0;;i++) {
        if (current == NULL) {
            splay_node *new_node = splay_node_new(tree, prev, value, cp);
            splay_tree_splay(tree, new_node);
            return new_node;
        }

        prev = current;
        int cmpres = tree->cmp(value, current->value);
        if (cmpres == -1) {
            current = current->left;
            cp = LEFT;
        } else if (cmpres == 0) {
            splay_tree_splay(tree, current);
            return current;
        } else {
            current = current->right;
            cp = RIGHT;
        }
    }
}


void splay_tree_delete(splay_tree *tree, splay_node *node)
{
    if (node->left != NULL && node->right != NULL) {
        splay_node_delete2(tree, node);
        return;
    } else if (node->left == NULL && node->right == NULL) {
        splay_node *parent = node->parent;
        tree->value_destructor(node->value);
        free(node);
        splay_tree_splay(tree, parent);
    } else {
        splay_node *child;
        splay_node *parent = node->parent;
        if (node->left != NULL)
            child = node->left;
        else
            child = node->right;
        
        child->parent = parent;
        if (is_left(node))
            parent->left = child;
        else
            parent->right = child;
        
        splay_tree_splay(tree, parent);
    }
}


splay_node *splay_tree_find(splay_tree *tree, void *value)
{
    splay_node *current = tree->root;
    splay_node *prev = NULL;

    for(;;) {
        if (current == NULL)
            return NULL;

        int cmpres = tree->cmp(value, current->value);
        if (cmpres == -1)
            current = current->left;
        else if (cmpres == 1)
            current = current->right;
        else {
            splay_tree_splay(tree, current);
            return current;
        }
        prev = current;
    }
}


void splay_tree_dealloc(splay_tree *tree)
{
    splay_node *current = tree->root;
    while (current != NULL) {
        if (current->left != NULL) {
            current = current->left;
            continue;
        } else if(current->right != NULL) {
            current = current->right;
            continue;
        } else {
            tree->value_destructor(current->value);
            splay_node *parent = current->parent;
            free(current);
            current = parent;
        }
    }
}
#include <splay_tree.h>
#include <stdbool.h>
#include <string.h>


#define swap(T, a, b) T swp = b; b = a; a = swp;


//
// Static functions
//

static bool is_left(splay_node *node)
{
    if (node->parent->left == node)
        return true;
    return false;
}

static void splay_tree_rotate(splay_tree *tree, splay_node *node)
{
    splay_node *parent = node->parent;

    if (node == tree->root) 
        return;

    if (tree->root == parent)
        tree->root = node;

    if (parent->left == node) {
        parent->left = node->right;
        node->right = parent;
    } else {
        parent->right = node->left;
        node->left = parent;
    }
}


static void splay_tree_splay_zig(splay_tree *tree, splay_node *node)
{
    splay_tree_rotate(tree, node);
}


static void splay_tree_splay_zigzig(splay_tree *tree, splay_node *node)
{
    splay_tree_rotate(tree, node->parent);
    splay_tree_rotate(tree, node);
}


static void splay_tree_splay_zigzag(splay_tree *tree, splay_node *node)
{
    splay_tree_rotate(tree, node);
    splay_tree_rotate(tree, node);
}


static splay_node *splay_node_new(splay_tree *tree, splay_node *parent, void *value)
{
    splay_node *ret = malloc(sizeof(splay_node));
    if (ret == NULL)
        return NULL;
    ret->parent = parent;
    ret->left = NULL;
    ret->right = NULL;
    memcpy(ret->value, value, tree->value_width);
    return ret;
}


//
// Library functions
//

void splay_tree_init(splay_tree *tree, size_t value_width, int (*cmp)(void *a, void *b))
{
    tree->root = NULL;
    tree->value_width = value_width;
    tree->cmp = cmp;
}


void splay_tree_splay(splay_tree *tree, splay_node_accessor node_accessor)
{
    splay_node *node = node_accessor.node;

    if (node->parent == tree->root)
        splay_tree_zig(tree, node);
    else if (is_left(node) == is_left(node->parent))
        splay_tree_zigzig(tree, node);
    else
        splay_tree_zigzag(tree, node);
}


void splay_tree_split(splay_tree *tree1, splay_tree *tree2, splay_node_accessor node_accessor)
{
    splay_node *node = node_accessor.node;

    splay_tree_splay(tree1, node_accessor);

    tree1->root = node->left;
    tree2->root = node;
    node->left = NULL;
}


splay_node_accessor splay_tree_insert(splay_tree *tree, void *value)
{
    if (tree->root == NULL) {
        splay_node *node = splay_node_new(tree, NULL, value);
        return (splay_node_accessor){node, 0};
    }

    unsigned depth = 0;
    splay_node *current = tree->root;
    splay_node *prev = NULL;
    int (*const cmp)(void *a, void *b) = tree->cmp;

    for(;;) {
        int cmpres = cmp(value, current->value);
        if (current == NULL)
            break;
        else if (cmpres == -1)
            current = current->left;
        else if (cmpres == 0)
            return (splay_node_accessor){current, 0};
        else
            current = current->right;
        prev = current;
        depth++;
    }

    current = splay_node_new(tree, prev, value);
    if (current == NULL)
        return (splay_node_accessor){NULL, depth};
    return (splay_node_accessor){current, depth};
}
#include <splay_tree.h>
#include <stdlib.h>


struct node {
    uint64_t descriptor;
    void *value;
    node *father;
    node *children[2];
};


void rotate(struct node *node)
{
    if (!node) return;
    bool is_left = (node->father>children[0] == node); // this is set if node is left child
    struct node *father = node->father, *dchild = node->children[is_left]; // father is obvious, dchild is node's child on the opposite side of node
    if (father->father) father->father[father->father->children[1] == father] = node; // this sets node as child of grandfather
    node->father = father->father; // this sets new father of node
    node->children[is_left] = father; // this sets former father as corresponding child
    father->father = node; // obvious
    father->children[!is_left] = dchild; // this sets former child as child of former parent
    if (dchild) dchild->father = father; // vice versa
}


void splay(struct node *node)
{
    if (!node->father) return; // if node is root, do nothing
    if (!node->father->father) { // if node's father is root, rotate once
        rotate(node); // zig
        return;
    }
    if (node->father->children[0] == node && node->father->father->children[0] == node->father) { // do grandfather, father, and node lie on a line?
        rotate(node->father); // yes -> zig-zig
        rotate(node);
    } else { // no -> zig-zag
        rotate(node);
        rotate(node);
    }
}


void splay_to_root(struct node *node)
{
    while (node->father)
        splay(node);
    return;
}


struct node *find_node(splay_tree *tree, uint64_t descriptor)
{
    struct node *node = tree->root;
    while (node) {
        if (descriptor > node->descriptor) node = node->children[1]
        else if (descriptor < node->descriptor) node = node->children[0];
    }
    return node;
}


struct node *insert_node(splay_tree *tree, uint64_t descriptor)
{
    struct node *node = tree->root;
    for (;;)) {
        if (node->descriptor == descriptor) return node;
        if (!node->children[0] && node->children[1]) {
            struct node *nnode = malloc(sizeof(struct node));
            if (!nnode) return NULL;
            nnode->descriptor = descriptor;
            nnode->father = node;
            node->children[0] = node->children[1] = NULL;
            return nnode;
        }

        if (descriptor > node->descriptor) node = node->children[1]
        else if (descriptor < node->descriptor) node = node->children[0];
    }
}


node *node_walk_recursive(node *node, bool (*ret)(void *el, void *udata), void *udata)
{
    if (ret(node->value, udata)) return node;
    void *ret = NULL;
    if (node->children[0] && (ret = node_walk_recursive(node->children[0], ret)))
        return ret;
    if (node->children[1] && (ret = node_walk_recursive(node->children[1], ret)))
        return ret;
    return NULL;
}


void walk_and_delete(struct node *node)
{
    if (!node) return;
    walk_and_delete(node->children[0]);
    walk_and_delete(node->children[1]);
    free(node);
    return;
}


void *splay_tree_find(splay_tree *tree, uint64_t descriptor)
{
    struct node *res = find_node(tree, tree->root, descriptor);
    if (res) {
            splay_to_root(res);
            return res->value;
    }
    else return NULL;
}


void *splay_tree_insert(splay_tree *tree, uint64_t descriptor)
{
    struct node *res = insert_node(tree, node, descriptor);
    if (!res) return NULL;
    splay_to_root(res);
    return res;
}


void *splay_tree_lower_bound(splay_tree *tree, uint64_t descriptor)
{
    struct node *node = tree->root;
    struct node *ret = NULL;
    for(;;) {
        if (!node) {
            splay_to_root(ret);
            return ret->value;
        }
        if (node->descriptor >= descriptor)
            node = node->children[0];
        else {
            ret = node;
            node = node->children[1];
        }
    }
}


void *splay_tree_walk_until(struct splay_tree *tree, bool (*ret)(void *el, void *udata), void *udata)
{
    if (!tree->root) return NULL;
    return node_walk_recursive(tree->root, ret, udata)->value;
}


void *splay_tree_next(struct splay_tree *tree, uint64_t descriptor)
{
    struct node *node = find_node(tree, descriptor);
    if (!node) return NULL;
    if (node->children[1])
        return node->children[1]->value;
    else
        return NULL;
}


void splay_tree_delete(splay_tree *tree, uint64_t descriptor)
{
    struct node *node = find_node(tree, descriptor);
    splay_to_root(node);
    struct node *right = node->children[1];
    struct node *left = node->children[0];
    if (right) {
        while (right->children[1])
            right = right->children[1];
        splay_to_root(right);
        right->chilren[1] = left;
        if (left) left->father = right;
    }
    free(node);
}


void splay_tree_dealloc(splay_tree *tree)
{
    walk_and_delete(tree->root);
    return;
}

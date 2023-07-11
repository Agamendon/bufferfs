#include <stdio.h>
#include <vector.h>
#include <splay_tree.h>


int cmp(int *a, int *b)
{
    if (*a < *b)
        return -1;
    else if (*a == *b)
        return 0;
    else
        return 1;
}


int main()
{
    splay_tree spl;
    splay_tree_init(&spl, sizeof(int), *cmp, *free);
    int a = 2;
    int b = 1;
    int c = 3;
    splay_tree_insert(&spl, &a);
    splay_tree_insert(&spl, &b);
    splay_tree_insert(&spl, &c);

    printf("%d\n", *((int *) splay_tree_find(&spl, &c)->value));
    printf("%d\n", *((int *) splay_tree_find(&spl, &b)->value));
    printf("%d\n", *((int *) splay_tree_find(&spl, &a)->value));
    return 0;
}
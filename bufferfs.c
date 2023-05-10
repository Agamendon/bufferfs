#include <stdio.h>
#include <bufferfs/vector.h>


vector v; // vector of integers


int main()
{
    if(vector_init(&v, sizeof(long long), 0) == NULL) {
        printf("Vector init returned NULL\n");
        return 0;
    }
    
    return 0;
}
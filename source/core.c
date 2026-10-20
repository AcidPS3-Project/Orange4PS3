#include "core.h"
#include "random.h"

void shuffle(_Bool *array, size_t n)
{
	shuffling = true;
	PlaySFX(&sfx_cylinder);
    if(n > 1)
    {
        for (size_t i = n - 1; i > 0; i--)
        {
            size_t j = rand() % (i + 1);
            
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    
    printf("Shuffled array:\n");
    for (size_t z = 0; z < n; z++) printf("%d ", array[z]);
    printf("\n");
}
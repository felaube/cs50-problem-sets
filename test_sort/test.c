#include <stdio.h>
#include <time.h>

void test_sort(int array[], int length);

int main(void)
{
    int n = 13;
    int array[13] = {9, 4, 6, 5, 8, 7, 2, 1, 6, 12, 0, 5, 10};
    int i;

    clock_t t;
    t = clock();
    for (int times = 0; times < 1000000; times++)
    {
        array[0] = 9;
        array[1] = 4;
        array[2] = 6;
        array[3] = 5;
        array[4] = 8;
        array[5] = 7;
        array[6] = 2;
        array[7] = 1;
        array[8] = 6;
        array[9] = 12;
        array[10] = 0;
        array[11] = 5;
        array[12] = 10;
        test_sort(array, n);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("Mege sort took %f seconds to execute \n", time_taken);


    for (i = 0; i < n; i++)
    {
        printf("%i ", array[i]);
    }

    printf("\n");
}

void test_sort(int array[], int length)
{
    if length == 1
    {
        return;
    }
    else
    {
        int new_array[length];
        int i, j;
        
        for(i = 0; i < length; i++)
        {
            new_array[i] = array[i];
        }
        
        for(i = 0; i < length; i++)
        {
            for(i = 0; i < length; i++)
            {
                
            }
        }
    }
}
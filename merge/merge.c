#include <stdio.h>
#include <time.h>

void merge_sort(int array[], int length);

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
        merge_sort(array, n);
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

void merge_sort(int array[], int length)
{
    if (length == 1)
    {
        return;
    }
    else
    {
        int i, j, k;
        // Create left and right arrays
        int left_array[length / 2];
        int right_array[length / 2 + length % 2];

        for (i = 0; i < length / 2; i++)
        {
            left_array[i] = array[i];
        }

        for (i = length / 2; i < length; i++)
        {
            right_array[i - length / 2] = array[i];
        }

        // Sort left array
        merge_sort(left_array, length / 2);

        // Sort right array
        merge_sort(right_array, length / 2 + length % 2);

        // Merge them
        i = 0; // left_array current index
        j = 0; // right_array current index
        k = 0; // array current index
        while ((i < length / 2) && (j < length / 2 + length % 2))
        {
            // Add elements until one of the arrays reaches its end
            if (left_array[i] > right_array[j])
            {
                array[k] = right_array[j];
                j++;
            }
            else
            {
                array[k] = left_array[i];
                i++;
            }
            k++;
        }
        // Add the remaining elements
        // Add the remaning elements from left_array
        while(i < length / 2)
        {
            array[k] = left_array[i];
            i++;
            k++;
        }

        // Add the remaning elements from right_array
        while(j < length / 2 + length % 2)
        {
            array[k] = right_array[j];
            j++;
            k++;
        }

    }
}
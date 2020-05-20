#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("This programm accepts exactly 1 command line argument\n");
        return 1;
    }

    FILE *raw = fopen(argv[1], "r");

    if (raw == NULL)
    {
        printf("This file cannot be opened for reading\n");
        return 1;
    }

    // Works with both array implementation (line below) and malloc implementation (2 lines below)
    //BYTE block[BLOCK_SIZE];
    BYTE *block = malloc(BLOCK_SIZE * sizeof(BYTE));

    int file_number = 0;

    FILE *jpgfile = NULL;

    // While not at the end of "raw", continue reading it
    while (1)
    {
        fread(block, sizeof(BYTE), BLOCK_SIZE, raw);
        // Operator >> shifts all bits in block[3] 4 spaces rightward, if the first bits were 1110,
        // Now those are the last 4, meaning that the number is now 14,
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] >> 4) == 14)
        {
            if (jpgfile == NULL)
            {
                // This is the first .jpg file found in raw
                jpgfile = fopen("000.jpg", "w");

            }
            else
            {
                // Reached jpgfile end
                // Close it and create another one
                fclose(jpgfile);
                char filename[sizeof("000.jpg")];

                // Increment for the next image
                file_number++;

                // Create next image
                sprintf(filename, "%03d.jpg", file_number);
                jpgfile = fopen(filename, "w");
            }

        }

        // Check if any jpg file was created, if it was, write the current block to the current jpg file
        if (jpgfile != NULL)
        {
            // Check if you've read beyond the end of the file "raw", if you didn't, save what you've read, else, break the loop
            if (!feof(raw))
            {
                fwrite(block, sizeof(BYTE), BLOCK_SIZE, jpgfile);
            }
            else
            {
                break;
            }
        }
    }

    fclose(jpgfile);
    fclose(raw);

    free(block);
}

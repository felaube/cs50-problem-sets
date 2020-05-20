#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main()
{
    // result == 1 means MasterCard, 2 means American Express, 3 VISA and 0 Invalid
    // Initial guess is that the card is Invalid
    int result = 0;

    int checksum = 0;

    // Prompt the user for the card number
    long number = get_long("Card Number: ");

    // Check if the first digits are 4, 34, 37, or 51-55, if not, declare the card as invalid
    if ((number / (long)pow(10, 14)) <= 55 && (number / (long)pow(10, 14)) >= 51)
    {
        // Potential MasterCard
        result = 1;
    }
    else if ((number / (long)pow(10, 13)) == 34 || (number / (long)pow(10, 13)) == 37)
    {
        // Potential American Expresss
        result = 2;
    }
    else if ((number / (long)pow(10, 12)) == 4 || (number / (long)pow(10, 15)) == 4)
    {
        // Potential VISA
        result = 3;
    }
    if (result != 0)
    {
        // Stores the current digit
        int digit;
        // Stores the position of the current digit
        int n_digit = 1;
        // Calculate checksum
        while (number > 0)
        {
            digit = (number % (long) pow(10, n_digit)) / (long) pow(10, n_digit - 1);

            // Check whether of not the current digit must be multiplied
            if (n_digit % 2 == 1)
            {
                checksum = checksum + digit;
            }
            else
            {
                if (digit < 5)
                {
                    checksum = checksum + 2 * digit;
                }
                else
                {
                    checksum = checksum + (digit * 2) % 10 + (digit * 2) / 10;
                }
            }

            // Subtract the current digit from the number
            number = number - digit * pow(10, n_digit - 1);

            // Update n_digit
            n_digit++;
        }

        // Checksum
        if (checksum % 10 != 0)
        {
            result = 0;
        }

    }
    switch (result)
    {
        case 0:
        {
            printf("INVALID\n");
            break;
        }
        case 1:
        {
            printf("MASTERCARD\n");
            break;
        }
        case 2:
        {
            printf("AMEX\n");
            break;
        }
        case 3:
        {
            printf("VISA\n");
            break;
        }
    }
}

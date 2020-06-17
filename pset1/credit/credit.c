#include <cs50.h>
#include <stdio.h>

int func(long credit_number, int count);

int main(void)
{
    long credit_number;
    do
    {
        credit_number = get_long("Your credit number: \n");
    } while(credit_number < 0);

    if(credit_number > 5100000000000000 && credit_number < 5600000000000000)
    {

        if(func(credit_number, 8) == 0)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (credit_number > 340000000000000 && credit_number < 380000000000000)
    {
        if(credit_number > 340000000000000 && credit_number < 350000000000000)
        {
            if(func(credit_number, 8) == 0)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if(credit_number > 370000000000000 && credit_number < 380000000000000)
        {
            if(func(credit_number, 8) == 0)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (credit_number > 4000000000000 && credit_number < 5000000000000000)
    {
        if(credit_number > 4000000000000 && credit_number < 5000000000000)
        {
            if(func(credit_number, 7) == 0)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if(credit_number > 4000000000000000 && credit_number < 5000000000000000)
        {
            if(func(credit_number, 8) == 0)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }

}

int func (long credit_number, int count)
{
    int temp = 0;
    int sum = 0;
    int sumAnother = 0;

    for (long i = 10, j = 0; j < count; i *= 100, j++)
    {
        temp = ((credit_number / i) % 10) * 2;
        if(temp >= 10)
        {
            sum += (temp / 1) % 10;
            sum += (temp / 10) % 10;
        }
        else
        {
            sum += temp;
        }
    }

    for (long i = 1, j = 0; j < count; i *= 100, j++)
    {
        sumAnother += ((credit_number / i) % 10);
    }

    return (sum + sumAnother) % 10;

}
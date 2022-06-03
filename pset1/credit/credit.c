#include <stdio.h>
#include <cs50.h>


int main(void)
{
    long creditNumber = get_long("Number: ");

    int length = 0;
    long checkLength = creditNumber;
    while (checkLength != 0)
    {
        checkLength /= 10;
        length++;
    }

    int counter = length - 2;
    long firstTwoNum = creditNumber;
    while (counter != 0)
    {
        firstTwoNum /= 10;
        counter--;
    }

    long lastNum = creditNumber;
    int sumOfTimesTwo = 0;
    int sumOfOther = 0;
    bool isNeeded = false;
    while (creditNumber > 0)
    {
        creditNumber = lastNum / 10;
        if (isNeeded)
        {
            int currNum = (lastNum % 10) * 2;
            if (currNum >= 10)
            {
                sumOfTimesTwo += currNum / 10;
                sumOfTimesTwo += currNum % 10;
            }
            else
            {
                sumOfTimesTwo += currNum;
            }
            isNeeded = false;
        }
        else
        {
            sumOfOther += lastNum % 10;
            isNeeded = true;
        }

        lastNum = creditNumber;
    }

    int sum = sumOfTimesTwo + sumOfOther;

    if (sum % 10 == 0)
    {
        if (length == 15 && (firstTwoNum == 34 || firstTwoNum == 37))
        {
            printf("AMEX\n");
        }
        else if ((length == 13 || length == 16) && (firstTwoNum / 10) == 4)
        {
            printf("VISA\n");
        }
        else if (length == 16 && (firstTwoNum == 51 || firstTwoNum == 52 || firstTwoNum == 53 || firstTwoNum == 54 || firstTwoNum == 55))
        {
            printf("MASTERCARD\n");
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
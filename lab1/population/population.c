#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startSize = 0;
    int endSize = 0;
    int years = 0;

    // TODO: Prompt for start size
    do
    {
        printf("Start size: ");
        scanf("%d", &startSize);
    }
    while (startSize < 9);

    // TODO: Prompt for end size
    do
    {
        printf("End size: ");
        scanf("%d", &endSize);
    }
    while (endSize < startSize);

    // TODO: Calculate number of years until we reach threshold
    int currentSize = startSize;
    while (currentSize < endSize)
    {
        int newLamas = (int) currentSize / 3;
        int passedLamas = (int) currentSize / 4;
        currentSize = currentSize + newLamas - passedLamas;
        years++;
    }

    // TODO: Print number of years
    printf("Years: %d\n", years);
}

/*
Start size: 100
End size: 200
Years: 9
*/
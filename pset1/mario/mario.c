#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;

    do
    {
        printf("Height: ");
        scanf("%d", &height);
    }
    while (height < 1 || height > 8);

    int blanks = height;
    int blocks = 0;
    for (int i = 0; i < height; i++)
    {
        blanks -= 1;
        for (int j = 0; j < blanks; j++)
        {
            printf(" ");
        }

        blocks = height - blanks;
        for (int y = 0; y < blocks; y++)
        {
            printf("#");
        }

        printf("  ");

        for (int x = 0; x < blocks; x++)
        {
            printf("#");
        }
        printf("\n");
    }
}
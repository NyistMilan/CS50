#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    FILE *file;

    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Image cannot be opened for reading...\n");
        return 1;
    }

    int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE];
    char imageName[8];
    int counter = 0;
    FILE *image;

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        //Check if it's a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //In case of first JPEG
            if (counter == 0)
            {
                sprintf(imageName, "%03i.jpg", counter);
                image = fopen(imageName, "w");
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, image);
                counter++;
            }
            //Starting a new JPEG
            else if (counter > 0)
            {
                fclose(image);
                sprintf(imageName, "%03i.jpg", counter);
                image = fopen(imageName, "w");
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, image);
                counter++;
            }

        }
        //Continue reading current image's bytes
        else if (counter > 0)
        {
            fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, image);
        }
    }

    //Close stuff
    fclose(image);
    fclose(file);
    return 0;
}
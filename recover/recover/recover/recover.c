#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;

bool isStartNewJpeg(BYTE buffer[]);
int main(int argc, char *argv[])
{
    int blockSize = 512;
    int fileNameSize = 8;
    
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    

    BYTE buffer[blockSize];
    int findex = 0;
    bool foundJpeg = false;
    FILE *out;
    while (fread(buffer, blockSize, 1, infile))
    {
        if (isStartNewJpeg(buffer))
        {
            if (!foundJpeg)
            {
                foundJpeg = true;
            }
            else
            {
                fclose(out);
            }
            
            char fileName[fileNameSize];
            sprintf(fileName, "%03i.jpg", findex++);
            out = fopen(fileName, "w");
            if (out == NULL)
            {
                return 1;
            }
            fwrite(buffer, blockSize, 1, out);
            
        }
        else if (foundJpeg)
        {
            fwrite(buffer, blockSize, 1, out);
        }
    }
    fclose(out);
    fclose(infile);
}

bool isStartNewJpeg(BYTE buffer[])
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
}
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

// aliases for C/C++ primitive data types
// https://msdn.microsoft.com/en-us/library/cc230309.aspx
typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }

    char *infile = argv[1];

    FILE *file = fopen(infile, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    BYTE *buffer = malloc(sizeof(BYTE) * 512);
    int i = 0;
    fseek(file, 512, SEEK_CUR);
    //fread(buffer, 512, 1, file);
    do {
        if (bytesRead == 0 && feof(inptr))
        {
            break;
        }

        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {

            char *filename = malloc(sizeof(char) * 4);
            sprintf(filename, "%03i.jpg", i++);
            FILE *img = fopen(filename, "w");
            if (img == NULL) {
                fclose(img);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }
            while(1) {
                fwrite(buffer, sizeof(buffer), 1, img);
                fread(buffer, 512, 1, file);
                if(buffer[0] == 0xff &&
                    buffer[1] == 0xd8 &&
                    buffer[2] == 0xff &&
                    (buffer[3] & 0xf0) == 0xe0) {
                        break;
                }
            }
           
            fclose(img);
        } else {
            fread(buffer, 512, 1, file);
        }
    } while(buffer[0] != 0x00 && buffer[1] != 0x00 && buffer[2] != 0x00);
}

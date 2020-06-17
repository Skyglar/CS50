#include <stdio.h>
#include <stdlib.h> // abs()

#include "bmp.h" // bmp structs

int main(int argc, char* argv[])
{
// ensure proper usage
if (argc != 4)
{
    printf("Usage: ./copy n infile outfile\n");
    return 1;
}
// remember filenames
int factor = atoi(argv[1]);
char* infile = argv[2];
char* outfile = argv[3];

// open input file
FILE* inptr = fopen(infile, "r");
if (inptr == NULL)
{
    printf("Could not open %s.\n", infile);
    return 2;
}

// open output file
FILE* outptr = fopen(outfile, "w");
if (outptr == NULL)
{
    fclose(inptr);
    fprintf(stderr, "Could not create %s.\n", outfile);
    return 3;
 }

 // read infile's BITMAPFILEHEADER
 BITMAPFILEHEADER bf;
 fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

 // read infile's BITMAPINFOHEADER
 BITMAPINFOHEADER bi;
 fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

 // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
 if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
 bi.biBitCount != 24 || bi.biCompression != 0)
 {
      fclose(outptr);
      fclose(inptr);
      fprintf(stderr, "Unsupported file format.\n");
      return 4;
 }

//create outfile bf and bi
BITMAPFILEHEADER obf;
BITMAPINFOHEADER obi;

// first make a copy of the input headers
obf = bf;
obi = bi;


// update values
obi.biWidth *= factor;
obi.biHeight *= factor;

int inPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
int outPadding = (4 - (obi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

obi.biSizeImage =
    ((sizeof(RGBTRIPLE) * obi.biWidth) + outPadding)
    * abs(obi.biHeight);

obf.bfSize = obi.biSizeImage +
    sizeof(BITMAPFILEHEADER) +
    sizeof(BITMAPINFOHEADER);

// write headers to outfile
fwrite(&obf, sizeof(BITMAPFILEHEADER), 1, outptr);
fwrite(&obi, sizeof(BITMAPINFOHEADER), 1, outptr);


// create buffer to hold a scanline's worth of pixels
RGBTRIPLE * scanline = malloc(sizeof(RGBTRIPLE) * obi.biWidth);

for(int i = 0; i < abs(bi.biHeight); i++)
{
    int pixel_tracker = 0;

    for(int j = 0; j < bi.biWidth; j++)
    {
        // create placeholder for pixel
        RGBTRIPLE pixel;

        // read in the pixel
        fread(&pixel, sizeof(RGBTRIPLE), 1, inptr);

        for(int k = 0; k < factor; k++)
        {
            // add to the buffer factor times
            // use pointer arithmetic
            // assigning the pixel in the correct place
            // by leveraging the current value of pixel_tracker
            *(scanline + pixel_tracker) = pixel;
            pixel_tracker++;
        }
    }

    // fseek over input padding to get to next row
    fseek(inptr, inPadding, SEEK_CUR);

    // loop through adding rows based on resize factor
    for(int l = 0; l < factor; l++)
    {
        fwrite(scanline, sizeof(RGBTRIPLE), obi.biWidth, outptr);

        // also add padding to each row inside the loop manually
        for(int m = 0; m < outPadding; m++)
        {
            fputc(0x00, outptr);
        }
    }

}


//free buffer
free(scanline);

// close infile
fclose(inptr);

// close outfile
fclose(outptr);

return 0;
}
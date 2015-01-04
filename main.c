#include <stdlib.h>
#include <stdio.h>

#define write00(x) fputc(0, x);fputc(0, x);fputc(0, x);fputc(0, x)

void writeLittleEndianInt(int i, FILE* file);

int main (int argc, char** argv)
{
    FILE *file, *conv;
    int i, h, w, size, r, g, b;

    if(argc == 3)
    {
        file = fopen(argv[1], "rb");
        conv = fopen(argv[2], "wb+");
        if(file != NULL)
        {
            w = fgetc(file) + (fgetc(file) << 8) + (fgetc(file) << 16) + (fgetc(file) << 24);
            h = fgetc(file) + (fgetc(file) << 8) + (fgetc(file) << 16) + (fgetc(file) << 24);
            fgetc(file);fgetc(file);fgetc(file);fgetc(file); //4 useless bytes?
            if(conv != NULL)
            {
                fputs("BM", conv);
                size = 3 * h * w + 54; //header size = 54
                writeLittleEndianInt(size, conv);     //File size
                writeLittleEndianInt(1764, conv);     //BMP file generator signature, useless
                writeLittleEndianInt(0x36, conv);     //Pixel array start offset
                //DIB header start @ 0x0E
                writeLittleEndianInt(40, conv);    //DIB header size (40 bytes)
                writeLittleEndianInt(w, conv);     //Image width
                writeLittleEndianInt(~(h-1), conv);    //Image height (negative to store from up to down)
                fputc(1, conv);fputc(0, conv);     //Color panes (must be 1)
                fputc(24, conv);fputc(0, conv);    //Bits per pixel
                write00(conv);                     //Compression (0 = none)
                writeLittleEndianInt(3*h*w, conv); //Size of the pixel array in bytes (?)
                write00(conv);                     //Horizontal resolution (set to 0, useless?)
                write00(conv);                     //Vertical resolution (set to 0, useless?)
                write00(conv);                     //Colors in palette (set to 0, no palette)
                write00(conv);                     //Number of important colors (set to 0, no palette)
                for(i = 0; i < w * h; i++)
                {
                    r = fgetc(file);
                    g = fgetc(file);
                    b = fgetc(file);
                    fputc(b, conv);
                    fputc(g, conv);
                    fputc(r, conv);
                    fgetc(file); //Alpha ?
                }
                fclose(file);
                fclose(conv);
            }
        }
        else
            printf("Error opening file.\n");
    }
    return 0;
}

void writeLittleEndianInt(int i, FILE* file)
{
    fputc((i&0x000000FF)      , file);
    fputc((i&0x0000FF00) >> 8 , file);
    fputc((i&0x00FF0000) >> 16, file);
    fputc((i&0xFF000000) >> 24, file);
}

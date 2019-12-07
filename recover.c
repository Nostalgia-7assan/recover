#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE ;

int main(int argc , char * argv[])
{
    // check for arguments
    if (argc != 2)
    {
        fprintf(stderr , "Usage: ./recover forensic_img\n");
        return 1 ;
    }
    FILE *input = fopen(argv[1] , "r") ;
    // check if the file is valid
    if (input == NULL)
    {
        fprintf(stderr , "Cannot open %s\n", argv[1]);
        return 2 ;
    }
    // buffer the bloks
    BYTE buffer[512];
    // filename 000.jpg
    char * filename = malloc(8);
    // the number of file
    int counter = 0 ;
    // pointer to a outout file
    FILE *output = NULL  ;
    bool flag = false;
    while(1)
    {
        int x = fread(&buffer , sizeof(buffer) , 1 ,input) ;
        if (x != 1)
        {
            break ;
        }
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            flag = true ;
            if (counter != 0)
            {
                fclose(output);
            }
            sprintf(filename , "%03i.jpg" , counter) ;
            output = fopen(filename, "w");
            fwrite(&buffer , sizeof(buffer) , 1 ,output) ;
            counter++ ;
        }
        else
        {
            if (flag == false)
            {
                continue ;
            }
            else
            {
                fwrite(&buffer , sizeof(buffer) , 1 ,output) ;
            }
        }
    }
    free(filename);
    fclose(input);
    fclose(output);

}
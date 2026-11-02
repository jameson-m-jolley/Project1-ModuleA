#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        printf("Usage: %s input.pgm output.pgm threshold\n", argv[0]);
        return 1;
    }

    char *input_file=argv[1];
    char *output_file=argv[2];
    char threshold=(char)atoi(argv[3]) + 128;
    printf("threshold = %d\n",threshold);
    FILE *Fin;
    FILE *Fout; 

    /* 
    ToDO 1: Open input file in read-only binary mode i.e. "rb"
    Hint: google "man fopen()"
    */
    Fin = fopen(input_file, "rb");


    /*
    ToDo 2: Do error checking to ensure file was opened sucessfully before proceeding
    */

    //======================Reading File Header=====================================
    //read the first line just to advance the cursor in file but dont use the line.

    if (Fin == NULL){
        printf("the file did not open :(");
        return 0;
    }


    char Line[256];

    fgets(Line,256,Fin);

    //read the width, and height of the file 
    
    int width, height, max_value;
    
    fgets(Line, sizeof(Line), Fin); // P5
    sscanf(Line, "%d %d", &width, &height);

    //read the maximum pixel value;
    sscanf(Line, "%d", &max_value);
    (void)max_value;
    (void)fgetc(Fin); // consume the newline before pixel data

    //====================Done Reading File Header===================================
    /*
    ToDo 3: Allocate memory to store width x height number of pixels for black and white image. 
    Note that each pixel has max value of 255, so an 8-bit char is sufficent to store the pixel value.
    Hint: google "man malloc";
    */
   int num_of_bites = width* height;
   char *mem = (char *)malloc(num_of_bites * sizeof(char));
    

    /*
    ToDo 4: Do Error checking to make sure memory was successfully allocated before proceeding.
    In case of failure, exit the program.
    */

    if (mem == NULL) {
        printf("error making memory :(");
    }


    /*
    ToDo 5: read all the content of input image file into allocated memory.
    Hint: Google "man fread(3p)"
    */
    size_t elements_read;
    elements_read = fread(mem , sizeof(char) , num_of_bites , Fin);
    printf("fread %zu bites\n", elements_read);


    /*ToDo 6: Do error checking to make sure that fread was sucessfull. If not, exit the program.
    Hoever, free the dynamically allocated memory before exiting.
    */
   if (elements_read != (size_t)num_of_bites){
        printf("something is wrong file may not exist\n");
        free(mem);
        return 0;
   };


    //ToDo 7: close the input file

    fclose(Fin);
    

    /*
    ToDo 8: Iterate over all bytes of the dynmaically allocated memory (where each byte stores one pixel of the input file) 
    and update its avlue by doing following operation.

    if (value of the byte is greater than or equal to threshold)
        {
        set value of the byte to 255.
        }
    else
        {
        set value of the byte to 0.
        }
    */
   for (int i = 0; i < num_of_bites; i++){
        if (mem[i]>=(char)threshold){
            mem[i] =(char)255;
        }
        else{
            mem[i] =0;
        }
   }
   


    /*
    ToDo 9: open the ouput file with "w+" permission
    Hint: google "man fopen"
    */
   Fout = fopen(output_file,"w+");


    /*
    ToDo 10: Do error checking to make sure that output file is opened sucessfully. If not, exit the program.
    However, free the dynamically allocated memory before exiting.
    */
if (Fout == NULL){
     printf("something is wrong opening output file\n");
     free(mem);
     return 0;
    }

    

    //======================Write header of the output file====================
    fprintf(Fout, "P5\n%d %d\n255\n", width, height);
    //===============Done writing header of the output file====================

    /*
    ToDo 11: Write output file with the content of dynamically allocated memory
    Hint: google "man fwrite(3p)"
    */
    size_t bites_out = fwrite(mem,sizeof(char),num_of_bites,Fout);
    

    /*
    ToDo 12: Do error checking to make sure that all bytes from allocated memory are written successfully to the output image file.
    If not, free the dynmaically allocated memory and exit the program.
    */
    if (bites_out != (size_t)num_of_bites){
        printf("error writing file \n");
        free(mem);
        return 0;
    }
    fflush(Fout);
    
    /*
    ToDo 13: Close output file.
    */
    fclose(Fout);
   
    
    /*
    ToDo 14: Free the memory that was allocated to store black and white image.
    Hint: Google "man free". 
    */
   free(mem);


    return 0;
}

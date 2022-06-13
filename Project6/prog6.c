/* Timothy Driscoll
   ECE 2220 Project 6, 4/15/19

   Program Description: This program is designed apply the use of dynamic memory
   allocation, manipulation of two dimensional arrays, manipulation of
   structures, use of pointers to structures, manipulation of files and use of
   commmand line arguments. This program takes in a bmp image and performs two
   different operations on the file, returning two output bmp files. One of the
   output files is shaded depending on the three values that the user passes
   in through the command line. The second output file is the orginal image
   after a filter is applied that works as some kind of edge detector.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Define the neccesary structures
//first of the two header structures
struct Header
  {
    unsigned short int Type; //magic identifier
    unsigned int size;       //File size in bytes
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset;    //Offset to data (in B)
  } header; //14 Bytes
//second of the two header structures
struct InfoHeader
  {
    unsigned int size;           // Header size in bytes
    int Width, Height;           // Width / Height of image
    unsigned short int Planes;   // Number of colour planes
    unsigned short int Bits;     // Bits per pixel
    unsigned int Compression;    // Compression type
    unsigned int ImageSize;      // Image size in bytes
    int xResolution, yResolution;// Pixels per meter
    unsigned int Colors;         // Number of colors
    unsigned int ImportantColors;// Important colors
  } infoHeader;                  // 40 Bytes

//pixel structure containing three unsigned chars each for a pixel (RGB)
struct Pixel
  {
    unsigned char red, green, blue;
  };

/*This function is designed to read in all the header and pixel data from the
input file. The only argument needed to be based in is the file pointer to the
input file where the data is read in from. The function first reads in the two
seperate header structures using fread, then all the pixels are read in to a
dynamically allocated two dimensional array that is returned from the function*/
struct Pixel **getData(FILE *inFile)
  {
    //Intialize any needed local variables
    struct Pixel **image;
    int i;

    //Read in all the header data using freads to get each component of the
    //structure
    fread(&header.Type, sizeof(header.Type),1,inFile);
    fread(&header.size, sizeof(header.size),1,inFile);
    fread(&header.Reserved1, sizeof(header.Reserved1),1,inFile);
    fread(&header.Reserved2, sizeof(header.Reserved2),1,inFile);
    fread(&header.Offset, sizeof(header.Offset),1,inFile);

    //Read in all the header info data using freads to get each component of the
    //structure
    fread(&infoHeader.size, sizeof(infoHeader.size),1,inFile);
    fread(&infoHeader.Width, sizeof(infoHeader.Width),1,inFile);
    fread(&infoHeader.Height, sizeof(infoHeader.Height),1,inFile);
    fread(&infoHeader.Planes, sizeof(infoHeader.Planes),1,inFile);
    fread(&infoHeader.Bits, sizeof(infoHeader.Bits),1,inFile);
    fread(&infoHeader.Compression, sizeof(infoHeader.Compression),1,inFile);
    fread(&infoHeader.ImageSize, sizeof(infoHeader.ImageSize),1,inFile);
    fread(&infoHeader.xResolution, sizeof(infoHeader.xResolution),1,inFile);
    fread(&infoHeader.yResolution, sizeof(infoHeader.yResolution),1,inFile);
    fread(&infoHeader.Colors, sizeof(infoHeader.Colors),1,inFile);
    fread(&infoHeader.ImportantColors, sizeof(infoHeader.ImportantColors),1,
                                                                    inFile);

    //Move to the start of the pixels in the file
    fseek(inFile,header.Offset,SEEK_SET);

    //allocate two dimensional array of pixel structures bassed off of the size
    //of the height and width of the image
    image = (struct Pixel**)malloc(sizeof(struct Pixel*)*infoHeader.Height);
    for(i = 0; i<infoHeader.Height; i++)
      {
        //malloc row by row of the array using the width of the image
        image[i] = (struct Pixel*)malloc(sizeof(struct Pixel)*infoHeader.Width);
        //After allocating the space for the rows fread a row of pixels from
        //the file
        fread(image[i],sizeof(struct Pixel),infoHeader.Width,inFile);
      }
    //Return the two dimensional array
    return image;
  }

/*This function is designed to write the updated pixel data back to a given
output file. It accepts two arguments first being the pointer to the output file
and the second being all the two dimensional array of pixels. The function uses
a series of fwrites to write the header data and pixel data to the output file*/
void writeToFile(FILE* outFile, struct Pixel** image)
  {
    //Declare any local variables
    int i;

    //write the header data to the output file by using fwrites
    fwrite(&header.Type, sizeof(header.Type),1,outFile);
    fwrite(&header.size, sizeof(header.size),1,outFile);
    fwrite(&header.Reserved1, sizeof(header.Reserved1),1,outFile);
    fwrite(&header.Reserved2, sizeof(header.Reserved2),1,outFile);
    fwrite(&header.Offset, sizeof(header.Offset),1,outFile);

    //write the header info data to the output file by using fwrites
    fwrite(&infoHeader.size, sizeof(infoHeader.size),1,outFile);
    fwrite(&infoHeader.Width, sizeof(infoHeader.Width),1,outFile);
    fwrite(&infoHeader.Height, sizeof(infoHeader.Height),1,outFile);
    fwrite(&infoHeader.Planes, sizeof(infoHeader.Planes),1,outFile);
    fwrite(&infoHeader.Bits, sizeof(infoHeader.Bits),1,outFile);
    fwrite(&infoHeader.Compression, sizeof(infoHeader.Compression),1,outFile);
    fwrite(&infoHeader.ImageSize, sizeof(infoHeader.ImageSize),1,outFile);
    fwrite(&infoHeader.xResolution, sizeof(infoHeader.xResolution),1,outFile);
    fwrite(&infoHeader.yResolution, sizeof(infoHeader.yResolution),1,outFile);
    fwrite(&infoHeader.Colors, sizeof(infoHeader.Colors),1,outFile);
    fwrite(&infoHeader.ImportantColors, sizeof(infoHeader.ImportantColors),1,
                                                                    outFile);

    //Move to the start of the pixels in the file
    fseek(outFile,header.Offset,SEEK_SET);

    //for loop to iterate through each row and write all the pixel structures
    //back to the output file
    for(i = 0; i<infoHeader.Height; i++)
      {
        fwrite(image[i],sizeof(struct Pixel),infoHeader.Width,outFile);
      }

  }
/*This function is designed to change all the individual red, green and blue
pixel values by a user defined amount. The function accepts a two dimensional
array of pixels, and then the three different integers representing the amounts
that the user would like to adjust the pixel values. The function then applies
the changes to a new two dimensional array of structures and returns it */
struct Pixel **shadeImage(struct Pixel **image, int redShift, int greenShift,
                                                              int blueShift)
  {
    //Declare any neccesary local variables
    struct Pixel **shadedImage;
    int i, j,tempPixelR,tempPixelG, tempPixelB;

    //allocate two dimensional array of pixel structures
    shadedImage = (struct Pixel**)malloc(sizeof(struct Pixel*)
                                          *infoHeader.Height);
    for(i = 0; i<infoHeader.Height; i++)
      {
        shadedImage[i] = (struct Pixel*)malloc(sizeof(struct Pixel)
                                                *infoHeader.Width);
      }

    //for loop to iterate through each row of array
    for(i=0; i < infoHeader.Height; i++)
      {
        //for loop to iterate through each pixel in the row
        for(j=0; j < infoHeader.Width; j++)
          {
            // RED
            //set orginal red pixel to a temp variable
            tempPixelR = image[i][j].red;
            //apaply the user offset to the temp pixel
            tempPixelR += redShift;
            //if the pixel is greater then 255 set it to 255
            if(tempPixelR > 255)
              tempPixelR = 255;
            //if the pixel is less then 0 set it to 0
            else if(tempPixelR < 0)
              tempPixelR = 0;
            //set the temp pixel value to the new shadedImage array
            shadedImage[i][j].red = tempPixelR;

            //GREEN
            //set orginal red pixel to a temp variable
            tempPixelG = image[i][j].green;
            //apaply the user offset to the temp pixel
            tempPixelG += greenShift;
            //if the pixel is greater then 255 set it to 255
            if(tempPixelG > 255)
              tempPixelG = 255;
            //if the pixel is less then 0 set it to 0
            else if(tempPixelG < 0)
              tempPixelG = 0;
            //set the temp pixel value to the new shadedImage array
            shadedImage[i][j].green = tempPixelG;

            //BLUE
            //set orginal red pixel to a temp variable
            tempPixelB = image[i][j].blue;
            //apaply the user offset to the temp pixel
            tempPixelB += blueShift;
            //if the pixel is greater then 255 set it to 255
            if(tempPixelB > 255)
              tempPixelB = 255;
            //if the pixel is less then 0 set it to 0
            else if(tempPixelB < 0)
              tempPixelB = 0;
            //set the temp pixel value to the new shadedImage array
            shadedImage[i][j].blue = tempPixelB;
          }
      }
    //return the updated (shaded) two dimension array
    return shadedImage;
  }

/*This function is designed to apply a filter to all the pixels in the image
working as a sort of edge detector. The function excepts a two dimensional
array of pixel pointers and returns the filtered version of this two
dimensional array. The function uses a matrixa and series of calculations to
calculate all the new pixel values. */
struct Pixel **filterImage(struct Pixel **image)
  {
    //Intialize all the neccesary local variables
    struct Pixel **edgeImage;
    char tempR, tempG, tempB;
    int i, j;
    //This is the matrix used to filter the pixels
    char Matrix[3][3] =
      {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
      };

    //allocate two dimensional array of pixel structures
    edgeImage = (struct Pixel**)malloc(sizeof(struct Pixel*)*infoHeader.Height);
    for(i = 0; i<infoHeader.Height; i++)
      {
        edgeImage[i] = (struct Pixel*)malloc(sizeof(struct Pixel)*
                                                infoHeader.Width);
      }

    //for loop to iterate through each row of array
    for(i=1; i < infoHeader.Height-1; i++)
      {
        //for loop to iterate through each pixel in the row
        for(j=1; j < infoHeader.Width-1; j++)
          {
            //reintialize all the temp pixel values to 0
            tempR = 0;
            tempG = 0;
            tempB = 0;

            //Using the filter matrix and orginal pixel array apply a series of
            //9 diffenent calculations to each individual pixel
            tempR += (Matrix[0][0] * image[i-1][j-1].red);
            tempG += (Matrix[0][0] * image[i-1][j-1].green);
            tempB += (Matrix[0][0] * image[i-1][j-1].blue);

            tempR += (Matrix[0][1] * image[i-1][j].red);
            tempG += (Matrix[0][1] * image[i-1][j].green);
            tempB += (Matrix[0][1] * image[i-1][j].blue);

            tempR += (Matrix[0][2] * image[i-1][j+1].red);
            tempG += (Matrix[0][2] * image[i-1][j+1].green);
            tempB += (Matrix[0][2] * image[i-1][j+1].blue);

            tempR += (Matrix[1][0] * image[i][j-1].red);
            tempG += (Matrix[1][0] * image[i][j-1].green);
            tempB += (Matrix[1][0] * image[i][j-1].blue);

            tempR += (Matrix[1][1] * image[i][j].red);
            tempG += (Matrix[1][1] * image[i][j].green);
            tempB += (Matrix[1][1] * image[i][j].blue);

            tempR += (Matrix[1][2] * image[i][j+1].red);
            tempG += (Matrix[1][2] * image[i][j+1].green);
            tempB += (Matrix[1][2] * image[i][j+1].blue);

            tempR += (Matrix[2][0] * image[i+1][j-1].red);
            tempG += (Matrix[2][0] * image[i+1][j-1].green);
            tempB += (Matrix[2][0] * image[i+1][j-1].blue);

            tempR += (Matrix[2][1] * image[i+1][j].red);
            tempG += (Matrix[2][1] * image[i+1][j].green);
            tempB += (Matrix[2][1] * image[i+1][j].blue);

            tempR += (Matrix[2][2] * image[i+1][j+1].red);
            tempG += (Matrix[2][2] * image[i+1][j+1].green);
            tempB += (Matrix[2][2] * image[i+1][j+1].blue);

            //Assign the calculated temp values to the new 2 dimensional pixel
            //array
            edgeImage[i][j].red = tempR;
            edgeImage[i][j].green = tempG;
            edgeImage[i][j].blue = tempB;

          }
      }
    //Return the updated 2 dimensional array of pixels
    return edgeImage;
  }

int main(int argc, char *argv[])
  {
    //If statement to make sure that the user has the neccesary command line
    // arguments
    if(argc != 6)
      {
        //Print statement to show what should be included
        printf("<input_file.bmp> <output_file> <-200-200>");
        printf(" <-200-200> <-200-200>\n");
        //Exit program if the user doesn't provide proper inputs
        exit(1);
      }

    //Intialize the three file variables (One Input, Two Output)
    FILE *inputFile, *outputFileEdge, *outputFileShade;
    //intialize the RGB shift values
    int redShift, greenShift, blueShift;

    //Create two character arrays to store the names of the output files
    char outputShadeName[50];
    char outputEdgeName[50];

    //copy in the user defined output file name to the two character arrays
    strcpy(outputShadeName,argv[2]);
    strcpy(outputEdgeName,argv[2]);

    //end the string the user defines just before the .bmp so it can be
    //concatinate
    outputShadeName[strlen(outputShadeName)-4] = '\0';
    outputEdgeName[strlen(outputEdgeName)-4] = '\0';

    //Concatinate the two file name extensions
    strcat(outputShadeName,"(shade).bmp");
    strcat(outputEdgeName,"(edge).bmp");

    //Set the three different shading values from the command line arguments
    redShift = atoi(argv[5]);
    greenShift = atoi(argv[4]);
    blueShift = atoi(argv[3]);

    //if statement to make sure the shift values are within the correct range
    if(redShift>200 || redShift < -200 || greenShift > 200 || greenShift < -200
        || blueShift > 200 || blueShift < -200)
      {
        //Exit the program if any value is out of range
        printf("Please make sure all your shift values are between -200");
        printf(" and 200.\nExiting the program...\n");
        exit(1);
      }

    //Open the two Input Files to be read from in binary form
    inputFile = fopen(argv[1],"rb");

    //Open the two output files to be written to in binary form
    outputFileEdge = fopen(outputEdgeName,"wb");
    outputFileShade = fopen(outputShadeName,"wb");

    //Determine if  the input file and both the output files can be opened,
    //exit the program is they cannot be opened
    if(inputFile == NULL)
      {
        printf("Error opening input_file. Exiting program.\n");
        exit(1);
      }
    else if(outputFileEdge == NULL)
      {
        printf("Error opening outputFileEdge. Exiting program.\n");
        exit(1);
      }
    else if(outputFileShade == NULL)
      {
        printf("Error opening outputFileShade. Exiting program.\n");
        exit(1);
      }

    //Declare the three neccesary two dimensional array of pixel structures
    struct Pixel **image;
    struct Pixel **shadedImage;
    struct Pixel **edgeImage;

    //Get the orginal image data and return the pixels it the the image array
    image = getData(inputFile);

    //call the shade image function and return the new pixel data to the
    //shaded image array
    shadedImage = shadeImage(image, redShift, greenShift, blueShift);

    //call the edge image function and return the new pixel data to the
    //edge image array
    edgeImage = filterImage(image);

    //Write back the updated images to their respective output files
    writeToFile(outputFileShade,shadedImage);
    writeToFile(outputFileEdge,edgeImage);

    //close all the input and output files
    fclose(inputFile);
    fclose(outputFileShade);
    fclose(outputFileEdge);

    return 0;
}

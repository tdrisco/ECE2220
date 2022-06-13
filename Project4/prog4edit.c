/* Timothy Driscoll
   ECE 2220 Project 4, 3/11/19

   Program Description: This program is designed apply the use of structures,
   pointers, command line arguments and dynamic memory allocation in order to
   display the proccess of convolution. It also demonstrates the proccess of
   writing to and reading from text files.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>

//Declare the project defined structure to be used.
struct TData
{
  int MinIndex, MaxIndex, Points;
  float *pValue;
} Input[2], Output;

/*This function is designed to read in the data from the input files. This
function only excepts the input file as an argument and an integer to indicate
which Input struct is being intialized. It then takes that file
and uses a series of loops to pull the data from the file and assign it to the
structure. A struct TData is then returned with its values intialized. */
struct TData getData(FILE*input, int InputNum)
  {
    //Intializet the neccesary variables for the function
    struct TData Input;
    int i=0;
    int k=0, c=0;
    int index[256];
    float pValues[256];

    //Print statement to tell the user what is happening
    printf("\nSaving the Data from file %d...\n",InputNum);

    //This is a while loop used to keep reading in data until the end of the
    //file has been reached.
    while(!feof(input))
      {
        //fscanf used to read in the float and integer value from the file into
        //to buffer arrays
        fscanf (input, "%d %f", &index[i], &pValues[i]);
        //Index counter increment
        i++;
      }
    //Assign the min, max and points value to the structure from data collected
    //in the loop
    Input.MinIndex = index[0];
    Input.MaxIndex = index[i-2];
    Input.Points = i-1;
    //Allocate the space for the pValues with indeces ranging from the min
    //value to the max value
    Input.pValue = (float *)malloc((Input.MaxIndex - Input.MinIndex + 1)
                    *sizeof(float));
    //For loop to assign the structures pValue variable from the buffer array
    for(k=Input.MinIndex;k<=Input.MaxIndex;k++)
      {
        //Indeces of pValue range from min to max x value (could be negative)
        Input.pValue[k] = pValues[c];
        //Increment counter that moves through the buffer array
        c++;
      }
    //Return the intialized structure
    return Input;
}

  /*This function is designed to reflect the points of a structure over the y
  axis. This function accepts a TData structure as an input. It then takes the
  structure and uses a series of loops to reflect the values. It then returns
  the newly adjusted structure. */
  struct TData flipPoints(struct TData Input)
    {
      //Declare local variables for function
      int buffer = Input.MaxIndex, i =0, k =0, min = Input.MinIndex;
      float bufferFloat[256];

      //Adjust the new max and min indeces for the structures values
      Input.MaxIndex = Input.MinIndex;
      Input.MinIndex = buffer;
      Input.MaxIndex = Input.MaxIndex*-1;
      Input.MinIndex = Input.MinIndex*-1;

      //Print statement to tell the user what is happening
      printf("Reflecting the points from file one across the y axis...\n\n");

      //For to assign the pValue variable from the struct to a buffer array
      for(k= buffer;k>=min;k--)
        {
          bufferFloat[i] = Input.pValue[k];
          i++;
        }
      //Adjust the memory allocation for pValue to accout for the new range
      Input.pValue = (float *)malloc((Input.MaxIndex - Input.MinIndex + 1)
                      *sizeof(float));

      i = 0;
      //Re-assign the pValue variable to its new range
      for(k=Input.MinIndex;k<=Input.MaxIndex;k++)
          {
            Input.pValue[k] = bufferFloat[i];
            //Counter going up from zero to match buffer indeces
            i++;
          }
      //Return the adjusted struct
      return Input;
    }

  /*This function is desiged to shift a structs values. This function excepts
  two inputs, both are TData structs. The function then takes the first struct
  and shifts it to line up with the minimum value of the second structure. The
  shifted structure is then returned.*/
  struct TData shiftPoints(struct TData flipped, struct TData Input2)
    {
      //Assign any neccesary local variables
      int flippedMax = flipped.MaxIndex;
      int flippedMin = flipped.MinIndex;
      int i =0, k =0;
      float bufferFloat[256];

      //Print statement to tell the user what is happening
      printf("Shifting the points from file one to align with the\n");
      printf("minimum x values from file two...\n\n");

      //Adjust the new shifted index values
      flipped.MaxIndex = Input2.MinIndex;
      flipped.MinIndex = flipped.MaxIndex - (flipped.Points - 1);

      //Using a for loop assign the original pValues to a buffer array
      for(k = flippedMin; k <= flippedMax; k++)
        {
          bufferFloat[i] = flipped.pValue[k];
          //Index variable starting at zero for the buffer array
          i++;
        }

      i = 0;
      //Second for loop to place the pValues back into the struct in new order
      for(k=flipped.MinIndex;k<=flipped.MaxIndex;k++)
          {
            flipped.pValue[k] = bufferFloat[i];
            //Index variable starting at zero for the buffer array
            i++;
          }
      //Return the adjusted struct
      return flipped;
    }
  /*This function is designed to intialize the data of the Output struct. It
  accepts three structs as inputs. It accepts the two Input structs and the
  Output Structure. The function then sets the Output structs min, max, points
  and allocates the memory for pValue. It then returns the TData struct
  Output*/
  struct TData setOutputData(struct TData flipped, struct TData Input,
                            struct TData output)
    {
      //Print statement to tell the user what is happening
      printf("\nSetting the values for the output structure...\n\n");

      //Intialize outputs MinIndex to the MinIndex of the non flipped struct
      output.MinIndex = Input.MinIndex;
      //Calculate the value of the MaxIndex by adding the number of points of
      //flipped to the MaxIndex of the other input and subtract one
      output.MaxIndex = Input.MaxIndex + flipped.Points - 1;
      //Calculate the total number of points by finding the difference between
      //the min and max index value
      output.Points = (output.MaxIndex-output.MinIndex)+1;
      //Allocate the memory for pValue based on the max and the min
      output.pValue = (float *)malloc((output.MaxIndex - output.MinIndex + 1)
                      *sizeof(float));
      //Return the intialized output struct
      return output;
    }

    /*This function is desiged to shift a structs values. This function excepts
    one inputs, a TData structs. The function then takes the struct and shifts
    its indeces by one. The shifted structure is then returned.*/
  struct TData shiftToNext(struct TData Input)
    {
      //Intialize the neccesary local variables
      int InputMax = Input.MaxIndex;
      int InputMin = Input.MinIndex;
      int i =0, k =0;
      float bufferFloat[256];

      //Shift the max and min indeces by one
      Input.MaxIndex = Input.MaxIndex+1;
      Input.MinIndex = Input.MinIndex +1;

      //For loop to assign the pValues to a buffer array
      for(k = InputMin; k <= InputMax; k++)
        {
          bufferFloat[i] = Input.pValue[k];
          i++;
        }

      i = 0;
      //Second for loop to re-assign the pValues, reflecting the shifted pValues
      for(k=Input.MinIndex;k<=Input.MaxIndex;k++)
          {
            Input.pValue[k] = bufferFloat[i];
            //Index variable starting at zero for the buffer array
            i++;
          }
      //Return the shifted structure
      return Input;
    }

  /*This function is designed to perform the convolution calculations. Its
  excepts three input structures the first two being the input structs and the
  third being the output struct. The function then multiplies the pValues of
  the two input functions sequential, then shifts one of the inputs by one and
  repeats the proccess thus performing convolution. The function finally returns
  the output struct which contains the calculated pValues.*/
  struct TData convolutionCalculation(struct TData flipped, struct TData Input,
                                      struct TData output)
    {
      //Intialize local variables
      int i =0, k =0;
      float sum =0;

      //Print statement to tell the user what is happening
      printf("Performing the final convolution calculations...\n\n");

      //Over arching for loop that covers the entire convoluion range
      //which spans from the Outputs struct's min to max
      for(i=output.MinIndex; i <= output.MaxIndex;i++)
        {
          //Reset sum to zero through each loop iteration
          sum = 0;
          //Nested for loop to move through the range of overlapping functions
          for(k = flipped.MinIndex; k<= Input.MaxIndex; k++)
            {
              //Two if statements to determine if individual functions are out
              //of their own range. Assign to zero if they are.
              if(k>flipped.MaxIndex)
                flipped.pValue[k] = 0;//Done to ensure pre-existig values don't
              if(k<Input.MinIndex)
                Input.pValue[k] = 0;//affect any of the calculations

              //Calculate the sum by adding it to itself and the product of the
              //two input's pValues at the given index
              sum = sum + (flipped.pValue[k]*Input.pValue[k]);
            }
            //Assign the sum float to the output's pValue at the given index
            output.pValue[i] = sum;
            //Shift one of the functions by one each iteration throught the loop
            flipped = shiftToNext(flipped);
        }
      //Return the output struct with itialized pValues
      return output;
    }

  /*This function is simply designed to print the calculated values to an
  output file. It has takes the output file and output TData struct as input
  varibles. It then prints the pValues and corresponding indeces to the file.
  The function has no return.*/
  void printToOutputFile(FILE *outputFile,struct TData output)
    {
      //Intialize local variables
      int i = 0;
      //Print statement to tell the user what is happening
      printf("Printing the calculated values to the output File...\n\n");

      //For loop to run through the range of the output struct's pValues
      for(i=output.MinIndex; i <= output.MaxIndex;i++)
        {
          //If statement that checks if given value is not equal to 0
          //This is so no zeros are printed to the output file
          if(output.pValue[i] != 0)
            //fprintf statement to print the data to the output file
            fprintf(outputFile,"%d\t%0.3f\n",i,output.pValue[i]);
        }
    }

int main(int argc, char *argv[])
  {
    //If statement to make sure that the user has the neccesary command line
    // arguments
    if(argc != 4)
      {
        //Print statement to show what should be included
        printf("<input1.txt> <input2.txt> <output.txt>\n");
        //Exit program if the user doesn't provide proper inputs
        exit(1);
      }

    //Intialize the three file variables (Two Input, One Output)
    FILE *inputFile1, *inputFile2, *outputFile;

    //Open the two Input Files to be read from
    inputFile1 = fopen(argv[1],"r");
    inputFile2 = fopen(argv[2],"r");
    //Open the output file to be written to
    outputFile = fopen(argv[3],"w");

    //Determine if both the input files can be opened, exit the program is they
    //camnot be opened
    if(inputFile1 == NULL)
      {
        printf("Error opening inputFile1. Exiting program.\n");
        exit(1);
      }
    else if(inputFile1 == NULL)
      {
        printf("Error opening inputFile2. Exiting program.\n");
        exit(1);
      }

    //Intailize the data for the two Input Structs from the input files
    Input[0] = getData(inputFile1,0);
    Input[1] = getData(inputFile2,1);
    //Intialize the output data
    Output = setOutputData(Input[0],Input[1],Output);
    //Reflect one of the inputs points over the x axis
    Input[0] = flipPoints(Input[0]);
    //Shift the Input struct thats points were reflected
    Input[0] = shiftPoints(Input[0],Input[1]);
    //Calculate the Output struct's pValues by calling the
    //convolutionCalculation function
    Output = convolutionCalculation(Input[0],Input[1],Output);
    //Print the calculated values to the output file
    printToOutputFile(outputFile,Output);
    //Tell the user that the program is complete
    printf("Done\n\n");

    return 0;
  }

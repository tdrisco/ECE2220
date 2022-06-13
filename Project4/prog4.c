/* Timothy Driscoll
   ECE 2220 Project 4, 3/11/19

   Program Description: This program is designed apply the use of structures,
   pointers, command line arguments and dynamic memory allocation in order to
   display the proccess of convolution.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>

struct TData
{
  int MinIndex, MaxIndex, Points;
  float *pValue;
} Input[2], Output;

//function to count the number of points in filepoints
struct TData getData(FILE*input, int j)
  {
    struct TData Input;
    int i=0;
    int k=0, c=0;
    int index[256];
    float pValues[256];

    while(!feof(input))
      {
        fscanf (input, "%d %f", &index[i], &pValues[i]);
        i++;
      }

    Input.MinIndex = index[0];
    Input.MaxIndex = index[i-2];
    Input.Points = i-1;
    Input.pValue = (float *)malloc((Input.MaxIndex - Input.MinIndex + 1)
                    *sizeof(float));

    for(k=Input.MinIndex;k<=Input.MaxIndex;k++)
      {
        Input.pValue[k] = pValues[c];
        c++;
      }
    return Input;
}

  struct TData flipPoints(struct TData Input)
    {
      int buffer = Input.MaxIndex, i =0, k =0, min = Input.MinIndex;

      Input.MaxIndex = Input.MinIndex;
      Input.MinIndex = buffer;
      Input.MaxIndex = Input.MaxIndex*-1;
      Input.MinIndex = Input.MinIndex*-1;
      float bufferFloat[256];

      for(k= buffer;k>=min;k--)
        {
          bufferFloat[i] = Input.pValue[k];
          i++;
        }

      Input.pValue = (float *)malloc((Input.MaxIndex - Input.MinIndex + 1)
                      *sizeof(float));

      i = 0;
      for(k=Input.MinIndex;k<=Input.MaxIndex;k++)
          {
            Input.pValue[k] = bufferFloat[i];
            i++;
          }

      return Input;
    }

  struct TData shiftPoints(struct TData flipped, struct TData Input2)
    {
      int flippedMax = flipped.MaxIndex;
      int flippedMin = flipped.MinIndex;
      int i =0, k =0;
      float bufferFloat[256];

      flipped.MaxIndex = Input2.MinIndex;
      flipped.MinIndex = flipped.MaxIndex - (flipped.Points - 1);

      for(k = flippedMin; k <= flippedMax; k++)
        {
          bufferFloat[i] = flipped.pValue[k];
          i++;
        }

      i = 0;
      for(k=flipped.MinIndex;k<=flipped.MaxIndex;k++)
          {
            flipped.pValue[k] = bufferFloat[i];
            i++;
          }

      return flipped;
    }

  struct TData setOutputData(struct TData flipped, struct TData Input,
                            struct TData output)
    {
      output.MinIndex = flipped.MinIndex + Input.MinIndex;
      output.MaxIndex = Input.MaxIndex + flipped.MaxIndex;
      output.Points = (output.MaxIndex-output.MinIndex)+1;
      output.pValue = (float *)malloc(output.Points*sizeof(float));

      return output;
    }

  struct TData shiftToNext(struct TData Input)
    {
      int InputMax = Input.MaxIndex;
      int InputMin = Input.MinIndex;
      int i =0, k =0;
      float bufferFloat[256];

      Input.MaxIndex = Input.MaxIndex+1;
      Input.MinIndex = Input.MinIndex +1;

      for(k = InputMin; k <= InputMax; k++)
        {
          bufferFloat[i] = Input.pValue[k];
          i++;
        }

      i = 0;
      for(k=Input.MinIndex;k<=Input.MaxIndex;k++)
          {
            Input.pValue[k] = bufferFloat[i];
            i++;
          }

      return Input;
    }

  struct TData convolutionCalculation(struct TData flipped, struct TData Input,
                                      struct TData output)
    {
      int i =0, k =0;
      float sum =0;
      int offset = flipped.MaxIndex;

      for(i=0; i <= output.Points;i++)
        {
          sum = 0;
          for(k = flipped.MinIndex; k<= Input.MaxIndex; k++)
            {
              if(k>flipped.MaxIndex)
                flipped.pValue[k] = 0;

              sum = sum + (flipped.pValue[k]*Input.pValue[k]);
            }
            output.pValue[i] = sum;
            printf("\n%d %f\n",offset+i,sum);
            flipped = shiftToNext(flipped);
        }
      return output;
    }

int main(int argc, char *argv[])
  {

    if(argc != 4)
      {
        printf("<input1.txt> <input2.txt> <output.txt>\n");
        exit(1);
      }

    FILE *inputFile1, *inputFile2, *outputFile;

    inputFile1 = fopen(argv[1],"r");
    inputFile2 = fopen(argv[2],"r");
    outputFile = fopen(argv[3],"w");

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

    Input[0] = getData(inputFile1,0);
    Input[1] = getData(inputFile2,1);
    Output = setOutputData(Input[0],Input[1],Output);
    //printf("\n%d %d %d %f\n",Input[0].MinIndex,Input[0].MaxIndex,Input[0].Points,Input[0].pValue[1]);

    Input[0] = flipPoints(Input[0]);
    //printf("\n%d %d %d %f\n",Input[0].MinIndex,Input[0].MaxIndex,Input[0].Points,Input[0].pValue[-5]);
    Input[0] = shiftPoints(Input[0],Input[1]);
    //printf("\n%d %d %d %f\n",Input[0].MinIndex,Input[0].MaxIndex,Input[0].Points,Input[0].pValue[-5]);
    Output = convolutionCalculation(Input[0],Input[1],Output);

    //printf("\n%d %d %d %f\n",Output.MinIndex,Output.MaxIndex,Output.Points,Output.pValue[-8]);
    //for(int c = Output.MinIndex; c<= Output.MaxIndex;c++)
    //{
    //printf("\n%d %f\n",c,Output.pValue[c]);
  //}
    //printf("\n%d\n",Input[0].Points);
    return 0;
  }

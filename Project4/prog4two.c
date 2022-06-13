#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>

struct TData
{
  int MinIndex, MaxIndex, Points;
  float *pValue;
} Input[2], Output;

struct TData collectData(FILE *filename, int i);
struct TData flipFunction(struct TData input);
struct TData initialShift(struct TData input0, struct TData input1);
struct TData littleShift(struct TData structInput);
void convolution(struct TData input0,struct TData input1,struct TData output,FILE *filename);


int main(int argc, char **argv)
{

  if(argc!=4)
  {
    printf("Error: Please enter two input files then one output file!\n");
    exit(1);
  }


  FILE *inputFile1, *inputFile2, *outputFile;
  inputFile1=fopen(argv[1],"r");
  inputFile2=fopen(argv[2],"r");
  outputFile=fopen(argv[3],"w");

  Input[0]=collectData(inputFile1,0);
  Input[1]=collectData(inputFile2,1);

  Output.MinIndex=Input[0].MinIndex+Input[1].MinIndex;
  Output.MaxIndex=Input[0].MaxIndex+Input[1].MaxIndex;
  Output.Points=((Output.MaxIndex-Output.MinIndex)+1);
  Output.pValue=malloc(sizeof(long double)*Output.Points);

  Input[0]=flipFunction(Input[0]);
  Input[0]=initialShift(Input[0],Input[1]);

  convolution(Input[0],Input[1],Output,outputFile);






return 0;
}

struct TData collectData(FILE *filename, int i)
{
  int j=0;
  int k,c=0;
  int range[256];
  float values[256];

  while(!feof(filename))
  {
    fscanf(filename,"%d %f",&range[j],&values[j]);
    j++;
  }

  Input[i].MinIndex=range[0];
  Input[i].MaxIndex=range[j-2];
  Input[i].Points=j-1;
  Input[i].pValue=malloc((Input[i].MaxIndex-Input[i].MinIndex+1)*sizeof(long double));

  for(k=Input[i].MinIndex;k<=Input[i].MaxIndex;k++)
  {
    Input[i].pValue[k]=values[c];
    c++;
  }

return Input[i];
}

struct TData flipFunction(struct TData input)
{
  int temp, index, i=0;
  float buffer[256];
  int originalMax=input.MaxIndex;
  int originalMin=input.MinIndex;

  temp=input.MaxIndex;
  input.MaxIndex=input.MinIndex;
  input.MinIndex=temp;
  input.MinIndex=input.MinIndex*-1;
  input.MaxIndex=input.MaxIndex*-1;

  for(index=originalMax;index>=originalMin;index--)
  {
    buffer[i]=input.pValue[index];
    i++;
  }

  input.pValue=malloc((input.MaxIndex-input.MinIndex+1)*sizeof(long double));
  i=0;

  for(index=input.MinIndex;index<=input.MaxIndex;index++)
  {
    input.pValue[index]=buffer[i];
    i++;
  }


return input;
}

struct TData initialShift(struct TData input0, struct TData input1)
{

  int i,j=0;
  float buffer[256];
  int originalMax=input0.MaxIndex;
  int originalMin=input0.MinIndex;

  input0.MaxIndex = (input1.MinIndex);
  input0.MinIndex = input0.MaxIndex-(input0.Points-1);

  for(i=originalMin;i<=originalMax;i++)
  {
    buffer[j]=input0.pValue[i];
    j++;
  }

  j=0;
  for(i=input0.MinIndex;i<=input0.MaxIndex;i++)
  {
    input0.pValue[i]=buffer[j];
    j++;
  }

return input0;
}

struct TData littleShift(struct TData structInput)
{
  int i,j=0;
  float temp[256];
  int originalMax=structInput.MaxIndex;
  int originalMin=structInput.MinIndex;

  structInput.MaxIndex = structInput.MaxIndex+1;
  structInput.MinIndex = structInput.MinIndex+1;

  for(i=originalMin;i<=originalMax;i++)
  {
    temp[j]=structInput.pValue[i];
    j++;
  }

  j=0;
  for(i=structInput.MinIndex;i<=structInput.MaxIndex;i++)
  {
    structInput.pValue[i]=temp[j];
    j++;
  }

return structInput;
}

void convolution(struct TData input0,struct TData input1,struct TData output,FILE *filename)
{
  int i,j;
  int preLittleShiftMax=input0.MaxIndex;

  for(i=0;i<output.Points;i++)
  {
    int sum=0;
    for(j=input0.MinIndex;j<=input1.MaxIndex;j++)
    {
      if(j>input0.MaxIndex)
      {
        input0.pValue[j]=0;
      }
      printf("%f\t%f\n",input0.pValue[j],input1.pValue[j]);
      sum+=(input0.pValue[j]*input1.pValue[j]);

    }
    printf("\n");
    fprintf(filename,"%d\t%d\n",i+preLittleShiftMax,sum);
    input0=littleShift(input0);

  }
}

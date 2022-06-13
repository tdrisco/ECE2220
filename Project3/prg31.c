/* Timothy Driscoll
   ECE 2220 Project 3, 2/20/19

   Program Description: This program is designed to recieve a two character
   input from the user and then performs bitwise opertaions on the data in
   order to implement an error correcting code.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>

/* This function is a binary printer. It takes in a decimal number as an input
then using bitwise operations it compares each bit with a one to determine
whether or not it should print a one or zero. This function doesn't have a
return it just prints the decimal number in binary*/
  void dtob(int inputDec)
    {
      //Intialize the neccesary values for the function
      int counter, compare;
      //for loop to run through and print out the 21 different bits
      for(counter = 20; counter >= 0; counter--)
        {
          //set the compare int to the individual bit of the inputDec based one
          //on the counter (this works in reverse order 20 to 0)
          compare = inputDec >> counter;
          // if statement to print a space after 8 bits are printed
          if(counter == 7)
            printf(" ");
          //If statement to compare the compare bit to 1 (if it = 1 print 1)
          if(compare & 1)
            printf("1");
          //Print a zero if the bit didn't equal 1
          else
            printf("0");
        }
        printf("\n");
    }
/* This function is designed to create the integer that is to be turned into
the code word by adding the parity bits to the users entry. The function
exepts and interger as an input. It then adds the parity bits to a new integer
through a series of shifts ands and ors. It returns the new integer that has
the parity bits all set to 0 in their correct location.*/
  int createCodeWord(int user32Bit)
    {
      //Intialize the needed variables for this function
      int buffer32Bit = 0, codeWord = 0;

      //Transfer the bits into code word using buffer32Bit
      buffer32Bit = user32Bit >> 11;
      codeWord = buffer32Bit << 16;

      buffer32Bit = user32Bit & 2047;
      buffer32Bit = buffer32Bit >> 4;
      buffer32Bit = buffer32Bit << 8;
      codeWord = codeWord | buffer32Bit;

      buffer32Bit = user32Bit & 14;
      buffer32Bit = buffer32Bit >> 1;
      buffer32Bit = buffer32Bit << 4;
      codeWord = codeWord | buffer32Bit;

      buffer32Bit = user32Bit & 1;
      buffer32Bit = buffer32Bit << 2;
      codeWord = codeWord | buffer32Bit;

      return codeWord;
    }

    int countOnes(int buffer32Bit)
      {
        int i, oneCount = 0;

        for(i =0; i<32;i++)
          {
            if(buffer32Bit & 1)
              oneCount++;

            buffer32Bit >>= 1;
          }
        return oneCount;
      }

int main(void)
{

    char userInput1, userInput2, buffer;
    int user32Bit = 0, codeWord = 0, numOfOnes = 0, buffer32Bit = 0;
    do
      {
        do
          {
            printf("Please enter either 2 ASCII values or the word end to quit");
            printf(" the program: ");
            scanf("%c%c%c",&userInput1,&userInput2,&buffer);
          } while(userInput1 < 0 && userInput1 > 127 && userInput2 < 0 &&
              userInput2 > 127);

          user32Bit = user32Bit | userInput2;
          user32Bit = user32Bit << 8;
          user32Bit = user32Bit | userInput1;

          printf("\n\t\t\t %c\t %c\n",userInput2,userInput1);
          printf("0x\t\b\b00\t\b00\t%x\t%x\n",userInput2,userInput1);
          printf("-------- -------- ");
          dtob(user32Bit);
          printf("\n");

          printf("\n");
          dtob(codeWord);
          printf("\n");

          codeWord = createCodeWord(user32Bit);
          printf("\n");
          dtob(codeWord);
          printf("\n");

          buffer32Bit = codeWord;
          buffer32Bit = codeWord & 1398101;

          //printf("\n");
          //dtob(codeWord);
          //printf("\n");

          numOfOnes = countOnes(buffer32Bit);
          printf("%d",numOfOnes);
          if(numOfOnes%2 != 0)
            {
              codeWord = codeWord | 1;
            }

          buffer32Bit = codeWord;
          buffer32Bit = codeWord & 419430;

          numOfOnes = countOnes(buffer32Bit);
          if(numOfOnes%2 != 0)
            {
              codeWord = codeWord | 2;
            }

          buffer32Bit = codeWord;
          buffer32Bit = codeWord & 200463;

          numOfOnes = countOnes(buffer32Bit);
          if(numOfOnes%2 != 0)
            {
              codeWord = codeWord | 8;
            }

          buffer32Bit = codeWord;
          buffer32Bit = codeWord & 32640;

          numOfOnes = countOnes(buffer32Bit);
          if(numOfOnes%2 != 0)
            {
              codeWord = codeWord | 128;
            }
          buffer32Bit = codeWord;
          buffer32Bit = codeWord & 2064384;

          numOfOnes = countOnes(buffer32Bit);
          if(numOfOnes%2 != 0)
            {
              codeWord = codeWord | 32768;
            }
          printf("\n");
          dtob(codeWord);
          printf("\n");
          printf("\n%X\n",codeWord);

          user32Bit = 0;
          codeWord = 0;
          buffer32Bit = 0;
    } while(userInput1 != 101 && userInput2 != 110 && buffer != 100);



  return(0);
}

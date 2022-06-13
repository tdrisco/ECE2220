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
whether or not it should print a one or zero. The function also takes in
another integer value that is used to determine how many digits to print. This
 function doesn't have a return it just prints the decimal number in binary*/
  void printInBinary(int inputDec, int numToPrint)
    {
      //Intialize the neccesary values for the function
      int counter, compare;
      //for loop to run through and print out the specified number of bits
      for(counter = numToPrint; counter >= 0; counter--)
        {
          //set the compare int to the individual bit of the inputDec based one
          //on the counter (this works in reverse order 20 to 0)
          compare = inputDec >> counter;
          // if statement to print a space after 8 bits are printed
          if(counter == 7)
            printf(" ");
          else if(counter == 15)
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

      //Transfer the first five bits from the userbit into the codeword
      buffer32Bit = user32Bit >> 11;
      codeWord = buffer32Bit << 16;

      //The first and statement is used to set the buffer bit to bits 1-11 of
      // the orginal user bit, using the and to set the other bits to zero
      buffer32Bit = user32Bit & 2047;
      //Then using two sequential shifts get bits 5-11 into the correct spot to
      //be ored into the code word (leaves space for parity bit)
      buffer32Bit = buffer32Bit >> 4;
      buffer32Bit = buffer32Bit << 8;
      codeWord = codeWord | buffer32Bit;

      //Similar to above the and statement makes sure everything is zero but
      // the last 4 bits of the orginal user bit
      buffer32Bit = user32Bit & 15;
      // Then with two more shifts get bits 2-4 in the correct spot
      // These shifts leave space for the parity bit
      buffer32Bit = buffer32Bit >> 1;
      buffer32Bit = buffer32Bit << 4;
      // Use an or statment to place bits 2-4 in the code word
      codeWord = codeWord | buffer32Bit;

      // This last and statement is used to set every bit bu the first one to 0
      // Then set the last bit to equal the buffer bit
      buffer32Bit = user32Bit & 1;
      //Shift the bit over two times making room for the final two parity bits
      buffer32Bit = buffer32Bit << 2;
      //Or the bit code word
      codeWord = codeWord | buffer32Bit;

      //Return the code word complete with parity bits (intialized to 0)
      return codeWord;
    }

/* This function accepts a single integer value as its input. The function then
takes this integer and uses bitwise operations to determine how many ones are
present in the binary conversion of the number. It returns this value as an
integer */
    int countOnes(int buffer32Bit)
      {
        //Intialize the loop counter and ones counter
        int i, oneCount = 0;

        // For loop that interates through each bit of the integer
        for(i =0; i<32;i++)
          {
            // If statement to compare bit to one with an and statement. If the
            // bit is a one it will be true and add one to the oneCount
            if(buffer32Bit & 1)
              oneCount++;

            //Shift the bit over by one each time through the loop
            buffer32Bit >>= 1;
          }
        //Return the counted number of ones in the binary conversion of the int
        return oneCount;
      }
/* This function has multiple integer values as inputs. The first integer is
the andBitBy. This integer is used to and with the codeWord integer which is
the code word value containing the parity bits that need to be set. The
andBitBy value sets all the bits in the codeword to zero that aren't to be
counted when determing the number of ones. The bitPosition value is used to
determine where the parity value is ored into the code word. And lastly the
paritySpot value is used for the print statments. This function determines
whether the parity bit should be a one or a zero and sets it creating the final
code word. It returns the final code word as an integer */
    int setParityBits(int andBitBy,int codeWord,int bitPosition,int paritySpot)
      {
        //Intialize the neccesary variables to run the program
        int buffer32Bit = 0, numOfOnes = 0;

        //Set the buffer bit to the codeWord anded by the andBitBy value to
        //set the neccesary bits to zero.
        buffer32Bit = codeWord & andBitBy;
        //Using the countOnes function determine the number of ones in the
        // the buffer32Bit
        numOfOnes = countOnes(buffer32Bit);

        //If the number of ones isn't devisable by two (odd) then set the
        //parity bit to one in the specified spot
        if(numOfOnes%2 != 0)
          {
            //Using an or statement set the parity bit to 1 in the correct
            // bit location (2^position)
            codeWord = codeWord | bitPosition;
            //Print out what the parity bit equals (1)
            printf("Parity Bit %2d = 1\n",paritySpot);
          }
        else
          {
            //print out what the parity bit equals (0)
            printf("Parity Bit %2d = 0\n",paritySpot);
          }

        //Return the final code word
        return codeWord;
      }

int main(void)
{
  //Intialize three characters to except intial characters entered by the user
    char userInput1, userInput2, buffer;
  //Intialize the two neccesary integers for the program
    int user32Bit = 0, codeWord = 0;

    //Using a do while loop have the user enter their two characters to
    // run the program making sure there in the range of all the characters
    // on the ASCII table
    do
      {
        printf("Please enter either 2 ASCII values or the word end to quit");
        printf(" the program: ");
        scanf("%c%c%c",&userInput1,&userInput2,&buffer);
      } while(userInput1 < 0 && userInput1 > 127 && userInput2 < 0 &&
          userInput2 > 127); // condition to make sure valid ASCII character

    //This while loop determines if the user entered the quit phrase "end"
    while(userInput1 != 101 && userInput2 != 110 && buffer != 100)
      {

          //Start off by using an or statment to place the users second
          // character into the user32Bit (integer)
          user32Bit = user32Bit | userInput2;
          //Shift this bit over 8 spots making space for the next character
          user32Bit = user32Bit << 8;
          //Using another or statement or the first charcter into the int
          user32Bit = user32Bit | userInput1;

          //Using a series of print statements print out the users two
          // characters as characters, hexadecimal and in binary
          printf("\n\t\t\t  %c\t   %c\n",userInput2,userInput1);
          printf("0x\t\b\b00\t\b00\t %X\t  %X\n",userInput2,userInput1);
          printf("-------- -------- ");
          //Use the dtob function to print the user32Bit int in binary
          printInBinary(user32Bit,15);
          printf("\n");

          //Use the createCodeWord function to create the code word, intializing
          // all the parity bits two zero in their respective locations.
          codeWord = createCodeWord(user32Bit);
          //Using the setParityBits function set each individual parity bit
          codeWord = setParityBits(1398101,codeWord,1,1);
          codeWord = setParityBits(419430,codeWord,2,2);
          codeWord = setParityBits(200463,codeWord,8,4);
          codeWord = setParityBits(32640,codeWord,128,8);
          codeWord = setParityBits(2064384,codeWord,32768,16);

          //Print out the code word integer in binary form
          printf("\nNew Code Word In Binary\n");
          printf("\n-------- ---");
          printInBinary(codeWord,20);

          //Print out the code word integer in hexadecimal form
          printf("\nNew Code Word In Hexadecimal\n");
          printf("\n0x000%X\n\n",codeWord);

          //Reset the user32Bit and codeWord to zero
          user32Bit = 0;
          codeWord = 0;

          //Ask the user if they want to run the program again and make sure
          // that they are entering valid ASCII characters
          do
            {
              printf("Please enter either 2 ASCII values or the word end to");
              printf(" quit the program: ");
              scanf("%c%c%c",&userInput1,&userInput2,&buffer);
            } while(userInput1 < 0 && userInput1 > 127 && userInput2 < 0 &&
                userInput2 > 127);//condition to check for valid character
    }



  return(0);
}

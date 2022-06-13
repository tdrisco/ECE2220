/* Timothy Driscoll
   ECE 2220 Project 1, 1/16/19

   Program Description: This program is designed to use the delta algoritham
   to pick 6 different lottery numbers and then test them against winning
   numbers. This program demonstrates the use of arrays, conditionals, loops,
   the rand() function, proccessing input, and formating output.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

  /*This function accepts an array and three integer values. It has no return
  values. The purpose of this fucntion is to take in user values and set
  them as the lottery number values. It takes into consideration the max and
  minnimum possible values aswell as position*/
  void setLotteryNumber(int *lotteryNumber,int minVal, int maxVal, int position)
    {
      //Do-while loop to keep having the user input values until it fits into
      //the desired range.
      do
        {
          //Print statement to instruct the user
          printf("Please input a number from %d to %d inclusive: ",minVal,
                                                                 maxVal);
          //Accept the user input and assign it to the array
          scanf("%d",&*(lotteryNumber + position));
        } while(*(lotteryNumber + position) < minVal ||
                *(lotteryNumber + position) > maxVal);//Control conditions
    }
  /*This function accepts an array as input. It has an int as a return value.
  The purpose of this function is to calculate the sum of the inputed
  integer array and return that sum as an integer value.*/
  int calculateSum(int *lotteryNumber,int sum)
    {
      //For loop to run through the array
      for(int i = 0; i < 6; i++)
        {
          //Add each value of the array
          sum = sum + *(lotteryNumber + i);
        }
      //Return the calculated sum
      return(sum);
    }
  /*This fucntion accepts an array as input. It has no return value. The
  purposed of this function is to sort the numbers in the given array
  in ascending order.*/
  void sortLotteryNumbers(int *lotteryNumber)
    {
      //Intermediate value holder
      int valHolder = 0;
      //Nested for loop to compare two numbers at a time.
      for(int i = 0; i < 6; i++)
        {
          for(int j = i + 1; j < 6; j++)
            {
              //Determine if the first number is bigger than the second
              if(*(lotteryNumber + i) > *(lotteryNumber + j))
                {
                  //Switch the order of the numbers
                  valHolder = *(lotteryNumber + i);
                  *(lotteryNumber + i) = *(lotteryNumber + j);
                  *(lotteryNumber + j) = valHolder;
                }
              }
            }
    }
    /*This function has a single integer array input. The function has no return
    value. The purpose of this function is to print out the array with (,)
    in between each value.*/
    void printLotteryNumbers(int *lotteryNumber)
      {
        //For loop to run through all the values in the array
        for(int i = 0; i < 6; i++)
          {
            //If its the last number don't include the (,)
            if(i == 5)
              printf("%d\n\n",*(lotteryNumber + i));
            else
              printf("%d, ",*(lotteryNumber + i));
          }
      }

      /*This function has a single integer array input. The function has no return
      value. The purpose of this function is to randomize the order of the
      array by switching around random indeces.*/
    void randomizeLotteryNumbers(int *lotteryNumber)
      {
        //Intialize neccesary values
        int j, valHolder;
        //For loop to iterate through array
        for(int i = 0; i < 6; i++)
          {
            //Assign random index
            j = rand() % 6;
            //Switch the two numbers
            valHolder = *(lotteryNumber + i);
            *(lotteryNumber + i) = *(lotteryNumber + j);
            *(lotteryNumber + j) = valHolder;
          }
      }

      /*This function has a single integer array input. The function has no return
      value. The purpose of this function is to calculate the final lottery
      numbers to be used. This is done by succesivley adding the delta
      numbers.*/
    void calculateLotteryNumbers(int *lotteryNumber)
      {
        //Intialize variables
        int j = 0;
        //For loop to iterate through the array
        for(int i = 1; i < 6; i++)
          {
            //Add the succesive values
            *(lotteryNumber + i) = *(lotteryNumber + i) + *(lotteryNumber + j);
            j++;
          }
      }

int main(void)
{
    //Intialize variables
    //Two arrays used to store the lottery numbers
    int *lotteryNumber;
    int *randomLotteryNum;
    //These varibales are all used as controls in various loops
    int sum = 0, match = 0, count = 0;

    /*set the seed for the random function as current time which will change
    the random numbers that are used each time.*/
    srand(time(0));

    //Allocate the memory for the two lottery number arrays
    lotteryNumber = malloc(6*sizeof(int));
    randomLotteryNum = malloc(6*sizeof(int));

    /*Intial statements to obtain user values for the delta sequence.
    A do while is used to continue to loop through if the numbers sum is
    greater then the maximum value of 50.*/
    do
      {
        // Checks if it is the first time in the loop.
        if(count == 0)
          {
            //Print statement for first time in loop
            printf("\nPlease enter the following numbers as they are asked");
            printf("for.\n\n");
          }
        else
          {
            //Print statement if sum is greater then 50 and they have to repeat
            printf("\nYour chosen nubers summed to be greater then 50 which ");
            printf("is the maximum value.\nPlease re-enter the numbers.\n\n");
          }

        //Using setLotteryNumber function set the 6 different delta numbers
        setLotteryNumber(lotteryNumber,1,5,0);
        setLotteryNumber(lotteryNumber,1,10,1);
        setLotteryNumber(lotteryNumber,1,10,2);
        setLotteryNumber(lotteryNumber,10,12,3);
        setLotteryNumber(lotteryNumber,8,15,4);
        setLotteryNumber(lotteryNumber,8,15,5);

        //Intialize sum to zero in each loop
        sum = 0;
        //Calculate sum using function and assign return to sum
        sum = calculateSum(lotteryNumber,sum);
        //Increment count each time through loop
        count += 1;
      } while (sum > 50); //Check sum condition (below 50)

      //Sort the delta numbers in ascending order
      sortLotteryNumbers(lotteryNumber);

      //Print out the sorted delta numbers using printLotteryNumbers function
      printf("\nYour delta numbers are: ");
      printLotteryNumbers(lotteryNumber);

      //Randomize the order of the delta numbers using randomizeLotteryNumbers
      //function
      randomizeLotteryNumbers(lotteryNumber);

      //Print out the final delta sequence using printLotteryNumbers function
      printf("\nYour final delta sequence is: ");
      printLotteryNumbers(lotteryNumber);

      //calculate the lottery numbers using calculateLotteryNumbers function
      calculateLotteryNumbers(lotteryNumber);

      //Print out the final lottery numbers using printLotteryNumbers function
      printf("\nYour lottery numbers are: ");
      printLotteryNumbers(lotteryNumber);

      //Reset count variable to 0
      count = 0;
      /*Do-while loop to simulate lottery numbers to compare to the users
      lottery numbers. It is designed to stop once a match occurs or if a
      million trys occur (whichever occurs first).*/
      do
        {
          //Assign 6 random lottery numbers (1 to 50)
          for(int i = 0; i < 6; i++)
            {
              *(randomLotteryNum + i) = (rand() % 49) + 1;
            }
          //Sort the random lottery numbers using sortLotteryNumbers function
          sortLotteryNumbers(randomLotteryNum);
          //Search for duplicates in each group of lottery numbers
          for(int i = 0; i < 5; i++)
            {
              for(int j = i + 1; j < 6; j++)
                {
                  //Compare lottery numbers two at a time
                  if(*(randomLotteryNum + i) == *(randomLotteryNum + j))
                    {
                      //If there is a duplicate reassign one of the numbers
                      *(randomLotteryNum + i) = (rand() % 49) + 1;
                      //Re-sort the numbers so they can be checked in order
                      sortLotteryNumbers(randomLotteryNum);
                      //Restart the check from the first number by setting i=0
                      i = 0;
                    }
                }
            }
          //Reintialize match to 0 for each loop iteration
          match = 0;
          //Run through all 6 lottery numbers using a for loop
          for(int i = 0; i < 6; i++)
            {
              //If lottery numbers match at one to match
              if(randomLotteryNum[i] == lotteryNumber[i])
              match = match + 1; //When match = 6 all 6 numbers matched (win)
            }
          //Add one to count each time through the loop
          count = count +1;
        } while(count < 1000000 && match != 6);//Check the two controls

      /*If match = 6 let the user know they one and tell them how many attempts
      that it took. Else idicates they didn't win after 1000000 attempts.*/
      if(match == 6)
        {
          printf("\nYour lottery numbers won after ");
          printf("%d simulated attempts.\n\n",count);
        }
      else
        {
          printf("\nYour lottery numbers didn't win after ");
          printf("%d simulated attempts.\n\n",count);
        }

  return(0);
}

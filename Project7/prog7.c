/* Timothy Driscoll
   ECE 2220 Project 7, 4/28/19

   Program Description: This program is designed apply the different functions
   including fork, getpid, wait, signal, alarm and kill. The program also deals
   with different time functions and generating random numbers. Lastly the
   function deals accessing a console window buffer. Using the applications of
   these different topics the program is designed to spawn three different child
   processes which are the subs that are all on a misson to fire all there
   missles and return to base before running out of fuel.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//typedef for casting functions in signal function
typedef void(*funcptr);

//Define the needed global variables to run the program
FILE *terminalptrs[4];
int numToBase;
int numMissles;
int numFuel;
int subPid;
int subIDnum;
int pids[4];

/* This function is designed to locate the open terminals on the computer and
open the first for terminals that can be written to. The function doesn't have
any input or return values. It uses a for loop to scan form 0 to 100 checking
to see which terminals are open.It also displays all the avaliable terminals and
the four that were opened.*/

void locateTerminals()
  {
    //intialize local variables
    int i, j = 0;
    char terminal[100];
    FILE *checkTerm;

    //Intial print Statement
    printf("\nAvaliable Terminals\n\n");

    //For loop to look for potentially open terminals
    for(i=0;i<100;i++)
      {
        //save the string containing the terminal names with new num each time
        sprintf(terminal,"/dev/pts/%d",i);
        fflush(stdout);

        //try and open the given terminal
        checkTerm = fopen(terminal, "r");
        //If the terminal can be opened
        if(checkTerm != NULL)
          {
            //print that the terminal is avaliable and open if its one of the
            //first for terminals
            printf("Terminal %d\n",i);
            if(j<4)
              {
                printf("Opening Terminal[%d]...\n",i);
                terminalptrs[j] = fopen(terminal, "w");
                j++;
              }

          }
      }
    printf("\n");
    //make sure that atleast for terminals were opened
    if(j<4)
      {
        printf("There are not enought terminals open to run the program\n\n");
        exit(1);
      }
  }

/*This is a simple fucntion to go through and close the open terminals using
fclose and a for loop. It doesnt have any inputs or outputs*/
void closeTerminals()
  {
    int i = 0;
    //run for loop to go through the terminals
    for(i = 0; i < 4; i++)
      {
        //close each terminals
        fclose(terminalptrs[i]);
      }
  }

/*This function is the printTime fucntion which is used to go through and print
out the time in a standard form that includes the date. This function is used
to indicate the start and end of the mission*/
void printTime()
  {
    //intialize the current time as a time structure
    time_t curtime;
    time(&curtime);

    //print the time in standard form
    printf("%s\n",asctime(localtime(&curtime)));
  }

/*This function is designed to execute each second as the alarm of 1 second
goes off. Each second the fuel of the sub is decremented. Every two seconds the
the distance from the base either gets decremented or incremented depending on
the missle conditon. Every three seconds a status update is printed telling the
user how many missles are left. how much fuel is left, and the distance from
base. This function is designed to quit when either the fuel runs out or when
the sub makes it back to the base with no missles. This function doesnt have
any inputs or outputs*/
void subActions(int subSignal)
  {
    //intialize the local varibles
    static int displayTrig=1, distanceTrig=1;
    char timeBuffer[50];

    //get the current time to be printed
    time_t currentTime;
    struct tm *subTime;
    currentTime = time(NULL);
    subTime = localtime(&currentTime);

    strftime(timeBuffer, 50, "Current Time: %X",subTime);

    //decrement the fuel each second
    numFuel = numFuel - (rand()%101 + 100);
    if(numFuel < 0)
      numFuel = 0;

    //decrement or increment the distance every two seconds
    distanceTrig = distanceTrig +1;
    if(distanceTrig == 2)
      {
        distanceTrig = 0;
        //missle conditon to check if sub can return
        if(numMissles == 0)
          {
            numToBase = numToBase - ((rand()%6) + 3);
            if(numToBase < 0)
              numToBase = 0;
          }
        else
          numToBase = numToBase + ((rand()%6) + 5);
      }

    displayTrig = displayTrig +1;
    //Give the user a status update with print statements every three seconds
    if(displayTrig == 3)
      {
        displayTrig = 0;
        fprintf(terminalptrs[subIDnum],"%s\n",timeBuffer);
        fprintf(terminalptrs[subIDnum],"Sub Process ID: %d\n",subPid);
        fprintf(terminalptrs[subIDnum],"Sub %d to base, ",subIDnum);
        fprintf(terminalptrs[subIDnum],"%d gallons left, ",numFuel);
        fprintf(terminalptrs[subIDnum],"%d missles left,\n",numMissles);
        fprintf(terminalptrs[subIDnum],"%d miles left from base.\n\n",numToBase);
      }

    //all the different exit conditions
    if(numFuel == 0)
      {
        //this is a failed misson
        fprintf(terminalptrs[subIDnum],"Sub %d is dead in the water.\n",subIDnum);
        fprintf(terminalptrs[subIDnum],"Rescue is on the way, sub %d.\n",subIDnum);
        exit(EXIT_FAILURE);
      }
    else if(numFuel < 500)
      {
        // not an exit condition but a final fuel warning
        fprintf(terminalptrs[subIDnum],"Sub %d running out of Fuel!\n",subIDnum);
      }
    if(numMissles == 0 && numToBase == 0)
      {
        //this is a successful mission exit
        fprintf(terminalptrs[subIDnum],"Sub %d complete the mission.\n",subIDnum);
        exit(EXIT_SUCCESS);
      }
    alarm(1);
  }

/*This function will send out a missle based of of a user input to the base. It
accepts a int value subSignal which can be related to the SIGUSR1 signal. This
function doesnt have any outputs*/
void fireMissle(int subSignal)
  {
    //make sure there are still missles to fire
    if(numMissles > 0)
      {
        //decrement missles
        numMissles = numMissles -1;
        //dislpay to user the process
        fprintf(terminalptrs[subIDnum],"Sub %d firing missle.\n",subIDnum);
        fprintf(terminalptrs[subIDnum],"Sub %d has %d missles left.\n",subIDnum, numMissles);
        if(numMissles == 0)
          {
            //if missles are gone tell user sub is returning
            fprintf(terminalptrs[subIDnum],"Sub %d returning to base.\n",subIDnum);
          }
      }
    else
      fprintf(terminalptrs[subIDnum],"Sub %d has no missles left.\n",subIDnum);
  }

/*This function will send a sub more fuel based of of a user input to the base.
It accepts a int value subSignal which can be related to the SIGUSR2 signal This
function doesnt have any outputs*/
void fuelSub(int subSignal)
  {
    //find random refuel amount
    int refuelAmount = (rand()%4001) + 1000;
    //add this to total fuel
    numFuel = numFuel + refuelAmount;

    //inform the user of what happened
    fprintf(terminalptrs[subIDnum],"Sub %d was refueled with ",subIDnum);
    fprintf(terminalptrs[subIDnum],"%d gallons of fuel.\n",numFuel);
  }

/*This function contains all of the different operations that each sub(child)
proccess will perform. It will also intialize all the random sub values including
the missles, and fuel. The distance from base should start at 0 each time. Based
on the alarm the sub will perform different tasks including refuel, the sub
actions, and fire missles. This function doesn't have any inputs or outputs*/
void setSubs()
  {
    //seed the random number generator
    srand(time(0));

    //get the child process id
    subPid = getpid();
    //various signal related function calls
    signal(SIGALRM, (funcptr)subActions);
    signal(SIGUSR1, (funcptr)fireMissle);
    signal(SIGUSR2, (funcptr)fuelSub);

    //set sub values randomly
    numToBase = 0;
    numMissles = (rand() % 5) + 6;
    numFuel = (rand() % 4001) + 1000;

    alarm(1);
    while(1)
      {
      }
  }

/*This function handles the user inputs by checking to see if its a valid
command and if it is a valid command it will determine which command it was
and for which sub it was then it will triger the process to execute. The function
doesnt have any inputs or outputs.*/
void userSubCommandProcess()
  {
    //intialize local variables
    int subNum;
    int i;
    char commandType;
    int subCount = 3;
    char userCommand[10];

    //check to make sure subs still running
    while(subCount > 0)
      {
        //get the user command
        scanf("%s", userCommand);
        fflush(stdin);

        //check to see if it is a command or quit
        if(strlen(userCommand) == 2)
          {
            //convert the second character to int corresponding to sub
            subNum = atoi(&userCommand[1]);
            //get first character to determine which command type
            commandType = userCommand[0];
            //check for valid sub num
            if(subNum >= 1 && subNum <= 3)
              {
                //check for a valid command and then send signal to the process
                if(commandType == 'l')
                  {
                    kill(pids[subNum], SIGUSR1);
                  }
                else if(commandType == 'r')
                  {
                    kill(pids[subNum], SIGUSR2);
                  }
                else if(commandType == 's')
                  {
                    kill(pids[subNum], 1);
                    subCount = subCount -1;
                  }
                else
                  // make sure valid command
                  printf("Please enter a valid command.\n");
              }
            else
              //make sure valid sub number
              printf("Please pick a valid sub number.\n");
          }
        //check for the q(quit) command
        else if(strlen(userCommand)==1)
          {
            //if it is quit end each sub process
            if(userCommand[0] == 'q')
              for(i=1; i<=3; i++)
                {
                  kill(pids[i], 1);
                  subCount = subCount-1;
                }
          }
        else
          {
            printf("Not a valid command.\n");
          }
      }
  }
/*This function represents the base actions which in turn represets the parent
fucntion. It will have all the parent functions wait until the processes end
then it will determine and print the respective exit statuses (failure or
success)*/
void baseActions()
  {
    //intialize local varibales
    int waitReturn[4], subStatus[4];
    int i = 0;
    char exitByte, signalByte;

    //wait for child processes to end
    waitReturn[1] = wait(&subStatus[1]);
    waitReturn[2] = wait(&subStatus[2]);
    waitReturn[3] = wait(&subStatus[3]);

    //for loop to go through and print out the respective exit statuses
    for(i = 1; i <= 3; i++)
      {
        //determine successful or failed mission
        exitByte = subStatus[i] >> 8;
        signalByte = subStatus[i] & 0x7F;

        //print out the corresponding output
        if(exitByte || signalByte)
          printf("Process %d misson failure...\n",waitReturn[i]);
        else
          printf("Process %d misson success...\n",waitReturn[i]);
      }
  }


int main(void)
  {
    //intialize variables
    int parentTog = 1;

    //locate and open the terminals
    locateTerminals(terminalptrs);

    //display the current time (start of mission)
    printf("The mission is about to begin. The start time is...\n");
    printTime();

    //get the base process id
    pids[0] = getpid();

    //spawn in the three child processes
    for(subIDnum = 1; subIDnum <= 3; subIDnum++)
      {
        pids[subIDnum] = fork();
        if(pids[subIDnum] == 0)
          {
            //do child stuff
            parentTog = 0;
            setSubs();
          }
      }

    //perform parent processes apon spawining child process
    if(parentTog)
      {
        pids[4] = fork();
        if(pids[4] == 0)
          {
            //handle the user input
            userSubCommandProcess();
          }
        else
          {
            //perform parent actions
            baseActions();
            //end parent process
            kill(pids[4],1);
            //close the terminals
            closeTerminals();
          }
      }

    //display the current time (end of mission)
    printf("The mission has ended. The end time is...\n");
    printTime();

    return 0;
  }

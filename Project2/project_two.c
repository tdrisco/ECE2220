/* Timothy Driscoll
   ECE 2220 Project 2, 2/4/19

   Program Description: This program is designed to create a menu with different
   options to store personal data. It is focused on how to store different types
   of data and use functions such as fgets and sscanf to read in input from
   stdin.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  char displayMenu()
    {
      char userChoice = 'x';
      printf("\n\t\t\t\tMain Menu\n\n");
      printf("1. Enter Name\t\t\t\t\t\t6. Display the Data\n");
      printf("2. Enter Years, Party, Office, and State\t\t7. Clear all Data\n");
      printf("3. Enter Age and Sex\t\t\t\t\t8. Quit Program\n");
      printf("4. Enter Contacts\n5. Enter Contributions and Lies\n\n");
      printf("Please enter the number of your menu selection: ");
      userChoice = getchar();
      while(getchar()!='\n');
      return(userChoice);
    }

  void displayInformation(char *firstName, char *middleIntial, char* lastName,char *suffix, int *age, char *sex, char *officeHeld, int *yearsServed, int *party, char *state, long *phoneNumber, char *twitter)
    {
      printf("\nYou have chose to display the data.\n");
      printf("\nFirst Name: %s",firstName);
      printf("\nMiddle Intial: %s",middleIntial);
      printf("\nLast Name: %s",lastName);
      printf("\nSuffix: %s",suffix);

      printf("\n\nOffice Held: %s",officeHeld);
      printf("\nYears Serves: %d\n",*yearsServed);
      if(*party == 1)
      printf("Political Part: Constitution");
      else if(*party == 2)
      printf("Political Part: Democrat");
      else if(*party == 3)
      printf("Political Part: Green");
      else if(*party == 4)
      printf("Political Part: Libertarian");
      else if(*party == 5)
      printf("Political Part: Repbulican");
      else
      printf("Political Part: x");
      printf("\nState: %s",state);

      printf("\n\nAge: %d",*age);
      printf("\nSex: %s", sex);

      printf("\n\nPhone Number: %ld",*phoneNumber);
      if(*twitter == 'x' && *(twitter+1) == '\0')
      printf("\nTwitter Handle: %s",twitter);
      else
      printf("\nTwitter Handle: @%s",twitter);

    }

  void setDefaultValues(char *firstName, char *middleIntial, char* lastName,char *suffix, int *age, char *sex, char *officeHeld, int *yearsServed, int *party, char *state, long *phoneNumber, char *twitter)
    {

      *firstName = 'x';
      *(firstName+1) = '\0';
      *middleIntial = ' ';
      *lastName = 'x';
      *(lastName+1) = '\0';
      *suffix = ' ';
      *(suffix+1) = '\0';

      *officeHeld = 'x';
      *(officeHeld+1) = '\0';
      *yearsServed = -0;
      *party = 0;
      *state = 'x';
      *(state+1) = '\0';

      *age = 0;
      *sex = 'x';

      *phoneNumber = 0;
      *twitter = 'x';
      *(twitter+1) = '\0';
    }

  void setName(char* firstName,char *middleIntial, char *lastName, char *suffix)
    {
      //char buffer[32];
      printf("\nYou have chosen to enter name data.\n");
      printf("First Name: ");
      fgets(firstName,32,stdin);
      if(*(firstName+strlen(firstName)-1) == '\n')
      *(firstName+strlen(firstName)-1) = '\0';
      printf("Middle Intial: ");
      *middleIntial = getchar();
      while(getchar()!='\n');
      printf("Last Name: ");
      fgets(lastName,32,stdin);
      if(*(lastName+strlen(lastName)-1) == '\n')
      *(lastName+strlen(lastName)-1) = '\0';
      printf("Suffix: ");
      fgets(suffix,4,stdin);
      if(*(suffix+strlen(suffix)-1) == '\n')
      *(suffix+strlen(suffix)-1) = '\0';
      while(getchar()!='\n');
    }

  void setOptionTwo(char *officeHeld, int *yearsServed, int *party, char *state)
    {
      printf("\nYou have chose to enter data about years, party, office and state\n");
      printf("Office Held: ");
      fgets(officeHeld,32,stdin);
      if(*(officeHeld+strlen(officeHeld)-1) == '\n');
      *(officeHeld+strlen(officeHeld)-1) = '\0';

      printf("Years Served: ");
      scanf("%d",yearsServed);

      printf("\nSelect one of the following political parties below by choosing the corresponding number.\n");
      printf("1. Constitution\n2. Democrat\n3. Green\n4. Libertarian\n5. Republican\n");
      printf("\nPolitical Party: ");
      scanf("%d",party);
      while(getchar()!='\n');


      printf("\nPlease select the state that you served in from the list below.\n");
      printf("\nAlabama - AL\t\tAlaska - AK\t\tArizona - AZ\t\tArkansas - AR");
      printf("\nCalifornia - CA\t\tColorado - CO\t\tConnecticut - CT\tDelaware - DE");
      printf("\nFlorida - FL\t\tGeorgia - GA\t\tHawaii - HI\t\tIdaho - ID");
      printf("\nIllinois - IL\t\tIndiana - IN\t\tIowa - IA\t\tKansas - KS");
      printf("\nKentucky - KY\t\tLouisiana - LA\t\tMaine - ME\t\tMaryland - MD");
      printf("\nMassachusetts - MA\tMichigan - MI\t\tMinnesota - MN\t\tMississippi - MS");
      printf("\nMissouri - MO\t\tMontana - MT\t\tNebraska - NE\t\tNevada - NV");
      printf("\nNew Hampshire - NH\tNew Jersey - NJ\t\tNew Mexico - NM\t\tNew York - NY");
      printf("\nNorth Carolina - NC\tNorth Dakota - ND\tOhio - OH\t\tOklahoma - OK");
      printf("\nOregon - OR\t\tPennsylvania - PA\tRhode Island - RI\tSouth Carolina - SC");
      printf("\nSouth Dakota - SD\tTennessee - TN\t\tTexas - TX\t\tUtah - UT");
      printf("\nVermont - VT\t\tVirginia - VA\t\tWashington - WA\t\tWest Virginia - WV");
      printf("\nWisconsin - WI\t\tWyoming - WY\n\n");
      printf("State (Abbreviation): ");
      fgets(state,4,stdin);
      if(*(state+strlen(state)-1) == '\n')
      *(state+strlen(state)-1) = '\0';

    }

  void setOptionThree(int* age, char* sex)
    {
      printf("\nYou have chosen to enter data about your age and sex\n");
      printf("Age (21 - 99): ");
      scanf("%d",age);
      printf("Sex (M or F): ");
      while(getchar()!='\n');
      *sex = getchar();
      while(getchar()!='\n');
    }
  void setOptionFour(long *phoneNumber, char *twitter)
    {
      printf("\nYou have chosen to enter data about you contact information.\n");
      printf("Phone Number (No Spaces or Dashes): ");
      scanf("%ld",phoneNumber);
      while(getchar()!='\n');
      printf("Twitter Handle: ");
      fgets(twitter,15,stdin);
      if(*(twitter+strlen(twitter)-1) == '\n')
      *(twitter+strlen(twitter)-1) = '\0';
    }

int main(void)
{
  char userMenuChoice = 'x';

  char *firstName;
  char *middleIntial;
  char *lastName;
  char *suffix;

  char *officeHeld;
  int *yearsServed;
  int *party;
  char *state;

  int *age;
  char *sex;

  long *phoneNumber;
  char *twitter;

  firstName = malloc(32*sizeof(char));
  lastName = malloc(32*sizeof(char));
  middleIntial = malloc(sizeof(char));
  suffix = malloc(4*sizeof(char));

  officeHeld = malloc(32*sizeof(char));
  yearsServed = malloc(2*sizeof(int));
  party = malloc(sizeof(int));
  state = malloc(3*sizeof(char));

  age = malloc(2*sizeof(int));
  sex = malloc(sizeof(char));

  phoneNumber = malloc(9*sizeof(int));
  twitter = malloc(15*sizeof(char));

  setDefaultValues(firstName,middleIntial,lastName,suffix,age,sex,officeHeld,yearsServed,party,state,phoneNumber,twitter);

  do
    {
      userMenuChoice = displayMenu();
      switch(userMenuChoice)
        {
          case '1':
          setName(firstName, middleIntial, lastName, suffix);
          break;
          case '2':
          setOptionTwo(officeHeld,yearsServed,party,state);
          break;
          case '3':
          setOptionThree(age,sex);
          break;
          case '4':
          setOptionFour(phoneNumber,twitter);
          break;
          case '5':
          break;
          case '6':
          displayInformation(firstName,middleIntial,lastName,suffix,age,sex,officeHeld,yearsServed,party,state,phoneNumber,twitter);
          break;
          case '7':
          setDefaultValues(firstName,middleIntial,lastName,suffix,age,sex,officeHeld,yearsServed,party,state,phoneNumber,twitter);
          break;
          case '8':
          printf("\nYou have chosen to quit the program.\nGoodbye\n");
          break;
          default:
          printf("\nYou will be returned to the menu please enter a valid option.\n");
        }
    } while(userMenuChoice != '8');


return(0);
}

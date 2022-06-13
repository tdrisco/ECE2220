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

// Define individual structures that store the data for each different
// menu option
  typedef struct
    {
      char firstName[32];
      char middleIntial;
      char lastName[32];
      char suffix[3];
    } nameData;

  typedef struct
    {
      char officeHeld[32];
      int yearsServed;
      int party;
      char state[3];
    } workData;

  typedef struct
    {
      int age;
      char sex;
    } personalData;

  typedef struct
    {
      long phoneNumber;
      char twitter[15];
    } contactData;

  typedef struct
    {
      double dollarsReceived;
      double liesTold;
    } money_liesData;

  /* A display menu function that has no arguments and returns a character
  value that represents the corresponding menu selection. The function prints
  out the entire menu for the program and asks the user to enter a selection */
  char displayMenu()
    {
      //Intialize the user selection to x and then print the neccesary
      //statements to display the menu
      char userChoice = 'x';
      printf("\n\t\t\t\tMain Menu\n\n");
      printf("1. Enter Name\t\t\t\t\t\t6. Display the Data\n");
      printf("2. Enter Years, Party, Office, and State\t\t7. Clear all Data\n");
      printf("3. Enter Age and Sex\t\t\t\t\t8. Quit Program\n");
      printf("4. Enter Contacts\n5. Enter Contributions and Lies\n\n");
      printf("Please enter the number of your menu selection: ");
      //Get the user selection and then clear the stdin feed
      userChoice = getchar();
      while(getchar()!='\n');
      //return the user selection
      return userChoice;
    }

  /* This function accepts the five different structures as arguments and
  doesn't have any return values. This function is used to display all the
  user inputed data. It will display the data regarless as to whether the user
  has entered anything, it will just display the values indicating that the
  user is yet to enter data */
  void displayInformation(nameData name, workData work, personalData personal,
                          contactData contact, money_liesData moneyLies)
    {
      // This block of print statements displays all the name data
      printf("\nYou have chose to display the data.\n");
      printf("\nFirst Name: %s",name.firstName);
      printf("\nMiddle Intial: %c",name.middleIntial);
      printf("\nLast Name: %s",name.lastName);
      printf("\nSuffix: %s",name.suffix);

      // This block of print statements displays all the work data
      printf("\n\nOffice Held: %s",work.officeHeld);
      printf("\nYears Served: %d\n",work.yearsServed);
      // Series of if statements to display the corresponding party to the
      // user selection
      if(work.party == 1)
      printf("Political Part: Constitution");
      else if(work.party == 2)
      printf("Political Part: Democrat");
      else if(work.party == 3)
      printf("Political Part: Green");
      else if(work.party == 4)
      printf("Political Part: Libertarian");
      else if(work.party == 5)
      printf("Political Part: Repbulican");
      else
      printf("Political Part: x");
      printf("\nState: %s",work.state);

      // These two statements print out the personal data
      printf("\n\nAge: %d",personal.age);
      printf("\nSex: %c", personal.sex);

      // if statement to determine if the user has entered a phone number yet
      if(contact.phoneNumber == 0)
      printf("\n\nPhone Number: %ld",contact.phoneNumber);
      else
        {
          // Intialize a buffer array to hold the phone number, and other
          // neccesary values
          int phoneNumPrint[10];
          int count = 9;
          int i;
          long phoneNumHolder = contact.phoneNumber;

          // Place each individual digit of the phone number into and int array
          while(phoneNumHolder >= 1)
            {
              // get each digits starting in the ones place and put it in array
              phoneNumPrint[count] = phoneNumHolder % 10;
              phoneNumHolder = phoneNumHolder / 10;
              count = count -1;
            }

          // Print out the phone number in the correct form
          printf("\n\nPhone Number: (");
          for(i=0;i<10;i++)
            {
              if(i==3)
              printf(")");
              else if(i==6)
              printf("-");
              printf("%d",phoneNumPrint[i]);
            }
        }

      // Print out the contact information. Also check if the user has entered
      // a twitter handle to determine how to print it
      if(contact.twitter[0] == 'x' && contact.twitter[1] == '\0')
      printf("\nTwitter Handle: %s",contact.twitter);
      else
      printf("\nTwitter Handle: @%s",contact.twitter);

      printf("\n\nUS Dollars Received: %0.1lE",moneyLies.dollarsReceived);
      printf("\nNumber of Lies Told: %0.1lE",moneyLies.liesTold);

      if(moneyLies.dollarsReceived != -1)
        {
          printf("\n\nRatio of Contributions to Lies Told: ");
          printf("%0.2lE",(moneyLies.dollarsReceived/moneyLies.liesTold));
          printf(" Dollars per Lie\n");
        }
    }

  /* This function returns a nameData structure and excpets one as an argument.
  It then intializes all the varibles within the structure to all the
  unknown values. */
  nameData setDefaultNameData(nameData name)
    {
      //Intializes all the values in the structure to values that they couldn't
      //possibly be so the user knows they haven't been entered for yet
      name.firstName[0] = 'x';
      name.firstName[1] = '\0';
      name.middleIntial = ' ';
      name.lastName[0] = 'x';
      name.lastName[1] = '\0';
      name.suffix[0] = ' ';
      name.suffix[1] = '\0';

      //return the intialized structure
      return name;
    }

    /* This function returns a workData structure and excpets one as an
    argument. It then intializes all the varibles within the structure to all
    the unknown values. */
    workData setDefaultWorkData(workData work)
      {
      //Intializes all the values in the structure to values that they couldn't
      //possibly be so the user knows they haven't been entered for yet
        work.officeHeld[0] = 'x';
        work.officeHeld[1] = '\0';
        work.yearsServed = -1;
        work.party = 0;
        work.state[0] = 'x';
        work.state[1] = '\0';

        //return the intialized structure
        return work;
      }

      /* This function returns a personalData structure and excpets one as an
      argument. It then intializes all the varibles within the structure to all
      the unknown values. */
    personalData setDefaultPersonalData(personalData personal)
      {
      //Intializes all the values in the structure to values that they couldn't
      //possibly be so the user knows they haven't been entered for yet
        personal.age = 0;
        personal.sex = 'x';

        //return the intialized structure
        return personal;
      }

    /* This function returns a contactData structure and excpets one as an
    argument. It then intializes all the varibles within the structure to all
    the unknown values. */
    contactData setDefaultContactData(contactData contact)
      {
        //Intializes all the values in the structure to values that they
        //couldn't possibly be so the user knows they haven't been entered for
        //yet
        contact.phoneNumber = 0;
        contact.twitter[0] = 'x';
        contact.twitter[1] = '\0';

        //return the intialized structure
        return contact;
      }

    /* This function returns a money_liesData structure and excpets one as an
    argument. It then intializes all the varibles within the structure to all
    the unknown values. */
    money_liesData setDefaultMoney_liesData(money_liesData moneyLies)
      {
        //Intializes all the values in the structure to values that they couldn't
        //possibly be so the user knows they haven't been entered for yet
        moneyLies.dollarsReceived = -1;
        moneyLies.liesTold = -1;

        // return the intialized structure
        return moneyLies;
      }

    /*This function returns a nameData structure and excpets one as an argument.
    The function asks the user to enter in all the different data and checks to
    ensure that all the entries are valid.*/
    nameData setName(nameData name)
      {
        // Have the user input the data for the first name using fgets
        printf("\nYou have chosen to enter name data.\n");
        printf("First Name: ");
        scanf("%s",name.firstName);
        //Make sure the first name is no longer then 33 characters
        while(strlen(name.firstName)>32)
          {
            printf("Please Re-Enter Your First Name: ");
            scanf("%s",name.firstName);
          }
        while(getchar()!='\n');
        // make sure the last character in the string is the null character
        if(name.firstName[strlen(name.firstName)-1] == '\n')
        name.firstName[strlen(name.firstName)-1] = '\0';

        // Using getchar to grab a single charcter have the user input their
        // middle intial
        printf("Middle Intial: ");
        name.middleIntial = getchar();
        //Check if user skips input (null) and replace newline character with
        // a blank space
        if(name.middleIntial == '\n')
          name.middleIntial = ' ';
        else
        // Make sure the feed is cleared of any newline characters
          while(getchar()!='\n');

        // Have the user input the data for the last name using fgets and again
        // make sure that the last character is the null character
        printf("Last Name: ");
        scanf("%s",name.lastName);
        //Make sure the last name is no more than 32 characters
        while(strlen(name.lastName)>32)
          {
            printf("Please Re-Enter Your Last Name: ");
            scanf("%s",name.lastName);
          }
        while(getchar()!='\n');
        if(name.lastName[strlen(name.lastName)-1] == '\n')
        name.lastName[strlen(name.lastName)-1] = '\0';

        // Perform the same task for the suffix as was done for the first and
        // last name
        printf("Suffix: ");
        fgets(name.suffix,10,stdin);
        //Check to see if the user entered null (skipped with newline charcter)
        if(name.suffix[0] == '\n')
          {
            //Reset suffix to be blank
            name.suffix[0] = ' ';
            name.suffix[1] = '\0';
          }
        //Check to make sure the string is under 3 characters
        while(strlen(name.suffix)>4)
          {
            printf("Please Re-Enter Your Suffix: ");
            fgets(name.suffix,10,stdin);
            if(name.suffix[0] == '\n')
              {
                //Reset suffix to be blank
                name.suffix[0] = ' ';
                name.suffix[1] = '\0';
              }
          }
        //Make sure the last character is null
        if(name.suffix[strlen(name.suffix)-1] == '\n')
        name.suffix[strlen(name.suffix)-1] = '\0';


        // return the structure
        return name;
      }

  /* This function returns a workData structure and excpets one as an argument.
  The function asks the user to enter in all the different data and checks to
  ensure that all the entries are valid.*/
  workData setWork(workData work)
    {
      // Have the user enter office held using fgets and make sure the last
      // character is the null character
      printf("\nYou have chose to enter data about years, party, office");
      printf("and state\n");
      printf("Office Held: ");
      scanf("%s",work.officeHeld);
      //Make sure that the office held is no longer then 32 charcters
      while(strlen(work.officeHeld)>32)
        {
          printf("Please Re-Enter Office Held: ");
          scanf("%s",work.officeHeld);
        }
      while(getchar()!='\n');

      // Have the user enter the number of years they served and use scanf to
      // get the value
      printf("Years Served: ");
      scanf("%d",&work.yearsServed);
      //Makes sure years served is within desired range
      while(work.yearsServed>99 || work.yearsServed<0)
        {
          printf("Please Re-Enter Years Served (0 - 99): ");
          scanf("%d",&work.yearsServed);
        }
      // Have the user pick there respective party corresponding to the
      // given list, using scanf
      printf("\nSelect one of the following political parties below by");
      printf("choosing the corresponding number.\n");
      printf("1. Constitution\n2. Democrat\n3. Green\n4. Libertarian\n");
      printf("5. Republican\n");
      printf("\nPolitical Party: ");
      scanf("%d",&work.party);
      //Make sure the user selected a valid option
      while(work.party>5 || work.party<1)
        {
          printf("Please Re-Enter Party Choice: ");
          scanf("%d",&work.party);
        }
      while(getchar()!='\n');

      // Print out all the abbreviation of all the states
      printf("\nPlease select the state that you served in from the options below.\n");
      printf("\nAL\tAK\tAZ\tAR");
      printf("\tCA\tCO\tCT\tDE");
      printf("\n\nFL\tGA\tHI\tID");
      printf("\tIL\tIN\tIA\tKS");
      printf("\n\nKY\tLA\tME\tMD");
      printf("\tMA\tMI\tMN\tMS");
      printf("\n\nMO\tMT\tNE\tNV");
      printf("\tNH\tNJ\tNM\tNY");
      printf("\n\nNC\tND\tOH\tOK");
      printf("\tOR\tPA\tRI\tSC");
      printf("\n\nSD\tTN\tTX\tUT");
      printf("\tVT\tVA\tWA\tWV");
      printf("\n\nWI\tWY\n\n");
      // Have the user pick which state they worked in using fgets.
      // Make sure the last character is the null character
      printf("State (Abbreviation): ");
      scanf("%s",work.state);
      //Make sure that the state entered is no more than two characters
      while(strlen(work.state)>2)
        {
          printf("Please Re-Enter the State Abbreviation: ");
          scanf("%s",work.state);
        }
      // In not the most efficiant way check to see if the user entered
      // a valid state abbreviation.
      while(strcmp(work.state,"AL")!= 0 && strcmp(work.state,"AK")!= 0
      && strcmp(work.state,"AZ")!= 0 && strcmp(work.state,"AR")!= 0
      && strcmp(work.state,"AR")!= 0 && strcmp(work.state,"CA")!= 0
      && strcmp(work.state,"CO")!= 0 && strcmp(work.state,"CT")!= 0
      && strcmp(work.state,"DE")!= 0 && strcmp(work.state,"FL")!= 0
      && strcmp(work.state,"GA")!= 0 && strcmp(work.state,"HI")!= 0
      && strcmp(work.state,"ID")!= 0 && strcmp(work.state,"IL")!= 0
      && strcmp(work.state,"IN")!= 0 && strcmp(work.state,"IA")!= 0
      && strcmp(work.state,"KS")!= 0 && strcmp(work.state,"KY")!= 0
      && strcmp(work.state,"LA")!= 0 && strcmp(work.state,"ME")!= 0
      && strcmp(work.state,"MD")!= 0 && strcmp(work.state,"MA")!= 0
      && strcmp(work.state,"MI")!= 0 && strcmp(work.state,"MN")!= 0
      && strcmp(work.state,"MS")!= 0 && strcmp(work.state,"MO")!= 0
      && strcmp(work.state,"MT")!= 0 && strcmp(work.state,"NE")!= 0
      && strcmp(work.state,"NV")!= 0 && strcmp(work.state,"NH")!= 0
      && strcmp(work.state,"NJ")!= 0 && strcmp(work.state,"NM")!= 0
      && strcmp(work.state,"NY")!= 0 && strcmp(work.state,"NC")!= 0
      && strcmp(work.state,"ND")!= 0 && strcmp(work.state,"OH")!= 0
      && strcmp(work.state,"OK")!= 0 && strcmp(work.state,"OR")!= 0
      && strcmp(work.state,"PA")!= 0 && strcmp(work.state,"RI")!= 0
      && strcmp(work.state,"SC")!= 0 && strcmp(work.state,"SD")!= 0
      && strcmp(work.state,"TN")!= 0 && strcmp(work.state,"TX")!= 0
      && strcmp(work.state,"UT")!= 0 && strcmp(work.state,"VT")!= 0
      && strcmp(work.state,"VA")!= 0 && strcmp(work.state,"WA")!= 0
      && strcmp(work.state,"WV")!= 0 && strcmp(work.state,"WI")!= 0
      && strcmp(work.state,"WY")!= 0)
        {
          printf("Please Re-Enter the State Abbreviation: ");
          scanf("%s",work.state);
        }
      while(getchar()!='\n');
      // Make sure last character in string is \0
      if(work.state[strlen(work.state)-1] == '\n')
      work.state[strlen(work.state)-1] = '\0';

      // return the structure
      return work;
    }
  /* This function returns a personalData structure and excpets one as an
  argument. The function asks the user to enter in all the different data and
  checks to ensure that all the entries are valid.*/
  personalData setPersonal(personalData personal)
    {
      // Have the user enter their name using scanf
      printf("\nYou have chosen to enter data about your age and sex\n");
      printf("Age (21 - 99): ");
      scanf("%d",&personal.age);
      //Make sure that the age is within desired range
      while(personal.age>99 || personal.age<21)
        {
          printf("Please Re-Enter Your Age (21 - 99): ");
          scanf("%d",&personal.age);
        }

      // Have the user their name using getchar becuase it is a single charcter
      printf("Sex (M or F): ");
      while(getchar()!='\n');
      personal.sex = getchar();
      while(personal.sex != 'm' && personal.sex != 'M' && personal.sex != 'f'
                                                       && personal.sex != 'F')
        {
          printf("Please Re-Enter Sex (M or F): ");
          while(getchar()!='\n');
          personal.sex = getchar();
        }
      // clear the command feed of new line characters
      while(getchar()!='\n');

      //return the structure
      return personal;
    }

  /* This function returns a contactData structure and excpets one as an argument.
  The function asks the user to enter in all the different data and checks to
  ensure that all the entries are valid.*/
  contactData setContact(contactData contact)
    {
      // Have the user enter their phone number using the scanf function
      printf("\nYou have chosen to enter data about you contact information.\n");
      printf("Phone Number (No Spaces or Dashes): ");
      scanf("%ld",&contact.phoneNumber);
      //Make sure phone number contains ten digits with atleast a leading 1
      while(contact.phoneNumber >9999999999 || contact.phoneNumber < 1000000000)
        {
          printf("Please Re-Enter Phone Number (No Spaces or Dashes): ");
          scanf("%ld",&contact.phoneNumber);
        }
      // clear new line characters from the command feed
      while(getchar()!='\n');

      // Have the user enter their twitter handle using fgets
      printf("Twitter Handle: ");
      scanf("%s",contact.twitter);
      //make sure twitter handle has no more then 15 characters
      while(strlen(contact.twitter)>15)
        {
          printf("Please Re-Enter Twitter Handle: ");
          scanf("%s",contact.twitter);
        }
      while(getchar()!='\n');
      // Make sure the last character is the null character
      if(contact.twitter[strlen(contact.twitter)-1] == '\n')
      contact.twitter[strlen(contact.twitter)-1] = '\0';

      // return the structure
      return contact;
    }

  money_liesData setMoneyLies(money_liesData moneyLies)
    {
      printf("\nYou have chosen to enter data about the money you earned\n");
      printf("and about the lies that you have told.\n");

      printf("\nUS Dollars Recieved: ");
      scanf("%lf",&moneyLies.dollarsReceived);
      //Make sure dollars received is in the desired range
      while(moneyLies.dollarsReceived < 0 || moneyLies.dollarsReceived > 1E50)
        {
          printf("Please Re-Enter US Dollars Recieved(0 - 1.0E50): ");
          scanf("%lf",&moneyLies.dollarsReceived);
        }

      printf("Number of Lies Told: ");
      scanf("%lf",&moneyLies.liesTold);
      //Make sure lies told is in the desired range
      while(moneyLies.liesTold < 0 || moneyLies.liesTold > 1E100)
        {
          printf("Please Re-Enter Number of Lies Told(0 - 1E100): ");
          scanf("%lf",&moneyLies.liesTold);
        }
      while(getchar()!='\n');

      return moneyLies;
    }

int main(void)
{
  // Intialize all the neccesary variables for the program
  char userMenuChoice = 'x';
  nameData name;
  workData work;
  personalData personal;
  contactData contact;
  money_liesData moneyLies;

  //Intialize all the stuctures to their respective default values
  name = setDefaultNameData(name);
  work = setDefaultWorkData(work);
  personal = setDefaultPersonalData(personal);
  contact = setDefaultContactData(contact);
  moneyLies = setDefaultMoney_liesData(moneyLies);

  // do while to contnue to display the menu until the user decides to quit
  do
    {
      // display menu and use a switch statement to determine which function
      // is called.
      userMenuChoice = displayMenu();
      switch(userMenuChoice)
        {
          case '1':
          name = setName(name);
          break;
          case '2':
          work = setWork(work);
          break;
          case '3':
          personal = setPersonal(personal);
          break;
          case '4':
          contact = setContact(contact);
          break;
          case '5':
          moneyLies = setMoneyLies(moneyLies);
          break;
          case '6':
          displayInformation(name,work,personal,contact,moneyLies);
          break;
          case '7':
          // Reset all the data to their default values
          name = setDefaultNameData(name);
          work = setDefaultWorkData(work);
          personal = setDefaultPersonalData(personal);
          contact = setDefaultContactData(contact);
          moneyLies = setDefaultMoney_liesData(moneyLies);
          break;
          case '8':
          //Quit statement
          printf("\nYou have chosen to quit the program.\nGoodbye\n");
          break;
          default:
          //If the user enters an invalid choice
          printf("\nYou will be returned to the menu please enter a");
          printf("valid option.\n");
        }
    } while(userMenuChoice != '8'); //Condition to keep displaying the menu


return(0);
}

/* Timothy Driscoll
   ECE 2220 Project 4, 3/11/19

   Program Description: This program is designed apply the use of opertaing on
   strings, dynamic memory allocation, file operations, command line arguments
   and calculating and storing statistics. This is all used to create a program
   that performs various string arguments on text files such as searching for
   text, replacing words, checking spelling and formating lines.
*/

//Include the neccesary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A display menu function that has no arguments and returns a int
value that represents the corresponding menu selection. The function prints
out the entire menu for the program and asks the user to enter a selection */
int displayMenu()
  {
    //Intialize the user selection to 0 and then print the neccesary
    //statements to display the menu
    int userChoice = 0;
    printf("\n\t\t\t\tMain Menu\n\n");
    printf("1. Check the spelling of the file using dictionary.\n");
    printf("2. Search for and replace a given string in the file.\n");
    printf("3. Save the modified file to the output file, output_file.\n");
    printf("4. Exit the program\n\n");
    printf("Please enter the number of your menu selection: ");
    //Obtain the users choice and clear the buffer feed of the newline character
    scanf("%d",&userChoice);
    getchar();
    //return the user selection
    return userChoice;
  }

/*This is a simple function that excepts a single ascii letter and returns its
lower case counterpart*/
char stringLower(char letter)
  {
    //Using the correct ascii range check if the character is and uppercase
    //letter
    if(letter>64&&letter<91)
      //convert the value to lower case by subtracting 32
      letter = letter + 32;
    //return the letter
    return letter;
  }


/*This function is designed to read in the data from the input file. This
function only excepts the input file as an argument and a pointer to a two
dimensional array, used to store the data. It then takes that file
and uses a loop to pull the data from the file and assign it to the
array. An integer is then returned indicating the number of rows read in. */
int getData(FILE*input, char ***text)
  {
    //Intialize neccesary local variables
    char lineBuffer[200];
    int lineLength;
    int i = 0;

    //while loop that reads from the input file line by line until there are
    //no more lines left
    while(fgets(lineBuffer,199,input))
    {
      //Realloc memory to include for a new line each loop through
      *text = (char **)realloc(*text,(i+1)*sizeof(char *));
      //determine the lenght of the line read using fgets
      lineLength = strlen(lineBuffer);
      //using the lenght of the line malloc the space for this line in the array
      (*text)[i] = (char *)malloc((lineLength+1)*sizeof(char));
      //copy the line from the buffer array to the text array
      strcpy((*text)[i],lineBuffer);
      //increment the row counter
      i++;
    }
    return i;
  }

/*This function is designed to determine if a given word is present in a given
dictionary. This fucntion accepts the file pointer to the given dictionary and
the character array containing the word to be scanned for. The function returns
an integer that indicates whether or not a match was found in the dictionary. */
int dictionaryScan(FILE *Dictionary, char word[120])
  {
    //Intialize any neccesary local variables
    int wordMatch, i;
    char wordCompare[120];
    fseek(Dictionary,0,SEEK_SET);

    //while loop to run through the dictionary file until reaching the end of
    //the file
    while(!feof(Dictionary))
      {
        //fscanf to grab one word from the dictionary through each loop
        fscanf(Dictionary,"%s",wordCompare);
        //set the word from the dictionary to be all lower case
        for(i=0;i<strlen(wordCompare);i++)
          wordCompare[i] = stringLower(wordCompare[i]);
        //use a string compare to determine if the word matches the word in
        //the dictionary
        wordMatch = strcmp(wordCompare,word);
        //If there is a match return a 1
        if(wordMatch == 0)
          return 1;
      }
    //If the program makes it through the while loop without finding a match
    //return a 0
    return 0;
  }

/*This function is designed to take a given word and replace it with a new user
defined word. It accepts multiple arguments. The arguments include the array
holding all of the text, the row and column indeces, and the word to be
replaced. The function then returns the integer value of the new column index.*/
int ReplaceWord(char **text,int i,int j,char word[120])
  {
    //Intiliaze neccesary local variables
    char buffer[200];
    int lengthDifference;
    char replacement[120];

    //have the user enter in their desired replacement string
    printf("\nPlease enter the replacement word or string: ");
    fgets(replacement,119,stdin);
    //set the last character to null
    replacement[strlen(replacement)-1] = '\0';
    //Determine the difference in length between the orginal and replacement
    lengthDifference = strlen(replacement)-strlen(word);

    //Copy the from the orginal word to the emd of the line into a buffer array
    strcpy(buffer,&text[i][j]);
    //Copy the replacement word into the position of the orginal word
    strcpy(&text[i][j-strlen(word)],replacement);
    //Copy the rest of the line back into the array from the buffer
    strcpy(&text[i][j+lengthDifference],buffer);
    //return the new column index
    return j+lengthDifference;

  }

/*This function is designed to go through the array containing all the text and
compare each word to a given dictionary and a personal dictionary if it has been
created. The function excepts a file pointer to the dictionary, the array
containing the text and the number of rows in the text.*/
void spellCheck(FILE *Dictionary, int rowNum, char **text)
  {
    //Declare neccessary local variables
    int i,j,k,c, wordMatch, replaceToggle;
    int userDictToggle = 0;
    char word[100], userDictName[120];
    FILE *userDictionary;

    //for loop to run through each row in the array
    for(i=0;i<rowNum;i++)
      {
        //for loop to run through each line in the array
        for(j=0;text[i][j]!='\n';j++)
          {
            //Itialize k to zero and word to null for each loop iteration
            k = 0;
            strcpy(word,"\0");
            //while loop to grab each individual word
            while((text[i][j]>64&&text[i][j]<91)||
                  (text[i][j]>96&&text[i][j]<122))
              {
                //set the word eqaul each word in text (one at a time)
                word[k] = text[i][j];
                //increment counters
                k++;
                j++;
              }
            //Add the null character to the end of the string
            word[k] = '\0';
            //convert the word to lower case
            for(c=0;c<strlen(word);c++)
              word[c] = stringLower(word[c]);
            //make sure word is greater than 0 characters
            if(k>0)
              {
                //reset word match toggle to 0
                wordMatch = 0;
                //scan the dictionary using function
                wordMatch = dictionaryScan(Dictionary,word);
                //scan the user dictionary if it exists
                if(wordMatch == 0 && userDictToggle == 1)
                    wordMatch = dictionaryScan(userDictionary,word);
                //what to do if there is no match
                if(wordMatch == 0)
                  {
                    if(wordMatch == 0 && userDictToggle ==1)
                      {
                        //Tell the user word wasn't found in the dictionary
                        //or there personal dictionary
                        printf("\n\"%s\" was not found in the dictionary",word);
                        printf(" or your personal dictionary.");
                      }
                    else
                      {
                        //if no personal dicitonary just tell the user the word
                        //wasn"t found in the dictionary
                       printf("\n\"%s\" was not found in the dictionary.",word);
                      }
                    //ask the user what they choose to do and read in response
                    printf("\n\nWhat would you like to do with this word.");
                    printf("\n1.Replace the word\n2.Ignore It");
                    printf("\n\nPlease enter your selection (1 or 2): ");
                    scanf("%d",&replaceToggle);
                    getchar();
                    //While loop to wait for a valid input
                    while(replaceToggle<1 || replaceToggle >2)
                      {
                        printf("Please enter a valid option (1 or 2): ");
                        scanf("%d",&replaceToggle);
                        getchar();
                      }
                    //if they choose to replace the word call the replace funct
                    if(replaceToggle == 1)
                      {
                        j = ReplaceWord(text,i,j,word);
                      }
                    //If they choose to ignore add to or create their personal
                    //dictionary
                    else
                      {
                        //if personal dicitonary doesn't exsist create it
                        if(userDictToggle == 0)
                          {
                            //get name for and create personal dictionary
                            printf("Please enter a name for your personal");
                            printf(" dictionary file (name.txt): ");
                            scanf("%s", userDictName);
                            userDictionary = fopen(userDictName,"w+");
                            userDictToggle = 1;
                          }
                        //add the word to the user's personal dictionary
                        fprintf(userDictionary,"%s\n",word);
                      }
                  }
              }
          }
      }

  }

/*This function is desgined to search for a user defined word or string and
replace it with a given string or word. The fucntion takes in two arguments.
The first argument is the number of rows that the array has and the second is
the two dimensional array containing all the text. The function doesn't have
any return values.*/
void searchAndReplace(int rows, char **text)
  {
    //Declare any neccesary local variables
    int colPosition, foundToggle  = 0, replaceIndex, caseSensitiveToggle;
    char wordToFind[100];
    char *replacementPtr;
    int i, j;

    printf("\nWould you like to perform case sensitive search?");
    printf("\n1.Yes");
    printf("\n2.No\n");
    printf("\nPlease choose (1 or 2): ");
    scanf("%d",&caseSensitiveToggle);
    getchar();

    //While loop to wait for a valid input
    while(caseSensitiveToggle<1 || caseSensitiveToggle >2)
      {
        printf("Please enter a valid option (1 or 2): ");
        scanf("%d",&caseSensitiveToggle);
        getchar();
      }

    //Ask the user for the string or word they would like to search for
    printf("\nWhat string would you like to search and replace for: ");
    fgets(wordToFind,100,stdin);
    //make the last charcter of the string null
    wordToFind[strlen(wordToFind)-1] = '\0';

    //For loop to iterate through each row of the two dimensional array
    for(i = 0; i < rows; i++)
      {
        char lowerCaseLine[strlen(text[i])];

        if(caseSensitiveToggle == 2)
          {
            strcpy(lowerCaseLine,text[i]);
            lowerCaseLine[strlen(lowerCaseLine)-1] = '0';

            for(j=0; j<strlen(wordToFind); j++)
              wordToFind[j] = stringLower(wordToFind[j]);
            for(j=0; j<strlen(lowerCaseLine); j++)
                lowerCaseLine[j] = stringLower(lowerCaseLine[j]);
            //use strstr function to see if string is in line
            replacementPtr = strstr(lowerCaseLine,wordToFind);
          }
        else
          {
            //use strstr function to see if string is in line
            replacementPtr = strstr(text[i],wordToFind);
          }
        //If statement to determine if the string was foud
        if(replacementPtr != NULL)
          {
            //update the found toggle
            foundToggle = 1;
            colPosition = 0;
            if(caseSensitiveToggle == 2)
              {
                //calculate the column position of the string
                colPosition = replacementPtr - &lowerCaseLine[0] +1;
              }
            else
              {
                //calculate the column position of the string
                colPosition = replacementPtr - text[i] +1;
              }
            //print out to the user where the string was found
            printf("\n\"%s\" was found in the text.\n",wordToFind);
            printf("Found on line %d at character %d.\n",(i+1),colPosition);
            //calculate the index of where the word is to be replaced
            replaceIndex = colPosition + strlen(wordToFind) - 1;
            //call the ReplaceWord function so the user can replace the string
            replaceIndex = ReplaceWord(text,i,replaceIndex, wordToFind);
          }
      }
    //if the string was not found tell the user
    if(foundToggle == 0)
    {
      printf("\n\"%s\" was not found in the text.\n",wordToFind);
    }
  }

/*This function is designed to print the updated text to the output file. The
fucntion accepts three different arguments. The arguments include the number
of rows in the array, the two dimensional array containing the text and the
pointer to the output file.*/
void saveToOutput(int rowNum, char **text, FILE *output)
  {
    //Intialize any needed local varibles
    int i = 0;

    //for loop to run through all the rows of the text array
    for(i = 0; i < rowNum; i++)
      {
        //print to the output file line by line
        fprintf(output," %s", text[i]);
      }
    //return the cursor back to the top of the output file
    fseek(output,0,SEEK_SET);
  }

int main(int argc, char *argv[])
  {
    //If statement to make sure that the user has the neccesary command line
    // arguments
    if(argc != 4)
      {
        //Print statement to show what should be included
        printf("<input_file.txt> <output_file.txt> <dictionary.txt>\n");
        //Exit program if the user doesn't provide proper inputs
        exit(1);
      }

    //Intialize the three file variables (Two Input, One Output)
    FILE *inputFile, *dictionary, *outputFile;

    //Open the two Input Files to be read from
    inputFile = fopen(argv[1],"r");
    dictionary = fopen(argv[3],"r");
    //Open the output file to be written to
    outputFile = fopen(argv[2],"w");

    //Determine if both the input files can be opened, exit the program is they
    //camnot be opened
    if(inputFile == NULL)
      {
        printf("Error opening input_file. Exiting program.\n");
        exit(1);
      }
    else if(dictionary == NULL)
      {
        printf("Error opening dictionary. Exiting program.\n");
        exit(1);
      }

    //Itialize needed variables
    int userMenuChoice;
    char **text = NULL;
    int rows;

    //call the get data function to read in text from the input file
    rows = getData(inputFile,&text);

    // do while to contnue to display the metnu until the user decides to quit
    do
      {
        // display menu and use a switch statement to determine which function
        // is called.
        userMenuChoice = displayMenu();
        switch(userMenuChoice)
          {
            case 1:
            //check the spelling using the dictionary
            spellCheck(dictionary,rows,text);
            printf("\nBe sure to save your updates file using option 3.\n");
            break;
            case 2:
            //search for a replace a given string in the file
            searchAndReplace(rows,text);
            printf("\nBe sure to save your updates file using option 3.\n");
            break;
            case 3:
            //save modified file to the output file
            saveToOutput(rows, text, outputFile);
            break;
            case 4:
            //Quit statement
            printf("\nYou have chosen to quit the program.\nGoodbye\n");
            //Close all the opened files
            fclose(inputFile);
            fclose(dictionary);
            fclose(outputFile);
            break;
            default:
            //If the user enters an invalid choice
            printf("\nYou will be returned to the menu please enter a");
            printf(" valid option.\n");
          }
      } while(userMenuChoice != 4); //Condition to keep displaying the menu

    return 0;
}

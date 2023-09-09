//  wordleGuessSecretWord.c
//  Have the program do the guessing to discover the secret wordle word.
//
//  Author: Dale Reed, 11/6/22
//  System: CLion and XCode
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
/*  Running the program looks like:

 */
#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
//#define WORDS_FILE_NAME "wordsLarge.txt"
#define WORDS_FILE_NAME  "wordsLarge.txt"
#define MAX_NUMBER_OF_WORDS 12947   // Number of words in the full set of words file
#define true 1   // Make boolean logic easier to understand
#define false 0  // Make boolean logic easier to understand

typedef struct wordCount wordCountStruct;
struct wordCount{
    char word[ WORD_LENGTH + 1];   // The word length plus NULL
    int score;                     // Score for the word
};


//-----------------------------------------------------------------------------------------
// Read in words from file into array.  We've previously read the data file once to
// find out how many words are in the file.
void readWordsFromFile(char fileName[], wordCountStruct *words, int *wordCount){
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert( inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[ 6];
    *wordCount = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( words[ *wordCount].word, inputString);
        words[ *wordCount].score = 0;
        (*wordCount)++;
    }

    fclose( inFilePtr);
} // end readWordsFromFile(..)


//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        return strcmp( ((wordCountStruct *)a)->word,  ((wordCountStruct *)b)->word );
    }
} //end compareFunction(..)
// -----------------------------------------------------------------------------------------
void compareWords(char secretWord[], char guessWord[], int guessNumber){
    char modified[] = "     ";

    strcpy(modified, guessWord);
    // printf("%s",modified);
    for(int i=0; i<WORD_LENGTH; i++){
        if(modified[i] == secretWord[i]){
            modified[i] = toupper(modified[i]);
            guessWord[i] = '-';
        }
    }
    
    char wrongPos[] = "     ";
    for(int i=0; i<WORD_LENGTH; i++){
        for(int j=0; j<WORD_LENGTH; j++){
            if(guessWord[i] == secretWord[j]){
                wrongPos[i] = '*';
            }
        }
    }
    printf("%5d. ", guessNumber);
    for(int i=0; i<WORD_LENGTH; i++) {
        printf("%c ", modified[i]);
    }
    printf("\n%5c  ", ' ');
    for(int i=0; i<WORD_LENGTH; i++){
        printf("%c ", wrongPos[i]);
    }
    printf("\n");

}
// -----------------------------------------------------------------------------------------
void changeWord(int startingPoint, int endingPoint, char guessWord[], char* secretWord[]){
 
}

// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(
        wordCountStruct allWords[],    // Array of all the words
        int wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
    char computerGuess[ 6];  // Allocate space for the computer guess
    
    printf("Trying to find secret word: \n");
    // Display secret word with a space between letters, to match the guess words below.
    printf("       ");
    for( int i=0; i<WORD_LENGTH; i++) { //Loops through the word to space it out
        printf("%c ", secretWord[ i]);
    }
    printf("\n");
    printf("\n");
    
    // Loop until the word is found
    int guessNumber = 1;
    char guessWord[] = "sores"; //First word we compare with
    int startingPoint = 0; 
    int endingPoint = wordCount - 1; //End of list Number
    int foundWord = 0;

    while (startingPoint <= endingPoint) { //Loops until the start is greater than the end, using Binary Search
        int mid = (startingPoint + endingPoint) / 2;
        strcpy(guessWord, allWords[mid].word); //Changes the original guessWord
        if (strcmp(guessWord, secretWord) == 0) { //When the word is found
            compareWords(secretWord, guessWord, guessNumber);//Compares Words
            foundWord++;
            break;
        }
        else if(strcmp(guessWord, secretWord) > 0){ //When the guessWord is greater than the secretWord
            endingPoint = mid - 1;
            strcpy(guessWord, allWords[mid].word); //Changes the guesWord
            compareWords(secretWord, guessWord, guessNumber); //Compares Words
            guessNumber++;
        }
        else {
            if(strcmp(guessWord, secretWord) < 0){ //When the guessWord is less than the secretWord
                startingPoint = mid + 1;
                strcpy(guessWord, allWords[mid].word); //Changes the guessWord
                compareWords(secretWord, guessWord, guessNumber); //Compares Words
                guessNumber++;
            }
        }
    }   

}


// -----------------------------------------------------------------------------------------
int main() {
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    // Declare space for all the words, of a maximum known size.
    wordCountStruct allWords[ MAX_NUMBER_OF_WORDS];
    // Start out the wordCount to be the full number of words.  This will decrease as
    //    play progresses each time through the game.
    int wordCount = 0;
    // The secret word that the computer will try to find, plus the return character from fgets.
    char secretWord[ WORD_LENGTH + 1];
    char userInput[ 81];                // Used for menu input of secret word

    // Read in words from file, update wordCount and display information
    readWordsFromFile( wordsFileName, allWords, &wordCount);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);

    // Run the word-guessing game three times
    for( int i=0; i<3; i++) {
        // Reset secret Word
        strcpy( secretWord, "");
        // Display prompt
        printf("-----------------------------------------------------------\n");
        printf("\n");
        printf("Enter a secret word or just r to choose one at random: ");
        scanf(" %s", userInput);
        // Eliminate the return character at end or userInput if it is there
        int length = (int) strlen( userInput);
        if( userInput[ length] == '\n') {
            userInput[ length] = '\0';
        }

        strcpy( secretWord, userInput);   // Store the secret word from user input
                
        // If input was 'r' then choose a word at random.
        if( strlen( secretWord) <= 1) {
            // Randomly select a secret word to be guessed.
            int randomIndex = rand() % wordCount;
            strcpy( secretWord, allWords[ randomIndex].word);
        }
        
        // Run the game once with the current secret word
        findSecretWord( allWords, wordCount, secretWord); 
        printf("Got it!\n");
    }

    printf("Done\n");
    printf("\n");
    return 0;
}
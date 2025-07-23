#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CLEAR() printf("\033[H\033[J")
#define MAX_WORD_LEN 6
#define MAX_WRONG_GUESSES 12
#define MAX_GUESSES 26

bool display_word_complete = false;
char secret_word[] = "apples";
char display_word[MAX_WORD_LEN +1];
char guessed_letter;
char guessed_letters_string[MAX_GUESSES];
int wrong_guess_count = 0;
int guessed_count = 0;


void init_game(void)
{
   for(int i = 0; i < strlen(secret_word); i++){
      display_word[i] = '_';
   }

   display_word[strlen(secret_word)] = '\0';
   guessed_letters_string[0] = '\0';
}

int main(void)
{
   

   init_game();

   while(!display_word_complete && wrong_guess_count < MAX_WRONG_GUESSES){
      
      //Show game state
      printf("\nYour Word: %s\n", display_word);
      printf("Wrong guesses left: %d\n", MAX_WRONG_GUESSES - wrong_guess_count);
      printf("Guessed letters: ");
      for(int i = 0; i < guessed_count; i++){
         printf("%c", guessed_letters_string[i]);
      }
      printf("\n");

      //PROMPT THE USER
      printf("Enter a letter: ");
      scanf(" %c", &guessed_letter);

      //check if already guessed
      bool already_guessed = false;
      for(int i = 0; i < guessed_count; i++){
         if(guessed_letters_string[i] == guessed_letter){
            already_guessed = true;
            break;
         }
      }

      if(already_guessed){
         printf("\n\nYou already guessed '%c'. Try a different letter.\n", guessed_letter);
         continue;
      }

      //Add to guessed_letters
      guessed_letters_string[guessed_count++] = guessed_letter;

      //check if guessed letter is in the word
      bool found = false;
      for(int i = 0; i < strlen(secret_word); i++){
         CLEAR();
         if(secret_word[i] == guessed_letter){
            display_word[i] = guessed_letter; 
            found = true;
         }
      }

      if(!found){
         wrong_guess_count++;
         printf("Wrong guess!\n");
      }

      
      //check if the word is fully guessed
      if(strcmp(secret_word, display_word) == 0){
         display_word_complete = true;
      }
      
   }

   //gameover
   if(display_word_complete){
      printf("\n\nYou guessed it right! The word was: %s\n\n", secret_word);
   }else{
      printf("\n\nOut of guesses! The word was: %s\n\n", secret_word);
   }
   return 0;
}

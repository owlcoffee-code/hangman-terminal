#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CLEAR() printf("\033[H\033[J")
#define MAX_WORD_LEN 6
#define MAX_WRONG_GUESSES 6
#define MAX_GUESSES 26

#define MAX_FRAMES 10
#define MAX_LINES 10
#define MAX_LINES_LEN 100

bool display_word_complete = false;
char secret_word[] = "i hate apples";
char display_word[MAX_WORD_LEN +1];
char guessed_letter;
char guessed_letters_string[MAX_GUESSES];
int wrong_guess_count = 0;
int guessed_count = 0;
char frames[MAX_FRAMES][MAX_LINES][MAX_LINES_LEN];


void print_frame(int frame_num);
int load_frames(const char *filename);
void init_game(void);
void main_loop(void);
void game_over(void);
void print_wrong_letter(void);

int print_title(void);
int main(void)
{
 
   int total_frames = load_frames("charset.txt");
   if(total_frames < 1 ){
      fprintf(stderr,"Failed to load any frames.\n");
      return 1;
   }

   init_game();
   main_loop();

   //gameover
   game_over();
   return 0;
}



void init_game(void)
{
   for(int i = 0; i < strlen(secret_word); i++){
      if(secret_word[i] == ' '){
         display_word[i] = ' ';
      }else {
         display_word[i] = '-';
      }
   }

   display_word[strlen(secret_word)] = '\0';
   guessed_letters_string[0] = '\0';
}

void main_loop(void)
{
   print_title();
   print_frame(wrong_guess_count);
   while(!display_word_complete && wrong_guess_count < MAX_WRONG_GUESSES){

      //Show game state
      printf("\nYour Word: %s\n", display_word);
      printf("Wrong guesses left: %d\n", MAX_WRONG_GUESSES - wrong_guess_count);

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
      CLEAR();
      print_title();
      print_frame(wrong_guess_count);
      print_wrong_letter();
   } 

}

void game_over(void)
{
   if(display_word_complete){
      printf("\n\nYou guessed it right! The word was: %s\n\n", secret_word);
   }else{
      printf("\n\nOut of guesses! The word was: %s\n\n", secret_word);
   }

}

int load_frames( const char *filename )
{
   FILE *file = fopen(filename, "rb");

   if(!file){
      perror("Error in loading frames: Cannot open file");
      return -1;
   }

   char line[MAX_LINES_LEN];
   int frame = -1;
   int line_num = 0;

   while(fgets(line, sizeof(line), file)){
      if(strncmp(line, "===FRAME===", 11) == 0){
         frame++;
         line_num = 0;
      }else if( frame >= 0 && line_num < MAX_LINES ){
         strncpy(frames[frame][line_num++], line, MAX_LINES_LEN);
      }
   }
   fclose(file);
   return frame + 1; //number of frames loaded
}



void print_frame(int frame_num)
{
   for(int i = 0; i < MAX_LINES; i++){
      if(strlen(frames[frame_num][i]) > 0){
         printf("   %s", frames[frame_num][i]);
      }
   }

}

void print_wrong_letter(void)
{
   for(int i = 0; i < guessed_count; i++){
      printf("%2c", guessed_letters_string[i]);
   }
   printf("\n");
}


int print_title(void)
{
   FILE *title = fopen("title.txt", "rb");
   if(!title){
      perror("couldn't find title document\n");
      return -1;
   }

   char main_title[MAX_LINES_LEN];

   while(fgets(main_title, sizeof(main_title), title)){
      printf("%s", main_title);
   }
   printf("\n\n"); 
   fclose(title);
   return 0;
}

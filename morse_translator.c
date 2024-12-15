#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
/* CopyLeft 2024 Amirreza Peter Firoozi */

#define ALLOCATION_ERROR 4
#define TOKEN_BUFFER_MAX_LEN 9
#define MORSE_DIGIT_LEN 5
#define MORSE_WORD_SEPARATOR '/'
#define TRANSLATED_WORD_SEPARATOR ' '

#define MORSE_UNRECOGNIZED_TOKEN -32

bool _OPTION_LOWERCASE = true;
bool _OPTION_ADDSPACE = false; 
//#define TOKEN_SEPARATOR ' '
const char TOKEN_SEPARATOR=' ';
const char* MORSE_TABLE_ALPHABET[] = {".-", "-...", "-.-.", "-..",
                             ".", "..-.", "--.", "....", 
                             "..", ".---", "-.-", ".-..",
                             "--", "-.", "---", ".--.",
                             "--.-", ".-.", "...", "-",
                             "..-", "...-", ".--", "-..-",
                             "-.--", "--.."};
                             
const char* MORSE_TABLE_DIGITS[] = {"-----", ".----", "..---", "...--",
                                    "....-", ".....", "-....", "--...",
                                    "---..", "----." 
                                    };

const char* MORSE_TABLE_PUNCTUATION[] = {".-.-.-", "--..--", "..--..", ".---.",
                                         "-.-.--", "-..-.", "-.--.", "-.--.-",
                                         ".-...", "---...", "-.-.-.", "-...-",
                                         ".-.-.", "-....-", "..--.-", ".-..-.",
                                         "...-..-", ".--.-."
                                        };
const char MORSE_TABLE_PUNCTUATION_LOOKUP[] = ".,?'!/()&:;=+-_\"$@";

char* read_token(char*);
char translate_token(char*);
int main (int argc, char* argv[])
{
    /*
     * get the input text;
     * separate it into words
     * words are separated by spaces. so ignore any spaces and store everything before that as the token to translate
     * translate the token: how?
     *  strcmp it against a set of predetermined sequences
     *  I can probably use a hash table?
     *  can I somehow create a named array in c?
     *
     * translate the token
     * empty the token
     * get the next token 
     * and do this while there is input
     * ignore any tokens that are unknown or just put a question mark \? in their stead
     *
     * 
     * Caveats: I'm using linear search for ease of implementation
     *          The magic number 7 could be macroed, researched further
     * TODO: Change to more performant /efficient search algorithms 
     */
    char* tok = (char*) malloc(TOKEN_BUFFER_MAX_LEN * sizeof(char));
    if (tok == NULL)
        exit(ALLOCATION_ERROR);

    // while there is more word remaining?
    while ((read_token(tok)[0] != 0)){
        //read tokens
        //printf("token: %s translated_token: %c\n", tok, translate_token(tok));
        char translated_token = translate_token(tok);
        if (translated_token == MORSE_UNRECOGNIZED_TOKEN)
            printf("[\\?\\]");
        else
            putchar(translated_token);

        if (_OPTION_ADDSPACE)
            putchar(' ');
    }

    free(tok);
    putchar('\n');
    return EXIT_SUCCESS;
}

char* read_token(char* current_token)
{

    // Should the variable memory be allocated outside or inside the function?
    char *ctp = current_token; // current_token_pointer
    char c; 
    unsigned int counter = TOKEN_BUFFER_MAX_LEN; 
    // remove preceding spaces
    while ((c=getc(stdin)) == ' ');
    if (c != ' ') ungetc(c, stdin);

    // scan a word from input
    while (((c=getc(stdin)) != EOF) && (c != ' ') && (c != '\n') && (--counter > 0)){
        if ((c == '.') || (c == '-')){
            *(ctp++) = c;
        }else if (c == MORSE_WORD_SEPARATOR){
            *(ctp++) = c;
            break;
        }else {
            break;
        }
    }
    *ctp = '\0';
    return current_token;
}

char translate_token(char* token)
{
    // translate '/' as ' '
    if (*token == MORSE_WORD_SEPARATOR) return TRANSLATED_WORD_SEPARATOR;

    // Should I assume data arriving here is sanitized?
    // if strlen == 5 > definitely a number! lookup for number replacement
    // if < 5 > lookup for alphabet
    unsigned int len = strlen(token);
    unsigned int i = 0;
    char translated_token = MORSE_UNRECOGNIZED_TOKEN;
    bool found_match = false;

    if (len < MORSE_DIGIT_LEN){
        for (; i < 26; ++i){
            if (strncmp(token, MORSE_TABLE_ALPHABET[i], TOKEN_BUFFER_MAX_LEN) == 0){
                translated_token = 'A' + i;
                break;
            }
        }

    }else if (len == MORSE_DIGIT_LEN){
        for (; i < 10; ++i){
            if (strncmp(token, MORSE_TABLE_DIGITS[i], MORSE_DIGIT_LEN) == 0){
                translated_token = '0' + i; 
                found_match = true;
                break;
            }
        }
    }else{
        if (found_match == false){
            for (; i < 18; ++i){
                if (strncmp(token, MORSE_TABLE_PUNCTUATION[i], TOKEN_BUFFER_MAX_LEN) == 0){
                    translated_token = MORSE_TABLE_PUNCTUATION_LOOKUP[i];
                    break;
                }
            }
        }
    }
    if (_OPTION_LOWERCASE == true && (translated_token != MORSE_UNRECOGNIZED_TOKEN)) translated_token = tolower(translated_token);
    return translated_token;
}

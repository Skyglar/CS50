#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    int key;
    if(argc != 2) {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    else {
        
        int size = strlen(argv[1]);
        char c[size];
        
        strcpy(c, argv[1]);
   
        for (int i = 0; i < size; i++) {
            if(isdigit(c[i])){
                printf("Usage: ./caesar keyword\n");
                return 1;
            }
        }
    }
    
    string plain_text = get_string("plaintext: ");
    
    int size = strlen(plain_text);
    int key_length = strlen(argv[1]);
    int j = 0;
    
    
    for(int i = 0; i < size; i++) {
        
        if(plain_text[i] >= 65 && plain_text[i] <= 91) {
            if (j > key_length - 1) {
                j = 0;
                key = shift(argv[1][j]);
                j++;
            }
            else {
                key = shift(argv[1][j]);
                j++;
            }
            plain_text[i] = 65 + (((plain_text[i] + key) - 65) % 26);
        }
        else if(plain_text[i] >= 97 && plain_text[i] <= 123) {
            if (j > key_length - 1) {
                j = 0;
                key = shift(argv[1][j]);
                j++;
            }
            else {
                key = shift(argv[1][j]);
                j++;
            }
            plain_text[i] = 97 + (((plain_text[i] + key) - 97) % 26);
        }
        else {
            plain_text[i] = plain_text[i];
        }
        
    }
    
    printf("ciphertext: %s\n", plain_text);
    
}

int shift(char c) {
    
    if(c >= 65 && c <= 91) {
        return c - 65;
    }
    else if (c >= 97 && c <= 122) {
        return c - 97;
    }
   return 0;
}
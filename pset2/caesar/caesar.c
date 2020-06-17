#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    int key;
    if(argc != 2) {
        printf("Usage: ./caesar key\n");
        return 1;
    }  else {
        int size = strlen(argv[1]);
        char c[size];

        strcpy(c, argv[1]);

        for (int i = 0; i < size; i++) {
            if(!isdigit(c[i])){
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        key = atoi(argv[1]);
    }
    string plain_text = get_string("plaintext: ");

    int size = strlen(plain_text);

    for(int i = 0; i < size; i++) {
        if(plain_text[i] >= 65 && plain_text[i] <= 91) {
            plain_text[i] = 65 + (((plain_text[i] + key) - 65) % 26);
        }
        else if(plain_text[i] >= 97 && plain_text[i] <= 123) {
            plain_text[i] = 97 + (((plain_text[i] + key) - 97) % 26);
        }
        else {
            plain_text[i] = plain_text[i];
        }

    }

    printf("ciphertext: %s\n", plain_text);
}
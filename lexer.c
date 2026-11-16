#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 256

int is_operator(char ch) {
    const char *operators = "+-*/%={}[]()><";
    return strchr(operators, ch) != NULL;
}

int main(void) {
    const char *filename = "text1.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open '%s'. Please ensure the file exists.\n", filename);
        return EXIT_FAILURE;
    }

    int ch;
    char buffer[MAX_BUFFER_SIZE];
    int len = 0;

    while ((ch = fgetc(fp)) != EOF) {
        // Handle identifiers: starts with a letter or underscore
        if (isalpha(ch) || ch == '_') {
            buffer[len++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                if (len < MAX_BUFFER_SIZE - 1) {
                    buffer[len++] = (char)ch;
                }
            }
            buffer[len] = '\0';
            printf("%s is identifier\n", buffer);
            len = 0;

            if (ch != EOF) {
                ungetc(ch, fp);
            }
            continue;
        }

        // Handle numbers: sequence of digits
        if (isdigit(ch)) {
            buffer[len++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
                if (len < MAX_BUFFER_SIZE - 1) {
                    buffer[len++] = (char)ch;
                }
            }
            buffer[len] = '\0';
            printf("%s is number\n", buffer);
            len = 0;

            if (ch != EOF) {
                ungetc(ch, fp);
            }
            continue;
        }

        // Handle operators
        if (is_operator((char)ch)) {
            printf("%c is operator\n", (char)ch);
            continue;
        }

        // Whitespace and other characters are ignored
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

/* Experiment 1: Lexical Analyser in C
 * Reads input.c and prints every token with its class:
 * keyword / identifier / number / string literal / char literal /
 * operator / delimiter / invalid. Ignores spaces and comments.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char keywords[][20] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if", "int",
    "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile",
    "while"
};

int isKeyword(char str[]) {
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char ch) {
    char operators[] = "+-*/%=<>!&|^";
    for (int i = 0; operators[i] != '\0'; i++) {
        if (ch == operators[i])
            return 1;
    }
    return 0;
}

int isDelimiter(char ch) {
    char delimiters[] = "();{}[],.:";
    for (int i = 0; delimiters[i] != '\0'; i++) {
        if (ch == delimiters[i])
            return 1;
    }
    return 0;
}

int main() {
    FILE *fp;
    char ch, buffer[100];
    int i = 0;

    fp = fopen("input.c", "r");
    if (fp == NULL) {
        printf("Cannot open input file.\n");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch))
            continue;

        if (ch == '/') {
            char next = fgetc(fp);
            if (next == '/') {                      /* line comment */
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                continue;
            } else if (next == '*') {               /* block comment */
                char prev;
                while ((ch = fgetc(fp)) != EOF) {
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                continue;
            } else {
                ungetc(next, fp);                   /* plain division */
            }
        }

        if (isalpha(ch) || ch == '_') {             /* keyword or identifier */
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            if (ch != EOF)
                ungetc(ch, fp);

            if (isKeyword(buffer))
                printf("%-20s : Keyword\n", buffer);
            else
                printf("%-20s : Identifier\n", buffer);
        }
        else if (isdigit(ch)) {                     /* number */
            i = 0;
            int dot = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && (isdigit(ch) || ch == '.')) {
                if (ch == '.')
                    dot++;
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            if (ch != EOF)
                ungetc(ch, fp);

            if (dot <= 1)
                printf("%-20s : Number\n", buffer);
            else
                printf("%-20s : Invalid Number\n", buffer);
        }
        else if (ch == '"') {                       /* string literal */
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != '"' && ch != EOF)
                buffer[i++] = ch;
            buffer[i++] = '"';
            buffer[i] = '\0';
            printf("%-20s : String Literal\n", buffer);
        }
        else if (ch == '\'') {                      /* character literal */
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != '\'' && ch != EOF)
                buffer[i++] = ch;
            buffer[i++] = '\'';
            buffer[i] = '\0';
            printf("%-20s : Character Literal\n", buffer);
        }
        else if (isOperator(ch)) {
            printf("%-20c : Operator\n", ch);
        }
        else if (isDelimiter(ch)) {
            printf("%-20c : Delimiter\n", ch);
        }
        else {
            printf("%-20c : Invalid Token\n", ch);
        }
    }

    fclose(fp);
    return 0;
}

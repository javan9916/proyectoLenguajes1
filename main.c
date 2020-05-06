#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define SIZE 128

typedef enum { CEL, RES, SPE } Type;

char* resRegex = "\\(?\\+?(506)?\\)?[ -]?(2[0-9]{1})[ -]?([0-9]{2})[ -]?([0-9]{2})[ -]?([0-9]{2})";
char* celRegex = "\\(?\\+?(506)?\\)?[- ]?([1|3-9]{1}[0-9]{1})[- ]?([0-9]{2})[- ]?([0-9]{2})[- ]?([0-9]{2})$";
char* speRegex = "(^[0-9]{3}$)|(^[0-9]{4}$)";

struct nodeList {
    char* phone;
    Type type;
    struct nodeList* next;
} *head;

void newPhone(char* p, Type type) {
    struct nodeList* nl = NULL;
    nl = (struct nodeList*) malloc(sizeof(struct nodeList));
    nl->phone = (char *) malloc(1);
    strcpy(nl->phone, p);
    nl->type = type;
    nl->next = NULL;

    if (head == NULL)
        head = nl;
    else {
        nl->next = head;
        head = nl;
    }
}


int match(const char* string, char* pattern) {
    int status;
    regex_t re;

    if (string != NULL) {
        if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
            return(0);      /* report error */
        }
        status = regexec(&re, string, (size_t) 0, NULL, 0);
        regfree(&re);
        if (status != 0) {
            return (0);      /* report error */
        }
        return(1);
    } else {
        return (0);
    }
}

int readText() {
    FILE *fp;
    char str[SIZE];
    char* filename = "/home/javier/CLionProjects/proyectoLenguajes1/test.txt";
    size_t len = sizeof(str);
    char* line = (char*) malloc(len);
    char* token;

    fp = fopen(filename, "r");

    if(line == NULL) {
        perror("Unable to allocate memory for the line buffer.");
        return 0;
    }

    line[0] = '\0';

    while (fgets(str, sizeof(str), fp) != NULL) {
        size_t len_used = strlen(line);
        size_t str_used = strlen(str);

        if(len - len_used < str_used) {
            len *= 2;
            if((line = realloc(line, len)) == NULL) {
                perror("Unable to reallocate memory for the line buffer.");
                free(line);
                return 0;
            }
        }

        strncpy(line + len_used, str, len - len_used);
        len_used += str_used;

        token = strtok(line, "\t");
        if (match(token, resRegex) == 1)
            newPhone(token, RES);
        else if (match(token, speRegex) == 1)
            newPhone(token, SPE);
        else if (match(token, celRegex) == 1)
            newPhone(token, CEL);

        if(line[len_used - 1] == '\n') {
            line[0] = '\0';
        }

        while( token != NULL ) {
            token = strtok(NULL, "\t");
            if (match(token, resRegex) == 1)
                newPhone(token, RES);
            else if (match(token, speRegex) == 1)
                newPhone(token, SPE);
            else if (match(token, celRegex) == 1)
                newPhone(token, CEL);
        }
    }

    fclose(fp);
    free(line);
}

void printPhones() {
    struct nodeList* current = head;
    printf("****** Numeros identificados ******\n\n");
    if (current == NULL)
        printf("Lista vacia");
    else {
        while (current != NULL) {
            if (current->type == 0) {
                printf("Telefono: %s\n", current->phone);
                printf("Tipo: Celular\n\n ");
            } else if (current->type == 1) {
                printf("Telefono: %s\n", current->phone);
                printf("Tipo: Residencial\n\n ");
            } else if (current->type == 2) {
                printf("Telefono: %s\n", current->phone);
                printf("Tipo: Especial\n\n ");
            }
            current = current->next;
        }
    }
}

int main() {
    readText();

    printPhones();

    return 0;
}

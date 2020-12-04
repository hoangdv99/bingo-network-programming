#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool checkString(char *string) {
    if (strlen(string) == 0) return false;
    for (int i = 0; i < strlen(string); i++){
        if (!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') || (string[i] >= '0' && string[i] <='9'))) return false;
    }
    return true;
}
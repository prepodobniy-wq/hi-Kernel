// src/string.c
// basic functions for working with strings

#include "string.h"

// writes the num to buffer as a string
// buffer must have space for at least 11 characters
void uint_to_string(unsigned int number, char *buffer)
{
    char digits[10];
    int count = 0;
    int i;

    if (number == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // extract the digits starting from the end
    // they come out in reverse order
    while (number > 0) {
        digits[count] = (char)('0' + number % 10);
        number = number / 10;
        count++;
    }

    // reversing digits in buffer
    for (i = 0; i < count; i++) {
        buffer[i] = digits[count - 1 - i];
    }
    buffer[count] = '\0';
}


//* ------------------------------------------------------------------------------------------------------------------------------
//* The following code came from https://github.com/nanobyte-dev/nanobyte_os/releases/tag/Part6                                  |                    
//* It has been modified by me to work with my kernel and to fit my needs.                                                       |
//* The non-modified parts of the code is licensed under the Unlicense, which is a public domain license.                        |
//* The modified parts of the code is licensed under the MIT licence and / or in accordance to the root licence of the project.  |
//* ------------------------------------------------------------------------------------------------------------------------------

//TODO: make this less bad

#include <stdio.h>

static void putchr(const char c) {
    put_char((char) c);
}

static void putstr(const char* str) {
    while(*str) {
        put_char(*str);
        str++;
    }
}

static int _strlen(char* string) {
    int i = 0;
    while (string[i] != '\0') i++;
    return i;
}

static void reverse(char string[]) {
    int c, i, j;
    for (i = 0, j = _strlen(string)-1; i < j; i++, j--) {
        c = string[i];
        string[i] = string[j];
        string[j] = c;
    }
}

static void int_to_string(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

#define PRINTF_STATE_NORMAL         0
#define PRINTF_STATE_LENGTH         1
#define PRINTF_STATE_LENGTH_SHORT   2
#define PRINTF_STATE_LENGTH_LONG    3
#define PRINTF_STATE_SPEC           4

#define PRINTF_LENGTH_DEFAULT       0
#define PRINTF_LENGTH_SHORT_SHORT   1
#define PRINTF_LENGTH_SHORT         2
#define PRINTF_LENGTH_LONG          3
#define PRINTF_LENGTH_LONG_LONG     4


void printf(const char* format, ...) {
    int* argp = (int*) &format;
    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    bool sign = false;

    argp++;

    while (*format) {
        switch (state) {
            case PRINTF_STATE_NORMAL:
                switch (*format) {
                    case '%':   state = PRINTF_STATE_LENGTH; break;
                    default:    putchr(*format); break;
                }
                break;

            case PRINTF_STATE_LENGTH:
                switch (*format) {
                    case 'h':   length = PRINTF_LENGTH_SHORT;
                                state = PRINTF_STATE_LENGTH_SHORT; break;

                    case 'l':   length = PRINTF_LENGTH_LONG;
                                state = PRINTF_STATE_LENGTH_LONG; break;

                    default:    goto PRINTF_STATE_SPEC_;
                }
                break;

            case PRINTF_STATE_LENGTH_SHORT:
                if (*format == 'h') {
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPEC;
                }
                else goto PRINTF_STATE_SPEC_;
                break;

            case PRINTF_STATE_LENGTH_LONG:
                if (*format == 'l') {
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPEC;
                }
                else goto PRINTF_STATE_SPEC_;
                break;

            case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch (*format) {
                    case 'c':   putchr((char)*argp);
                                argp++;
                                break;

                    case 's':   if (length == PRINTF_LENGTH_LONG || length == PRINTF_LENGTH_LONG_LONG) {
                                    putstr(*(char**)argp);
                                    argp += 2;
                                }
                                else {
                                    putstr(*(char**)argp);
                                    argp++;
                                }
                                break;

                    case '%':   putchr('%');
                                break;

                    case 'd':
                    case 'i':
                                argp = printf_number(argp, length, sign);
                                break;

                    case 'u':
                                argp = printf_number(argp, length, sign);
                                break;

                    case 'X':
                    case 'x':   char buffer[32];
                                int_to_string((int)argp, buffer);
                                putstr(buffer);
                                break;
                    case 'p':
                                argp = printf_number(argp, length, sign);
                                break;

                    case 'o':
                                argp = printf_number(argp, length, sign);
                                break;

                    // ignore invalid spec
                    default:    break;
                }

                // reset state
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                sign = false;
                break;
        }

        format++;
    }
}

const char g_HexChars[] = "0123456789abcdef";

static int* printf_number(int* argp, int length, int sign) {
    char buffer[32];
    unsigned long long number = NULL;

    // process length
    switch (length) {
        case PRINTF_LENGTH_SHORT_SHORT:
        case PRINTF_LENGTH_SHORT:
        case PRINTF_LENGTH_DEFAULT:
            if (sign) {
                int n = *argp;
                if (n < 0)
                {
                    n = -n;
                }
                number = (unsigned long long)n;
            }
            else {
                number = *(unsigned int*)argp;
            }
            argp++;
            break;

        case PRINTF_LENGTH_LONG:
            if (sign) {
                long int n = *(long int*)argp;
                if (n < 0)
                {
                    n = -n;
                }
                number = (unsigned long long)n;
            }
            else {
                number = *(unsigned long int*)argp;
            }
            argp += 2;
            break;

        case PRINTF_LENGTH_LONG_LONG:
            if (sign) {
                long long int n = *(long long int*)argp;
                if (n < 0)
                {
                    n = -n;
                }
                number = (unsigned long long)n;
            }
            else {
                number = *(unsigned long long int*)argp;
            }
            argp += 4;
            break;
    }

    // convert number to ASCII
    int_to_string(number, buffer);

    putstr(buffer);

    return argp;
}
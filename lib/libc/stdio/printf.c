#include <stdio.h>

void print_string(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        put_char(string[i]);
    }
}

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

void printf(const char* format, ...) {

    va_list args;
    va_start(args, format);

    // loop through format string
    for (int i = 0; i < strlen(format); i++) {

        // if we find a %, we need to parse the next char
        if (format[i] == '%') {

            // get the next char
            i++;

            // We have 33 chars just because we can
            char buffer[33];

            // check what it is
            switch (format[i]) {
                // if its a %, print a %
                case '%':
                    put_char('%');
                    break;
                // if its a c, print a char
                case 'c':
                    put_char(va_arg(args, int));
                    break;
                // if its a s, print a string
                case 's':
                    print_string(va_arg(args, char*));
                    break;
                // if its a d, print an int
                case 'd':
                    print_string(itoa(va_arg(args, int), buffer, 10));
                    break;
                // if its a x, print a hex
                case 'p':
                case 'x':
                    print_string(itoa(va_arg(args, int), buffer, 16));
                    break;
                // if its a b, print a binary
                case 'b':
                    print_string(itoa(va_arg(args, int), buffer, 2));
                    break;
                // if its a f, print a float
                case 'f':
                    print_string("TODO: make this");
                    break;
                // if its a u, print an unsigned int
                case 'u':
                    print_string(itoa(va_arg(args, unsigned int), buffer, 10));
                    break;
                // if its a o, print an octal
                case 'o':
                    print_string(itoa(va_arg(args, int), buffer, 8));
                    break;

            }

        } else {

            // if its not a %, print the char
            put_char(format[i]);

        }

    }

}

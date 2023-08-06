#include "system/gui/fonts.h"

// Taken from https://github.com/mkilgore/protura/blob/master/src/drivers/video/internal.h
font_t blink_default;
blink_default.data = {
	[33] = { 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //    33 '!'
           { 0x00, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    34 '"'
           { 0x00, 0x00, 0x24, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00 }, //    35 '#'
           { 0x00, 0x10, 0x10, 0x7C, 0x92, 0x90, 0x90, 0x7C, 0x12, 0x12, 0x92, 0x7C, 0x10, 0x10, 0x00, 0x00 }, //    36 '$'
           { 0x00, 0x00, 0x64, 0x94, 0x68, 0x08, 0x10, 0x10, 0x20, 0x2C, 0x52, 0x4C, 0x00, 0x00, 0x00, 0x00 }, //    37 '%'
           { 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x30, 0x4A, 0x44, 0x44, 0x44, 0x3A, 0x00, 0x00, 0x00, 0x00 }, //    38 '&'
           { 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    39 '''
           { 0x00, 0x00, 0x08, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00 }, //    40 '('
           { 0x00, 0x00, 0x20, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00 }, //    41 ')'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x18, 0x7E, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    42 '*'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    43 '+'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00 }, //    44 ','
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    45 '-'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //    46 '.'
           { 0x00, 0x00, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00 }, //    47 '/'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x4A, 0x52, 0x62, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    48 '0'
           { 0x00, 0x00, 0x08, 0x18, 0x28, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00 }, //    49 '1'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00 }, //    50 '2'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x1C, 0x02, 0x02, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    51 '3'
           { 0x00, 0x00, 0x02, 0x06, 0x0A, 0x12, 0x22, 0x42, 0x7E, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00 }, //    52 '4'
           { 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x02, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    53 '5'
           { 0x00, 0x00, 0x1C, 0x20, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    54 '6'
           { 0x00, 0x00, 0x7E, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //    55 '7'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    56 '8'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x04, 0x38, 0x00, 0x00, 0x00, 0x00 }, //    57 '9'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //    58 ':'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00 }, //    59 ';'
           { 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00 }, //    60 '<'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    61 '='
           { 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00 }, //    62 '>'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x04, 0x08, 0x08, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00 }, //    63 '?'
           { 0x00, 0x00, 0x7C, 0x82, 0x9E, 0xA2, 0xA2, 0xA2, 0xA6, 0x9A, 0x80, 0x7E, 0x00, 0x00, 0x00, 0x00 }, //    64 '@'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //    65 'A'
           { 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x00, 0x00, 0x00, 0x00 }, //    66 'B'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x40, 0x40, 0x40, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    67 'C'
           { 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x00, 0x00 }, //    68 'D'
           { 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00 }, //    69 'E'
           { 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00 }, //    70 'F'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x40, 0x4E, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    71 'G'
           { 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //    72 'H'
           { 0x00, 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00 }, //    73 'I'
           { 0x00, 0x00, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00 }, //    74 'J'
           { 0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00, 0x00, 0x00 }, //    75 'K'
           { 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00 }, //    76 'L'
           { 0x00, 0x00, 0x82, 0xC6, 0xAA, 0x92, 0x92, 0x82, 0x82, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00 }, //    77 'M'
           { 0x00, 0x00, 0x42, 0x42, 0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //    78 'N'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    79 'O'
           { 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00 }, //    80 'P'
           { 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x4A, 0x3C, 0x02, 0x00, 0x00, 0x00 }, //    81 'Q'
           { 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00, 0x00, 0x00 }, //    82 'R'
           { 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x3C, 0x02, 0x02, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    83 'S'
           { 0x00, 0x00, 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //    84 'T'
           { 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    85 'U'
           { 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00 }, //    86 'V'
           { 0x00, 0x00, 0x82, 0x82, 0x82, 0x82, 0x82, 0x92, 0x92, 0xAA, 0xC6, 0x82, 0x00, 0x00, 0x00, 0x00 }, //    87 'W'
           { 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x24, 0x24, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //    88 'X'
           { 0x00, 0x00, 0x82, 0x82, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //    89 'Y'
           { 0x00, 0x00, 0x7E, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00 }, //    90 'Z'
           { 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00 }, //    91 '['
           { 0x00, 0x00, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00 }, //    92 '\'
           { 0x00, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x00, 0x00 }, //    93 ']'
           { 0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    94 '^'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00 }, //    95 '_'
           { 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //    96 '`'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x02, 0x3E, 0x42, 0x42, 0x42, 0x3E, 0x00, 0x00, 0x00, 0x00 }, //    97 'a'
           { 0x00, 0x00, 0x40, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x00, 0x00, 0x00, 0x00 }, //    98 'b'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x40, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //    99 'c'
           { 0x00, 0x00, 0x02, 0x02, 0x02, 0x3E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x00, 0x00, 0x00, 0x00 }, //   100 'd'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x40, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //   101 'e'
           { 0x00, 0x00, 0x0E, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //   102 'f'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x3C, 0x00 }, //   103 'g'
           { 0x00, 0x00, 0x40, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //   104 'h'
           { 0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00 }, //   105 'i'
           { 0x00, 0x00, 0x04, 0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38, 0x00 }, //   106 'j'
           { 0x00, 0x00, 0x40, 0x40, 0x40, 0x42, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x00, 0x00, 0x00, 0x00 }, //   107 'k'
           { 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00 }, //   108 'l'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x00, 0x00, 0x00, 0x00 }, //   109 'm'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //   110 'n'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00 }, //   111 'o'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x00 }, //   112 'p'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x00 }, //   113 'q'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x5E, 0x60, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00 }, //   114 'r'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x40, 0x40, 0x3C, 0x02, 0x02, 0x7C, 0x00, 0x00, 0x00, 0x00 }, //   115 's'
           { 0x00, 0x00, 0x10, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00 }, //   116 't'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x00, 0x00, 0x00, 0x00 }, //   117 'u'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00 }, //   118 'v'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x82, 0x92, 0x92, 0x92, 0x92, 0x7C, 0x00, 0x00, 0x00, 0x00 }, //   119 'w'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00 }, //   120 'x'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x3C, 0x00 }, //   121 'y'
           { 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00 }, //   122 'z'
           { 0x00, 0x00, 0x0C, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x00, 0x00, 0x00, 0x00 }, //   123 '{'
           { 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 }, //   124 '|'
           { 0x00, 0x00, 0x30, 0x08, 0x08, 0x08, 0x04, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00, 0x00, 0x00, 0x00 }, //   125 '}'
           { 0x00, 0x62, 0x92, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //   126 '~'
};
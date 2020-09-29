#pragma once
#include <stdbool.h>

#define DIRECTION_FROMBEGIN -1
#define DIRECTION_FROMEND 1

typedef struct
{
    unsigned char* begin;
    int length;
    unsigned char currentSymbol;
    unsigned char* nextSymbol;

} Line;

unsigned char toLower(unsigned char c);

bool isAlpha(unsigned char c);

void swap(Line** first, Line** second);

unsigned char getSymbol(int length, int index, unsigned char** current, int direction);

int sortLines(const unsigned char* text, int lineCount, unsigned char** lines, int* lineLengths, int maxLineLength, int direction);

unsigned char* readText(const char* filename);

void writeText(const char* filename, unsigned char** lines, int lineCount);

void writeRaw(const char* filename, const unsigned char* text);

int splitText(unsigned char* text, unsigned char*** lines, int** lineLengths, int* maxLineLength);

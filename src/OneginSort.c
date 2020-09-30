#include "OneginSort.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <memory.h>

#define ALPHABET_SIZE 256

#define FIRST_LETTER_LOWER (unsigned char)'à'
#define LAST_LETTER_LOWER (unsigned char)'ÿ'
#define FIRST_LETTER_UPPER (unsigned char)'À'
#define LAST_LETTER_UPPER (unsigned char)'ß'

unsigned char toLower(unsigned char c)
{
    if (FIRST_LETTER_UPPER <= c && c <= LAST_LETTER_UPPER)
        return c - FIRST_LETTER_UPPER + FIRST_LETTER_LOWER;
    return c;
}

bool isAlpha(unsigned char c)
{
    return (FIRST_LETTER_LOWER <= c && c <= LAST_LETTER_LOWER)
        || (FIRST_LETTER_UPPER <= c && c <= LAST_LETTER_UPPER);
}

unsigned char getSymbol(int length, int index, unsigned char** current, int direction)
{
    if (index >= length)
        return '\0';
    while (!isAlpha(**current))
        *current += direction;
    unsigned char res = **current;
    *current += direction;
    return toLower(res);
}

void swap(Line** first, Line** second)
{
    Line* tmp = *first;
    *first = *second;
    *second = tmp;
}

int sortLines(int lineCount, unsigned char** lines, int* lineLengths, int maxLineLength, int direction)
{
    if (lines == NULL || lineLengths == NULL || lineCount == 0)
    {
        errno = EINVAL;
        return 1;
    }
    int symbolCounts[ALPHABET_SIZE + 1];
    Line* lineStructs = (Line*)calloc(lineCount, sizeof(*lineStructs));
    for (int i = 0; i < lineCount; ++i)
    {
        lineStructs[i].begin = lines[i];
        lineStructs[i].length = lineLengths[i];
        if (direction == DIRECTION_FROMBEGIN)
            lineStructs[i].nextSymbol = lines[i + 1] - 1;
        else
            lineStructs[i].nextSymbol = lines[i];
    }
    Line* lineStructsCopy = (Line*)calloc(lineCount, sizeof(*lineStructsCopy));
    for (int iter = maxLineLength - 1; iter >= 0; --iter)
    {
        memset(symbolCounts, 0, sizeof(symbolCounts));
        for (int line = 0; line < lineCount; ++line)
        {
            lineStructs[line].currentSymbol = getSymbol(lineStructs[line].length, iter, &lineStructs[line].nextSymbol, direction);
            ++symbolCounts[lineStructs[line].currentSymbol + 1];
        }
        for (int i = 1; i < sizeof(symbolCounts) / sizeof(*symbolCounts); ++i)
            symbolCounts[i] += symbolCounts[i - 1];

        for (int line = 0; line < lineCount; ++line)
        {
            int newPosition = symbolCounts[lineStructs[line].currentSymbol]++;
            lineStructsCopy[newPosition] = lineStructs[line];
        }
        swap(&lineStructs, &lineStructsCopy);
    }
    if (maxLineLength % 2 == 0)
    {
        for (int i = 0; i < lineCount; ++i)
            lines[i] = lineStructs[i].begin;
    }
    else
    {
        for (int i = 0; i < lineCount; ++i)
            lines[i] = lineStructsCopy[i].begin;
    }
    free(lineStructs);
    free(lineStructsCopy);
    return 0;
}

unsigned char* readText(const char* filename)
{
    FILE* file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*)calloc(size, sizeof(*buffer));
    fread(buffer, sizeof(*buffer), size, file);
    fclose(file);
    return buffer;
}

void writeLines(const char* filename, unsigned char** lines, int lineCount)
{
    FILE* file = fopen(filename, "w");
    for (int i = 0; i < lineCount; ++i)
    {
        size_t length;
        for (length = 0; lines[i][length] != '\n'; ++length);
        fwrite(lines[i], sizeof(unsigned char), length + 1, file);
    }
    fclose(file);
}

void writeText(const char* filename, const unsigned char* text)
{
    FILE* file = fopen(filename, "w");
    fputs(text, file);
    fclose(file);
}

int splitText(unsigned char* text, unsigned char*** lines, int** lineLengths, int* maxLineLength)
{
    if (text == NULL)
    {
        lines = NULL;
        lineLengths = NULL;
        maxLineLength = 0;
        errno = EINVAL;
        return -1;
    }

    int lineCount = 0;
    for (int i = 0; text[i] != '\0'; ++i)
        if (text[i] == '\n')
            ++lineCount;

    if (lineCount == 0)
    {
        lines = NULL;
        lineLengths = NULL;
        maxLineLength = 0;
        errno = EINVAL;
        return -1;
    }

    *lines = (unsigned char**)calloc(lineCount + 1, sizeof(**lines));
    *lineLengths = (int*)calloc(lineCount, sizeof(**lineLengths));
    *maxLineLength = 0;
    int lineLength = 0;
    (*lines)[0] = text;
    int ind = 1;
    for (int i = 0; text[i] != '\0'; ++i)
    {
        if (text[i] == '\n')
        {
            if (lineLength == 0)
            {
                // We should ignore empty lines while sorting, so erase it
                (*lines)[ind - 1] = text + i + 1;
                --lineCount;
            }
            else
            {
                (*lines)[ind] = text + i + 1;
                (*lineLengths)[ind - 1] = lineLength;
                ++ind;
            }
            if (lineLength > *maxLineLength)
                *maxLineLength = lineLength;
            lineLength = 0;
        }
        else if (isAlpha(text[i]))
            ++lineLength;
    }
    return lineCount;
}

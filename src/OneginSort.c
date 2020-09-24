#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#define ALPHABET_SIZE 256

#define FIRST_LETTER_LOWER (unsigned char)'à'
#define LAST_LETTER_LOWER (unsigned char)'ÿ'
#define FIRST_LETTER_UPPER (unsigned char)'À'
#define LAST_LETTER_UPPER (unsigned char)'ß'

bool isAlpha(unsigned char c)
{
    return (FIRST_LETTER_LOWER <= c && c <= LAST_LETTER_LOWER)
        || (FIRST_LETTER_UPPER <= c && c <= LAST_LETTER_UPPER);
}

unsigned char getSymbol(unsigned char* begin, int length, int index, unsigned char** current)
{
    if (index >= length)
        return '\0';
    while (!isAlpha(**current))
        --*current;
    unsigned char res = *((*current)--);
    return res;
}

void swap(unsigned char*** first, unsigned char*** second)
{
    unsigned char** tmp = *first;
    *first = *second;
    *second = tmp;
}

void swapInts(int** first, int**second)
{
    int* tmp = *first;
    *first = *second;
    *second = tmp;
}

void sortFromBeginning(const unsigned char* text, int lineCount, unsigned char** lines, int* lineLengths, int maxLineLength)
{
    int symbolCounts[ALPHABET_SIZE + 1];
    unsigned char** nextSymbols = (unsigned char**)calloc(lineCount, sizeof(*nextSymbols));
    unsigned char** nextSymbolsCopy = (unsigned char**)calloc(lineCount, sizeof(*nextSymbolsCopy));
    for (int i = 0; i < lineCount; ++i)
        nextSymbols[i] = lines[i + 1] - 1;
    unsigned char* currentSymbols = (unsigned char*)calloc(lineCount, sizeof(*currentSymbols));
    unsigned char** linesCopy = (unsigned char**)calloc(lineCount + 1, sizeof(*linesCopy));
    int* lineLengthsCopy = (int*)calloc(lineCount, sizeof(*lineLengthsCopy));
    for (int iter = maxLineLength - 1; iter >= 0; --iter)
    {
        memset(symbolCounts, 0, sizeof(symbolCounts));
        for (int line = 0; line < lineCount; ++line)
        {
            currentSymbols[line] = getSymbol(lines[line], lineLengths[line], iter, nextSymbols + line);
            ++symbolCounts[currentSymbols[line] + 1];
        }
        for (int i = 1; i < sizeof(symbolCounts) / sizeof(*symbolCounts); ++i)
            symbolCounts[i] += symbolCounts[i - 1];

        for (int line = 0; line < lineCount; ++line)
        {
            int newPosition = symbolCounts[currentSymbols[line]]++;
            linesCopy[newPosition] = lines[line];
            nextSymbolsCopy[newPosition] = nextSymbols[line];
            lineLengthsCopy[newPosition] = lineLengths[line];
        }
        swap(&lines, &linesCopy);
        swap(&nextSymbols, &nextSymbolsCopy);
        swapInts(&lineLengths, &lineLengthsCopy);
    }
    if (maxLineLength % 2 != 0)
    {
        swap(&lines, &linesCopy);
        for (int i = 0; i < lineCount; ++i)
            lines[i] = linesCopy[i];
    }
    free(nextSymbols);
    free(nextSymbolsCopy);
    free(currentSymbols);
    free(linesCopy);
}

unsigned char* readText(const unsigned char* filename)
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

void writeText(const unsigned char* filename, unsigned char** lines, int lineCount)
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

int splitText(unsigned char* text, unsigned char*** lines, int** lineLengths, int* maxLineLength)
{
    int lineCount = 0;
    for (int i = 0; text[i] != '\0'; ++i)
        if (text[i] == '\n')
            ++lineCount;

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
            (*lines)[ind] = text + i + 1;
            (*lineLengths)[ind - 1] = lineLength;
            ++ind;
            if (lineLength > *maxLineLength)
                *maxLineLength = lineLength;
            lineLength = 0;
        }
        else if (isAlpha(text[i]))
                ++lineLength;
    }
    return lineCount;
}

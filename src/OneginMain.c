#include "OneginSort.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <locale.h>

int main()
{
    unsigned char* data = readText("Onegin.txt");
    unsigned char** lines;
    int maxLineLength;
    int* lineLengths;
    int lineCount = splitText(data, &lines, &lineLengths, &maxLineLength);
    unsigned char** originalLines = (unsigned char**)calloc(lineCount, sizeof(*originalLines));
    memcpy(originalLines, lines, lineCount * sizeof(*lines));
    sortLines(lineCount, lines, lineLengths, maxLineLength, DIRECTION_FROMBEGIN);
    writeLines("fromBegin.txt", lines, lineCount);
    memcpy(lines, originalLines, lineCount * sizeof(*originalLines));
    sortLines(lineCount, lines, lineLengths, maxLineLength, DIRECTION_FROMEND);
    writeLines("fromEnd.txt", lines, lineCount);
    writeText("original.txt", data);
    free(data);
    free(lines);
    free(lineLengths);
    free(originalLines);
    return 0;
}
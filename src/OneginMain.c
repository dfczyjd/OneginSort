#include "OneginSort.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>

int main()
{
    unsigned char* data = readText("input.txt");
    unsigned char** lines;
    int maxLineLength;
    int* lineLengths;
    int lineCount = splitText(data, &lines, &lineLengths, &maxLineLength);
    unsigned char** originalLines = (unsigned char**)calloc(lineCount, sizeof(*originalLines));
    memcpy(originalLines, lines, lineCount * sizeof(*lines));
    sortLines(data, lineCount, lines, lineLengths, maxLineLength, DIRECTION_FROMBEGIN);
    writeText("fromBegin.txt", lines, lineCount);
    memcpy(lines, originalLines, lineCount * sizeof(*originalLines));
    sortLines(data, lineCount, lines, lineLengths, maxLineLength, DIRECTION_FROMEND);
    writeText("fromEnd.txt", lines, lineCount);
    writeRaw("original.txt", data);
    free(data);
    free(lines);
    free(lineLengths);
    free(originalLines);
    return 0;
}
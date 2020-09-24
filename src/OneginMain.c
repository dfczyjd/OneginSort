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
    sortFromBeginning(data, lineCount, lines, lineLengths, maxLineLength);
    writeText("output.txt", lines, lineCount);
    free(data);
    free(lines);
    free(lineLengths);
    free(originalLines);
    return 0;
}
#pragma once

void sortFromBeginning(const unsigned char* text, int lineCount, unsigned char** lines, int* lineLengths, int maxLineLength);

unsigned char* readText(const unsigned char* filename);

void writeText(const unsigned char* filename, unsigned char** lines, int lineCount);

int splitText(unsigned char* text, unsigned char*** lines, int** lineLengths, int* maxLineLength);

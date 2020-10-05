#pragma once

#define DIRECTION_FROMBEGIN -1  /**< Lines should be sorted by beginnings */
#define DIRECTION_FROMEND 1     /**< Lines should be sorted by ends */

/**
    \brief Sorts lines of the poem

    Sorts lines of the poem alphabetically from given end using radix sort.
    \param lineCount Number of lines
    \param lines Array of pointers to beginnings of lines. Last element must be pointer to the symbol after the text
    \param lineLengths Array of numbers of letters in lines
    \param maxLineLength Maximum number of letters of all lines
    \param direction End from which to compare lines. Must be either DIRECTION_FROMBEGIN or DIRECTION_FROMEND
    \returns Non-zero, if an error has occured, 0 otherwise
*/
int sortLines(int lineCount, unsigned char** lines, int* lineLengths, int maxLineLength, int direction);

/**
    \brief Splits poem into lines

    Split poem into lines, skipping emty ones, and counts number of letters in them.
    \param text Text of the poem
    \param [out] lines Array of pointers to beginnings of lines. Last element is a pointer to the symbol after the text.
    \param [out] lineLengths Array of numbers of letters in lines
    \param [out] maxLineLength Maximum number of letters of all lines
    \returns Number of lines in a poem
*/
int splitText(unsigned char* text, unsigned char*** lines, int** lineLengths, int* maxLineLength);

/**
    \brief Reads text from file

    Reads text from file. If an error occures, returns <CODE>NULL</CODE>. Otherwise returns the text from the file.
    \param filename Name of the file to read
    \returns Text from the file, or <CODE>NULL</CODE>, if error occured.
*/
unsigned char* readText(const char* filename);

/**
    \brief Writes sorted lines to the file
    \param filename Name of the file to write to
    \param lines Array of pointers to beginnings of lines
    \param lineCount Number of lines
*/
void writeLines(const char* filename, unsigned char** lines, int lineCount);

/**
    \brief Writes text to the file
    \param filename Name of the file to write to
    \param text Text to write
*/
void writeText(const char* filename, const unsigned char* text);

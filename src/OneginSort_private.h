#pragma once
#include <stdbool.h>

/**
    \brief A structure that represents a line of the poem
*/
typedef struct
{
    unsigned char* begin;
    int length;
    unsigned char currentSymbol;
    unsigned char* nextSymbol;

} Line;

/**
    \brief Converts a Cyrillic letter to lowercase
    \param c Letter to convert
    \returns Given letter in lowercase if symbol is from Cyrillic alphabet, or given symbol if not.
*/
unsigned char toLower(unsigned char c);

/**
    \brief Checks if given symbol is a Cyrillic letter
    \param c Symbol to check
    \returns <CODE>true</CODE>, if given symbol is Cyrillic, <CODE>false</CODE> otherwise
*/
bool isAlpha(unsigned char c);

/**
    \brief Swaps two given <CODE>Line</CODE> pointers
    \param first Pointer to first pointer
    \param second Pointer to second pointer
*/
void swap(Line** first, Line** second);

/**
    \brief Gets the next symbol from a line

    Gets first letter from given current position and converts it to lowercase. If index is beyond the line's bounds, 0 is returned.
    \param length Number of letters in line (not counting other symbols)
    \param index Index of the required letter among all in the line
    \param current Pointer to the current position
    \param direction Direction of sorting. Next symbol will be searched in the opposite one.
*/
unsigned char getSymbol(int length, int index, unsigned char** current, int direction);
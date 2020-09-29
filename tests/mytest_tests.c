#include "mytest.h"
#include "../src/OneginSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

const char tempfile[] = "tmpfile.txt";

void toLowerTest()
{
    EXPECT_EQUAL(toLower('Ц'), (unsigned char)'ц');
    EXPECT_EQUAL(toLower('ц'), (unsigned char)'ц');
    EXPECT_EQUAL(toLower('Z'), (unsigned char)'Z');
    EXPECT_EQUAL(toLower('.'), (unsigned char)'.');
}

void isAlphaTest()
{
    EXPECT_TRUE(isAlpha('ш'));
    EXPECT_TRUE(isAlpha('Ш'));
    EXPECT_FALSE(isAlpha('I'));
    EXPECT_FALSE(isAlpha(','));
}

void swapTest()
{
    Line* left = (Line*)calloc(1, sizeof(*left));
    Line* right = (Line*)calloc(1, sizeof(*right));
    Line* leftCopy = left;
    Line* rightCopy = right;
    swap(&left, &right);
    EXPECT_EQUAL(left, rightCopy);
    EXPECT_EQUAL(right, leftCopy);
    free(left);
    free(right);
}

void splitTextNullTest()
{
    unsigned char* text = NULL;
    char** lines = NULL;
    int* lineLengths = NULL;
    int maxLineLength = 0;

    int lineCount = splitText(text, &lines, &lineLengths, &maxLineLength);
    EXPECT_ERROR(EINVAL);
    errno = 0;
    EXPECT_EQUAL(lineCount, -1);
    EXPECT_EQUAL(lines, NULL);
    EXPECT_EQUAL(lineLengths, NULL);
    EXPECT_EQUAL(maxLineLength, 0);
}

void splitTextEmptyTest()
{
    unsigned char text[] = "";
    char** lines = NULL;
    int* lineLengths = NULL;
    int maxLineLength = 0;

    int lineCount = splitText(text, &lines, &lineLengths, &maxLineLength);
    EXPECT_ERROR(EINVAL);
    errno = 0;
    EXPECT_EQUAL(lineCount, -1);
    EXPECT_EQUAL(lines, NULL);
    EXPECT_EQUAL(lineLengths, NULL);
    EXPECT_EQUAL(maxLineLength, 0);
}

void splitTextCorrectTest()
{
    unsigned char text[] =
"Строка 1\n\
Строка номер 2\n\
Третья строка, или №3\n";
    char** lines = NULL;
    int* lineLengths = NULL;
    int maxLineLength = 0;

    int lineCount = splitText(text, &lines, &lineLengths, &maxLineLength);
    EXPECT_EQUAL(lineCount, 3);
    EXPECT_EQUAL(maxLineLength, 15);

    ASSERT_NOT_NULL(lines);
    EXPECT_EQUAL(lines[0], text);
    EXPECT_EQUAL(lines[1], text + 9);
    EXPECT_EQUAL(lines[2], text + 24);
    EXPECT_EQUAL(lines[3], text + 46);

    ASSERT_NOT_NULL(lineLengths);
    EXPECT_EQUAL(lineLengths[0], 6);
    EXPECT_EQUAL(lineLengths[1], 11);
    EXPECT_EQUAL(lineLengths[2], 15);
}

void getSymbolTest()
{
    unsigned char* line = "Строка для теста. С @,небуквенными?*символа34ми.\n";
    int length = 36;
    unsigned char* current = NULL;
    unsigned char next;

    current = line + 16; // first dot
    next = getSymbol(length, 21, &current, DIRECTION_FROMBEGIN);
    EXPECT_EQUAL(current, line + 14);
    EXPECT_EQUAL(next, (unsigned char)'а');

    current = line + 14; // pre-last letter of 'теста'
    next = getSymbol(length, 22, &current, DIRECTION_FROMBEGIN);
    EXPECT_EQUAL(current, line + 13);
    EXPECT_EQUAL(next, (unsigned char)'т');

    current = line + 21; // colon
    next = getSymbol(length, 20, &current, DIRECTION_FROMBEGIN);
    EXPECT_EQUAL(current, line + 17);
    EXPECT_EQUAL(next, (unsigned char)'с');

    current = line;
    next = getSymbol(length, 40, &current, DIRECTION_FROMEND);
    EXPECT_EQUAL(current, line);
    EXPECT_EQUAL(next, (unsigned char)'\0');

    current = line + 43; // '3'
    next = getSymbol(length, 34, &current, DIRECTION_FROMEND);
    EXPECT_EQUAL(current, line + 46);
    EXPECT_EQUAL(next, (unsigned char)'м');
}

void sortLinesNullTest()
{
    EXPECT_EQUAL(sortLines(NULL, 1, NULL, NULL, 3, DIRECTION_FROMBEGIN), 1);
    EXPECT_ERROR(EINVAL);
    errno = 0;
}

void sortLinesFromBeginTest()
{
    unsigned char text[] =
"Строка под номером 1\n\
Строка номер 2\n\
Третья строка, или №3\n";
    unsigned char* lines[4];
    lines[0] = text;
    lines[1] = text + 21;
    lines[2] = text + 36;
    lines[3] = text + 58;
    int lineLengths[3];
    lineLengths[0] = 16;
    lineLengths[1] = 11;
    lineLengths[2] = 15;

    EXPECT_EQUAL(sortLines(text, 3, lines, lineLengths, 16, DIRECTION_FROMBEGIN), 0);
    ASSERT_NOT_NULL(lines);
    EXPECT_EQUAL(lines[0], text + 21);
    EXPECT_EQUAL(lines[1], text);
    EXPECT_EQUAL(lines[2], text + 36);
    EXPECT_EQUAL(lines[3], text + 58);
}

void sortLinesFromEndTest()
{
    unsigned char text[] =
        "Строка под номером 1\n\
Строка номер 2\n\
Третья строка, или №3\n";
    unsigned char* lines[4];
    lines[0] = text;
    lines[1] = text + 21;
    lines[2] = text + 36;
    lines[3] = text + 58;
    int lineLengths[3];
    lineLengths[0] = 16;
    lineLengths[1] = 11;
    lineLengths[2] = 15;

    EXPECT_EQUAL(sortLines(text, 3, lines, lineLengths, 16, DIRECTION_FROMEND), 0);
    ASSERT_NOT_NULL(lines);
    EXPECT_EQUAL(lines[0], text + 36);
    EXPECT_EQUAL(lines[1], text);
    EXPECT_EQUAL(lines[2], text + 21);
    EXPECT_EQUAL(lines[3], text + 58);
}

int main()
{
    initializeTests(10);
    registerTest("toLower", toLowerTest);
    registerTest("isAlpha", isAlphaTest);
    registerTest("swap", swapTest);
    registerTest("splitText (text is NULL)", splitTextNullTest);
    registerTest("splitText (text is empty)", splitTextEmptyTest);
    registerTest("splitText (correct)", splitTextCorrectTest);
    registerTest("getSymbol", getSymbolTest);
    registerTest("sortLines (NULLs as ptrs)", sortLinesNullTest);
    registerTest("sortLines (from beginnings)", sortLinesFromBeginTest);
    registerTest("sortLines (from ends)", sortLinesFromEndTest);
    runTests();
    clearTests();
    return 0;
}
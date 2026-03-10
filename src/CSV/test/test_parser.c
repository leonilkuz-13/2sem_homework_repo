#include "parser.h"

static bool checkField(Field* field, const char* expectedStr, FieldType expectedType,
                       size_t expectedCol)
{
    if (!field)
        return false;
    if (expectedStr == NULL) {
        if (field->field != NULL)
            return false;
    } else {
        if (!field->field || strcmp(field->field, expectedStr) != 0)
            return false;
    }
    return (field->type == expectedType && field->colNum == expectedCol);
}

bool testDetectType()
{
    bool allPassed = true;

    if (detectType("") != TypeNone) {
        puts("testDetectType 1 (empty) failed");
        allPassed = false;
    }
    if (detectType("abc") != TypeString) {
        puts("testDetectType 2 (string) failed");
        allPassed = false;
    }
    if (detectType("123") != TypeNumber) {
        puts("testDetectType 3 (integer) failed");
        allPassed = false;
    }
    if (detectType("45.67") != TypeNumber) {
        puts("testDetectType 4 (float) failed");
        allPassed = false;
    }
    if (detectType("  789  ") != TypeNumber) {
        puts("testDetectType 5 (number with spaces) failed");
        allPassed = false;
    }
    if (detectType("12a") != TypeString) {
        puts("testDetectType 6 (number + letter) failed");
        allPassed = false;
    }
    if (detectType("-123") != TypeNumber) {
        puts("testDetectType 7 (negative number) failed");
        allPassed = false;
    }
    if (detectType("3.14e-10") != TypeNumber) {
        puts("testDetectType 9 (exponential) failed");
        allPassed = false;
    }
    if (detectType("   ") != TypeString) {
        puts("testDetectType 10 (only spaces) failed");
        allPassed = false;
    }

    if (allPassed)
        puts("testDetectType passed");
    else
        puts("testDetectType failed");
    return allPassed;
}

bool testParseSimple()
{
    Row* row = initRow(0);
    bool success = true;

    if (!parse(&row, "a,b,c"))
        success = false;
    else {
        if (row->fieldCnt != 3 || !checkField(&row->field[0], "a", TypeString, 0)
            || !checkField(&row->field[1], "b", TypeString, 1)
            || !checkField(&row->field[2], "c", TypeString, 2))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "123,45.6,hello"))
        success = false;
    else {
        if (row->fieldCnt != 3 || !checkField(&row->field[0], "123", TypeNumber, 0)
            || !checkField(&row->field[1], "45.6", TypeNumber, 1)
            || !checkField(&row->field[2], "hello", TypeString, 2))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, ",,"))
        success = false;
    else {
        if (row->fieldCnt != 3 || !checkField(&row->field[0], "", TypeNone, 0)
            || !checkField(&row->field[1], "", TypeNone, 1)
            || !checkField(&row->field[2], "", TypeNone, 2))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, " 123 , abc "))
        success = false;
    else {
        if (row->fieldCnt != 2 || !checkField(&row->field[0], " 123 ", TypeNumber, 0)
            || !checkField(&row->field[1], " abc ", TypeString, 1))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "one"))
        success = false;
    else {
        if (row->fieldCnt != 1 || !checkField(&row->field[0], "one", TypeString, 0))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, ""))
        success = false;
    else {
        if (row->fieldCnt != 0 || row->field != NULL)
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "a,"))
        success = false;
    else {
        if (row->fieldCnt != 2 || !checkField(&row->field[0], "a", TypeString, 0)
            || !checkField(&row->field[1], "", TypeNone, 1))
            success = false;
    }
    clearRow(&row);

    if (success)
        puts("testParseSimple passed");
    else
        puts("testParseSimple failed");
    return success;
}

bool testParseQuoted()
{
    Row* row = initRow(0);
    bool success = true;

    if (!parse(&row, "\"hello\",world"))
        success = false;
    else {
        if (row->fieldCnt != 2 || !checkField(&row->field[0], "hello", TypeString, 0)
            || !checkField(&row->field[1], "world", TypeString, 1))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "\"\"\"hello\"\"\",world"))
        success = false;
    else {
        if (row->fieldCnt != 2 || !checkField(&row->field[0], "\"hello\"", TypeString, 0)
            || !checkField(&row->field[1], "world", TypeString, 1))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "\"  hello  \",world"))
        success = false;
    else {
        if (row->fieldCnt != 2 || !checkField(&row->field[0], "  hello  ", TypeString, 0)
            || !checkField(&row->field[1], "world", TypeString, 1))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "\"\",next"))
        success = false;
    else {
        if (row->fieldCnt != 2 || !checkField(&row->field[0], "", TypeNone, 0)
            || !checkField(&row->field[1], "next", TypeString, 1))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "a,\"b\",c"))
        success = false;
    else {
        if (row->fieldCnt != 3 || !checkField(&row->field[0], "a", TypeString, 0)
            || !checkField(&row->field[1], "b", TypeString, 1)
            || !checkField(&row->field[2], "c", TypeString, 2))
            success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (parse(&row, "  \"  hello  \"  ,world") != false) {
        puts("testParseQuoted (spaces before quote) failed");
        success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (success)
        puts("testParseQuoted passed");
    else
        puts("testParseQuoted failed");
    return success;
}

bool testParseError()
{
    Row* row = initRow(0);
    bool success = true;

    if (parse(&row, "\"hello,world") != false) {
        puts("testParseError 1 (unclosed quote) failed");
        success = false;
    }
    clearRow(&row);
    row = initRow(0);

    if (parse(&row, "\"a\"b,c") != false) {
        puts("testParseError 3 (char after closing quote) failed");
        success = false;
    }
    clearRow(&row);

    if (success)
        puts("testParseError passed");
    else
        puts("testParseError failed");
    return success;
}

bool testParseEdge()
{
    Row* row = initRow(0);
    bool success = true;

    {
        char* longLine = (char*)malloc(2000);
        if (!longLine)
            return false;
        for (int i = 0; i < 500; i++)
            longLine[i] = 'a';
        longLine[500] = ',';
        for (int i = 501; i < 1001; i++)
            longLine[i] = 'b';
        longLine[1001] = '\0';
        if (!parse(&row, longLine)) {
            puts("testParseEdge 1 (long line) failed");
            success = false;
        } else {
            if (row->fieldCnt != 2 || strlen(row->field[0].field) != 500
                || strlen(row->field[1].field) != 500) {
                puts("testParseEdge 1 (long line) field lengths mismatch");
                success = false;
            }
        }
        free(longLine);
        clearRow(&row);
        row = initRow(0);
    }

    if (!parse(&row, "   ,next")) {
        puts("testParseEdge 2 (space field) failed");
        success = false;
    } else {
        if (row->fieldCnt != 2 || strcmp(row->field[0].field, "   ") != 0
            || row->field[0].type != TypeString) {
            puts("testParseEdge 2 (space field) field mismatch");
            success = false;
        }
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, "a\nb,c")) {
        puts("testParseEdge 3 (newline) failed");
        success = false;
    } else {
        if (row->fieldCnt != 2 || strcmp(row->field[0].field, "a\nb") != 0
            || row->field[1].len != 1) {
            puts("testParseEdge 3 (newline) field mismatch");
            success = false;
        }
    }
    clearRow(&row);
    row = initRow(0);

    if (!parse(&row, ",,,")) {
        puts("testParseEdge 4 (multiple commas) failed");
        success = false;
    } else {
        if (row->fieldCnt != 4 || strcmp(row->field[0].field, "") != 0
            || strcmp(row->field[1].field, "") != 0 || strcmp(row->field[2].field, "") != 0
            || strcmp(row->field[3].field, "") != 0)
            success = false;
    }
    clearRow(&row);

    if (success)
        puts("testParseEdge passed");
    else
        puts("testParseEdge failed");
    return success;
}

void runParserTests()
{
    bool result = true;
    result = result && testDetectType();
    result = result && testParseSimple();
    result = result && testParseQuoted();
    result = result && testParseError();
    result = result && testParseEdge();

    if (result) {
        puts("\nALL PARSER TESTS PASSED");
    } else {
        puts("\nSOME PARSER TESTS FAILED");
    }
}

int main()
{
    runParserTests();
    return 0;
}
#pragma once
// containers
#include <map>
#include <string>
#include <vector>

// For exit() function
#include <stdlib.h>

// For freopen() function
#include <cstdio>

// For pow() and fmod() functions
#include <cmath>

// For file handling
#include <fstream>

// For JSON parsing
#include <json.hpp>

typedef struct
{
    int *integer;
    double *real;
    std::string *text;
    int type;
} Multitype;

// Functions for solving expressions
int solveForInteger(std::string &expr, int &position);
std::string solveForText(std::string &expr, int &position);
double solveForReal(std::string &expr, int &position);
int checkIntegralCondition(std::string &expr);
int checkTextCondition(std::string &expr);
int checkRealCondition(std::string &expr);
int checkCondition(std::string &expr);

// Functions for interpreting the code
void interpret(std::string &expr);
void initiate(std::string &expr);
void printOutput(std::string &expr, int &position);
void readFromUser(std::string &expr, int &position);
void printError(int errCode);
void endProgram();

// Utility functions
void trim(std::string &str, int &position);
int power(int number, int exponent);
int isNumber(std::string &str);
int isString(std::string &str);
int isReal(std::string &str);
void readUntilNextSpace(std::string &expr, int &position, std::string &temp);
int nextExpressionExists(std::string &expr, int &position);
int findKeyword(std::string &expr, int &position, int keyword);
int validateName(std::string name);
Multitype getVariable(std::string varName);

// Loading and preprocessing functions
void preprocess(std::string fileName);
void configure();

std::map<std::string, int> integerVariables;
std::map<std::string, std::string> textVariables;
std::map<std::string, double> realVariables;
std::map<std::string, std::vector<int>> integerArrays;
std::map<std::string, std::vector<std::string>> textArrays;
std::map<std::string, std::vector<double>> realArrays;
std::map<std::string, std::vector<std::string>> subroutines;

enum DATA_TYPES
{
    INTEGER,
    TEXT,
    REAL,
    INTEGER_ARRAY,
    TEXT_ARRAY,
    REAL_ARRAY
};

enum OPERATORS
{
    PLUS,
    MINUS,
    UPON,
    INTO,
    MODULO,
    EXPONENT,
    LESS_THAN,
    GREATER_THAN,
    EQUAL_TO,
    NOT_EQUAL_TO
};

enum KEYWORDS
{
    LET,
    IS,
    IF,
    ELSE,
    THEN,
    SKIP,
    STOP,
    DO,
    WHILE,
    ESCAPE,
    RECHECK,
    TILLHERE,
    AND,
    GOTO,
    READ,
    PRINT,
    NEWLINE,
    LEAVE,
    END,
    SUBROUTINE
};

enum ERRORS
{
    INVALID_SYNTAX,
    INVALID_DATA_TYPE,
    INVALID_OPERATOR,
    VARIABLE_NOT_FOUND,
    INDEX_OUT_OF_BOUNDS,
    INVALID_NAME
};

// Default configuration
std::string preprocessedFileName = "processed_program.symls";
const std::string configFileName = "symlsConfig.json";
int DEFAULT_ARRAY_SIZE = 64;
int DEFAULT_INTEGER_VALUE = 0;
std::string DEFAULT_TEXT_VALUE = "";
double DEFAULT_REAL_VALUE = 0.0;
std::vector<std::string> reservedWords = {"let", "is", "if", "else", "then", "skip", "stop", "do", "while", "escape", "recheck", "till-here", "and", "goto", "read", "print", "newline", "leave", "end", "subroutine", "plus", "minus", "upon", "into", "modulo", "exponent", "less-than", "greater-than", "equal-to", "not-equal-to", "integer", "text", "real", "integer-array", "text-array", "real-array"};

std::map<std::string, int> mathOperators = {
    {"plus", PLUS},
    {"minus", MINUS},
    {"upon", UPON},
    {"into", INTO},
    {"modulo", MODULO},
    {"exponent", EXPONENT}};

std::map<int, std::string> mathOperatorTostring = {
    {PLUS, "plus"},
    {MINUS, "minus"},
    {UPON, "upon"},
    {INTO, "into"},
    {MODULO, "modulo"},
    {EXPONENT, "exponent"}};

std::map<std::string, int> logicalOperators = {
    {"less-than", LESS_THAN},
    {"greater-than", GREATER_THAN},
    {"equal-to", EQUAL_TO},
    {"not-equal-to", NOT_EQUAL_TO}};

std::map<std::string, int> dataTypes = {
    {"integer", INTEGER},
    {"text", TEXT},
    {"real", REAL},
    {"integer-array", INTEGER_ARRAY},
    {"text-array", TEXT_ARRAY},
    {"real-array", REAL_ARRAY}};

std::map<int, std::string> dataTypeToString = {
    {INTEGER, "integer"},
    {TEXT, "text"},
    {REAL, "real"},
    {INTEGER_ARRAY, "integer-array"},
    {TEXT_ARRAY, "text-array"},
    {REAL_ARRAY, "real-array"}};

std::map<int, std::string> keywordsToString = {
    {LET, "let"},
    {IS, "is"},
    {IF, "if"},
    {ELSE, "else"},
    {THEN, "then"},
    {SKIP, "skip"},
    {STOP, "stop"},
    {DO, "do"},
    {WHILE, "while"},
    {ESCAPE, "escape"},
    {RECHECK, "recheck"},
    {TILLHERE, "till-here"},
    {AND, "and"},
    {GOTO, "goto"},
    {READ, "read"},
    {PRINT, "print"},
    {NEWLINE, "newline"},
    {LEAVE, "leave"},
    {END, "end"},
    {SUBROUTINE, "subroutine"}};

std::map<std::string, int> keywords = {
    {"let", LET},
    {"is", IS},
    {"if", IF},
    {"else", ELSE},
    {"then", THEN},
    {"skip", SKIP},
    {"stop", STOP},
    {"do", DO},
    {"while", WHILE},
    {"escape", ESCAPE},
    {"recheck", RECHECK},
    {"till-here", TILLHERE},
    {"and", AND},
    {"goto", GOTO},
    {"read", READ},
    {"print", PRINT},
    {"newline", NEWLINE},
    {"leave", LEAVE},
    {"end", END},
    {"subroutine", SUBROUTINE}};

std::map<int, std::string> errorMassage = {
    {INVALID_SYNTAX, "Invalid syntax"},
    {INVALID_DATA_TYPE, "Invalid data type"},
    {INVALID_OPERATOR, "Invalid operator"},
    {VARIABLE_NOT_FOUND, "Variable not found"},
    {INDEX_OUT_OF_BOUNDS, "Index out of bounds"},
    {INVALID_NAME, "Invalid name"}};

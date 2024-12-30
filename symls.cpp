#include <iostream>

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

using namespace std;

// For handling multiple data types in a single variable
typedef struct
{
    int *integer;
    double *real;
    string *text;
    int type;
} Multitype;

// Functions for solving expressions
int solveForInteger(string &expr, int &position);
string solveForText(string &expr, int &position);
double solveForReal(string &expr, int &position);
int checkIntegralCondition(string &expr);
int checkTextCondition(string &expr);
int checkRealCondition(string &expr);
int checkCondition(string &expr);

// Functions for interpreting the code
void interpret(string &expr);
void initiate(string &expr);
void printOutput(string &expr, int &position);
void readFromUser(string &expr, int &position);
void printError(int errCode);
void endProgram();

// Utility functions
void trim(string &str, int &position);
int power(int number, int exponent);
int isNumber(string &str);
int isString(string &str);
int isReal(string &str);
void readUntilNextSpace(string &expr, int &position, string &temp);
int nextExpressionExists(string &expr, int &position);
int findKeyword(string &expr, int &position, int keyword);
int validateName(string name);
Multitype getVariable(string varName);

// Loading and preprocessing functions
void preprocess(string fileName);
void configure();

// To store the variables
map<string, int> integerVariables;
map<string, string> textVariables;
map<string, double> realVariables;
map<string, vector<int>> integerArrays;
map<string, vector<string>> textArrays;
map<string, vector<double>> realArrays;
map<string, vector<string>> subroutines;

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
string preprocessedFileName = "processed_program.symls";
const string configFileName = "symlsConfig.json";
int DEFAULT_ARRAY_SIZE = 64;
int DEFAULT_INTEGER_VALUE = 0;
string DEFAULT_TEXT_VALUE = "";
double DEFAULT_REAL_VALUE = 0.0;
vector<string> reservedWords = {"let", "is", "if", "else", "then", "skip", "stop", "do", "while", "escape", "recheck", "till-here", "and", "goto", "read", "print", "newline", "leave", "end", "subroutine", "plus", "minus", "upon", "into", "modulo", "exponent", "less-than", "greater-than", "equal-to", "not-equal-to", "integer", "text", "real", "integer-array", "text-array", "real-array"};

map<string, int> mathOperators = {
    {"plus", PLUS},
    {"minus", MINUS},
    {"upon", UPON},
    {"into", INTO},
    {"modulo", MODULO},
    {"exponent", EXPONENT}};

map<int, string> mathOperatorToString = {
    {PLUS, "plus"},
    {MINUS, "minus"},
    {UPON, "upon"},
    {INTO, "into"},
    {MODULO, "modulo"},
    {EXPONENT, "exponent"}};

map<string, int> logicalOperators = {
    {"less-than", LESS_THAN},
    {"greater-than", GREATER_THAN},
    {"equal-to", EQUAL_TO},
    {"not-equal-to", NOT_EQUAL_TO}};

map<string, int> dataTypes = {
    {"integer", INTEGER},
    {"text", TEXT},
    {"real", REAL},
    {"integer-array", INTEGER_ARRAY},
    {"text-array", TEXT_ARRAY},
    {"real-array", REAL_ARRAY}};

map<int, string> dataTypeToString = {
    {INTEGER, "integer"},
    {TEXT, "text"},
    {REAL, "real"},
    {INTEGER_ARRAY, "integer-array"},
    {TEXT_ARRAY, "text-array"},
    {REAL_ARRAY, "real-array"}};

map<int, string> keywordsToString = {
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

map<string, int> keywords = {
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

map<int, string> errorMassage = {
    {INVALID_SYNTAX, "Invalid syntax"},
    {INVALID_DATA_TYPE, "Invalid data type"},
    {INVALID_OPERATOR, "Invalid operator"},
    {VARIABLE_NOT_FOUND, "Variable not found"},
    {INDEX_OUT_OF_BOUNDS, "Index out of bounds"},
    {INVALID_NAME, "Invalid name"}};

// Status variables
int currentLine = 0, escape = 0, recheckCondition = 0;

int main(int argc, char *argv[])
{
    // Load the configuration from symlsConfig.json
    configure();

    // Store the newline character in the textVariables map
    textVariables.insert({keywordsToString.at(NEWLINE), "\n"});

    // If the file name is not provided from command line
    if (argc < 2)
    {
        string line;
        while (getline(cin, line))
        {
            currentLine++;
            interpret(line);
            cout<<endl;
        }
        return 0;
    }

    string fileName = argv[1];

    // If the file name is entered withput extension
    if (fileName.find(".symls") == string::npos)
    {
        fileName.append(".symls");
    }

    // Preprocess the file
    preprocess(fileName);

    // Open the preprocessed file
    ifstream program(preprocessedFileName);

    // If the file is not found
    if (program.fail())
    {
        cout << "File not found" << endl;
        return 0;
    }

    // Interpret the program line by line
    string line;
    while (getline(program, line))
    {
        currentLine++;
        interpret(line);
    }
    return 0;
}

// Interpreter functions

void interpret(string &expr)
{
    // expression1 and expression2 and ....
    // set cursor at the beginning of the line
    int cursor = 0;

    // To remove the leading spaces
    trim(expr, cursor);

    // If the line is empty
    if (expr[cursor] == '\0')
    {
        return;
    }

    // To read the first word of the line
    string temp = "";
    readUntilNextSpace(expr, cursor, temp);
    trim(expr, cursor);

    // To break the line into individual expressions
    if (temp != keywordsToString.at(IF) && temp != keywordsToString.at(WHILE) && temp != keywordsToString.at(LEAVE))
    {
        int j = cursor;
        // If the next expression exists split the line and interpret them separately
        if (nextExpressionExists(expr, j))
        {
            string part1 = expr.substr(0, j);
            string part2 = expr.substr(j + keywordsToString.at(AND).length());
            interpret(part1);
            interpret(part2);
            return;
        }
    }
    if (temp == keywordsToString.at(LET))
    {
        initiate(expr);
    }
    else if (temp == keywordsToString.at(PRINT))
    {
        printOutput(expr, cursor);
    }
    else if (temp == keywordsToString.at(IF))
    {
        // if condition then expression else expression stop
        string condition = "";
        int thenPosition = cursor;

        // Find the position of THEN keyword
        findKeyword(expr, thenPosition, THEN);

        // The condition is between cursor and thenPosition
        condition = expr.substr(cursor, thenPosition - cursor);

        // Move the cursor to the position after THEN
        cursor = thenPosition + keywordsToString.at(THEN).length();
        trim(expr, cursor);

        // Find the position of ELSE keyword
        int conjugateElsePosition = 0;
        int ifCount = 1;
        for (conjugateElsePosition = cursor; conjugateElsePosition < expr.length() - max(keywordsToString.at(ELSE).length(), keywordsToString.at(IF).length()) && ifCount != 0; conjugateElsePosition++)
        {
            if (expr.substr(conjugateElsePosition, keywordsToString.at(IF).length()) == keywordsToString.at(IF))
            {
                ifCount++;
            }
            else if (expr.substr(conjugateElsePosition, keywordsToString.at(ELSE).length()) == keywordsToString.at(ELSE))
            {
                ifCount--;
            }
            if (ifCount == 0)
            {
                break;
            }
        }
        int conjugateStopPosition = conjugateElsePosition + keywordsToString.at(ELSE).length();
        int elseCount = 1;
        for (conjugateStopPosition = conjugateElsePosition + keywordsToString.at(ELSE).length(); conjugateStopPosition < expr.length() - keywordsToString.at(STOP).length(); conjugateStopPosition++)
        {
            if (conjugateStopPosition < expr.length() - keywordsToString.at(ELSE).length() && expr.substr(conjugateStopPosition, keywordsToString.at(ELSE).length()) == keywordsToString.at(ELSE))
            {
                elseCount++;
            }
            else if (expr.substr(conjugateStopPosition, keywordsToString.at(STOP).length()) == keywordsToString.at(STOP))
            {
                elseCount--;
            }
            if (elseCount == 0)
            {
                break;
            }
        }

        // Seperate the expressions for true and false conditions
        string expressionWhenTrue, expressionWhenFalse;

        // The part between then and else is the expression for true condition and between else and stop is for false condition
        expressionWhenTrue = expr.substr(cursor, conjugateElsePosition - cursor);
        expressionWhenFalse = expr.substr(conjugateElsePosition + keywordsToString.at(ELSE).length(), conjugateStopPosition - conjugateElsePosition - keywordsToString.at(ELSE).length());

        if (checkCondition(condition))
        {
            interpret(expressionWhenTrue);
        }
        else
        {
            interpret(expressionWhenFalse);
        }

        // Move the cursor to the position after STOP
        cursor = conjugateStopPosition + keywordsToString.at(STOP).length();
        if (nextExpressionExists(expr, cursor))
        {
            // Interpret the next expression
            string nextExpression = expr.substr(cursor + keywordsToString.at(AND).length());
            interpret(nextExpression);
            return;
        }
    }
    else if (temp == keywordsToString.at(WHILE))
    {
        // while condition do expression and expression till-here
        string condition = "";

        // Find the position of DO keyword
        int doPosition = cursor;
        findKeyword(expr, doPosition, DO);

        // The condition is between cursor and doPosition
        condition = expr.substr(cursor, doPosition - cursor);

        // Move the cursor to the position after DO
        cursor = doPosition + keywordsToString.at(DO).length();
        trim(expr, cursor);

        // Find the position of TILLHERE keyword
        int tillherePosition = 0;
        int whileCount = 1;
        for (tillherePosition = cursor; tillherePosition < expr.length() - keywordsToString.at(TILLHERE).length(); tillherePosition++)
        {
            if (tillherePosition < expr.length() - keywordsToString.at(WHILE).length() && expr.substr(tillherePosition, keywordsToString.at(WHILE).length()) == keywordsToString.at(WHILE))
            {
                whileCount++;
            }
            else if (expr.substr(tillherePosition, keywordsToString.at(TILLHERE).length()) == keywordsToString.at(TILLHERE))
            {
                whileCount--;
            }
            if (whileCount == 0)
            {
                break;
            }
        }

        // The expression is between cursor and tillherePosition
        string expression = expr.substr(cursor, tillherePosition - cursor);
        while (checkCondition(condition))
        {
            // If escape was triggered then break out of the loop
            if (escape)
            {
                escape = 0;
                break;
            }

            // If recheck condition was triggered then continue the loop
            if (recheckCondition)
            {
                recheckCondition = 0;
                continue;
            }

            interpret(expression);
        }

        // Move the cursor to the position after TILLHERE
        cursor = tillherePosition + keywordsToString.at(TILLHERE).length();
        if (nextExpressionExists(expr, cursor))
        {
            // Interpret the next expression
            string nextExpression = expr.substr(cursor + keywordsToString.at(AND).length());
            interpret(nextExpression);
            return;
        }
    }
    else if (temp == keywordsToString.at(GOTO))
    {
        // goto subroutine-name

        // Store the name of the subroutine
        string subroutineName = "";
        trim(expr, cursor);
        readUntilNextSpace(expr, cursor, subroutineName);

        // Interpret the subroutine
        for (auto &line : subroutines[subroutineName])
        {
            // If escape was triggered then break out of the subroutine
            if (escape)
            {
                escape = 0;
                return;
            }
            interpret(line);
        }
        return;
    }
    else if (temp == keywordsToString.at(READ))
    {
        readFromUser(expr, cursor);
    }
    else if (temp == keywordsToString.at(LEAVE))
    {
        return;
    }
    else if (temp == keywordsToString.at(SKIP))
    {
        return;
    }
    else if (temp == keywordsToString.at(END))
    {
        endProgram();
    }
    else if (temp == keywordsToString.at(ESCAPE))
    {
        escape = 1;
        return;
    }
    else if (temp == keywordsToString.at(RECHECK))
    {
        recheckCondition = 1;
        return;
    }
    else
    {
        // Store the variable in custom Multitype class along with it's type
        Multitype var = getVariable(temp);

        // Type is not -1 means the variable is found
        if (var.type != -1)
        {
            // Check if the next keyword is IS
            trim(expr, cursor);
            temp = "";
            readUntilNextSpace(expr, cursor, temp);
            if (temp != keywordsToString.at(IS))
            {
                printError(INVALID_SYNTAX);
                exit(0);
            }

            // Solve the expression and store the value in the variable according to it's type
            if (var.type == INTEGER)
            {
                *var.integer = solveForInteger(expr, cursor);
            }
            else if (var.type == TEXT)
            {
                *var.text = solveForText(expr, cursor);
            }
            else if (var.type == REAL)
            {
                *var.real = solveForReal(expr, cursor);
            }
        }
        // If the variable was not found
        else
        {
            printError(VARIABLE_NOT_FOUND);
            exit(0);
        }
    }
}

int solveForInteger(string &expr, int &position)
{
    trim(expr, position);

    // If the expression is empty
    if (expr[position] == '\0')
    {
        return 0;
    }

    int result = 0;

    string temp = "";
    readUntilNextSpace(expr, position, temp);

    // If the next word is a number
    if (isNumber(temp))
    {
        result = stoi(temp);
    }

    else
    {
        // If the variable is of number type then store the value in result
        Multitype var = getVariable(temp);
        if (var.type == INTEGER)
        {
            result = *var.integer;
        }
        else if (var.type == REAL)
        {
            result = *var.real;
        }
        // If the variable was not found
        else
        {
            printError(VARIABLE_NOT_FOUND);
            exit(0);
        }
    }

    trim(expr, position);
    if (expr[position] == '\0')
    {
        return result;
    }

    // Read the operator
    temp = "";
    trim(expr, position);
    readUntilNextSpace(expr, position, temp);
    int op;
    if (mathOperators.find(temp) != mathOperators.end())
    {
        op = mathOperators.at(temp);
    }
    // If the operator was invalid
    else
    {
        printError(INVALID_SYNTAX);
        exit(0);
    }

    switch (op)
    {
    case PLUS:
        result += solveForInteger(expr, position);
        break;
    case MINUS:
        result -= solveForInteger(expr, position);
        break;
    case UPON:
        result /= solveForInteger(expr, position);
        break;
    case INTO:
        result *= solveForInteger(expr, position);
        break;
    case MODULO:
        result %= solveForInteger(expr, position);
        break;
    case EXPONENT:
        result = power(result, solveForInteger(expr, position));
        break;
    }
    return result;
}

string solveForText(string &expr, int &position)
{

    // Return empty string if the expression is empty
    trim(expr, position);
    if (expr[position] == '\0')
    {
        return "";
    }

    string result = "";
    string temp = "";
    readUntilNextSpace(expr, position, temp);

    // If the next word is a string literal
    if (isString(temp))
    {
        result = temp.substr(1, temp.length() - 2);
    }
    else
    {
        // If the variable is of string type then store the value in result
        Multitype var = getVariable(temp);
        if (var.type == TEXT)
        {
            result = *var.text;
        }
        else
        {
            printError(VARIABLE_NOT_FOUND);
            exit(0);
        }
    }

    trim(expr, position);
    if (expr[position] == '\0')
    {
        return result;
    }

    string op = "";
    readUntilNextSpace(expr, position, op);
    if (mathOperators.at(op) == PLUS)
    {
        trim(expr, position);
        result += solveForText(expr, position);
    }
    else
    {
        printError(INVALID_SYNTAX);
        exit(0);
    }
    return result;
}

double solveForReal(string &expr, int &position)
{
    trim(expr, position);
    if (expr[position] == '\0')
    {
        return 0.0;
    }

    double result = 0.0;

    string temp = "";
    readUntilNextSpace(expr, position, temp);
    if (isNumber(temp))
    {
        result = stod(temp);
    }

    else
    {
        Multitype var = getVariable(temp);
        if (var.type == REAL)
        {
            result = *var.real;
        }
        else if (var.type == INTEGER)
        {
            result = *var.integer;
        }
        else
        {
            printError(VARIABLE_NOT_FOUND);
            exit(0);
        }
    }

    trim(expr, position);
    if (expr[position] == '\0')
    {
        return result;
    }

    temp = "";
    trim(expr, position);
    readUntilNextSpace(expr, position, temp);
    int op;
    if (mathOperators.find(temp) != mathOperators.end())
    {
        op = mathOperators.at(temp);
    }
    else
    {
        printError(INVALID_SYNTAX);
        exit(0);
    }

    switch (op)
    {
    case PLUS:
        result += solveForReal(expr, position);
        break;
    case MINUS:
        result -= solveForReal(expr, position);
        break;
    case UPON:
        result /= solveForReal(expr, position);
        break;
    case INTO:
        result *= solveForReal(expr, position);
        break;
    case MODULO:
        result = fmod(result, solveForReal(expr, position));
        break;
    case EXPONENT:
        result = pow(result, solveForReal(expr, position));
        break;
    }
    return result;
}

int checkIntegralCondition(string &expr)
{
    // leftHalf is logic rightHalf
    int cursor = 0;
    trim(expr, cursor);

    // Seperate the conditional parts and the logical operator
    string logic = "";
    string leftHalf, rightHalf;

    findKeyword(expr, cursor, IS);
    leftHalf = expr.substr(0, cursor);

    cursor += keywordsToString.at(IS).length();
    trim(expr, cursor);

    readUntilNextSpace(expr, cursor, logic);

    trim(expr, cursor);
    rightHalf = expr.substr(cursor);

    // Dummy variables to pass as a parameter to solving functions
    int dummy1 = 0, dummy2 = 0;

    switch (logicalOperators.at(logic))
    {
    case LESS_THAN:
        return (solveForInteger(leftHalf, dummy1) < solveForInteger(rightHalf, dummy2));
    case GREATER_THAN:
        return (solveForInteger(leftHalf, dummy1) > solveForInteger(rightHalf, dummy2));
    case EQUAL_TO:
        return (solveForInteger(leftHalf, dummy1) == solveForInteger(rightHalf, dummy2));
    case NOT_EQUAL_TO:
        return (solveForInteger(leftHalf, dummy1) != solveForInteger(rightHalf, dummy2));
    }
}

int checkTextCondition(string &expr)
{
    // leftHalf is logic rightHalf
    int cursor = 0;
    trim(expr, cursor);

    string logic = "";
    string leftHalf, rightHalf;

    findKeyword(expr, cursor, IS);
    leftHalf = expr.substr(0, cursor);

    cursor += keywordsToString.at(IS).length();
    trim(expr, cursor);

    readUntilNextSpace(expr, cursor, logic);

    trim(expr, cursor);
    rightHalf = expr.substr(cursor);

    int dummy1 = 0, dummy2 = 0;

    switch (logicalOperators.at(logic))
    {
    case LESS_THAN:
        return (solveForText(leftHalf, dummy1) < solveForText(rightHalf, dummy2));
    case GREATER_THAN:
        return (solveForText(leftHalf, dummy1) > solveForText(rightHalf, dummy2));
    case EQUAL_TO:
        return (solveForText(leftHalf, dummy1).compare(solveForText(rightHalf, dummy2)) == 0);
    case NOT_EQUAL_TO:
        return (solveForText(leftHalf, dummy1).compare(solveForText(rightHalf, dummy2)) != 0);
    }
}

int checkRealCondition(string &expr)
{
    // leftHalf is logic rightHalf
    int cursor = 0;
    trim(expr, cursor);

    string logic = "";
    string leftHalf, rightHalf;

    findKeyword(expr, cursor, IS);
    leftHalf = expr.substr(0, cursor);

    cursor += keywordsToString.at(IS).length();
    trim(expr, cursor);

    readUntilNextSpace(expr, cursor, logic);

    trim(expr, cursor);
    rightHalf = expr.substr(cursor);

    int dummy1 = 0, dummy2 = 0;

    switch (logicalOperators.at(logic))
    {
    case LESS_THAN:
        return (solveForReal(leftHalf, dummy1) < solveForReal(rightHalf, dummy2));
    case GREATER_THAN:
        return (solveForReal(leftHalf, dummy1) > solveForReal(rightHalf, dummy2));
    case EQUAL_TO:
        return (solveForReal(leftHalf, dummy1) == solveForReal(rightHalf, dummy2));
    case NOT_EQUAL_TO:
        return (solveForReal(leftHalf, dummy1) != solveForReal(rightHalf, dummy2));
    }
}

int checkCondition(string &expr)
{
    int cursor = 0;
    trim(expr, cursor);
    if (expr[cursor] == '\0')
    {
        return 0;
    }

    string temp = "";
    readUntilNextSpace(expr, cursor, temp);

    if (isString(temp))
    {
        return checkTextCondition(expr);
    }
    if (isNumber(temp))
    {
        if (isReal(temp))
        {
            return checkRealCondition(expr);
        }
        return checkIntegralCondition(expr);
    }

    Multitype var = getVariable(temp);
    if (var.type != -1)
    {
        if (var.type == INTEGER)
        {
            return checkIntegralCondition(expr);
        }
        else if (var.type == TEXT)
        {
            return checkTextCondition(expr);
        }
        else if (var.type == REAL)
        {
            return checkRealCondition(expr);
        }
    }
    else
    {
        printError(VARIABLE_NOT_FOUND);
        exit(0);
    }
}

void initiate(string &expr)
{
    // let dataType name is value
    // let dataType name
    // let dataType-array name-size

    int type = -1;
    string name = "";
    string temp = "";

    int cursor = 0;

    // To remove the extra "let" keyword from the beginning
    trim(expr, cursor);
    readUntilNextSpace(expr, cursor, temp);
    trim(expr, cursor);

    // Read the data type
    temp = "";
    readUntilNextSpace(expr, cursor, temp);
    if (temp == dataTypeToString.at(INTEGER))
    {
        type = INTEGER;
    }
    else if (temp == dataTypeToString.at(TEXT))
    {
        type = TEXT;
    }
    else if (temp == dataTypeToString.at(REAL))
    {
        type = REAL;
    }
    else if (temp == dataTypeToString.at(INTEGER_ARRAY))
    {
        type = INTEGER_ARRAY;
    }
    else if (temp == dataTypeToString.at(TEXT_ARRAY))
    {
        type = TEXT_ARRAY;
    }
    else if (temp == dataTypeToString.at(REAL_ARRAY))
    {
        type = REAL_ARRAY;
    }
    else
    {
        printError(INVALID_DATA_TYPE);
        exit(0);
    }

    // Read the name of the variable
    temp = "";
    trim(expr, cursor);
    readUntilNextSpace(expr, cursor, temp);

    // If the variable is an array
    int size = -1;
    if (type == INTEGER_ARRAY || type == TEXT_ARRAY || type == REAL_ARRAY)
    {
        string arrayName;
        for (int i = 0; i < temp.length(); i++)
        {
            // Seperate name and size the part before '-' is the name and after is the size
            if (temp[i] == '-')
            {
                arrayName = temp.substr(0, i);
                if (validateName(arrayName) == 0)
                {
                    printError(INVALID_NAME);
                    exit(0);
                }

                string indexString = temp.substr(temp.find('-') + 1);
                int dummy = 0;
                int index = solveForInteger(indexString, dummy);
                size = index;
                break;
            }

            // If the size is not provided then set the size to default
            if (i == temp.length() - 1)
            {
                arrayName = temp;
                size = DEFAULT_ARRAY_SIZE;
            }
        }
        if (type == INTEGER_ARRAY)
        {
            integerArrays[arrayName] = vector<int>(size, DEFAULT_INTEGER_VALUE);
        }
        else if (type == TEXT_ARRAY)
        {
            textArrays[arrayName] = vector<string>(size, DEFAULT_TEXT_VALUE);
        }
        else if (type == REAL_ARRAY)
        {
            realArrays[arrayName] = vector<double>(size, DEFAULT_REAL_VALUE);
        }
        return;
    }

    name = temp;
    if (validateName(name) == 0)
    {
        printError(INVALID_NAME);
        exit(0);
    }

    // If the value is not provided
    temp = "";
    trim(expr, cursor);
    if (expr[cursor] == '\0')
    {
        switch (type)
        {
        case INTEGER:
            integerVariables[name] = DEFAULT_INTEGER_VALUE;
            break;
        case TEXT:
            textVariables[name] = DEFAULT_TEXT_VALUE;
            break;
        case REAL:
            realVariables[name] = DEFAULT_REAL_VALUE;
            break;
        }
        return;
    }

    // Read the IS keyword
    readUntilNextSpace(expr, cursor, temp);
    if (temp == keywordsToString.at(IS))
    {
        cursor++;
        temp = "";
        trim(expr, cursor);
    }
    else
    {
        printError(INVALID_SYNTAX);
        exit(0);
    }

    // Initiate the variable with the value
    if (type == INTEGER)
    {
        integerVariables[name] = solveForInteger(expr, cursor);
    }
    else if (type == TEXT)
    {
        textVariables[name] = solveForText(expr, cursor);
    }
    else if (type == REAL)
    {
        realVariables[name] = solveForReal(expr, cursor);
    }
}

void printOutput(string &expr, int &position)
{
    // print "a: " then a

    trim(expr, position);
    if (expr[position] == '\0')
    {
        return;
    }

    string temp = "";
    readUntilNextSpace(expr, position, temp);

    // If the value to be printed is a string literal print is without quotes
    if (isString(temp))
    {
        cout << temp.substr(1, temp.length() - 2);
    }

    // If the value to be printed is variable
    else
    {
        readUntilNextSpace(expr, position, temp);
        Multitype var = getVariable(temp);
        if (var.type != -1)
        {
            if (var.type == INTEGER)
            {
                cout << *var.integer;
            }
            else if (var.type == TEXT)
            {
                cout << *var.text;
            }
            else if (var.type == REAL)
            {
                cout << *var.real;
            }
        }
        else
        {
            printError(VARIABLE_NOT_FOUND);
            exit(0);
        }
    }

    temp = "";
    trim(expr, position);
    if (expr[position] == '\0')
    {
        return;
    }

    // Check if the printing is chained
    readUntilNextSpace(expr, position, temp);
    if (temp == keywordsToString.at(THEN))
    {
        printOutput(expr, position);
    }

    else
    {
        printError(INVALID_SYNTAX);
        exit(0);
    }
}

void endProgram()
{
    exit(0);
}

void readFromUser(string &expr, int &position)
{
    trim(expr, position);
    if (expr[position] == '\0')
    {
        return;
    }

    // Get the variable name
    string temp = "";
    readUntilNextSpace(expr, position, temp);

    // Get the address of variable in Multitype and read the value from user
    Multitype var = getVariable(temp);
    if (var.type != -1)
    {
        if (var.type == INTEGER)
        {
            cin >> *var.integer;
        }
        else if (var.type == TEXT)
        {
            cin >> *var.text;
        }
        else if (var.type == REAL)
        {
            cin >> *var.real;
        }
    }
    else
    {
        printError(VARIABLE_NOT_FOUND);
        exit(0);
    }

    trim(expr, position);
    if (expr[position] == '\0')
    {
        return;
    }

    // Check if the reading is chained
    temp = "";
    readUntilNextSpace(expr, position, temp);
    if (temp == keywordsToString.at(THEN))
    {
        readFromUser(expr, position);
    }
    else
    {
        printError(INVALID_SYNTAX);
        exit(0);
    }
}

void printError(int errCode)
{
    cout << "Error at line : " << currentLine << endl;
    cout << errorMassage.at(errCode) << endl;
}

// Utility Functions

int power(int number, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result *= number;
    }
    return result;
}

// Move the cursor to the first character
void trim(string &str, int &position)
{
    while (str[position] == ' ' && str[position] != '\0')
    {
        position++;
    }
}

// Read the expression until the next space
void readUntilNextSpace(string &expr, int &position, string &temp)
{
    char checkFor = ' ';
    if (expr[position] == '"')
    {
        checkFor = '"';
        temp += expr[position];
        position++;
    }
    while (expr[position] != checkFor && expr[position] != '\0')
    {
        temp += expr[position];
        position++;
    }
    if (checkFor == '"' && expr[position] != '\0')
    {
        temp += expr[position];
        position++;
    }
}

// Check if the next expression exists (AND keyword)
int nextExpressionExists(string &expr, int &position)
{
    return findKeyword(expr, position, AND);
}

// Find a specific keyword in the expression and move the cursor to the first character of the keyword
int findKeyword(string &expr, int &position, int keyword)
{
    while (position < expr.length() - keywordsToString.at(keyword).length())
    {
        if (expr[position] == '"')
        {
            position++;
            while (expr[position] != '"' && position < expr.length())
            {
                position++;
            }
            position++;
        }
        if (expr.substr(position, keywordsToString.at(keyword).length()) == keywordsToString.at(keyword))
        {
            if ((position == 0 || expr[position - 1] == ' ') && (expr[position + keywordsToString.at(keyword).length()] == ' ' || expr[position + keywordsToString.at(keyword).length()] == '\0'))
                return true;
        }
        position++;
    }
    return false;
}

// Check if the string is a number
int isNumber(string &str)
{
    int dotCount = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            if (str[i] == '-' && i == 0)
            {
                continue;
            }
            if (str[i] == '.' && dotCount == 0)
            {
                dotCount++;
                continue;
            }
            return false;
        }
    }
    return true;
}

// Check if the string is a string literal (Enclosed in double quotes)
int isString(string &str)
{
    if (str[0] == '"' && str[str.length() - 1] == '"')
    {
        return true;
    }
    return false;
}

// Check if the string is a real number
int isReal(string &str)
{
    int dotCount = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            if (str[i] == '-' && i == 0)
            {
                continue;
            }
            if (str[i] == '.' && dotCount == 0)
            {
                dotCount++;
                continue;
            }
            return false;
        }
    }
    if (dotCount == 1)
    {
        return true;
    }
    return false;
}

// Validate the name of the variable
int validateName(string name)
{
    // Name cannot be empty
    if (name[0] == '\n')
        return false;

    // Name cannot start with a number
    if (name[0] >= '0' && name[0] <= '0')
        return false;

    // Name can only contain alphabets and numbers
    for (int i = 0; i < name.length(); i++)
    {
        if (name[i] >= '0' && name[i] <= '9')
            continue;
        if (name[i] >= 'a' && name[i] <= 'z')
            continue;
        if (name[i] >= 'A' && name[i] <= 'Z')
            continue;
        return false;
    }

    // Name cannot be a reserved keyword
    for (auto reserved : reservedWords)
    {
        if (name.compare(reserved) == 0)
            return false;
    }
    return true;
}

Multitype getVariable(string varName)
{
    // If the variable is an array
    Multitype variable;
    if (varName.find('-') != string::npos)
    {
        // Seperate the name and index
        string arrayName = varName.substr(0, varName.find('-'));
        string indexString = varName.substr(varName.find('-') + 1);

        int dummy = 0;
        int index = solveForInteger(indexString, dummy);

        // Check the type of the array and store the address of the variable
        if (integerArrays.find(arrayName) != integerArrays.end())
        {
            if (index >= 0 && index < integerArrays[arrayName].size())
            {
                variable.integer = &integerArrays[arrayName][index];
                variable.type = INTEGER;
            }

            // If the index is out of bounds
            else
            {
                printError(INDEX_OUT_OF_BOUNDS);
                exit(0);
            }
        }
        else if (realArrays.find(arrayName) != realArrays.end())
        {
            if (index >= 0 && index < realArrays[arrayName].size())
            {
                variable.real = &realArrays[arrayName][index];
                variable.type = REAL;
            }
            else
            {
                printError(INDEX_OUT_OF_BOUNDS);
                exit(0);
            }
        }
        else if (textArrays.find(arrayName) != textArrays.end())
        {
            if (index >= 0 && index < textArrays[arrayName].size())
            {
                variable.text = &textArrays[arrayName][index];
                variable.type = TEXT;
            }
            else
            {
                printError(INDEX_OUT_OF_BOUNDS);
                exit(0);
            }
        }
    }
    else if (integerVariables.find(varName) != integerVariables.end())
    {
        variable.integer = &integerVariables[varName];
        variable.type = INTEGER;
    }
    else if (textVariables.find(varName) != textVariables.end())
    {
        variable.text = &textVariables[varName];
        variable.type = TEXT;
    }
    else if (realVariables.find(varName) != realVariables.end())
    {
        variable.real = &realVariables[varName];
        variable.type = REAL;
    }
    else
    {
        variable.type = -1;
    }
    return variable;
}

// Preprocessor functions

void preprocess(string fileName)
{
    // Open the file in which the preprocessed code will be stored
    ofstream processedProgram(preprocessedFileName);

    // If the file is opened successfully
    if (processedProgram.is_open())
    {
        // Open the program file
        ifstream program(fileName);

        string line, modifiedLine = "";

        // Read the file line by line
        while (getline(program, line))
        {
            modifiedLine = "";
            int cursor = 0;
            trim(line, cursor);

            // skip the line if it is empty
            if (line[cursor] == '\0')
            {
                continue;
            }

            // Skip comments
            if (line.length() >= keywordsToString.at(LEAVE).length() && line.substr(cursor, keywordsToString.at(LEAVE).length()) == keywordsToString.at(LEAVE))
            {
                continue;
            }

            // Read the line character by character
            while (cursor < line.length())
            {
                // Ignore string literalts (part inside double quotes)
                if (line[cursor] == '"')
                {
                    modifiedLine.push_back(line[cursor]);
                    cursor++;

                    while (line[cursor] != '"' && cursor < line.length())
                    {
                        modifiedLine.push_back(line[cursor]);
                        cursor++;
                    }

                    if (cursor == line.length())
                    {
                        break;
                    }

                    else
                    {
                        modifiedLine.push_back(line[cursor]);
                        cursor++;
                    }
                }

                // Append the character to the modified line if it is not a space
                else if (line[cursor] != ' ')
                {
                    modifiedLine.push_back(line[cursor]);
                    cursor++;
                }

                // Append only one space if there are multiple spaces continuously
                else if (modifiedLine.length() > 0 && modifiedLine[modifiedLine.length() - 1] != ' ')
                {
                    modifiedLine.push_back(' ');
                    cursor++;
                }
                else
                    cursor++;
            }
            // Remove the last space if it exists
            if (modifiedLine.back() == ' ')
            {
                modifiedLine.pop_back();
            }

            processedProgram << modifiedLine;

            // Read the last word of the line
            string lastWord = "";
            if (!modifiedLine.empty())
            {
                size_t lastSpace = modifiedLine.find_last_of(' ');
                if (lastSpace != string::npos)
                {
                    lastWord = modifiedLine.substr(lastSpace + 1);
                }

                // If there is only one word in the line
                else
                {
                    lastWord = modifiedLine;
                }
            }

            // If the last word is not AND, THEN, ELSE, DO then chain the next line with the current line
            if (lastWord.compare(keywordsToString.at(AND)) != 0 && lastWord.compare(keywordsToString.at(THEN)) != 0 && lastWord.compare(keywordsToString.at(ELSE)) != 0 && lastWord.compare(keywordsToString.at(DO)) != 0)
            {
                processedProgram << endl;
            }

            else
                processedProgram << " ";
        }
        program.close();
        processedProgram.close();

        // Open the preprocessed file to preload the subroutines
        program.open(preprocessedFileName);
        while (getline(program, line))
        {
            string temp = "";
            int cursor = 0;

            // Filter the lines which start with the keyword SUBROUTINE
            trim(line, cursor);
            readUntilNextSpace(line, cursor, temp);
            if (temp.compare(keywordsToString.at(SUBROUTINE)) == 0)
            {
                string subroutineName = "";
                trim(line, cursor);

                // Read the name of the subroutine
                readUntilNextSpace(line, cursor, subroutineName);

                // Store the code of the subroutine
                vector<string> subroutineCode;
                while (getline(program, line))
                {
                    if (line == keywordsToString.at(END))
                    {
                        break;
                    }
                    subroutineCode.push_back(line);
                }
                subroutines[subroutineName] = subroutineCode;
            }
        }
    }
}

void configure()
{
    using json = nlohmann::json;

    // Open the configuration file
    ifstream configFile(configFileName);
    if (configFile.fail())
    {
        return;
    }

    // Store the data of the configuration file in a string
    string configurations((istreambuf_iterator<char>(configFile)), istreambuf_iterator<char>());
    configFile.close();

    // Parse the configuration file using nolhmann json library
    json config = json::parse(configurations);

    // Cofigure I/O files
    string inputFile = config["io"]["inputFile"];
    string outputFile = config["io"]["outputFile"];
    string errorFile = config["io"]["errorFile"];
    preprocessedFileName = config["io"]["preProcessedFile"];
    if (inputFile.compare("stdin") != 0)
    {
        freopen(inputFile.c_str(), "r", stdin);
    }
    if (outputFile.compare("stdout") != 0)
    {
        freopen(outputFile.c_str(), "w", stdout);
    }
    if (errorFile.compare("stderr") != 0)
    {
        freopen(errorFile.c_str(), "w", stderr);
    }

    // Configure the keywords, data types, operators and error messages
    keywords[config["keywords"]["let"]] = LET;
    keywords[config["keywords"]["is"]] = IS;
    keywords[config["keywords"]["if"]] = IF;
    keywords[config["keywords"]["else"]] = ELSE;
    keywords[config["keywords"]["then"]] = THEN;
    keywords[config["keywords"]["skip"]] = SKIP;
    keywords[config["keywords"]["stop"]] = STOP;
    keywords[config["keywords"]["do"]] = DO;
    keywords[config["keywords"]["while"]] = WHILE;
    keywords[config["keywords"]["escape"]] = ESCAPE;
    keywords[config["keywords"]["recheck"]] = RECHECK;
    keywords[config["keywords"]["tillhere"]] = TILLHERE;
    keywords[config["keywords"]["and"]] = AND;
    keywords[config["keywords"]["goto"]] = GOTO;
    keywords[config["keywords"]["read"]] = READ;
    keywords[config["keywords"]["print"]] = PRINT;
    keywords[config["keywords"]["newline"]] = NEWLINE;
    keywords[config["keywords"]["leave"]] = LEAVE;
    keywords[config["keywords"]["end"]] = END;
    keywords[config["keywords"]["subroutine"]] = SUBROUTINE;

    keywordsToString[LET] = config["keywords"]["let"];
    keywordsToString[IS] = config["keywords"]["is"];
    keywordsToString[IF] = config["keywords"]["if"];
    keywordsToString[ELSE] = config["keywords"]["else"];
    keywordsToString[THEN] = config["keywords"]["then"];
    keywordsToString[SKIP] = config["keywords"]["skip"];
    keywordsToString[STOP] = config["keywords"]["stop"];
    keywordsToString[DO] = config["keywords"]["do"];
    keywordsToString[WHILE] = config["keywords"]["while"];
    keywordsToString[ESCAPE] = config["keywords"]["escape"];
    keywordsToString[RECHECK] = config["keywords"]["recheck"];
    keywordsToString[TILLHERE] = config["keywords"]["tillhere"];
    keywordsToString[AND] = config["keywords"]["and"];
    keywordsToString[GOTO] = config["keywords"]["goto"];
    keywordsToString[READ] = config["keywords"]["read"];
    keywordsToString[PRINT] = config["keywords"]["print"];
    keywordsToString[NEWLINE] = config["keywords"]["newline"];
    keywordsToString[LEAVE] = config["keywords"]["leave"];
    keywordsToString[END] = config["keywords"]["end"];
    keywordsToString[SUBROUTINE] = config["keywords"]["subroutine"];

    dataTypes[config["datatypes"]["integer"]] = INTEGER;
    dataTypes[config["datatypes"]["text"]] = TEXT;
    dataTypes[config["datatypes"]["real"]] = REAL;
    dataTypes[config["datatypes"]["integerArray"]] = INTEGER_ARRAY;
    dataTypes[config["datatypes"]["textArray"]] = TEXT_ARRAY;
    dataTypes[config["datatypes"]["realArray"]] = REAL_ARRAY;

    dataTypeToString[INTEGER] = config["datatypes"]["integer"];
    dataTypeToString[TEXT] = config["datatypes"]["text"];
    dataTypeToString[REAL] = config["datatypes"]["real"];
    dataTypeToString[INTEGER_ARRAY] = config["datatypes"]["integerArray"];
    dataTypeToString[TEXT_ARRAY] = config["datatypes"]["textArray"];
    dataTypeToString[REAL_ARRAY] = config["datatypes"]["realArray"];

    mathOperators[config["operators"]["math"]["plus"]] = PLUS;
    mathOperators[config["operators"]["math"]["minus"]] = MINUS;
    mathOperators[config["operators"]["math"]["into"]] = INTO;
    mathOperators[config["operators"]["math"]["upon"]] = UPON;
    mathOperators[config["operators"]["math"]["modulo"]] = MODULO;
    mathOperators[config["operators"]["math"]["exponent"]] = EXPONENT;

    logicalOperators[config["operators"]["logic"]["equals"]] = EQUAL_TO;
    logicalOperators[config["operators"]["logic"]["notEquals"]] = NOT_EQUAL_TO;
    logicalOperators[config["operators"]["logic"]["greaterThan"]] = GREATER_THAN;
    logicalOperators[config["operators"]["logic"]["lessThan"]] = LESS_THAN;

    errorMassage[INVALID_SYNTAX] = config["errorMessages"]["invalidSyntax"];
    errorMassage[INVALID_DATA_TYPE] = config["errorMessages"]["invalidDatatype"];
    errorMassage[INVALID_OPERATOR] = config["errorMessages"]["invalidOperator"];
    errorMassage[INDEX_OUT_OF_BOUNDS] = config["errorMessages"]["indexOutOfBounds"];
    errorMassage[INVALID_NAME] = config["errorMessages"]["invalidVariableName"];
    errorMassage[VARIABLE_NOT_FOUND] = config["errorMessages"]["variableNotFound"];

    DEFAULT_ARRAY_SIZE = config["defaults"]["arraySize"];
    DEFAULT_INTEGER_VALUE = config["defaults"]["integerValue"];
    DEFAULT_REAL_VALUE = config["defaults"]["realValue"];
    DEFAULT_TEXT_VALUE = config["defaults"]["textValue"];

    // Store the reserved words in a vector to validate the variable names
    reservedWords = {};
    for (auto pair : keywordsToString)
    {
        reservedWords.push_back(pair.second);
    }
    for (auto pair : dataTypeToString)
    {
        reservedWords.push_back(pair.second);
    }
    for (auto pair : mathOperators)
    {
        reservedWords.push_back(pair.first);
    }
    for (auto pair : logicalOperators)
    {
        reservedWords.push_back(pair.first);
    }
}

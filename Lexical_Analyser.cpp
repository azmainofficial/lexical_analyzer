#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cctype>

using namespace std;

// List of preprocessor directives
const char* preprocessorDirectives[] = {
    "include", "define", "undef", "if", "ifdef", "ifndef", "else", "elif",
    "endif", "line", "error", "pragma"
};

// List of existing libraries
const char* existingLibraries[] = {
    "iostream", "fstream", "sstream", "vector", "list", "deque", "array", "set",
    "map", "unordered_set", "unordered_map", "stack", "queue", "bitset",
    "algorithm", "numeric", "iterator", "string", "cstring", "utility",
    "functional", "tuple", "memory", "chrono", "ctime", "random", "type_traits",
    "exception", "stdexcept", "thread", "mutex", "future", "filesystem",
    "cstddef", "cstdint", "climits", "cfloat", "cmath", "cstdlib", "Boost.Asio",
    "Boost.Filesystem", "Boost.Regex", "Boost.Thread", "Eigen", "Qt", "POCO",
    "OpenCV", "PCL", "wxWidgets", "TBB", "Cereal", "GLM", "SDL", "SFML",
    "Google Test", "Catch2", "fmt"
};

// List of C++ keywords
const char* keywords[] = {
    "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit",
    "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch",
    "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const",
    "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await",
    "co_return", "co_yield", "decltype", "default", "delete", "do", "double",
    "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
    "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace",
    "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq",
    "private", "protected", "public", "register", "reinterpret_cast", "requires",
    "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast",
    "struct", "switch", "synchronized", "template", "this", "thread_local", "throw",
    "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq", "asm", "size_t"
};

// List of separators
const char separators[] = { ',', ';' };

// List of brackets
const char brackets[] = { '(', ')', '{', '}', '[', ']' };

// List of operators
const char operators[] = { '+', '-', '*', '/', '%', '=', '<', '>', '!', '&', '|', '^', '~' };

// Function to check if a character is a separator
bool isSeparator(char ch) {
    for (char sep : separators) {
        if (ch == sep) {
            return true;
        }
    }
    return false;
}

// Function to check if a character is a bracket
bool isBracket(char ch) {
    for (char br : brackets) {
        if (ch == br) {
            return true;
        }
    }
    return false;
}

// Function to check if a character is an operator
bool isOperator(char ch) {
    for (char op : operators) {
        if (ch == op) {
            return true;
        }
    }
    return false;
}

// Function to check if a word is a preprocessor directive
bool isPreprocessorKeyword(const char* ptr) {
    int arraySize = sizeof(preprocessorDirectives) / sizeof(preprocessorDirectives[0]);
    for (int i = 0; i < arraySize; ++i) {
        if (strncmp(ptr, preprocessorDirectives[i], strlen(preprocessorDirectives[i])) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a word is an existing library
bool isExistingLibrary(const char* libraryName) {
    size_t numLibraries = sizeof(existingLibraries) / sizeof(existingLibraries[0]);
    for (size_t i = 0; i < numLibraries; ++i) {
        if (std::strcmp(libraryName, existingLibraries[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a word is a C++ keyword
bool isCPlusPlusKeyword(const char* word) {
    size_t numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < numKeywords; ++i) {
        if (std::strcmp(word, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a word is a valid C++ variable name
bool isValidVariableName(const char* word) {
    if (isCPlusPlusKeyword(word)) {
        return false;
    }
    if (!std::isalpha(word[0]) && word[0] != '_') {
        return false;
    }
    for (size_t i = 1; i < strlen(word); ++i) {
        if (!std::isalnum(word[i]) && word[i] != '_') {
            return false;
        }
    }
    return true;
}

// Function to check if a string is a numeric value
bool isNumericValue(const std::string& str) {
    bool hasDecimal = false;
    for (char ch : str) {
        if (!std::isdigit(ch)) {
            if (ch == '.' && !hasDecimal) {
                hasDecimal = true;
            } else {
                return false;
            }
        }
    }
    return !str.empty();
}

int main() {
    int keywordCount = 0;
    int libraryCount = 0;
    int variableCount = 0;
    int separatorCount = 0;
    int bracketCount = 0;
    int operatorCount = 0;
    int numericValueCount = 0;

    string filePath = "Input.txt";
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    bool inString = false;

    while (getline(file, line)) {
        const char* charArray = line.c_str();
        const char* ptr = charArray;

        while (*ptr != '\0') {
            // Handle preprocessor directives
            if (*ptr == '#') {
                while (*(++ptr) == ' ') {}
                if (isPreprocessorKeyword(ptr)) {
                    ++keywordCount;
                }
            }
            // Handle existing libraries
            else if (*ptr == '<') {
                const char* endBracket = strchr(ptr, '>');
                if (endBracket) {
                    string libraryName(ptr + 1, endBracket - ptr - 1);
                    if (isExistingLibrary(libraryName.c_str())) {
                        ++libraryCount;
                    }
                    ptr = endBracket + 1;
                } else {
                    ++ptr;
                }
            }
            // Handle string literals
            else if (*ptr == '"') {
                inString = !inString;
                ++ptr;
            }
            // Handle separators, brackets, and operators
            else if (!inString && isSeparator(*ptr)) {
                ++separatorCount;
                ++ptr;
            }
            else if (!inString && isBracket(*ptr)) {
                ++bracketCount;
                ++ptr;
            }
            else if (!inString && isOperator(*ptr)) {
                ++operatorCount;
                ++ptr;
            }
            // Handle C++ keywords, variable names, and numeric values
            else if (!inString && (isalpha(*ptr) || *ptr == '_')) {
                const char* startWord = ptr;
                while (isalnum(*ptr) || *ptr == '_') {
                    ++ptr;
                }
                string word(startWord, ptr - startWord);
                if (isCPlusPlusKeyword(word.c_str())) {
                    ++keywordCount;
                } else if (isValidVariableName(word.c_str())) {
                    ++variableCount;

                    // Check for numeric value assignment
                    const char* nextPtr = ptr;
                    while (*nextPtr == ' ' || *nextPtr == '=') {
                        if (*nextPtr == '=') {
                            ++nextPtr;
                            while (*nextPtr == ' ') ++nextPtr;
                            const char* startValue = nextPtr;
                            while (isdigit(*nextPtr) || *nextPtr == '.') ++nextPtr;
                            string value(startValue, nextPtr - startValue);
                            if (isNumericValue(value)) {
                                ++numericValueCount;
                            }
                            break;
                        }
                        ++nextPtr;
                    }
                }
            }
            else {
                ++ptr;
            }
        }
    }

    file.close();

    cout << "Keyword count: " << keywordCount << endl;
    cout << "Library count: " << libraryCount << endl;
    cout << "Variable count: " << variableCount << endl;
    cout << "Separator count: " << separatorCount << endl;
    cout << "Bracket count: " << bracketCount << endl;
    cout << "Operator count: " << operatorCount << endl;
    cout << "Numeric value count: " << numericValueCount << endl;

    return 0;
}

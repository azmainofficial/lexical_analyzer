#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// this function is to check is the word is a keyword or not
bool isKeyword(const string& word) {
    // if this word match with any keyword then return True
    return word == "alignas" || word == "alignof" || word == "and" || word == "and_eq" || word == "asm" ||
           word == "auto" || word == "bitand" || word == "bitor" || word == "bool" || word == "break" ||
           word == "case" || word == "catch" || word == "char" || word == "char8_t" || word == "char16_t" ||
           word == "char32_t" || word == "class" || word == "compl" || word == "concept" || word == "const" ||
           word == "consteval" || word == "constexpr" || word == "constinit" || word == "const_cast" ||
           word == "continue" || word == "co_await" || word == "co_return" || word == "co_yield" ||
           word == "decltype" || word == "default" || word == "delete" || word == "do" || word == "double" ||
           word == "dynamic_cast" || word == "else" || word == "enum" || word == "explicit" || word == "export" ||
           word == "extern" || word == "false" || word == "float" || word == "for" || word == "friend" ||
           word == "goto" || word == "if" || word == "inline" || word == "int" || word == "long" ||
           word == "mutable" || word == "namespace" || word == "new" || word == "noexcept" || word == "not" ||
           word == "not_eq" || word == "nullptr" || word == "operator" || word == "or" || word == "or_eq" ||
           word == "private" || word == "protected" || word == "public" || word == "register" || word == "reinterpret_cast" ||
           word == "requires" || word == "return" || word == "short" || word == "signed" || word == "sizeof" ||
           word == "static" || word == "static_assert" || word == "static_cast" || word == "struct" || word == "switch" ||
           word == "template" || word == "this" || word == "thread_local" || word == "throw" || word == "true" ||
           word == "try" || word == "typedef" || word == "typeid" || word == "typename" || word == "union" ||
           word == "unsigned" || word == "using" || word == "virtual" || word == "void" || word == "volatile" ||
           word == "wchar_t" || word == "while" || word == "xor" || word == "xor_eq";
}

// this function is to check is the word is a identifier or not
bool isIdentifier(const string& word) {
    // if its a valid identifier then return true
    return regex_match(word, regex("[a-zA-Z_][a-zA-Z0-9_]*"));
}

bool isConstant(const string& word) {
    bool decimalPoint = false; // Flag to indicate if a decimal point has been encountered
    bool digitFound = false;   // Flag to indicate if a digit has been found
    
    for (size_t i = 0; i < word.size(); ++i) { // Iterate through each character in the string
        if (isdigit(word[i])) {                // Check if the current character is a digit
            digitFound = true;                 // Set digitFound to true if a digit is found
        } else if (word[i] == '.' && !decimalPoint) { // Check if the current character is a decimal point and if it's the first one encountered
            decimalPoint = true;               // Set decimalPoint to true if a decimal point is found
        } else {                               // If the current character is neither a digit nor a valid decimal point
            return false;                      // Return false as the string is not a constant
        }
    }
    return digitFound;                         // Return true if at least one digit was found, indicating a valid constant
}

// this function is to check is the word is a operator or not
bool isOperator(const string& word) {
        // if this word match with any operator then return True
    return word == "+" || word == "-" || word == "*" || word == "/" || word == "=" || word == "<<" || word == ">>" || 
           word == ">" || word == "<" || word == ">=" || word == "<=" || word == "==" || word == "!=";
}

// Check if the given string is a punctuation character
bool isPunctuation(const string& word) {
    return word == ";" || word == "," || word == "." || word == "!" || word == "?"; // Add more punctuation as needed
}

// Check if the given string is a parenthesis character
bool isParenthesis(const string& word) {
    return word == "(" || word == ")" || word == "{" || word == "}" || word == "[" || word == "]";
}

// Check if the given string is a string literal
bool isStringLiteral(const string& word) {
    return regex_match(word, regex("\".*\""));
}

// Check if the given string is a preprocessor directive
bool isPreprocessor(const string& word) {
    return !word.empty() && word[0] == '#'; // Ensure the word is not empty before checking the first character
}

// Add a token to the tokens vector if it's not empty
void addToken(vector<string>& tokens, const string& token) {
    if (!token.empty()) {
        tokens.push_back(token);
    }
}


// this function is to check odd cases like (a+b) where multiple tokens are in a same word
void handleUnknownToken(const string& token, vector<string>& identifiers, vector<string>& operators, vector<string>& constants, vector<string>& punctuations, vector<string>& parenthesis) {
    string current; // Temporary string to store characters that could form an identifier or constant

    for (char c : token) { // Iterate through each character in the token
        if (isalnum(c) || c == '_') { // Check if the character is alphanumeric or an underscore
            current += c; // Add the character to the current string
        } else { // If the character is not alphanumeric or an underscore
            if (!current.empty()) { // If the current string is not empty
                if (isIdentifier(current)) { // Check if the current string is an identifier
                    identifiers.push_back(current); // Add to identifiers
                } else if (isConstant(current)) { // Check if the current string is a constant
                    constants.push_back(current); // Add to constants
                }
                current.clear(); // Clear the current string for the next sequence
            }
            // Check and add the single character as an operator, punctuation, or parenthesis
            if (isOperator(string(1, c))) { 
                operators.push_back(string(1, c));
            } else if (isPunctuation(string(1, c))) {
                punctuations.push_back(string(1, c));
            } else if (isParenthesis(string(1, c))) {
                parenthesis.push_back(string(1, c));
            }
        }
    }
    
    // Handle any remaining characters in the current string after the loop
    if (!current.empty()) {
        if (isIdentifier(current)) {
            identifiers.push_back(current);
        } else if (isConstant(current)) {
            constants.push_back(current);
        }
    }
}


int main() {
    ifstream file("input.txt"); // Open the file input.txt
    string line;

    // Vectors to store different categories of tokens
    vector<string> keywords, identifiers, constants, operators, punctuations, parenthesis, stringLiterals, preProcessors;

    // Read the file line by line
    while (getline(file, line)) {
        stringstream ss(line); // Create a stringstream from the line
        string token;
        // Read each token from the line
        while (ss >> token) {
            // Check and categorize each token
            if (isPreprocessor(token)) {
                preProcessors.push_back(token);
            } else if (isKeyword(token)) {
                keywords.push_back(token);
            } else if (isStringLiteral(token)) {
                stringLiterals.push_back(token);
            } else if (isIdentifier(token)) {
                identifiers.push_back(token);
            } else if (isConstant(token)) {
                constants.push_back(token);
            } else if (isOperator(token)) {
                operators.push_back(token);
            } else if (isPunctuation(token)) {
                punctuations.push_back(token);
            } else if (isParenthesis(token)) {
                parenthesis.push_back(token);
            } else {
                // Handle unknown tokens
                handleUnknownToken(token, identifiers, operators, constants, punctuations, parenthesis);
            }
        }
    }

    // Print the results
    cout << "Keywords: (" << keywords.size() << ") -> ";
    for (const string& kw : keywords) cout << kw << ", ";
    cout << "\nIdentifiers: (" << identifiers.size() << ") -> ";
    for (const string& id : identifiers) cout << id << ", ";
    cout << "\nConstants: (" << constants.size() << ") -> ";
    for (const string& cn : constants) cout << cn << ", ";
    cout << "\nOperators: (" << operators.size() << ") -> ";
    for (const string& op : operators) cout << op << ", ";
    cout << "\nPunctuations: (" << punctuations.size() << ") -> ";
    for (const string& pu : punctuations) cout << pu << ", ";
    cout << "\nParenthesis: (" << parenthesis.size() << ") -> ";
    for (const string& pa : parenthesis) cout << pa << ", ";
    cout << "\nString Literals: (" << stringLiterals.size() << ") -> ";
    for (const string& sl : stringLiterals) cout << sl << ", ";
    cout << "\nPre Processors: (" << preProcessors.size() << ") -> ";
    for (const string& pr : preProcessors) cout << pr << ", ";
    cout << endl;

    return 0;
}


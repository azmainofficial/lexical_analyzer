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

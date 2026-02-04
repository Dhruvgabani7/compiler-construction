#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cctype>

using namespace std;

struct Token {
    string type;
    string value;
    int line;
};

set<string> keywords = {
    "int", "char", "float", "double", "return", "if", "else", "while", "for", "void"
};

set<char> operators = {
    '+', '-', '*', '/', '=', '<', '>', '!'
};

set<char> punctuations = {
    ';', ',', '(', ')', '{', '}', '[', ']'
};

set<string> symbolTable;
vector<Token> tokens;
vector<string> lexicalErrors;

bool isKeyword(const string &str) {
    return keywords.count(str);
}

bool isIdentifier(const string &str) {
    if (!isalpha(str[0]) && str[0] != '_') return false;
    for (char c : str) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

bool isNumber(const string &str) {
    for (char c : str)
        if (!isdigit(c)) return false;
    return true;
}

void lexicalAnalyze(const string &line, int lineNo) {
    int i = 0;
    int n = line.length();

    while (i < n) {
        if (isspace(line[i])) {
            i++;
            continue;
        }

        /* Identifier / Keyword */
        if (isalpha(line[i]) || line[i] == '_') {
            string word;
            while (i < n && (isalnum(line[i]) || line[i] == '_'))
                word += line[i++];

            if (isKeyword(word)) {
                tokens.push_back({"Keyword", word, lineNo});
            } else if (isIdentifier(word)) {
                tokens.push_back({"Identifier", word, lineNo});
                symbolTable.insert(word);
            }
        }

        /* Number or Invalid Constant */
        else if (isdigit(line[i])) {
            string num;
            while (i < n && isalnum(line[i]))
                num += line[i++];

            if (isNumber(num))
                tokens.push_back({"Constant", num, lineNo});
            else
                lexicalErrors.push_back("Line " + to_string(lineNo) +
                    " : " + num + " invalid lexeme");
        }

        /* Character Constant */
        else if (line[i] == '\'') {
            string ch;
            ch += line[i++];
            if (i < n) ch += line[i++];
            if (i < n && line[i] == '\'') {
                ch += line[i++];
                tokens.push_back({"Constant", ch, lineNo});
            } else {
                lexicalErrors.push_back("Line " + to_string(lineNo) +
                    " : invalid character constant");
            }
        }

        /* Operator */
        else if (operators.count(line[i])) {
            tokens.push_back({"Operator", string(1, line[i]), lineNo});
            i++;
        }

        /* Punctuation */
        else if (punctuations.count(line[i])) {
            tokens.push_back({"Punctuation", string(1, line[i]), lineNo});
            i++;
        }

        /* Unknown Character */
        else {
            lexicalErrors.push_back("Line " + to_string(lineNo) +
                " : " + string(1, line[i]) + " invalid character");
            i++;
        }
    }
}

int main() {
    ifstream file("input.c");
    if (!file) {
        cout << "Error opening file\n";
        return 1;
    }

    string line;
    int lineNo = 0;
    bool blockComment = false;

    while (getline(file, line)) {
        lineNo++;

        if (blockComment) {
            if (line.find("*/") != string::npos)
                blockComment = false;
            continue;
        }

        if (line.find("/*") != string::npos) {
            blockComment = true;
            continue;
        }

        if (line.find("//") != string::npos)
            line = line.substr(0, line.find("//"));

        lexicalAnalyze(line, lineNo);
    }

    cout << "\nTOKENS\n";
    for (auto &t : tokens)
        cout << t.type << ": " << t.value << endl;

    cout << "\nLEXICAL ERRORS\n";
    for (auto &e : lexicalErrors)
        cout << e << endl;

    cout << "\nSYMBOL TABLE ENTRIES\n";
    int idx = 1;
    for (auto &s : symbolTable)
        cout << idx++ << ") " << s << endl;

    return 0;
}

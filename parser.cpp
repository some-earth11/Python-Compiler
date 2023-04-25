#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Token class to hold information about each token
class Token {
public:
    string type;
    string value;

    Token(string type, string value) {
        this->type = type;
        this->value = value;
    }
};

// Parser class to parse input tokens and generate an abstract syntax tree (AST)
class Parser {
public:
    vector<Token> tokens;
    int pos;
    Token current_token;

    Parser(vector<Token> tokens) {
        this->tokens = tokens;
        this->pos = 0;
        this->current_token = this->tokens[this->pos];
    }

    // Advance the current position and set the current token
    void advance() {
        this->pos++;
        if (this->pos > this->tokens.size() - 1) {
            this->current_token = Token("EOF", "");
        }
        else {
            this->current_token = this->tokens[this->pos];
        }
    }

    // Verify that the current token matches the expected type and advance to the next token
    void eat(string token_type) {
        if (this->current_token.type == token_type) {
            this->advance();
        }
        else {
            cerr << "Error: Expected " << token_type << ", but found " << this->current_token.type << endl;
            exit(1);
        }
    }

    // Parse an integer expression and return its value
    int parse_integer() {
        Token token = this->current_token;
        this->eat("INTEGER");
        return stoi(token.value);
    }

    // Parse an expression of the form "INTEGER PLUS INTEGER" and return its value
    int parse_expression() {
        int left = this->parse_integer();
        Token op = this->current_token;
        if (op.type == "PLUS") {
            this->eat("PLUS");
        }
        else {
            cerr << "Error: Invalid operator" << endl;
            exit(1);
        }
        int right = this->parse_integer();
        return left + right;
    }

    // Parse the entire program and return its AST
    int parse() {
        return this->parse_expression();
    }
};

int main() {
    string text = "10 + 20";
    Lexer lexer = Lexer(text);
    vector<Token> tokens;
    Token token = lexer.get_next_token();
    while (token.type != "EOF") {
        tokens.push_back(token);
        token = lexer.get_next_token();
    }
    Parser parser = Parser(tokens);
    int result = parser.parse();
    cout << "Result: " << result << endl;
    return 0;
}

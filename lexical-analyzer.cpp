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

// Lexer class to perform lexical analysis on input Python code
class Lexer {
public:
    string text;
    int pos;
    char current_char;

    Lexer(string text) {
        this->text = text;
        this->pos = 0;
        this->current_char = this->text[this->pos];
    }

    // Advance the current position and set the current character
    void advance() {
        this->pos++;
        if (this->pos > this->text.length() - 1) {
            this->current_char = '\0';
        }
        else {
            this->current_char = this->text[this->pos];
        }
    }

    // Skip whitespace characters
    void skip_whitespace() {
        while (this->current_char != '\0' && isspace(this->current_char)) {
            this->advance();
        }
    }

    // Get the next token in the input text
    Token get_next_token() {
        while (this->current_char != '\0') {
            if (isspace(this->current_char)) {
                this->skip_whitespace();
                continue;
            }
            
            if (isdigit(this->current_char)) {
                return Token("INTEGER", this->get_integer());
            }
            
            if (this->current_char == '+') {
                this->advance();
                return Token("PLUS", "+");
            }

            // Handle other token types here

            // If we haven't matched any token type, raise an error
            cerr << "Error: Invalid character" << endl;
            exit(1);
        }

        // If we've reached the end of the input text, return an EOF token
        return Token("EOF", "");
    }

    // Get a sequence of digits representing an integer
    string get_integer() {
        string result = "";
        while (this->current_char != '\0' && isdigit(this->current_char)) {
            result += this->current_char;
            this->advance();
        }
        return result;
    }
};

int main() {
    string text = "10 + 20";
    Lexer lexer = Lexer(text);
    Token token = lexer.get_next_token();
    while (token.type != "EOF") {
        cout << "Token(" << token.type << ", " << token.value << ")" << endl;
        token = lexer.get_next_token();
    }
    return 0;
}
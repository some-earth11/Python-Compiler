#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

// Token class definition
class Token {
public:
    string type;
    string value;
    
    Token(string type, string value) {
        this->type = type;
        this->value = value;
    }
};

// Lexer class definition
class Lexer {
public:
    Lexer(string text) {
        this->text = text;
        this->pos = 0;
        this->current_char = this->text[0];
    }

    void error() {
        throw runtime_error("Invalid character");
    }

    void advance() {
        this->pos++;
        if (this->pos < this->text.length()) {
            this->current_char = this->text[this->pos];
        } else {
            this->current_char = '\0';
        }
    }

    void skip_whitespace() {
        while (this->current_char == ' ') {
            this->advance();
        }
    }

    int integer() {
        int result = 0;
        while (this->current_char >= '0' && this->current_char <= '9') {
            result = result * 10 + (this->current_char - '0');
            this->advance();
        }
        return result;
    }

    Token get_next_token() {
        while (this->current_char != '\0') {
            if (this->current_char == ' ') {
                this->skip_whitespace();
                continue;
            }
            if (this->current_char >= '0' && this->current_char <= '9') {
                return Token("INTEGER", to_string(this->integer()));
            }
            if (this->current_char == '+') {
                this->advance();
                return Token("PLUS", "+");
            }
            if (this->current_char == '-') {
                this->advance();
                return Token("MINUS", "-");
            }
            if (this->current_char == '*') {
                this->advance();
                return Token("MULTIPLY", "*");
            }
            if (this->current_char == '/') {
                this->advance();
                return Token("DIVIDE", "/");
            }
            this->error();
        }
        return Token("EOF", "");
    }

private:
    string text;
    int pos;
    char current_char;
};

// AST class definition
class AST {
public:
    virtual ~AST() {}
    virtual int eval() = 0;
};

// Number AST node class definition
class Num : public AST {
public:
    int value;
    
    Num(int value) {
        this->value = value;
    }

    int eval() {
        return this->value;
    }
};

// Binary operation AST node class definition
class BinOp : public AST {
public:
    Token op;
    AST *left;
    AST *right;
    
    BinOp(Token op, AST *left, AST *right) {
        this->op = op;
        this->left = left;
        this->right = right;
    }

    ~BinOp() {
        delete this->left;
        delete this->right;
    }

    int eval() {
        if (this->op.type == "PLUS") {
            return this->left->eval() + this->right->eval();
        }
        if (this->op.type == "MINUS") {
            return this->left->eval() - this->right->eval();
        }
        if (this->op.type == "MULTIPLY") {
            return this->left->eval() * this->right->eval();
        }
        if (this->op.type == "DIVIDE") {
            if (this->right->eval() == 0) {
                throw runtime_error("Division by zero");
            }
            return this->left->eval() / this->right->eval();
        }
        throw runtime_error("Unknown operator");
    }
};

// Parser class definition
class Parser {
public:
    Parser(Lexer lexer) {
        this->lexer = lexer;
        this->current_token = this->lexer.get_next_token();
    }
    void error() {
    throw runtime_error("Invalid syntax");
}

void eat(string token_type) {
    if (this->current_token.type == token_type) {
        this->current_token = this->lexer.get_next_token();
    } else {
        this->error();
    }
}

AST* factor() {
    Token token = this->current_token;
    if (token.type == "INTEGER") {
        this->eat("INTEGER");
        return new Num(stoi(token.value));
    }
    if (token.type == "LPAREN") {
        this->eat("LPAREN");
        AST* node = this->expr();
        this->eat("RPAREN");
        return node;
    }
    this->error();
    return nullptr;
}

AST* term() {
    AST* node = this->factor();

    while (this->current_token.type == "MULTIPLY" || this->current_token.type == "DIVIDE") {
        Token token = this->current_token;
        if (token.type == "MULTIPLY") {
            this->eat("MULTIPLY");
        } else if (token.type == "DIVIDE") {
            this->eat("DIVIDE");
        }
        node = new BinOp(token, node, this->factor());
    }

    return node;
}

AST* expr() {
    AST* node = this->term();

    while (this->current_token.type == "PLUS" || this->current_token.type == "MINUS") {
        Token token = this->current_token;
        if (token.type == "PLUS") {
            this->eat("PLUS");
        } else if (token.type == "MINUS") {
            this->eat("MINUS");
        }
        node = new BinOp(token, node, this->term());
    }

    return node;
}

AST* parse() {
    return this->expr();
}
private:
    Lexer lexer;
    Token current_token;
};

// Main function
int main() {
    while (true) {
        try {
            string text;
            cout << "calc> ";
            getline(cin, text);
            Lexer lexer(text);
            Parser parser(lexer);
            AST* tree = parser.parse();
            int result = tree->eval();
            cout << result << endl;

            delete tree;
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }
    return 0;
}

// Token types
enum TokenType {
    NUMBER,
    IDENTIFIER,
    ASSIGN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END,
    INVALID
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

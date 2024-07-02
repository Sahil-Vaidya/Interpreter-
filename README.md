# Simple Interpreter
This project is a simple interpreter that tokenizes, parses, and evaluates arithmetic expressions and assignments in a basic scripting language. It includes a lexer, parser, and evaluator to process and execute the input code.

## Features
- Lexer: Tokenizes the input string into tokens such as numbers, identifiers, operators, and parentheses.
- Parser: Builds an Abstract Syntax Tree (AST) from the tokens.
- Evaluator: Evaluates the AST to compute results and handle variable assignments.
- Supports:
    - Arithmetic operations: +, -, *, /
    - Assignment operations: =
    - Parentheses for grouping
    - Variables

## Components
### Token Types
Enumerations representing different types of tokens:

- `NUMBER`
- `IDENTIFIER`
- `ASSIGN`
- `PLUS`
- `MINUS`
- `MULTIPLY`
- `DIVIDE`
- `LPAREN`
- `RPAREN`
- `END`
- `INVALID`

## Token Structure
Structure to hold token type and value:
```bash
struct Token {
    TokenType type;
    string value;
};
```
## Lexer
Class that tokenizes the input string into a vector of tokens.
```bash
class Lexer {
public:
    explicit Lexer(const string &input);
    vector<Token> tokenize();
private:
    string input;
    size_t pos;
    string readNumber();
    string readIdentifier();
};
```

## Abstract Syntax Tree (AST) Nodes
Various node types representing different components of the AST:

- `NUMBER_NODE`
- `IDENTIFIER_NODE`
- `ASSIGN_NODE`
- `PLUS_NODE`
- `MINUS_NODE`
- `MULTIPLY_NODE`
- `DIVIDE_NODE`

## Base Node Class
```bash
struct Node {
    virtual ~Node() = default;
    virtual NodeType getType() const = 0;
};
```
## Number Node
```bash
struct NumberNode : Node {
    int value;
    explicit NumberNode(int value);
    NodeType getType() const override;
};
```
## Identifier Node
```bash
struct IdentifierNode : Node {
    string name;
    explicit IdentifierNode(const string &name);
    NodeType getType() const override;
};
```
## Binary Operation Node
```bash
struct BinaryOpNode : Node {
    NodeType type;
    unique_ptr<Node> left;
    unique_ptr<Node> right;
    BinaryOpNode(NodeType type, unique_ptr<Node> left, unique_ptr<Node> right);
    NodeType getType() const override;
};
```
## Assignment Node
```bash
struct AssignNode : Node {
    unique_ptr<IdentifierNode> left;
    unique_ptr<Node> right;
    AssignNode(unique_ptr<IdentifierNode> left, unique_ptr<Node> right);
    NodeType getType() const override;
};
```

## Parser
Class that builds the AST from the tokens.
```bash
class Parser {
public:
    explicit Parser(const vector<Token> &tokens);
    unique_ptr<Node> parse();
private:
    const vector<Token> &tokens;
    size_t pos;
    unique_ptr<Node> assignment();
    unique_ptr<Node> expression();
    unique_ptr<Node> term();
    unique_ptr<Node> factor();
    void consume(TokenType type);
    Token currentToken() const;
};
```
## Evaluator
Class that evaluates the AST to produce results and handle variable assignments.
```bash
class Evaluator {
public:
    int eval(unique_ptr<Node> &node);
    unordered_map<string, int> getEnv() const;
private:
    unordered_map<string, int> env;
};
```

## How to Run
1. Compile the code using a C++ compiler:
```bash
g++ -o interpreter main.cpp
```
2. Run the compiled executable:
```bash
./interpreter
```
## License
This project is licensed under the MIT License. See the LICENSE file for details.

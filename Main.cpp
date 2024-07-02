#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <memory>

using namespace std;

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

// Lexer class
class Lexer {
public:
    explicit Lexer(const string &input) : input(input), pos(0) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < input.size()) {
            char current = input[pos];
            if (isdigit(current)) {
                tokens.push_back({NUMBER, readNumber()});
            } else if (isalpha(current)) {
                tokens.push_back({IDENTIFIER, readIdentifier()});
            } else if (current == '=') {
                tokens.push_back({ASSIGN, "="});
                pos++;
            } else if (current == '+') {
                tokens.push_back({PLUS, "+"});
                pos++;
            } else if (current == '-') {
                tokens.push_back({MINUS, "-"});
                pos++;
            } else if (current == '*') {
                tokens.push_back({MULTIPLY, "*"});
                pos++;
            } else if (current == '/') {
                tokens.push_back({DIVIDE, "/"});
                pos++;
            } else if (current == '(') {
                tokens.push_back({LPAREN, "("});
                pos++;
            } else if (current == ')') {
                tokens.push_back({RPAREN, ")"});
                pos++;
            } else if (isspace(current)) {
                pos++;
            } else {
                tokens.push_back({INVALID, string(1, current)});
                pos++;
            }
        }
        tokens.push_back({END, ""});
        return tokens;
    }

private:
    string input;
    size_t pos;

    string readNumber() {
        size_t start = pos;
        while (pos < input.size() && isdigit(input[pos])) {
            pos++;
        }
        return input.substr(start, pos - start);
    }

    string readIdentifier() {
        size_t start = pos;
        while (pos < input.size() && isalnum(input[pos])) {
            pos++;
        }
        return input.substr(start, pos - start);
    }
};

// Node types for the AST
enum NodeType {
    NUMBER_NODE,
    IDENTIFIER_NODE,
    ASSIGN_NODE,
    PLUS_NODE,
    MINUS_NODE,
    MULTIPLY_NODE,
    DIVIDE_NODE
};

// Base Node class
struct Node {
    virtual ~Node() = default;
    virtual NodeType getType() const = 0;
};

// Number Node
struct NumberNode : Node {
    int value;
    explicit NumberNode(int value) : value(value) {}
    NodeType getType() const override { return NUMBER_NODE; }
};

// Identifier Node
struct IdentifierNode : Node {
    string name;
    explicit IdentifierNode(const string &name) : name(name) {}
    NodeType getType() const override { return IDENTIFIER_NODE; }
};

// Binary Operation Node
struct BinaryOpNode : Node {
    NodeType type;
    unique_ptr<Node> left;
    unique_ptr<Node> right;
    BinaryOpNode(NodeType type, unique_ptr<Node> left, unique_ptr<Node> right)
        : type(type), left(move(left)), right(move(right)) {}
    NodeType getType() const override { return type; }
};

// Assignment Node
struct AssignNode : Node {
    unique_ptr<IdentifierNode> left;
    unique_ptr<Node> right;
    AssignNode(unique_ptr<IdentifierNode> left, unique_ptr<Node> right)
        : left(move(left)), right(move(right)) {}
    NodeType getType() const override { return ASSIGN_NODE; }
};

// Parser class
class Parser {
public:
    explicit Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    unique_ptr<Node> parse() {
        return assignment();
    }

public:
    const vector<Token> &tokens;
    size_t pos;

    unique_ptr<Node> assignment() {
        auto node = expression();
        if (currentToken().type == ASSIGN) {
            consume(ASSIGN);
            node = make_unique<AssignNode>(
                make_unique<IdentifierNode>(dynamic_cast<IdentifierNode*>(node.release())->name),
                expression());
        }
        return node;
    }

    unique_ptr<Node> expression() {
        auto node = term();
        while (currentToken().type == PLUS || currentToken().type == MINUS) {
            if (currentToken().type == PLUS) {
                consume(PLUS);
                node = make_unique<BinaryOpNode>(PLUS_NODE, move(node), term());
            } else if (currentToken().type == MINUS) {
                consume(MINUS);
                node = make_unique<BinaryOpNode>(MINUS_NODE, move(node), term());
            }
        }
        return node;
    }

    unique_ptr<Node> term() {
        auto node = factor();
        while (currentToken().type == MULTIPLY || currentToken().type == DIVIDE) {
            if (currentToken().type == MULTIPLY) {
                consume(MULTIPLY);
                node = make_unique<BinaryOpNode>(MULTIPLY_NODE, move(node), factor());
            } else if (currentToken().type == DIVIDE) {
                consume(DIVIDE);
                node = make_unique<BinaryOpNode>(DIVIDE_NODE, move(node), factor());
            }
        }
        return node;
    }

    unique_ptr<Node> factor() {
        if (currentToken().type == NUMBER) {
            int value = stoi(currentToken().value);
            consume(NUMBER);
            return make_unique<NumberNode>(value);
        } else if (currentToken().type == IDENTIFIER) {
            string name = currentToken().value;
            consume(IDENTIFIER);
            return make_unique<IdentifierNode>(name);
        } else if (currentToken().type == LPAREN) {
            consume(LPAREN);
            auto node = expression();
            consume(RPAREN);
            return node;
        } else {
            throw runtime_error("Unexpected token: " + currentToken().value);
        }
    }

    void consume(TokenType type) {
        if (currentToken().type == type) {
            pos++;
        } else {
            throw runtime_error("Unexpected token: " + currentToken().value);
        }
    }

    Token currentToken() const {
        if (pos < tokens.size()) {
            return tokens[pos];
        }
        return {END, ""};
    }
};

// Interpreter :It processes the AST directly to execute the code and produce results
class Evaluator {
public:
    int eval(unique_ptr<Node> &node) {
        switch (node->getType()) {
            case NUMBER_NODE:
                return dynamic_cast<NumberNode*>(node.get())->value;
            case IDENTIFIER_NODE:
                return env[dynamic_cast<IdentifierNode*>(node.get())->name];
            case PLUS_NODE: {
                auto opNode = dynamic_cast<BinaryOpNode*>(node.get());
                return eval(opNode->left) + eval(opNode->right);
            }
            case MINUS_NODE: {
                auto opNode = dynamic_cast<BinaryOpNode*>(node.get());
                return eval(opNode->left) - eval(opNode->right);
            }
            case MULTIPLY_NODE: {
                auto opNode = dynamic_cast<BinaryOpNode*>(node.get());
                return eval(opNode->left) * eval(opNode->right);
            }
            case DIVIDE_NODE: {
                auto opNode = dynamic_cast<BinaryOpNode*>(node.get());
                int rightVal = eval(opNode->right);
                if (rightVal == 0) {
                    throw runtime_error("Division by zero");
                }
                return eval(opNode->left) / rightVal;
            }
            case ASSIGN_NODE: {
                auto assignNode = dynamic_cast<AssignNode*>(node.get());
                int value = eval(assignNode->right);
                env[assignNode->left->name] = value;
                return value;
            }
            default:
                throw runtime_error("Unknown node type");
        }
    }

    unordered_map<string, int> getEnv() const {
        return env;
    }

private:
    unordered_map<string, int> env;
};


int main() {

    // Add your example and check it.
    
    string code = "a = 5 b = a + 2 * 3 c = (a + b) / 2";
    
    // Calculating the sum of a and b
    int a = 3, b = 4;
    int sum = a + b;
    cout << "Sum of a and b is: " << sum << endl;  

    if(a>b){
        cout<<"a is greater"<<endl;
    }else{
        cout<<"b is greater"<<endl;
    }


    //Lexer
    Lexer lexer(code);
    auto tokens = lexer.tokenize();

    cout << "Tokens:\n";
    for (const auto &token : tokens) {
        cout << "Token(Type: " << token.type << ", Value: \"" << token.value << "\")\n";
    }

    //Parser
    Parser parser(tokens);
    vector<unique_ptr<Node>> nodes;
    while (parser.currentToken().type != END) {
        nodes.push_back(parser.parse());
    }

    // Interpreter
    Evaluator evaluator;
    for (auto &node : nodes) {
        evaluator.eval(node);
    }

    // Printing the results
    auto env = evaluator.getEnv();
    cout << "\nOutput:\n";
    for (const auto &entry : env) {
        cout << entry.first << " = " << entry.second << endl;
    }

    return 0;
}

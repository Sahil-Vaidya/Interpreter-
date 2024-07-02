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
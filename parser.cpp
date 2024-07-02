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
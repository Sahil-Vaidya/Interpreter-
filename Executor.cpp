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
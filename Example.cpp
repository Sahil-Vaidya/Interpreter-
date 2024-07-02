int main() {
    string code = "a = 5\nb = a + 2 * 3\nc = (a + b) / 2\n";
    
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

#include <iostream>
#include <vector>
#include <sstream>

template <typename T>
class LinkedListStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr){}
    };
    Node* topNode;
public:
    LinkedListStack() : topNode(nullptr) {}

    ~LinkedListStack() {
        while(!isEmpty()) {
            pop();
        }
    }

    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
    }

    T pop() {
        if (isEmpty()) throw std::runtime_error("Стек пуст");
        Node* temp = topNode;
        T val = temp->data;
        topNode = topNode->next;
        delete temp;
        return val;
    }

    T peek() {
        if (isEmpty()) throw std::runtime_error("Стек пуст");
        return topNode->data;
    }


    bool isEmpty() const {
        return topNode == nullptr;
    }
};

int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
} 

std::string infixToPostfix(const std::string& infix) {
    std::string postfix = "";
    LinkedListStack<char> operators;

    for(char c : infix) {
        if (std::isspace(c)) continue;

        if (std::isalnum(c)) {
            postfix += c;
            postfix += ' ';
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == ')') {
            while(!operators.isEmpty() && operators.peek() != '(') {
                postfix += operators.pop();
                postfix += ' ';
            }
            if (!operators.isEmpty() && operators.peek() == '('){
                operators.pop();
            }
        }
        else {
            while(!operators.isEmpty() && priority(operators.peek()) >= priority(c)){
                postfix += operators.pop();
                postfix += ' ';
            }
            operators.push(c);
        }
    }
    while(!operators.isEmpty()) {
        postfix += operators.pop();
        postfix += ' ';
    }


    return postfix;
}

int calculatePostfix(const std::string& postfix) {
    std::stringstream ss(postfix);
    std::string token;
    LinkedListStack<int> intStack;
    while (ss >> token) {
        if (isdigit(token[0])) {
            intStack.push(std::stoi(token));
        }
        else if (token[0] == '+') {
            int right = intStack.pop();
            int left = intStack.pop();
            intStack.push(left + right);
        }
        else if (token[0] == '-') {
            int right = intStack.pop();
            int left = intStack.pop();
            intStack.push(left - right);
        }
        else if (token[0] == '*') {
            int right = intStack.pop();
            int left = intStack.pop();
            intStack.push(left * right);
        }
        else if (token[0] == '/') {
            int right = intStack.pop();
            if (right == 0) throw std::logic_error("Деление на ноль");
            int left = intStack.pop();
            intStack.push(left / right);
        }
    }

    return intStack.pop();
}

int main() {
    std::string inputString = "2 + 3 * 4";
    std::cout << infixToPostfix(inputString) << std::endl;

    std::string strCalcPostfix = "10 2 3 + *";
    std::cout << calculatePostfix(strCalcPostfix) << std::endl;

    return 0;
}

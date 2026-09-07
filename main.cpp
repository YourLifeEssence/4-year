#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

template <typename T>
class LinkedListStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* topNode;

public:
    LinkedListStack() : topNode(nullptr) {}
    
    ~LinkedListStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
    }

    T pop() {
        if (isEmpty()) throw std::runtime_error("Стек пуст (underflow)");
        Node* temp = topNode;
        T val = temp->data;
        topNode = topNode->next;
        delete temp;
        return val;
    }

    T peek() const {
        if (isEmpty()) throw std::runtime_error("Стек пуст");
        return topNode->data;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }
};

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

std::string infixToPostfix(const std::string& infix) {
    LinkedListStack<char> s;
    std::string postfix = "";

    for (char c : infix) {
        if (std::isspace(c)) continue;

        if (std::isalnum(c)) {
            postfix += c;
            postfix += ' ';
        } 
        else if (c == '(') {
            s.push(c);
        } 
        else if (c == ')') {
            while (!s.isEmpty() && s.peek() != '(') {
                postfix += s.pop();
                postfix += ' ';
            }
            if (!s.isEmpty() && s.peek() == '(') {
                s.pop();
            }
        } 
        else {
            while (!s.isEmpty() && precedence(s.peek()) >= precedence(c)) {
                postfix += s.pop();
                postfix += ' ';
            }
            s.push(c);
        }
    }

    while (!s.isEmpty()) {
        postfix += s.pop();
        postfix += ' ';
    }

    return postfix;
}

int evaluatePostfix(const std::string& postfix) {
    LinkedListStack<int> s;
    std::stringstream ss(postfix);
    std::string token;

    while (ss >> token) {
        if (std::isdigit(token[0]) || (token.size() > 1 && token[0] == '-')) {
            s.push(std::stoi(token));
        } else {
            int val2 = s.pop();
            int val1 = s.pop();
            char op = token[0];
            
            switch (op) {
                case '+': s.push(val1 + val2); break;
                case '-': s.push(val1 - val2); break;
                case '*': s.push(val1 * val2); break;
                case '/': 
                    if (val2 == 0) throw std::runtime_error("Деление на ноль");
                    s.push(val1 / val2); 
                    break;
                default: throw std::runtime_error("Неизвестный оператор");
            }
        }
    }
    return s.pop();
}

int main() {
    system("chcp 65001 > nul");

    std::string infix = "3+5*2-8/4";
    std::string postfix = infixToPostfix(infix);
    std::cout << "Инфикс: " << infix << "\n";
    std::cout << "Постфикс: " << postfix << "\n";

    std::string multiDigitPostfix = "12 3 4 * +";
    std::cout << "Постфикс многосимвольное: " << multiDigitPostfix << "\n";
    std::cout << "Результат вычисления: " << evaluatePostfix(multiDigitPostfix) << "\n";

    return 0;
}
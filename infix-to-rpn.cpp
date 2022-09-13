
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

const double M_PI = 3.14159265358979323846;
const double EULER = 2.71828182845904523536;

using namespace std;

std::map<std::string, int> operations = {
    {"^", 1}, {"sin", 1}, {"cos", 1}, {"*", 3},  {"/", 3},  {"+", 4},
    {"-", 4}, {"_", 1},   {"log", 1}, {"ln", 1}, {"lg", 1}, {"sqrt", 1},
{"tg", 1}, {"ctg", 1} };

vector<char> signs = {'^', '*', '/', '+', '-', '(', ')', 'e', 'x', ','};
vector <string> numbers = { "e", "x", "P" };
vector <string> prefix_functions = { "sin", "lg", "ln", "sqrt", "cos",
"tg", "ctg" };

vector <string> binary_operations = { "+", "-", "^", "/", "*" };
vector <string> unary_operations = { "sin", "cos", "_", "log", "ln", "lg", "sqrt", "tg", "ctg" };

bool ProcessBraces(std::string& element,
    std::stack<std::string>& buffer,
    std::stack<std::string>& result) {
    if (element == "(") {  // 2
        buffer.push(element);
        return false;
    } else if (element == ",") {
        while (!buffer.empty() && buffer.top() != "(") {
            std::string tmp = buffer.top();
            buffer.pop();
            result.push(tmp);
        }
        buffer.push(",");
        return false;
    } else if (element == ")") {  // 4
        while (!buffer.empty() && buffer.top() != "(") {
            std::string tmp = buffer.top();
            buffer.pop();
            if (tmp == ",") {
                continue;
            }
            result.push(tmp);
        }
        if (!buffer.empty() && buffer.top() == "(") {
            buffer.pop();
            return false;
        } else {
            return true;
        }
    }
}


void Punkt3a(std::string& operation, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {
    while (!buffer.empty() && buffer.top() != "(" &&
        operations.count(buffer.top()) != 0 &&
        operations[buffer.top()] <= operations[operation]) {
        result.push(buffer.top());
        buffer.pop();
    }
    buffer.push(operation);
}

void Punkt3b(std::string& operation, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {

    std::string x = buffer.top();

    if (operations[x] == operations[operation]) {
        if (find(prefix_functions.begin(), prefix_functions.end(), x) !=
            prefix_functions.end()) {
            Punkt3a(operation, buffer, result);
        } else {
            buffer.push(operation);
        }
    } else {
        buffer.push(operation);
    }
}

void ProcessOperation(std::string& operation, std::stack<std::string>& buffer,
    std::stack<std::string>& result) {

    if (find(prefix_functions.begin(), prefix_functions.end(), operation) != 
        prefix_functions.end()) {
        buffer.push(operation);
        return;
    }

    if (operations[buffer.top()] < operations[operation]) {
        Punkt3a(operation, buffer, result);
    } else
        Punkt3b(operation, buffer, result);
}

inline bool IsNumber(string s) {
    return (s[0] >= '0' && s[0] <= '9') ||
        find(numbers.begin(), numbers.end(), s) != numbers.end();
}

bool CheckString(vector <string> &input) {
    for (int i = 1; i < input.size(); ++i) {
        /*if (find(possiible_symbols.begin(),
            possiible_symbols.end(), str[i]) == possiible_symbols.end()) {
            if (str[i] > '9' || str[i] < '0') {
                std::cout << "Unknown symbol '" << str[i];
                return false;
            }
        }*/ // заменить на нормальную проверку


        if (input[i] == "-") {
            if (find(binary_operations.begin(), binary_operations.end(), input[i - 1]) !=
                binary_operations.end() || input[i - 1] == "(" || input[i - 1] == ")")
                input[i] = '_';
        } else if (input[i - 1] == "(" || operations.count(input[i - 1]) ||
            input[i - 1] == ",") {
            if (!(input[i] == "(" || IsNumber(input[i]) ||
                find(unary_operations.begin(), unary_operations.end(), input[i]) !=
                unary_operations.end())) {
                std::cout << "ERROR";
                return false;
            }
        } else if (input[i - 1] == ")" || IsNumber(input[i])) {
            if (!(input[i] == ")" || input[i] == "," ||
                find(binary_operations.begin(), binary_operations.end(), input[i]) !=
                binary_operations.end())) {
                std::cout << "ERROR";
                return false;
            }
        }
    }
    return true;
}

vector <string> ParseString(string str) {
    vector<string> parsed_string;
    string word = "";

    parsed_string.push_back("(");
    for (int i = 0; i < str.size(); ++i) {
        if (find(signs.begin(), signs.end(), str[i]) != signs.end()) {
            if (word != "") {
                parsed_string.push_back(word);
                word = "";
            }
            parsed_string.push_back(string(1, str[i]));
            continue;
        }

        if (str[i] >= '0' && str[i] <= '9') {
            word += str[i];
            if (i == str.size() - 1) parsed_string.push_back(word);
            continue;
        }

        if (word != "" && IsNumber(word)) {
            parsed_string.push_back(word);
            word = "";
        }
        word += str[i];
        if (word == "pi") {
            parsed_string.push_back("P");
            word = "";
        }
        if (operations.count(word)) {
            parsed_string.push_back(word);
            word = "";
        }
    }
    parsed_string.push_back(")");
    return parsed_string;
}

double Calculate(std::stack <std::string>& line) {
    std::stack <double> result;
    while (!line.empty()) {
        std::string tmp = line.top();
        line.pop();
        if (operations.count(tmp)) {
            if (find(unary_operations.begin(), unary_operations.end(), tmp)
                != unary_operations.end() && tmp != "log") {
                double number = result.top();
                result.pop();
                if (tmp == "_") {
                    result.push(-number);
                } else if (tmp == "sin") {
                    result.push(sin(number));
                } else if (tmp == "cos") {
                    result.push(cos(number));
                } else if (tmp == "tg") {
                    result.push(tan(number));
                } else if (tmp == "ctg") {
                    result.push(1 / (number));
                } else if (tmp == "sqrt") {
                    result.push(sqrt(number));
                } else if (tmp == "ln") {
                    result.push(log(number));
                } else if (tmp == "lg") {
                    result.push(log10(number));
                }
            } else {
                double number1 = result.top();
                result.pop();
                double number2 = result.top();
                result.pop();
                if (tmp == "+") {
                    result.push(number1 + number2);
                } else if (tmp == "-") {
                    result.push(number2 - number1);
                } else if (tmp == "*") {
                    result.push(number1 * number2);
                } else if (tmp == "/") {
                    result.push(number2 / number1);
                } else if (tmp == "^") {
                    result.push(pow(number2, number1));
                } else if (tmp == "log") {
                    result.push(log(number1) / log(number2));
                }
            }

        } else {
            double push = atoi(tmp.c_str());
            if (tmp == "P") push = M_PI;
            else if (tmp == "e") push = EULER;
            result.push(push);
        }
    }
    return result.top();
}


void PrintPoliz(std::stack <std::string>& result) {
    cout << '\n';
    std::stack <std::string> tmp;
    while (!result.empty()) {
        tmp.push(result.top());
        result.pop();
    }
    std::stack <std::string> tmp2 = tmp;
    while (!tmp.empty()) {
        std::cout << tmp.top() << ' ';
        tmp.pop();
    }

    std::cout << '\n';
    std::cout << Calculate(tmp2);
}



int main() {
    string str;
    cin >> str;
    
    vector <string> parsed_string = ParseString(str);
    if (!CheckString(parsed_string)) {
        cout << "Incorrect expression";
    }

    bool error = false;
    std::stack<std::string> result;
    std::stack<std::string> buffer;
    for (int i = 0; i < parsed_string.size(); ++i) {
        string element = parsed_string[i];
        if (IsNumber(element)) {
            result.push(element);
        } else {
            if (element == "(" || element == ")" || element == ",") {
                if (ProcessBraces(element, buffer, result)) {
                    error = true;
                    cout << "Incorrect expression"; 
                }
            } else if (operations.count(element) != 0 && !error) {
                ProcessOperation(element, buffer, result);
            }
        }
    }

    PrintPoliz(result);

}
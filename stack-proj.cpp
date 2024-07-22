/***************************************************************/
/* Programmer: Ayana Tran
/* Date: 04/04/2024
/* Purpose: Uses two stacks to evaluate arithmetic expressions using infix
/*          operators
/* Input: arithmetic expression
/* Output: size and content of stack
/***************************************************************/

#include <algorithm>  // remove
#include <cctype>     // isdigit, isspace
#include <iostream>   // cin, cout
#include <stack>      // stack
#include <string>     // string
using namespace std;

// Text prompt for user to enter arithmetic expression
void prompt()
{
    cout << "Enter 'q' to exit\n";
    cout << "-Assume operations are proper——operation has 2 operands and 1 "
            "operation\n"
         << "-Multiplication must be placed when used before/after "
            "parentheses\n";
    cout << "Input an arithmetic expression ( using +, -, *, /, () ): ";
}

// Prints stacks and gets rid of stacks content
void printStacksRef(stack<int>& intStk, stack<char>& opStk)
{
    cout << "size of integer stack: " << intStk.size() << "\n";
    cout << "size of operator stack: " << opStk.size() << "\n";

    cout << "integer stack: ";
    while (!intStk.empty())
    {
        cout << "\n" << intStk.top();
        intStk.pop();
    }

    cout << "\noperator stack: ";
    while (!opStk.empty())
    {
        cout << "\n" << opStk.top();
        opStk.pop();
    }
    cout << "\n";
}

// Print stacks
void printStacks(stack<int> intStk, stack<char> opStk)
{
    printStacksRef(intStk, opStk);
}

// Returns true if previous operator is lesser precedence than current operator,
// otherwise returns false
bool isLesserPrecedence(char prevOperator, char currentOperator)
{
    return ((prevOperator == '+' || prevOperator == '-') &&
            (currentOperator == '*' || currentOperator == '/'));
}

// Evaluates one operation and returns result
// otherwise returns -1 to indicate failed result
int evaluateOperation(int firstNum, int secondNum, char op)
{
    switch (op)
    {
        case '+':
            return firstNum + secondNum;

        case '-':
            return firstNum - secondNum;

        case '*':
            return firstNum * secondNum;

        case '/':
            return firstNum / secondNum;
    }

    return -1;
}

// Appends a char's int value to the end of an integer and returns result
int charToInt(int firstInt, char secondChar)
{
    return (firstInt * 10) + (secondChar - '0');
}

int main()
{
    stack<int> integerStack;
    stack<char> operatorStack;
    int i;

    string str;
    prompt();
    getline(cin, str);

    // Used to test w/o inputting; May use as input tests:
    // **Make sure to use a site like Desmos for precedence checking
    // and not Win system calculator
    // string str = "8 / 2 -  3";   // 1
    // string str = "-8+2";         // -6
    // string str = "8--2";         // 10
    // string str = "8--2+1"        // 11
    // string str = "-8--2";        // -6
    // string str = "8-(4-2)*3";    // 2
    // string str = "(8+2)/2";      // 5
    // string str = "2+1-(-1-1)";   // 5
    // string str = "(8+2)*(3-2)-(4/2)";    // 8
    // string str = "(8+(2+3-(3*2)*5/2))";  // -2
    // string str = "8+(2+3-(3*2)*5/2)";    // -2
    // string str = "82";
    // string str = "-82";
    // string str = "-29-3";        // -32
    // string str = "2--30";        // 32
    // string str = "2--382*2";     // 766
    // string str = "121/-2";       // -60
    // string str = "4*-401"        // -1604
    // string str = "-10+20*(1+(160/40)-2)";               // 50
    // string str = "-121*-301-(1200+452)-312"              // 34,457
    // string str = "-11+311*(7010/10)-42/(2*4+(6/-3))";    // 217,993
    // string str = "-1+3*(4/2)-6/(1+3+(1*2))";            // 4
    // string str = "-(5/-3)";      // 1

    while (str != "q")
    {
        i = 0;
        cout << "\nExpression: " << str << "\n\n";

        // Sanitizes string or input: removes white space
        str.erase(remove(str.begin(), str.end(), ' '), str.end());

        // Resolve higher precedence operations and pushes result on int stack
        while (i < str.size())
        {
            // Cases for digit:
            // single or multi digit, negative single or multi digit
            if (isdigit(str[i]) || (i == 0 && str[i] == '-') ||
                (str[i] == '-' && !isdigit(str[i - 1]) && str[i - 1] != ')'))
            {
                int newInt;
                int negNum = 1;

                if (i == 0 && str[i] == '-' && str[i + 1] == '(')
                {
                    integerStack.push(-1);
                    operatorStack.push('*');
                    ++i;
                    operatorStack.push(str[i]);
                    ++i;
                }
                else if (str[i] == '-')
                {
                    negNum = -1;
                    ++i;
                }

                newInt = str[i] - '0';

                while (isdigit(str[i + 1]))
                {
                    newInt = charToInt(newInt, str[i + 1]);
                    ++i;
                }

                integerStack.push(negNum * newInt);
            }
            // Cases for operators
            else
            {
                // If operator stack is empty OR current char is '('
                // OR operator stack top() is '(' OR previous operator is lesser
                // precedence than current operator
                // ...push operator on operator stack
                if (operatorStack.empty() || str[i] == '(' ||
                    isLesserPrecedence(operatorStack.top(), str[i]) ||
                    operatorStack.top() == '(')
                {
                    operatorStack.push(str[i]);
                }
                // Evaluating an operation that's already in the stack when
                // meeting ')' until '(' is met. then pop '('
                else if (str[i] == ')')
                {
                    while (operatorStack.top() != '(')
                    {
                        int secondInt = integerStack.top();
                        integerStack.pop();
                        int firstInt = integerStack.top();
                        integerStack.pop();
                        char op = operatorStack.top();
                        operatorStack.pop();

                        integerStack.push(
                            evaluateOperation(firstInt, secondInt, op));
                    }
                    operatorStack.pop();
                }
                // Tf previous operator is greater precedence than current
                // operator, evaluate left-hand-side expression then push
                // result onto int stack. push current operator onto operator
                // stack
                else if (!isLesserPrecedence(operatorStack.top(), str[i]))
                {
                    int secondInt = integerStack.top();
                    integerStack.pop();
                    int firstInt = integerStack.top();
                    integerStack.pop();
                    char op = operatorStack.top();
                    operatorStack.pop();

                    integerStack.push(
                        evaluateOperation(firstInt, secondInt, op));

                    operatorStack.push(str[i]);
                }
            }
            ++i;
        }

        cout << "After evaluating higher precedence operations: \n";
        printStacks(integerStack, operatorStack);

        // Resolve lower precedence operations and pushes result on int
        // stack
        while (integerStack.size() != 1)
        {
            int secondInt = integerStack.top();
            integerStack.pop();
            int firstInt = integerStack.top();
            integerStack.pop();
            char op = operatorStack.top();
            operatorStack.pop();

            integerStack.push(evaluateOperation(firstInt, secondInt, op));
        }

        cout << "\nAfter evaluating lower precendence operations: \n";
        printStacksRef(integerStack, operatorStack);
        cout << "\n\n";

        prompt();
        getline(cin, str);
    }
}

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include "mystack.h" //use my collection
#include <stdlib.h>
#include <string>
#include <math.h>
#include <stdio.h>
#include <ctype.h>

using namespace std;


//functions declaration
/**
 * Implement operations between two numbers
 * @param firstParam - First parameter
 * @param secondParam - Second parameter
 * @param symbol - The sign of operation "+-^/*"
 * @return - double result of calculation
 */
double executeOperation(double firstParam, double secondParam, char symbol) {
    double res;
    if(symbol == '+'){
        res = firstParam + secondParam;
    }
    if(symbol == '-'){
        res = firstParam - secondParam;
    }
    if(symbol == '*'){
        res = firstParam * secondParam;
    }
    if(symbol == '/'){
        res = firstParam / secondParam;
    }
    if(symbol == '^'){
        res = pow(firstParam, secondParam);
    }
    return res;
}


/**
 * Return true if char is equals arithmetic signs, not alphabetic char or digit
 * @param ch - Char from input formula
 * @return - true if it is operation sign
 */
bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^');
}

/**
 * Make token from string using current position of "cursor"
 * Send the current cursor position, and the line from which we select token
 * @param pos - Position of cursor in string
 * @param inputFormula - User's string
 * @return - token - or its variable, or its operation sign
 */
string splitIntoTokens(int pos, string inputExpression) {
    string res;
    if(isOperator(inputExpression[pos])){
        res = inputExpression[pos];
    }else{
        while(!isOperator(inputExpression[pos])){
            res += inputExpression[pos];
            pos++;
            if(pos == inputExpression.length()){
                break;
            }
        }
    }
    return res;
}

/**
 * Assigns a priority to operators
 * @param ch - Operator sign
 * @return - Priority of operator
 */
int getPriority(char ch) {
    if(ch == '+' || ch == '-'){
        return 0;
    }else if(ch == '*' || ch == '/'){
        return 1;
    }else if(ch == '^'){
        return 2;
    }else{
        return -1;
    }
}

/**
 * Retrieving end result. Calculating elements of queue
 * @param postfixNotat - Queue of operators and operands in postfix notation
 * @return - Result of user's expression
 */
double processingQueueOfPostfixNotation(Queue<string> postfixNotat) {
    double res;
    Vector<string> vec;
    while(!postfixNotat.isEmpty()){
        vec.add(postfixNotat.dequeue());
    }
    for(int i = 0; i < vec.size(); i++){
        if(vec.size() == 1){
            break;
        }
        string tmp = vec[i];
        if(isOperator(tmp[0]) && i >= 2){
            string tok = vec[i];
            double secondOperand = atof(vec[i-1].c_str());
            double firstOperand = atof(vec[i-2].c_str());
            double calcResult =  executeOperation(firstOperand,secondOperand,tok[0]);
            string r2 = to_string(calcResult);
            vec.set(i-2, r2);
            vec.remove(i-1);
            vec.remove(i-1);
            i = 0;
        }
    }
    return res = atof(vec[0].c_str());
}
/**
 * Replace string of user input.
 * If we have one variable appearing two or more times - function
 * replace this variable automatically.
 * @param inputExpression - String - user's formula
 * @param variablesMap - Collection of variables
 * @return - True - all variables are replaced and we can continue calculate
 */
bool replaceVariablesInExpression(string &inputExpression, Map<string, string> &variablesMap) {
    bool passCondition = true;
    string key;
    for(int i = 0; i < inputExpression.length(); i++){ //if we have variables - ask user to enter a number and replace string
        if(isalpha(inputExpression[i])){
            while(isalpha(inputExpression[i])){
                key += inputExpression[i];
                i++;
            }
            if(variablesMap.containsKey(key)){
                int kLen = key.length();
                inputExpression.replace(i-kLen, kLen, variablesMap[key]);
                /*cout << "EXP " << inputExpression << endl;*/
            }else{
                passCondition = false;
                cout << "In input expression, was not find this variable!" << endl;
            }
        }
    }
    return passCondition;
}
/**
 * If token is operator. Compare current token with top sign
 * in stack, if sign in stack have bigger priority then put him in
 * queue, and token put in stack
 * @param operandStack - Stack of operators
 * @param postfixNotat - Queue. Postfix notation of formula
 * @param token - Token for user's string
 */
void addOperatorToPostfixNotation(myStack<string>& operandStack, Queue<string>& postfixNotation, string token) {
    if(operandStack.isEmpty()){ //if stack is empty then we add first sign to stack
        operandStack.push(token);
    }else if(!operandStack.isEmpty()){
        if(getPriority(token[0])/* op1 */ <= getPriority((operandStack.peek())[0])/* op2 */){
            //compare current token with top sign in stack, if sign in stack have bigger priority than put it in
            //queue, and token put in stack
            string z = operandStack.pop();
            if(z != "("){
                postfixNotation.enqueue(z);
            }
            operandStack.push(token);
            /**
             * if in stack we have more than 1 element than comparing
             * them and push some of them to queue. in this way we have
             * always one element in stack
             */
            if(operandStack.size() >= 2){
                if(!isOperator(postfixNotation.back()[0])){
                    string firstOperand = operandStack.pop();
                    string secondOperand = operandStack.pop();
                    if(getPriority(firstOperand[0]) <= getPriority(secondOperand[0])){
                        if(secondOperand != "(" & firstOperand != "("){
                            postfixNotation.enqueue(secondOperand);
                            operandStack.push(firstOperand);
                        }
                    }else{
                        if(secondOperand != "(" && firstOperand != "("){
                            postfixNotation.enqueue(firstOperand);
                            operandStack.push(secondOperand);
                        }
                    }
                }
            }
        }else{
            operandStack.push(token);
        }
    }
}

/**
 * On input we have a string that user's write
 * This function show to user - result
 * @param inputExpression - string of user's input
 * @param variablesMap - Collection of variables
 */
void calculateResult(string &inputExpression, Map<string, string> &variablesMap) {
    if(replaceVariablesInExpression(inputExpression, variablesMap)){
        myStack<string> operandStack;
        Queue<string> postfixNotation;
        for(int pos = 0; pos < inputExpression.length(); pos++){
            string token;
            token = splitIntoTokens(pos, inputExpression);
            int tokenLenght = token.length();
            if(tokenLenght > 1){
                pos += tokenLenght - 1;
            }
            //if token is number then add to queue
            if(!isOperator(token[0])){
                postfixNotation.enqueue(token);
            }
            if(token == "("){
                operandStack.push(token);//add "(" to stack
            }
            if(token == ")"){
                while(operandStack.peek() != "("){
                    string a = operandStack.pop();
                    postfixNotation.enqueue(a);
                }
                if(operandStack.peek() == "("){
                    operandStack.pop();
                }
            }
            //if token is operator sign "+-/*^"
            if(isOperator(token[0]) && (token != "(" && token != ")")){
                addOperatorToPostfixNotation(operandStack, postfixNotation, token);
            }
        }
        //add last element in stack to queue
        while(!operandStack.isEmpty()){
            string a = operandStack.pop();
            postfixNotation.enqueue(a);
        }
        double endRes = processingQueueOfPostfixNotation(postfixNotation);
        cout << "RESULT = " << endRes << endl;
    }
}

/**
 * Check if user input have right quantity of round brackets
 * @param inputExpression - string of user's input
 */
bool checkValidBracketsInput(string &inputExpression) {
    int openB = 0;
    int closeB = 0;
    for(int i = 0; i < inputExpression.length(); i++){
        char ch = inputExpression[i];
        if(ch == '('){
            openB++;
        }
        if(ch == ')'){
            closeB++;
        }
    }
    if((openB == closeB) == false){
        cout << "Your brackets is invalid! Please try again!" << endl;
    }
    return openB == closeB;
}

/**
 * Check if we don't have two operators after each other
 * @param inputExpression - string of user's input
 * @return - true if we have right expression
 */
bool checkValidOperatorsInput(string &inputExpression) {
    bool res;
    for(int i = 0; i < inputExpression.length(); i++){
        char ch = inputExpression[i];
        char ch2 = inputExpression[i+1];
        if((isOperator(ch) && (ch != '(') && (ch != ')')) && (isOperator(ch2) && (ch2 != '(') && (ch2 != ')'))){
            res = false;
            cout << "Wrong writing operator's. Your formula is invalid! ";
            break;
        }else{
            res = true;
        }
    }
    if(isOperator(inputExpression[(inputExpression.length()) - 1])){
        res = false;
    }
    return res;
}

/**
 * Add to collection a new variable.
 * In collection we store key-value. Key - name of variable. Value - value of variable.
 * @param inputVariable - string of user's input
 * @param variablesMap - place where we store variables
 */
void addVariableToCalculator(string inputVariable, Map<string, string> &variablesMap) {
    string key;
    string value;
    int cursorPosition = 0;
    while(inputVariable[cursorPosition] != '=' && cursorPosition < inputVariable.length()){
        if(isalpha(inputVariable[cursorPosition])){
            key += inputVariable[cursorPosition];
            cursorPosition++;
        }else{
            cout << "Your variable's name is incorrect." << endl;
            cursorPosition = inputVariable.length();
        }
    }
    cursorPosition++;
    if(cursorPosition >= inputVariable.length()){
        cout << "Your input is incorrect. Please try again!" << endl;
    }else{
        //now make value
        while (cursorPosition < inputVariable.length()){
            value += inputVariable[cursorPosition];
            cursorPosition++;
        }
        variablesMap[key] = value;
    }
}


int main() {
    //for storing ours variables create collection map with variable as a key
    Map<string, string> variablesContainer;
    while(true){
        cout << "Enter your variables. Example a=1" << endl;
        string inputVariable = getLine("Enter variable:");
        if(inputVariable == "exit"){
            break;
        }
        addVariableToCalculator(inputVariable, variablesContainer);
    }

    while(true){
        cout << "This is a calculator!" << endl;
        string inputExpression = getLine("Enter formula:");
        if(inputExpression == "exit"){
            break;
        }
        if(checkValidBracketsInput(inputExpression) && checkValidOperatorsInput(inputExpression)){
            calculateResult(inputExpression, variablesContainer);
        }
    }
    return 0;
}







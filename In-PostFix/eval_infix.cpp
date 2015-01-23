
#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;


bool lessThanOrEqual(const char c1, const char c2) {
    
    if (c1 == '|')
        return true;
    
    else if (c1 == '&' && (c2 == '!' | c2 == '&'))
        return true;
    
    else if (c1 == '!' && c2 == '!')
        return true;
    
    return false;
}

bool isValid(const char prev, const char op){
    if (op == '(') {
        if (prev >= '0' && prev <= '9')
            return false;
    }
    
    if (op == '&' || op == '|') {
        if (prev == '\0' || (prev != ')' && !(prev >= '0' && prev <= '9')))
            return false;
    }

    if (op >= '0' && op <= '9') {
        if (prev >= '0' && prev <= '9')
            return false;
    }
    
    return true;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result){
    // Evaluates a boolean expression
    // Postcondition: If infix is a syntactically valid infix boolean
    //   expression, then postfix is set to the postfix form of that
    //   expression, result is set to the value of the expression (where
    //   in that expression, each digit k represents element k of the
    //   values array), and the function returns zero.  If infix is not a
    //   syntactically valid expression, the function returns 1.  (In that
    //   case, postfix may or may not be changed, but result must be
    //   unchanged.)
    
    
    // Infix to Postfix
    stack<char> ops;
    postfix = "";
    char prev = '\0';
    for (int i = 0; i < infix.length(); i++) {
        char ch = infix[i];
        
        if (!isValid(prev, ch))
            return 1;
        
        switch (ch){
            case '0':case '1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
                postfix += ch;
                prev = ch;
                break;
                
            case '(':
                ops.push(ch);
                prev = ch;
                break;
                
            case ')':
                // pop stack until matching '('
                if (ops.empty())
                    return 1;
                
                while (ops.top() != '(') {
                    postfix += ops.top();
                    ops.pop();
                }
                ops.pop(); // remove the '('
                prev = ch;
                break;
                
            case '&':case '|':case '!':
                while (!ops.empty() && ops.top() != '(' && lessThanOrEqual(ch, ops.top())) {
                    postfix += ops.top();
                    ops.pop();
                }
                ops.push(ch);
                prev = ch;
                break;
                
            case ' ':
                break;
                
            default:
                return 1;
        }
    }
    
    while (!ops.empty()) {
        if (ops.top() == '(' || ops.top() == ')') {
            return 1;
        }
        postfix += ops.top();
        ops.pop();
    }
    
    // Evaluate postfix
    
    stack<bool> stk;
    int stksize = 0;
    for (int i = 0; i < postfix.length(); i++) {
        char op = postfix[i];
        if (op != '&' && op != '|' && op != '!') {
            stk.push(values[(op - '0')]);
            stksize++;
        }
        
        else if (op == '!'){
            if (stksize == 0)
                return 1;
            
            bool op1 = stk.top();
            stk.pop();
            stksize--;
            stk.push(!op1);
            stksize++;
        }
            
        else{
            if (stksize < 2)
                return 1;
            
            bool op1 = stk.top();
            stk.pop();
            stksize--;
            bool op2 = stk.top();
            stk.pop();
            stksize--;
            
            if (op == '&')
                stk.push(op1 & op2);
            else if (op == '|')
                stk.push(op1 || op2);
            stksize++;
        }
    }
    if (stksize != 1)
        return 1;
    
    result = stk.top();
    
    return 0;
}



//int main()
//{
//    bool ba[10] = {
//      //  0      1      2      3      4      5      6      7      8      9
//        true,  true,  true,  false, false, false, true,  false, true,  false
//    };
//    string pf;
//    bool answer;
//    assert(evaluate("(()", ba, pf, answer) == 1);
//    assert(evaluate("(6))", ba, pf, answer) == 1);
//    assert(evaluate(")", ba, pf, answer) == 1);
//    assert(evaluate("(", ba, pf, answer) == 1);
//    assert(evaluate("&", ba, pf, answer) == 1);
//
//    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
//          &&  pf == "43!03&&|" && !answer);
//    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
//    assert(evaluate("8|", ba, pf, answer) == 1);
//    assert(evaluate("4 5", ba, pf, answer) == 1);
//    assert(evaluate("01", ba, pf, answer) == 1);
//    assert(evaluate("()", ba, pf, answer) == 1);
//    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
//    assert(evaluate("2(&8)", ba, pf, answer) == 1);
//    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
//    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
//           &&  pf == "43!03&&|"  &&  !answer);
//    assert(evaluate("", ba, pf, answer) == 1);
//    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
//    ba[2] = false;
//    ba[9] = true;
//    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
//    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
//    
//    cout << "Passed all tests" << endl;
//}


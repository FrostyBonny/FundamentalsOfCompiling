#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
using namespace std;

fstream file;
vector<string> token;
string str;

int declaration_list();

int statement_list();

int A();

int declaration_stat();

int B();

int if_stat();

int bool_expression();

int arithmetic_expression();

int term();

int factor();

int D();

int C();

int statement();

int while_stat();

int for_stat();

int assignment_expression();

int read_stat();

int write_stat();

int compound_stat();

int assignment_stat();

void error(string line, int state) {
    cout << "第" << line << "行" << "发生" << state << "错误" << endl;
}

vector<string> splite(const string &s, const string &c)//分割字符用的
{
    std::string::size_type pos1, pos2;
    vector<string> v;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
    {
        v.push_back(s.substr(pos1));
    }
    return v;
}

void getNextLine() {
    if(file.eof()){
        return;
    }
    getline(file, str);
    if(str == ""){
        token[0] = "错误";
        token[1] = "错误";
        return;
    }
    token = splite(str, " ");
    cout << token[0] << " " << token[1] <<" "<< token[2] << endl;
}

void readFile() {
    file.open("E:\\result.txt", ios::app | ios::out | ios::in);
    if (!file.is_open()) {
        cout << "读取词法分析结果文件出错" << endl;
    }
}

int program() {
    int state = 0;
    getNextLine();
    if (token[0] != "{") {
        state = 1;
        return state;

    }
    getNextLine();
    state = declaration_list();
    if (state != 0) {
        return state;
    }
    state = statement_list();
    if (state != 0) {
        return state;
    }
    if (token[0] != "}") {
        state = 2;
    }
    return state;
}

int statement_list() {
    int state = 0;
    if (token[0] == "if" || token[0] == "while" || token[0] == "for" || token[0] == "read" ||
        token[0] == "write" || token[0] == "{" || token[0] == "ID" || token[0] == ";") {
//        getNextLine();
        state = B();
    }else if(token[0] == "}"){
        getNextLine();
    }
    else {
        state = 1;
    }
    return state;
}

int B() {
    int state = 0;
    if (token[0] == "if" || token[0] == "while" || token[0] == "for" || token[0] == "read" ||
        token[0] == "write" || token[0] == "{" || token[0] == "ID" || token[0] == ";") {
//        getNextLine();
        state = statement();
        if(state != 0){
            return state;
        }
        state = B();
    }else if(token[0] == "}"){
        state = 0;
    }else{
        state = 1;
    }
    return state;
}

int statement() {
    int state = 0;
    if (token[0] == "if") {
        state = if_stat();
    } else if (token[0] == "while") {
        state = while_stat();
    } else if (token[0] == "for") {
        state = for_stat();
    } else if (token[0] == "read") {
        state = read_stat();
    } else if (token[0] == "write") {
        state = write_stat();
    } else if (token[0] == "{") {
        state = compound_stat();
    } else if (token[0] == "ID") {
        state = assignment_stat();
    } else if (token[0] == ";") {
        getNextLine();
        state = 0;
    }
    return state;
}

int assignment_stat() {
    int state = 0;
    state = assignment_expression();
    return state;
}

int compound_stat() {
    int state = 0;
    if(token[0] != "{"){
        state = 1;
        return state;
    }
    getNextLine();
    state = statement_list();
    if(state != 0){
        return state;
    }
//    getNextLine();
    if(token[0] != "}"){
        state = 1;
        return state;
    }
    getNextLine();
    return state;
}

int write_stat() {
    int state = 0;
    if(token[0] != "write"){
        state = 1;
        return state;
    }
    getNextLine();
    state = arithmetic_expression();
    return state;
}

int read_stat() {
    int state = 0;
    if(token[0] != "read"){
        state = 1;
        return state;
    }
    getNextLine();
    if(token[0] != "ID"){
        state = 1;
    }
    getNextLine();
    return state;
}

int for_stat() {
    int state = 0;
    if(token[0] != "for"){
        state  = 1;
        return state;
    }
    getNextLine();
    if(token[0] != "("){
        state = 1;
        return state;
    }
    getNextLine();
    state = assignment_expression();
    if(state != 0){
        return state;
    }
    if(token[0] != ";"){
        state = 1;
        return state;
    }
    getNextLine();
    state = bool_expression();
    if(state != 0){
        return state;
    }
    if(token[0] != ";"){
        state = 1;
        return state;
    }
    getNextLine();
    state = assignment_expression();
    if(state != 0){
        return state;
    }
    if(token[0] != ")"){
        state = 1;
        return state;
    }
    getNextLine();
    state = statement();
    return state;
}

int assignment_expression() {
    int state = 0;
    if(token[0] != "ID"){
        state = 1;
        return state;
    }
    getNextLine();
    if(token[0] != "="){
        state = 1;
        return state;
    }
    getNextLine();
    state = arithmetic_expression();
    return state;
}

int while_stat() {
    int state = 0;
    if(token[0] != "while"){
        state  = 1;
        return state;
    }
    getNextLine();
    if(token[0] != "("){
        state = 1;
        return state;
    }
    getNextLine();
    state = bool_expression();
    if (state != 0) {
        return state;
    }
//    getNextLine();
    if(token[0] != ")"){
        state = 1;
        return state;
    }
    getNextLine();
    state = statement();
    return state;
}

int if_stat() {
    int state = 0;
    if(token[0] != "if"){
        state = 1;
        return state;
    }
    getNextLine();
    if (token[0] != "(") {
        state = 1;
        return state;
    }
    getNextLine();
    state = bool_expression();
    if (state != 0) {
        return state;
    }
    if (token[0] != ")") {
        state = 1;
        return state;
    }
    getNextLine();
    state = statement();
    if(state  > 0){
        return state;
    }
    if(token[0] == "else"){
        getNextLine();
        state = statement();
    }
    return state;
}

int bool_expression() {
    int state = 0;
    if (token[0] == "(" || token[0] == "ID" || token[0] == "NUM") {
        state = arithmetic_expression();
        if(state != 0){
            return state;
        }
        if(token[0] == ">"||token[0] == "<"||token[0] == ">="||
           token[0] == "<="||token[0] == "=="||token[0] == "!="){
            getNextLine();
            state = arithmetic_expression();
        }else{
            state = 1;
        }
    }
    return state;
}

int arithmetic_expression() {
    int state = 0;
    if (token[0] == "(" || token[0] == "ID" || token[0] == "NUM") {
        state = term();
        if(state != 0){
            return state;
        }
        state = C();
    }else{
        state = 1;
    }
    return state;
}

int C() {
    int state = 0;
    if(token[0] == "+"||token[0] == "-"){
        getNextLine();
        state = term();
        if(state != 0){
            return state;
        }
        state = C();
    }else if(token[0] == ">"||token[0] == "<"||token[0] == ">="||
             token[0] == "<="||token[0] == "=="||token[0] == "!="||
             token[0] == ")"||token[0] == ";"){
        state = 0;
    }else{
        state = 1;
    }
    return state;
}

int term() {
    int state = 0;
    if(token[0] == "("||token[0] == "ID"||token[0] == "NUM"){
        state = factor();
        if(state != 0){
            return state;
        }
        state = D();
    }else{
        state = 1;
    }
    return state;
}

int D() {
    int state = 0;
    if(token[0] == "*"||token[0] == "/"){
        getNextLine();
        state = factor();
        if(state != 0){
            return state;
        }
//        getNextLine();
        state = D();
    }else if(token[0] == ">"||token[0] == "<"||token[0] == ">="||
             token[0] == "<="||token[0] == "=="||token[0] == "!="||
             token[0] == "+"||token[0] == "-"||token[0] == ";"||token[0] == ")"){
        state = 0;
    }
    else{
        state = 1;
    }
    return state;
}

int factor() {
    int state;
    if(token[0] == "("){
        getNextLine();
        state = arithmetic_expression();
        if(state != 0){
            return state;
        }
        getNextLine();
    }else if(token[0] == "ID"||token[0] == "NUM"){
        getNextLine();
    }else {
        state = 1;
    }
    return state;
}

int declaration_list() {
    int state = 0;
    if (token[0] == "int") {
        state = A();
        return state;
    } else {
        state = 3;
        return state;
    }
}

int A() {
    int state = 0;
    if (token[0] == "int") {
        state = declaration_stat();
        if (state > 0) {
            return state;
        }
        state = A();
        return state;
    } else if (token[0] == "if" || token[0] == "while" || token[0] == "for" ||
               token[0] == "read" || token[0] == "write" ||
               token[0] == "{" || token[0] == ";" ||
               token[0] == "ID" || token[0] == "(" ||
               token[0] == "NUM" || token[0] == "}") {
        return 0;
    } else {
        state = 4;
        return state;
    }
}

int declaration_stat() {
    int state = 0;
    if (token[0] != "int") {
        state = 5;
    }
    getNextLine();
    if (token[0] != "ID") {
        state = 6;
        return state;
    }
    getNextLine();
    if (token[0] != ";") {
        state = 7;
        return state;
    }
    getNextLine();
    return state;
}

int main() {
    int state = 0;
    readFile();
    state = program();
    if (state != 0) {
        error(token[2], state);
    } else {
        cout << "成功" << endl;
    }
    file.close();
    return 0;
}
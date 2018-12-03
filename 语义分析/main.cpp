#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

struct table {
    string name;
    int address;
};

fstream file;
fstream file_out;
vector<string> token;
string str;
vector<table *> varTable;
//int vartablep = 0;
int datap = 0;
int labelp = 0;

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

int name_def(string name);

int look_up(string name, int *address);

void error(string line, int state) {
    cout << "第" << line << "行" << "发生" << state << "错误" << endl;
}

vector<string> splite(const string &s, const string &c)//分割字符用的
{
    std::string::size_type pos1, pos2;
    vector<string> v;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) {
        v.push_back(s.substr(pos1));
    }
    return v;
}

void getNextLine() {
    if (file.eof()) {
        return;
    }
    getline(file, str);
    if (str == "") {
        token[0] = "错误";
        token[1] = "错误";
        return;
    }
    token = splite(str, " ");
    cout << token[0] << " " << token[1] << " " << token[2] << endl;
}

void readFile() {
    file.open("E:\\result.txt", ios::app | ios::out | ios::in);
    if (!file.is_open()) {
        cout << "读取词法分析结果文件出错" << endl;
    }
    file_out.open("E:\\final.txt", ios::trunc | ios::out);
    if (!file_out.is_open()) {
        cout << "打开输出文件出错" << endl;
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
    } else if (token[0] == "}") {
        getNextLine();
    } else {
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
        if (state != 0) {
            return state;
        }
        state = B();
    } else if (token[0] == "}") {
        state = 0;
    } else {
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
    if (token[0] != "{") {
        state = 1;
        return state;
    }
    getNextLine();
    state = statement_list();
    if (state != 0) {
        return state;
    }
//    getNextLine();
    if (token[0] != "}") {
        state = 1;
        return state;
    }
    getNextLine();
    return state;
}

int write_stat() {
    int state = 0;
    if (token[0] != "write") {
        state = 1;
        return state;
    }
    getNextLine();
    state = arithmetic_expression();
    if(state != 0){
        return state;
    }
    //  1123代码
    if(token[1] != ";"){
        state = 4;
        return state;
    }
    file_out<<"        OUT"<<endl;
    file_out<<"        POP"<<endl;
    getNextLine();
    return state;
}

int read_stat() {
    int state = 0;
    int address;
    if (token[0] != "read") {
        state = 1;
        return state;
    }
    getNextLine();
    if (token[0] != "ID") {
        state = 1;
    }
    state = look_up(token[1],&address);
    if(state != 0) return state;

    file_out<<"        IN"<<endl;
    file_out<<"        STO "<<address<<endl;
    file_out<<"        POP"<<endl;

    getNextLine();
    //  1123代码
    if(token[1] != ";"){
        state = 4;
        return state;
    }
    getNextLine();
    return state;
}

int for_stat() {
    int state = 0;
    int lable1;
    int lable2;
    int lable3;
    int lable4;
    if (token[0] != "for") {
        state = 1;
        return state;
    }
    getNextLine();
    if (token[0] != "(") {
        state = 1;
        return state;
    }
    getNextLine();
    state = assignment_expression();
    if (state != 0) {
        return state;
    }
    if (token[0] != ";") {
        state = 1;
        return state;
    }
    lable1 = labelp++;
    file_out<<"LABEL"<<lable1<<":"<<endl;
    getNextLine();
    state = bool_expression();
    if (state != 0) {
        return state;
    }

    lable2 = labelp++;
    file_out<<"        BRF LABEL"<<lable2<<endl;
    lable3 = labelp++;
    file_out<<"        BR LABEL"<<lable3<<endl;

    if (token[0] != ";") {
        state = 1;
        return state;
    }
    lable4 = labelp++;
    file_out<<"LABEL"<<lable4<<":"<<endl;
    getNextLine();
    state = assignment_expression();
    if (state != 0) {
        return state;
    }
    file_out<<"        BR LABEL"<<lable1<<endl;
    if (token[0] != ")") {
        state = 1;
        return state;
    }
    file_out<<"LABEL"<<lable3<<":"<<endl;
    getNextLine();
    state = statement();
    if(state != 0) return state;
    file_out<<"        BR LABEL"<<lable4<<endl;
    file_out<<"LABEL"<<lable2<<":"<<endl;
    return state;
}

int assignment_expression() {
    int state = 0;
    int address;
    if (token[0] != "ID") {
        state = 1;
        return state;
    }
    state = look_up(token[1], &address);
    if (state != 0) {
        return state;
    }
    getNextLine();
    if (token[0] != "=") {
        state = 1;
        return state;
    }
    getNextLine();
    state = arithmetic_expression();
    if (state != 0) {
        return state;
    }
    //  1!!!!!!!
    file_out << "        STO " << address << endl;
    file_out << "        POP" << endl;
    return state;
}

int while_stat() {
    int state = 0;
    int lable1;
    int lable2;

    lable1 = labelp++;
    file_out<<"LABEL"<<lable1<<":"<<endl;

    if (token[0] != "while") {
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
//    getNextLine();
    if (token[0] != ")") {
        state = 1;
        return state;
    }

    lable2 = labelp++;
    file_out<<"        BRF LABEL"<<lable2<<endl;

    getNextLine();
    state = statement();
    if(state != 0){
        return state;
    }
    file_out<<"        BR LABEL"<<lable1<<endl;
    file_out<<"LABEL"<<lable2<<":"<<endl;

    return state;
}

int if_stat() {
    int state = 0;
    int lable1;
    int lable2;
    if (token[0] != "if") {
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
    //  ！！！！！！！！
    lable1 = labelp++;
    file_out<<"        BRF LABEL"<<lable1<<endl;

    getNextLine();
    state = statement();
    if (state > 0) {
        return state;
    }
    //  !!!!!!!1
    lable2 = labelp++;
    file_out<<"        BR LABEL"<<lable2<<endl;
    file_out<<"LABEL"<<lable1<<":"<<endl;
    if (token[0] == "else") {
        getNextLine();
        state = statement();
    }
    file_out<<"LABEL"<<lable2<<":"<<endl;

    return state;
}

int bool_expression() {
    int state = 0;
    if (token[0] == "(" || token[0] == "ID" || token[0] == "NUM") {
        state = arithmetic_expression();
        if (state != 0) {
            return state;
        }
        if (token[0] == ">" || token[0] == "<" || token[0] == ">=" ||
            token[0] == "<=" || token[0] == "==" || token[0] == "!=") {
            string temp = token[0];
            getNextLine();
            state = arithmetic_expression();
            if (state != 0) {
                return state;
            }
            //  !!!!!!!!!!!!!!
            if (temp == ">") file_out << "        GT" << endl;
            if (temp == ">=") file_out << "        GE" << endl;
            if (temp == "<") file_out << "        LES" << endl;
            if (temp == "<=") file_out << "        LE" << endl;
            if (temp == "==") file_out << "        EQ" << endl;
            if (temp == "!=") file_out << "        NOTEQ" << endl;


        } else {
            state = 1;
        }
    }
    return state;
}

int arithmetic_expression() {
    int state = 0;
    if (token[0] == "(" || token[0] == "ID" || token[0] == "NUM") {
        state = term();
        if (state != 0) {
            return state;
        }
        state = C();
    } else {
        state = 1;
    }
    return state;
}

int C() {
    int state = 0;
    if (token[0] == "+" || token[0] == "-") {
        string temp = token[0];
        getNextLine();
        state = term();
        if (state != 0) {
            return state;
        }
        //  !!!!!!!!!!!!!
        if (temp == "+") file_out << "        ADD" << endl;
        if (temp == "-") file_out << "        SUB" << endl;
        state = C();
    } else if (token[0] == ">" || token[0] == "<" || token[0] == ">=" ||
               token[0] == "<=" || token[0] == "==" || token[0] == "!=" ||
               token[0] == ")" || token[0] == ";") {
        state = 0;
    } else {
        state = 1;
    }
    return state;
}

int term() {
    int state = 0;
    if (token[0] == "(" || token[0] == "ID" || token[0] == "NUM") {
        state = factor();
        if (state != 0) {
            return state;
        }
        state = D();
    } else {
        state = 1;
    }
    return state;
}

int D() {
    int state = 0;
    if (token[0] == "*" || token[0] == "/") {
        string temp = token[0];
        getNextLine();
        state = factor();
        if (state != 0) {
            return state;
        }
        //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (temp == "*") file_out << "        MULT" << endl;
        if (temp == "/") file_out << "        DIV" << endl;

//        getNextLine();
        state = D();
    } else if (token[0] == ">" || token[0] == "<" || token[0] == ">=" ||
               token[0] == "<=" || token[0] == "==" || token[0] == "!=" ||
               token[0] == "+" || token[0] == "-" || token[0] == ";" || token[0] == ")") {
        state = 0;
    } else {
        state = 1;
    }
    return state;
}

int factor() {
    int state = 0;
    if (token[0] == "(") {
        getNextLine();
        state = arithmetic_expression();
        if (state != 0) {
            return state;
        }
        getNextLine();
    } else if (token[0] == "ID") {
        int address;
        state = look_up(token[1], &address);
        if (state != 0) {
            return state;
        }
        file_out << "        LOAD " << address << endl;   //  !!!!!!!!!!!!!!!!!!
        getNextLine();
    } else if (token[0] == "NUM") {
        file_out << "        LOADI " << token[1] << endl;   //  !!!!!!!!!!!!!!!!!!
        getNextLine();
    } else {
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
        state = 5;//缺少类型标识符
    }
    getNextLine();
    if (token[0] != "ID") {
        state = 6;//不是标识符号
        return state;
    }
    state = name_def(token[1]);
    getNextLine();
    if (token[0] != ";") {
        state = 7;//缺少分号
        return state;
    }
    getNextLine();
    return state;
}

int name_def(string name) {
    int state = 0;
    vector<table *>::iterator iterator1 = varTable.begin();
    for (; iterator1 != varTable.end(); iterator1++) {
        if ((*iterator1)->name == name) {
            state = 30;//  变量重复定义
            return state;
        }
    }
    table *temp = new table;
    temp->name = name;
    temp->address = datap;
    datap++;
    varTable.push_back(temp);
    return state;
}

int look_up(string name, int *address) {
    int state = 0;
    vector<table *>::iterator iterator1 = varTable.begin();
    for (; iterator1 != varTable.end(); iterator1++) {
        if ((*iterator1)->name == name) {
            *address = (*iterator1)->address;
            return state;
        }
    }
    state = 23;//变量未声明
    return state;
}

void closeFile() {
    file.close();
    file_out.close();
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
    closeFile();
    return 0;
}
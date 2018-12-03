#include <iostream>
#include <string.h>
#include <fstream> 
#include <vector> 
#include <string>
using namespace std;

//类型判断 
char kind[10][12] = {" ","ID","保留字","NUM","分界符","运算符","注释"};
char keep[9][9] = {"if","else","for","while","int","write","read"};

//判断字符是否存在某个集合 
int isInclude(char ch,char str[]){
	for(int i = 0;i < strlen(str);i++){
		if(ch == str[i]){
			return 1;
		}
	} 
	return 0;
}


//写入文件 
void wirteToTxt(vector<char> str,int type,int line){
	fstream file;
	file.open("E:\\result.txt", ios::app | ios::out);
	int flag = 0;
	//  保留字是标识符的子集，所以当时标识符的时候需要判断是不是保留字。 
	if(type == 1){
		for(int i = 0;i < 7;i++){
			if(str.size() == strlen(keep[i])) {
				flag = 1;
				for(int j = 0;j < str.size();j++){
					if(str[j] != keep[i][j]) flag = 0;
				}
			}
			
			if(flag == 1) break;
		}
	}
	if(flag) type = 2; 
	if (!file.is_open())
	{
	    cout << "open worker txt file fail" << endl;
	}
	else
	{
		if(type == 1||type == 3){
			file<<kind[type]<<" ";
		}else{
			for(int i = 0;i < str.size();i++){
				file << str[i];
			}
			file<<" ";
		}
		for(int i = 0;i < str.size();i++){
			file << str[i];
		}
		file<<" "<<line<<endl;	
	    file.close();
   }
}

//报告错误 
void error(int line,int ch,char e){
	cout<<"第"<<line<<"行"<<" 第"<<ch<<"位置\""<<e<<"\"字符"<<"发生错误"<<endl; 
}

void errorNotEnd(int line,int ch,vector<char> e){
	cout<<"第"<<line<<"行"<<" 第"<<ch<<"位置";
	for(int i = 0;i < e.size();i++){
		cout<<e[i];
	}
	cout<<"字符"<<"发生错误"<<endl;
}

//词法分析 
void analysis(){
	//声明一些集合以及一些变量 
	string s;
	char num[10] = "123456789";
	char english[60] = "abcdefghigklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ";
	char demarcation[6] = "();{}";
	char state2[2] = "0";
	char state7[5] = "+-*";
	char state5[5] = "><=";
	char state6[2] = "!";
	char state8[2] = "/";
	char state9[2] = "*";
	vector<char> tempChar;
	
	fstream file;
    int line = 0; 
    string r;
	file.open("E:\\in.txt",ios::app|ios::out|ios::in);
	if (!file.is_open())
    {
        cout<< "creat worker file fail" << endl;
        return;
    }
    
    fstream file1;
	file1.open("E:\\result.txt", ios::trunc | ios::out);
	file1.close();
    
    int oldState = 0;
    
    while( getline(file,s) )
    {    
    	//方便最后一个字符的读写。 
    	s += ' ';
    	line++;
    	int ch = 0;
    	int state = 0;
    	if(oldState != 0) state = oldState;
		while(ch != s.size()){
			//判断状态，以及切换状态或者输出 
			switch(state){
				case 0:
					if(isInclude(s[ch],num)){
						tempChar.push_back(s[ch]);
						state = 3;
					}else if(isInclude(s[ch],english)){
						tempChar.push_back(s[ch]);
						state = 1;
					}else if(isInclude(s[ch],state2)){
						tempChar.push_back(s[ch]);
						state = 2;
					}else if(isInclude(s[ch],demarcation)){
						tempChar.push_back(s[ch]);
						state = 4;
					}else if(isInclude(s[ch],state7)){
						tempChar.push_back(s[ch]);
						state = 7;
					}else if(isInclude(s[ch],state5)){
						tempChar.push_back(s[ch]);
						state = 5;
					}else if(isInclude(s[ch],state6)){
						tempChar.push_back(s[ch]);
						state = 6;
					}else if(isInclude(s[ch],state8)){
						tempChar.push_back(s[ch]);
						state = 8;
					}else if(s[ch] != ' '){
						error(line,ch,s[ch]);
					}
					break;
				case 1:
					if(!isInclude(s[ch],num)&&!isInclude(s[ch],english)){
						state = 0;
						//输出，然后清空. 
						wirteToTxt(tempChar,1,line);
						tempChar.clear();
						ch--;
					}else{
						tempChar.push_back(s[ch]);
					}
					break;
				case 2:				
					state = 0;
					//输出，然后清空. 
					wirteToTxt(tempChar,3,line);
					tempChar.clear();
					ch--;
					break;
				case 3:
					if(isInclude(s[ch],state2)||isInclude(s[ch],num)){
						tempChar.push_back(s[ch]);
					}else if(!isInclude(s[ch],num)){
						state = 0;
						//输出，然后清空. 
						wirteToTxt(tempChar,3,line);
						tempChar.clear();
						ch--;
					}
					break;
				case 4:
					state = 0;
					//输出，然后清空. 
					wirteToTxt(tempChar,4,line);
					tempChar.clear();
					ch--;
					break;
				case 5:
					if(s[ch] == '='){
						tempChar.push_back(s[ch]);
						state = 7;
					}else{
						state = 0;
						//输出，然后清空. 
						wirteToTxt(tempChar,5,line);
						tempChar.clear();
						ch--;
					}
					break;
				case 6:
					if(s[ch] == '='){
						tempChar.push_back(s[ch]);
						state = 7;
					}else{
						//报错 
						errorNotEnd(line,ch,tempChar);
						tempChar.clear();
						state = 0;
					}
					break;
				case 7:
					state = 0;
					//输出，然后清空. 
					wirteToTxt(tempChar,5,line);
					tempChar.clear();
					ch--;
					break;
				case 8:
					if(s[ch] != '*'){
						state = 0;
						//输出，然后清空.
						wirteToTxt(tempChar,4,line);
						tempChar.clear();
						ch--; 
					}else{
						oldState = 9;
						state = 9;
					}
					break;
				case 9:
					if(s[ch] != '*'){
						state = 10;
						oldState = 10;
						tempChar.push_back(s[ch]);
					}else if(s[ch] == '*'){
						state = 11;
						oldState = 11;
						tempChar.push_back(s[ch]);
					}
					if(file.eof()){
						cout<<"第"<<line<<"行"<<" 第"<<ch<<"位置存在注释符不匹配"<<endl;
					}
					break;
				case 10:
					if(s[ch] != '*'){
						state = 10;
						oldState = 10;
						tempChar.push_back(s[ch]);
					}else if(s[ch] == '*'){
						state = 11;
						oldState = 11;
						tempChar.push_back(s[ch]);
					}
					if(file.eof()){
						cout<<"第"<<line<<"行"<<" 第"<<ch<<"位置存在注释符不匹配"<<endl;
					}
					break;
				case 11:
					if(s[ch] == '/'){
						state = 12;
						oldState = 12;
						tempChar.push_back(s[ch]);
					}else if(s[ch] == '*'){
						state = 11;
						oldState = 11;
						tempChar.push_back(s[ch]);
					}else {
						state = 10;
						oldState = 10;
						tempChar.push_back(s[ch]);
					}
					if(file.eof()){
						cout<<"第"<<line<<"行"<<" 第"<<ch<<"位置存在注释符不匹配"<<endl;
					}
					break;
				case 12:
					state = 0;
					oldState = 0;
					//输出，然后清空. 
					wirteToTxt(tempChar,6,line);
					tempChar.clear();
					ch--;
					break;
			}
			ch++;
		}
    }
}

int main(){

	analysis();
	cout<<"done";
	return 0;
} 



	

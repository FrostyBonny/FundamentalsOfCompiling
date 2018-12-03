#include <iostream>
#include <string.h>
#include <fstream> 
#include <vector> 
#include <string>
using namespace std;

//�����ж� 
char kind[10][12] = {" ","ID","������","NUM","�ֽ��","�����","ע��"};
char keep[9][9] = {"if","else","for","while","int","write","read"};

//�ж��ַ��Ƿ����ĳ������ 
int isInclude(char ch,char str[]){
	for(int i = 0;i < strlen(str);i++){
		if(ch == str[i]){
			return 1;
		}
	} 
	return 0;
}


//д���ļ� 
void wirteToTxt(vector<char> str,int type,int line){
	fstream file;
	file.open("E:\\result.txt", ios::app | ios::out);
	int flag = 0;
	//  �������Ǳ�ʶ�����Ӽ������Ե�ʱ��ʶ����ʱ����Ҫ�ж��ǲ��Ǳ����֡� 
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

//������� 
void error(int line,int ch,char e){
	cout<<"��"<<line<<"��"<<" ��"<<ch<<"λ��\""<<e<<"\"�ַ�"<<"��������"<<endl; 
}

void errorNotEnd(int line,int ch,vector<char> e){
	cout<<"��"<<line<<"��"<<" ��"<<ch<<"λ��";
	for(int i = 0;i < e.size();i++){
		cout<<e[i];
	}
	cout<<"�ַ�"<<"��������"<<endl;
}

//�ʷ����� 
void analysis(){
	//����һЩ�����Լ�һЩ���� 
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
    	//�������һ���ַ��Ķ�д�� 
    	s += ' ';
    	line++;
    	int ch = 0;
    	int state = 0;
    	if(oldState != 0) state = oldState;
		while(ch != s.size()){
			//�ж�״̬���Լ��л�״̬������� 
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
						//�����Ȼ�����. 
						wirteToTxt(tempChar,1,line);
						tempChar.clear();
						ch--;
					}else{
						tempChar.push_back(s[ch]);
					}
					break;
				case 2:				
					state = 0;
					//�����Ȼ�����. 
					wirteToTxt(tempChar,3,line);
					tempChar.clear();
					ch--;
					break;
				case 3:
					if(isInclude(s[ch],state2)||isInclude(s[ch],num)){
						tempChar.push_back(s[ch]);
					}else if(!isInclude(s[ch],num)){
						state = 0;
						//�����Ȼ�����. 
						wirteToTxt(tempChar,3,line);
						tempChar.clear();
						ch--;
					}
					break;
				case 4:
					state = 0;
					//�����Ȼ�����. 
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
						//�����Ȼ�����. 
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
						//���� 
						errorNotEnd(line,ch,tempChar);
						tempChar.clear();
						state = 0;
					}
					break;
				case 7:
					state = 0;
					//�����Ȼ�����. 
					wirteToTxt(tempChar,5,line);
					tempChar.clear();
					ch--;
					break;
				case 8:
					if(s[ch] != '*'){
						state = 0;
						//�����Ȼ�����.
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
						cout<<"��"<<line<<"��"<<" ��"<<ch<<"λ�ô���ע�ͷ���ƥ��"<<endl;
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
						cout<<"��"<<line<<"��"<<" ��"<<ch<<"λ�ô���ע�ͷ���ƥ��"<<endl;
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
						cout<<"��"<<line<<"��"<<" ��"<<ch<<"λ�ô���ע�ͷ���ƥ��"<<endl;
					}
					break;
				case 12:
					state = 0;
					oldState = 0;
					//�����Ȼ�����. 
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



	

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

char *prog;         //存储源代码字符串
char token[10];     //接收关键字，变量，运算符和界符
char ch;
int no;            //各单词符号对应的数字
int line = 1, col = 1; 
int length;
int m, p = 0;
string table[14] = {"program","begin","if","then","while","do","end","const","var","procedure","call","odd","read","write"};

void read()         //读取源代码
{
	FILE *fp;
	fp = fopen("code.txt", "r");
	fseek(fp, 0, SEEK_END);
	int file_size;
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	prog = (char *)malloc(file_size * sizeof(char));
	fread(prog, file_size, sizeof(char), fp);
	//关闭文件流
	fclose(fp);
}

void scaner()
{
    length = 0;
    for (int i = 0; i < 10; i ++)
		token[i] = '\0';

	ch = prog[p++];

    while(ch == ' ' ){       //消除空格
        ch = prog[p++];
        col++;
    }
    
    while(ch == '\n'){
        ch = prog[p++];
        line++;
        col = 1;
    }


    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
        m = 0;
        while((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')){
            token[m++] = ch;
            ch = prog[p++];
            col++;
            length++;
        }
        p--;
        int flag = 0;
        for(int i = 0; i < 14; i ++)
            if(token == table[i]){
                no = 1;
                flag = 1;
                break;
            }
        if(flag == 0)
            no = 2;
    }

    else if(ch >= '0' && ch <= '9'){
        m = 0;
        int flag = 0;
        while((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
            token[m++] = ch;
            ch = prog[p++];
            if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
                no = 7;
                flag = 1;
            }
            col++;
            length++;
        }
        p--;
        if(flag == 0)
            no = 3;
    }

    else {
        switch(ch){
            case '+': 
                token[0] = ch;
                no = 4;
                col++;
                length = 1;
                break;
            case '-': 
                token[0] = ch;
                no = 4;
                col++;
                length = 1;
                break;
            case '*': 
                token[0] = ch;
                no = 4;
                col++;
                length = 1;
                break;
            case '/': 
                token[0] = ch;
                no = 4;
                col++;
                length = 1;
                break;
            case '=':
                m = 0;
                token[m++] = ch;
                ch = prog[p++];
                col++;
                length = 1;
                if(ch == '='){
                    token[m++] = ch;
                    col++;
                    length = 2;
                }
                else
                    p--;
                no = 4;
                break;
            case '>':
                m = 0;
                token[m++] = ch;
                ch = prog[p++];
                col++;
                length = 1;
                if(ch == '='){
                    token[m++] = ch;
                    col++;
                    length = 2;
                }
                else
                    p--;
                no = 4;
                break;
            case '<':
                m = 0;
                token[m++] = ch;
                ch = prog[p++];
                col++;
                length = 1;
                if(ch == '=' || ch == '>'){
                    token[m++] = ch;
                    col++;
                    length = 2;
                }
                else
                    p--;
                no = 4;
                break;
            case '#': 
                token[0] = ch;
                no = 4;
                col++;
                length = 1;
                break;
            case ':':
                m = 0;
                token[m++] = ch;
                ch = prog[p++];
                col++;
                length = 1;
                if(ch == '='){
                    token[m++] = ch;
                    no = 4;
                    col++;
                    length = 2;
                }
                else{
                    p--;
                    no = 6;
                }
                break;
            case ',':
                token[0] = ch;
                no = 5;
                col++;
                length = 1;
                break;
            case ';':
                token[0] = ch;
                no = 5;
                col++;
                length = 1;
                break;
            case '(':
                token[0] = ch;
                no = 5;
                col++;
                length = 1;
                break;
            case ')':
                token[0] = ch;
                no = 5;
                col++;
                length = 1;
                break;
            default:
                p += 2;
                no = 8;
                col++;
                break;
        }

    }

}

int main()
{
    read();

	fstream fp("mid.txt" , ios::out);

    while(prog[p] != '\0'){
        scaner();
        switch(no){
            case 1:
                fp << token << " keyword " << line << " " << col - length << endl;
                break;
            case 2:
                fp << token << " identifier " << line << " " << col - length << endl;
                break;
            case 3:
                fp << token << " constant " << line << " " << col - length << endl;
                break;
            case 4:
                fp << token << " operator " << line << " " << col - length << endl;
                break;
            case 5:
                fp << token << " delimiter " << line << " " << col - length << endl; 
                break;
            case 6:
                fp <<"Waring line:" << line << " col:" << col - length << " ':'without'='" << endl;
                break;
            case 7:
                fp <<"Waring line:" << line << " col:" << col - length << " string start with number" << endl;
                break;
            case 8:
                fp <<"Waring:" << line << "行" << col - length << "列" << "非法字符" << endl;
                break;
        }
        
    }

    fp.close();
}

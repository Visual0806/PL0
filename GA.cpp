/*                                                                                      First集
<prog> → program <id>；<block>                                                          First(prog) = { program }
<block> → [<condecl>][<vardecl>][<proc>]<body>                                          First(block) = { const , var , procedure , begin }
<condecl> → const <const>{,<const>};                                                    First(condecl) = { const }
<const> → <id>:=<integer>                                                               First(const) = { l }
<vardecl> → var <id>{,<id>};                                                            First(vardecl) = { var }
<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}                               First(proc) = { procedure }
<body> → begin <statement>{;<statement>}end                                             First(body) = { begin }
<statement> → <id> := <exp>                                                             First(statement) = { l , if , while , call , begin , read , write }
               |if <lexp> then <statement>[else <statement>]
               |while <lexp> do <statement>
               |call <id>（[<exp>{,<exp>}]）
               |<body>
               |read (<id>{，<id>})
               |write (<exp>{,<exp>})
<lexp> → <exp> <lop> <exp>|odd <exp>                                                    First(lexp) = { + , - , l , d , ( ,odd }
<exp> → [+|-]<term>{<aop><term>}                                                        First(exp) = { + , - , l , d , ( }
<term> → <factor>{<mop><factor>}                                                        First(term) = { l , d , ( }
<factor>→<id>|<integer>|(<exp>)                                                         First(factor) = { l , d , ( }
<lop> → =|<>|<|<=|>|>=                                                                  First(lop) = { = , < , > }
<aop> → +|-                                                                             First(aop) = { + , - }
<mop> → *|/                                                                             First(mop) = { * , / }
<id> → l{l|d}   （注：l表示字母）                                                        First(id) = { l }
<integer> → d{d}                                                                        First(integer) = { d }
注释：
<prog>：程序 ；<block>：块、程序体 ；<condecl>：常量说明 ；<const>：常量；
<vardecl>：变量说明 ；<proc>：分程序 ； <body>：复合语句 ；<statement>：语句；
<exp>：表达式 ；<lexp>：条件 ；<term>：项 ； <factor>：因子 ；<aop>：加法运算符；
<mop>：乘法运算符； <lop>：关系运算符
odd：判断表达式的奇偶性。
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

using namespace std;

class Word{
    public:
        string value;
        string key;
        int line;
        int column;

        void print();
};

string token;
Word word;
int errorStack[100] = {-1};
int *errorType = errorStack;
bool error = false;

void Word::print() {
    cout << "-----------------" << endl;
    cout << "Value: " << value << endl;
    cout << "Key: " << key << endl;
    cout << "Position: [" << line << "," << column << "]" << endl;
    cout << "-----------------" << endl;
}

void readline()
{
	fstream fp("mid.txt" , ios::in);

    if(getline(fp, token)){
        istringstream iss(token);
        iss >> word.value >> word.key >> word.line >> word.column;
    }

    fp.close();
}

void Error(int type)
{
    switch(type){
        case 0:
        case 1:
    }

}

void Prog()
{
    readline();
    if(word.value == "program" || *errorType == 0){
        if(*errorType == 0) 
            errorType++;
        readline();
        if(word.key == "identifier" || *errorType == 1){
            if(*errorType == 1) 
                errorType++; 
            else 
                readline();
            if(word.value == ";" || *errorType == 2){
                if(*errorType == 2 && word.value != ";") 
                    errorType++; 
                else 
                    readline();
                Block();
            } 
            else
                Error(2);
        } 
        else
            Error(1);
    } 
    else
        Error(0);
}

void Block()
{
    if(word.value == "const" && !error){
        readline();
        Condecl();
    } 
    // else if(word.key == "identifier" && *errorType != 8 && !error){
    //     Error(8);
    //     while(word.key != "EOP" && word.key != "RESERVED"){
    //         readline();
    //     }
    //     readline();
    // } 
    // else if(*errorType == 8 && !error) {
    //     errorType++;
    //     while(word.key != "EOP" && word.key != "RESERVED"){
    //         readline();
    //     }
    //     readline();
    // }

    if(word.value == "var" && !error){
        readline();
        Vardecl();
    } 
    // else if(word.key == "ID" && *errorType != 8 && !error){
    //     Error(8); // Cannot resolve type
    //     while(word.key != "EOP" && word.key != "RESERVED"){
    //         readline();
    //     }
    //     readline();
    // } 
    // else if(*errorType == 8 && !error) {
    //     errorType++;
    //     while(word.key != "EOP" && word.key != "RESERVED"){
    //         readline();
    //     }
    //     readline();
    // }

    if(word.value == "procedure" && !error){
        readline();
        Proc();
    }
    // else if(word.key == "ID" && *errorType != 8 && !error){
    //     Error(8); // Cannot resolve type
    //     while(word.key != "EOP" && word.key != "RESERVED"){
    //         readline();
    //     }
    //     readline();
    // }
    // else if(*errorType == 8 && !error) {
    //     errorType++;
    //     while(word.key != "EOP" && word.key != "RESERVED"){
    //         readline();
    //     }
    //     readline();
    // }

    if(!error){
        Body();
    }
}

void Condecl()
{
    if(word.value == "identifier"  || *errorType == 3){
        if(word.key != "identifier") 
            errorType++;
        readline();
        if(word.value == ":=" || *errorType == 4){
            if(word.value != ":=") 
                errorType++; 
            else 
                readline();

            if(word.key == "constant" || *errorType == 5){
                // if(word.key != "constant" && *errorType == 5){
                //     errorType++;
                //     while(word.value != "," && word.value != ";"){
                //         readline();
                //         if(word.key == "identifier") 
                //             break;
                //     }
                // } 
                // else 
                //     readline();

                while(word.value == "," || *errorType == 27){
                    if(*errorType == 27 && word.value != ",") 
                        errorType++; 
                    else 
                        readline();

                    if(word.key == "identifier"|| *errorType == 3){
                        if(word.key != "identifier") 
                            errorType++; 
                        else 
                            readline();

                        if(word.value == ":=" || *errorType == 4){
                            if(word.value != ":=") 
                                errorType++; 
                            else 
                                readline();

                            if(word.key == "constant" || *errorType == 5){
                                if(word.key != "constant"){
                                    errorType++;
                                    while(word.value != "," && word.key != ";"){
                                        readline();
                                    }
                                } 
                                else
                                    readline();
                            } 
                            else
                                Error(5); // Missing INT
                        } 
                        else 
                            Error(4); // Missing AOP
                    } 
                    else
                        Error(3); // Missing id
                    if(error) 
                        break;
                }

                // if(word.key == "ID" && !error) 
                //     Error(27);// Missing comma
                
                // while (word.value == ",") 
                //     readline();

                if (word.value == ";" || *errorType == 2 || error){
                    if(word.value != ";" && !error) 
                        errorType++; 
                    else 
                        readline();
                
                } 
                else 
                    Error(2); // Missing EOP
            } 
            else 
                Error(5); // Missing INT
        } 
        else
            Error(4); // Missing AOP
    } 
    else
        Error(3); // Missing id
}

void Vardecl()
{
    if(word.key == "identifier"){
        readline();

        while(word.value == "," || *errorType == 26){
            if(*errorType == 26 && word.value != ",") 
                errorType++; 
            else 
                readline();

            if (word.key == "identifier"){
                readline();
            } 
            else
                Error(7);// Missing ID
        }

        if(word.key == "identifier")
            Error(26);

        if (word.value == ";" || *errorType == 2){
            if (*errorType == 2 && word.value != ";") 
                errorType++;
            else 
                readline();
        } 
        else 
            Error(2); // Missing EOP
    } 
    else
        Error(6); // Missing ID
}

void Proc()
{

}

void Body()
{

}

void Statement()
{

}

void Lexp()
{

}

void Exp()
{

}

void Term()
{

}

void Factor()
{

}

int main()
{
    readline();
}

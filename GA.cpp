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
<lexp> → <exp> <lop> <exp>|odd <exp>                                                    First(lexp) = { + , - , l , d , ( , odd }
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

fstream fp("GA_out.txt" , ios::out);
fstream fp1("mid.txt" , ios::in);
string token;
Word word;

void Word::print() {
    fp << "-----------------" << endl;
    fp << "Value: " << value << endl;
    fp << "Key: " << key << endl;
    fp << "Position: [" << line << "," << column << "]" << endl;
    fp << "-----------------" << endl;
}

void Prog();
void Block();
void Condecl();
void Vardecl();
void Proc();
void Body();
void Statement();
void Exp();
void Lexp();
void Term();
void Factor();

void readline()
{
    if(getline(fp1, token)){
        istringstream iss(token);
        iss >> word.value >> word.key >> word.line >> word.column;
    }
}

void Error(int type)
{
    switch(type){
        case 0:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'program'" << endl;
            break;
        case 1:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <id>" << endl;
            break;
        case 2:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing ';'" << endl;
            break;
        case 3:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <body>" << endl;
            break;
        case 4:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing ':='" << endl;
            break;
        case 5:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <integer>" << endl;
            break;
        case 6:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing ','" << endl;
            break;
        case 7:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing '('" << endl;
            break;
        case 8:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing ')'" << endl;
            break;
        case 9:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'end'" << endl;
            break;
        case 10:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'then'" << endl;
            break;
        case 11:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'do'" << endl;
            break;
        case 12:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <exp>" << endl;
            break;
        case 13:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <Statement>" << endl;
            break;
        case 14:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <lexp>" << endl;
            break;
        case 15:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <lop>" << endl;
            break;
        case 16:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing <factor>" << endl;
            break;
        case 17:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'procedure'" << endl;
            break;
        case 18:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'const'" << endl;
            break;
        case 19:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'var'" << endl;
            break;
        case 20:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Missing 'begin'" << endl;
            break;
    }

}

//<prog> → program <id>；<block>                                                          First(prog) = { program }
void Prog()
{
    readline();
    if(word.value == "program"){
        readline();
        if(word.key == "identifier"){
            readline();
            if(word.value == ";"){
                readline();
                Block();
            } 
            else
                Error(2);   //Missing ';'
        } 
        else
            Error(1);   //Missing <id>
    } 
    else
        Error(0);   //Missing 'program'
}

//<block> → [<condecl>][<vardecl>][<proc>]<body>                                          First(block) = { const , var , procedure , begin }
void Block()
{
    if(word.value == "const"){
        Condecl();
    } 

    if(word.value == "var"){
        Vardecl();
    } 

    if(word.value == "procedure"){
        Proc();
    }

    if(word.value == "begin"){
        Body();
    }
    else{
        Error(3);   //Missing <body>
    }
}

//<condecl> → const <const>{,<const>};                                                    First(condecl) = { const }
void Condecl()
{
    if(word.value == "const"){
        readline();
        if(word.key == "identifier"){
            readline();
            if(word.value == ":="){
                readline();
                if(word.key == "constant"){
                    readline();
                    while(word.value == ","){
                        readline();
                        if(word.key == "identifier"){
                            readline();
                            if(word.value == ":="){
                                readline();
                                if(word.key == "constant"){
                                    readline();
                                }
                                else
                                    Error(5);   //Missing <integer>
                            } 
                            else 
                                Error(4);   //Missing ':='
                        } 
                        else
                            Error(1);   //Missing <id>
                    }

                    if(word.value == ";")
                        readline();
                    else 
                        Error(2);   //Missing ';'
                } 
                else 
                    Error(5);   //Missing <integer>
            } 
            else
                Error(4);   //Missing ':='
        } 
        else
            Error(1);   //Missing <id>
    }
    else
        Error(18);  //Missing 'const'
}

//<vardecl> → var <id>{,<id>};                                                            First(vardecl) = { var }
void Vardecl()
{
    if(word.value == "var"){
        readline();
        if(word.key == "identifier"){
            readline();
            while(word.value == ","){
                readline();
                if (word.key == "identifier"){
                    readline();
                } 
                else
                    Error(1);   //Missing <id>
            }

            if(word.key == "identifier")
                Error(6);   //Missing ','

            if(word.value == ";"){
                readline();
            } 
            else 
                Error(2);   //Missing ';'
        } 
        else
            Error(1);   //Missing <id>
    }
    else
        Error(19);  //Missing 'var'
}

//<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}                               First(proc) = { procedure }
void Proc()
{   
    if(word.value == "procedure"){
        readline();
        if(word.key == "identifier"){
            readline();
            if(word.value == "("){
                readline();
                if(word.key == "identifier"){
                    readline();
                    while(word.value == ","){
                        readline();
                        if(word.key == "identifier")
                            readline();
                        else    
                            Error(1);   //Missing <id>
                    }

                    if(word.key == "identifier")
                        Error(6);   //Missing ','                   
                }

                if(word.value == ")"){
                    readline();
                    if(word.value == ";"){
                        readline();
                        Block();

                        while(word.value == ";"){
                            readline();
                            Proc();
                        }
                    }
                    else    
                        Error(2);   //Missing ';'
                }
                else    
                    Error(8);   //Missing ')'
            }
            else
                Error(7);   //Missing '('
        }
        else
            Error(1);   //Missing <id>
    }
    else
        Error(17);  //Missing 'procedure'

}

//<body> → begin <statement>{;<statement>}end                                             First(body) = { begin }
void Body()
{
    if(word.value == "begin"){
        readline();
        Statement();
        while(word.value == ";"){
            readline();
            Statement();
        }

        if(word.value == "end"){
            readline();
        }
        else 
            Error(9);   //Missing 'end'
    }
    else
        Error(20);  //Missing 'begin'
}

// <statement> → <id> := <exp>                                                             First(statement) = { l , if , while , call , begin , read , write }
//                |if <lexp> then <statement>[else <statement>]
//                |while <lexp> do <statement>
//                |call <id>（[<exp>{,<exp>}]）
//                |<body>
//                |read (<id>{，<id>})
//                |write (<exp>{,<exp>})
void Statement()
{
    if(word.key == "identifier"){
        readline();
        if(word.value == ":="){
            readline();
            Exp();
        }
        else
            Error(4);   //Missing ':='
    }

    else if(word.value == "if"){
        readline();
        Lexp();
        if(word.value == "then"){
            readline();
            Statement();
            if(word.value == "else"){
                readline();
                Statement();
            }
        }
        else
            Error(10);  //Missing 'then'
    }
    
    else if(word.value == "while"){
        readline();
        Lexp();
        if(word.value == "do"){
            readline();
            Statement();
        }
        else    
            Error(11);  //Missing 'do'
    }

    else if(word.value == "call"){
        readline();
        if(word.key == "identifier"){
            readline();
            if(word.value == "("){
                readline();
                if(word.value == "+" || word.value == "-" || word.key == "identifier" || word.key == "constant" || word.value == "("){
                    Exp();

                    while(word.value == ","){
                        readline();
                        if(word.value == "+" || word.value == "-" || word.key == "identifier" || word.key == "constant" || word.value == "(")
                            Exp();
                        else
                            Error(12);  //Missing <exp>
                    }                   
                }
                
                if(word.value == ")")
                    readline();
                else
                    Error(8);   //Missing ')'
            }
            else
                Error(7);   //Missing '('
        }
        else
            Error(1);   //Missing <id>
    }

    else if(word.value == "begin"){
        Body();
    }

    else if(word.value == "read"){
        readline();
        if(word.value == "("){
            readline();
            if(word.key == "identifier"){
                readline();
                while(word.value == ","){
                    readline();
                    if(word.key == "identifier"){
                        readline();
                    }
                    else
                        Error(1);   //Missing <id>  
                }

                if(word.key == "identifier"){
                    Error(6);   //Missing ','
                }

                if(word.value == ")"){
                    readline();
                }
                else
                    Error(8);   //Missing ')'
            }
            else
                Error(1);   //Missing <id>  
        }
        else
            Error(7);   //Missing '('
    }

    else if(word.value == "write"){
        readline();
        if(word.value == "("){
            readline();
            Exp();
            while(word.value == ","){
                readline();
                Exp();
            }

            if(word.value == "+" || word.value == "-" || word.key == "identifier" || word.key == "constant" || word.value == "("){
                Error(6);   //Missing ','
            }

            if(word.value == ")"){
                readline();
            }
            else
                Error(8);   //Missing ')'
        }
        else
            Error(7);   //Missing '('
    }

    else
        Error(13);  //Missing <statement>
}

//<lexp> → <exp> <lop> <exp>|odd <exp>                                                    First(lexp) = { + , - , l , d , ( , odd }
//<lop> → =|<>|<|<=|>|>=
void Lexp()
{
    if(word.value == "+" || word.value == "-" || word.key == "identifier" || word.key == "constant" || word.value == "("){
        Exp();
        if(word.value == "=" || word.value == "<>" || word.value == "<" || word.value == "<=" || word.value == ">" || word.value == ">="){
            readline();
            Exp();
        }
        else
            Error(15);  //Missing <lop>
    }

    else if(word.value == "odd"){
        readline();
        Exp();
    }

    else
        Error(14);  //Missing <lexp>

}

//<exp> → [+|-]<term>{<aop><term>}                                                        First(exp) = { + , - , l , d , ( }
//<aop> → +|-            
void Exp()
{
    if(word.value == "+" || word.value == "-"){
        readline();
    }

    Term();

    while(word.value == "+" || word.value == "-"){
        readline();
        Term();
    }
}

//<term> → <factor>{<mop><factor>}                                                        First(term) = { l , d , ( }
//<mop> → *|/ 
void Term()
{
    Factor();
    while(word.value == "*" || word.value == "/"){
        readline();
        Factor();
    }

}

//<factor>→<id>|<integer>|(<exp>)                                                         First(factor) = { l , d , ( }
void Factor()
{
    if(word.key == "identifier"){
        readline();
    }

    else if(word.key == "constant"){
        readline();
    }

    else if(word.value == "("){
        Exp();
        if(word.value == ")"){
            readline;
        }
        else
            Error(8);   //Missing ')'
    }

    else
        Error(16);  //Missing <factor>;
}

int main()
{
    Prog();
    fp.close();
    fp1.close();
}

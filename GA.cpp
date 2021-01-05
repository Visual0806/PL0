/*                                                                                      First集
<prog> → program <id>；<block>                                                          First(prog) = { program }
<block> → [<condecl>][<vardecl>][<proc>]<body>                                          First(block) = { const , var , procedure , begin }
<condecl> → const <const>{,<const>};                                                    First(condecl) = { const }
<const> → <id>:=<integer>                                                               First(const) = { l }
<vardecl> → var <id>{,<id>};                                                            First(vardecl) = { var }
<proc> → procedure <id>([<id>{,<id>}]);<block>{;<proc>}                                 First(proc) = { procedure }
<body> → begin <statement>{;<statement>}end                                             First(body) = { begin }
<statement> → <id> := <exp>                                                             First(statement) = { l , if , while , call , begin , read , write }
               |if <lexp> then <statement>[else <statement>]
               |while <lexp> do <statement>
               |call <id>（[<exp>{,<exp>}]）
               |<body>
               |read (<id>{，<id>})
               |write (<exp>{,<exp>})       v:=(a+c)*(b-d)
<lexp> → <exp> <lop> <exp>|odd <exp>                                                    First(lexp) = { + , - , l , d , ( , odd }
<exp> → [+|-]<term>{<aop><term>}                                                        First(exp) = { + , - , l , d , ( }
<term> → <factor>{<mop><factor>}                                                        First(term) = { l , d , ( }
<factor>→<id>|<integer>|(<exp>)                                                         First(factor) = { l , d , ( }
<lop> → =|<>|<|<=|>|>=                                                                  First(lop) = { = , < , > }
<aop> → +|-                                                                             First(aop) = { + , - }
<mop> → *|/                                                                             First(mop) = { * , / }
<id> → l{l|d}   (注：l表示字母)                                                          First(id) = { l }
<integer> → d{d}                                                                        First(integer) = { d }
注释：
<prog>：程序 ；<block>：块、程序体 ；<condecl>：常量说明 ；<const>：常量；
<vardecl>：变量说明 ；<proc>：分程序 ； <body>：复合语句 ；<statement>：语句；
<exp>：表达式 ；<lexp>：条件 ；<term>：项 ； <factor>：因子 ；<aop>：加法运算符；
<mop>：乘法运算符； <lop>：关系运算符
odd：判断表达式的奇偶性。
*/
/*
    OPR 0 0	    过程调用结束后,返回调用点并退栈
    OPR 0 1	    栈顶元素取反
    OPR 0 2	    次栈顶与栈顶相加，退两个栈元素，结果值进栈
    OPR 0 3	    次栈顶减去栈顶，退两个栈元素，结果值进栈
    OPR 0 4	    次栈顶乘以栈顶，退两个栈元素，结果值进栈
    OPR 0 5	    次栈顶除以栈顶，退两个栈元素，结果值进栈
    OPR 0 6	    栈顶元素的奇偶判断，结果值在栈顶
    OPR 0 7
    OPR 0 8	    次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
    OPR 0 9	    次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
    OPR 0 10    次栈顶是否小于栈顶，退两个栈元素，结果值进栈
    OPR 0 11    次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
    OPR 0 12    次栈顶是否大于栈顶，退两个栈元素，结果值进栈
    OPR 0 13    次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
    OPR 0 14	栈顶值输出至屏幕,并且输出一个空格
    OPR 0 15	屏幕输出换行
    OPR 0 16	从命令行读入一个输入置于栈顶
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 

#include "Symbol.cpp"
//#include "Pcode.cpp"
#include "Interpreter.cpp"

using namespace std;

class Word{
    public:
        string value;
        string key;
        int line;
        int column;
};

fstream fp("GA_out.txt" , ios::out);
fstream fp1("LA_out.txt" , ios::in);
string token;
Word word;

int level = 0;
int address = 0;
string name;

SymTable Table;
APcode Pcode;

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
        case 21:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] '" << word.value << "' has been declared" << endl;
            break;
        case 22:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] '" << name << "' is not Var" << endl;
            break;
        case 23:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] '" << name << "' is not Proc" << endl;
            break;
        case 24:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] " << "Number of parameters is wrong" << endl;
            break;
        case 25:
            fp << "[Grammar ERROR] " << " [" << word.line << "," << word.column << "] '" << word.value << "' has not been declared" << endl;
            break;
    }
}

//<prog> → program <id>;<block>                                                          First(prog) = { program }
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
            else{
                Error(2);   //Missing ';'
                Block();
            }
        } 
        else{
            Error(1);   //Missing <id>
            if(word.value == ";"){
                readline();
                Block();
            } 
            else{
                Error(2);   //Missing ';'
                Block();
            }
        }
    } 
    else
        Error(0);   //Missing 'program'
}

//<block> → [<condecl>][<vardecl>][<proc>]<body>                                          First(block) = { const , var , procedure , begin }
void Block()
{
    int addr0 = address;                  //记录本层之前的数据量（本层活动记录的起始地址），以便恢复时返回
    int tx0 = Table.getTablePtr();       //记录本层过程在符号表的初始位置
    int cx0;							  //保存需要回填的jmp指令在pcode中的索引
    int propos = 0;					      //本层过程在符号表中的位置

    if(tx0 > 0){
        propos = Table.getLevelProc();                 //过程所在位置
        tx0 = tx0 - Table.getRow(propos).getSize();    //记录 <本层变量> 在符号表的开始位置
    }

    if(tx0 == 0){           //最外层还未开辟活动存储空间
        address = 3;        //每一层最开始位置的三个空间用来存放静态链SL、动态链DL、和返回地址RA
    }
    else{                   //紧接着放形参的个数,形参个数存储在过程的size域
        address = 3 + Table.getAllTable()[propos].getSize();
    }

    //暂存当前Pcode.codePtr的值，即jmp,0,0在codePtr中的位置，用来一会回填
    cx0 = Pcode.getCodePtr();
    Pcode.gen(Pcode.getJMP(),0,0);

    if(word.value == "const"){
        Condecl();
    }

    if(word.value == "var"){
        Vardecl();
    }

    if(word.value == "procedure"){
        Proc();
        level--;
    }

    if(tx0 > 0){        //实参数数值传入形参变量
        for(int i = 0;i < Table.getAllTable()[propos].getSize();i++){
            Pcode.gen(Pcode.getSTO(),0,Table.getAllTable()[propos].getSize() + 3 - 1 - i);
        }
    }
    
    //回填至无条件指令
    Pcode.getPcodeArray()[cx0].setA(Pcode.getCodePtr());
    Pcode.gen(Pcode.getINT(),0,address);        //生成本层程序活动记录内存的代码
    
    if(tx0 != 0){   //过程入口地址填入value
        Table.getAllTable()[propos].setValue(Pcode.getCodePtr() - 1 - Table.getAllTable()[propos].getSize());    
    }

    Body();

    Pcode.gen(Pcode.getOPR(),0,0);      //生成退出过程的代码，若是主程序，则直接退出程序

    address = addr0;                    //分程序结束，恢复address 和符号表下一项即将填写那一项指针
    Table.setTablePtr(tx0);
    if(Table.getAllTable()[tx0].getLevel() > 0)
        Table.clearTheFowllowing(tx0);
}

//<condecl> → const <const>{,<const>};                                                    First(condecl) = { const }
void Condecl()
{
    if(word.value == "const"){
        readline();
        if(word.key == "identifier"){
            name = word.value;
            if(Table.isNowExistSTable(word.value,level)){
                Error(21);
            }
            readline();
            if(word.value == ":="){
                readline();
                if(word.key == "constant"){
                    Table.enterConst(name,level,word.value,address);//登录符号表
                    readline();
                    while(word.value == ","){
                        readline();
                        if(word.key == "identifier"){
                            name = word.value;
                            if(Table.isNowExistSTable(word.value,level)){
                                Error(21);
                            }
                            readline();
                            if(word.value == ":="){
                                readline();
                                if(word.key == "constant"){  
                                    Table.enterConst(name,level,word.value,address);//登录符号表
                                    readline();
                                }
                                else
                                    Error(5);   //Missing <integer>
                            } 
                            else{
                                Error(4);   //Missing ':='
                                if(word.key == "constant"){
                                    Table.enterConst(name,level,word.value,address);//登录符号表
                                    readline();
                                }
                                else
                                    Error(5);   //Missing <integer>
                            }
                        } 
                        else{
                            Error(1);   //Missing <id>
                            if(word.value == ":="){
                                readline();
                                if(word.key == "constant"){
                                    Table.enterConst(name,level,word.value,address);//登录符号表
                                    readline();
                                }
                                else
                                    Error(5);   //Missing <integer>
                            } 
                            else{
                                Error(4);   //Missing ':='
                                if(word.key == "constant"){
                                    Table.enterConst(name,level,word.value,address);//登录符号表
                                    readline();
                                }
                                else
                                    Error(5);   //Missing <integer>
                            }
                        }
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
            name = word.value;
            if(Table.isNowExistSTable(name,level)){
                Error(21);
            }
            Table.enterVar(name,level,address);//登录符号表
            address++;
            readline();
            while(word.value == ","){
                readline();
                if (word.key == "identifier"){
                    name = word.value;
                    if(Table.isNowExistSTable(name,level)){
                        Error(21);
                    }
                    Table.enterVar(name,level,address);//登录符号表
                    address++;
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
        int count = 0;
        int propos = 0;
        readline();
        if(word.key == "identifier"){
            name = word.value;
            if(Table.isNowExistSTable(name,level)){
                Error(21);
            }
            propos = Table.getTablePtr();
            Table.enterProc(name,level,address);//登录符号表
            level++;                //level值加一，因为其后的所有定义均在该新的proc中完成
            readline();
            if(word.value == "("){
                readline();
                if(word.key == "identifier"){
                    Table.enterVar(word.value,level,3 + count) ;//（3+count）形式单元位于连接数据之后
                    count++;
                    Table.getAllTable()[propos].setSize(count);
                    readline();
                    while(word.value == ","){
                        readline();
                        if(word.key == "identifier"){
                            Table.enterVar(word.value,level,3 + count) ;//（3+count）形式单元位于连接数据之后（P244）
                            count++;
                            Table.getAllTable()[propos].setSize(count);
                            readline();
                        }
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
        name = word.value;
        if(!Table.isPreExistSTable(word.value,level)){        //检查标识符是否在符号表中存在
            Error(21);
        }
        readline();
        if(word.value == ":="){
            readline();
            Exp();
            TableUnit temp = Table.getRow(Table.getNameRow(name));
            if(temp.getType() == Table.getVar()){ //检查标识符是否为变量类型
                Pcode.gen(Pcode.getSTO(),level-temp.getLevel(),temp.getAddress());  
                //STO L ，a 将数据栈栈顶的内容存入变量
            }
            else{       
                Error(22);  //类型不一致的错误
            }
        }
        else{
            Error(4);   //Missing ':='
            Exp();
        }
    }

    else if(word.value == "if"){
        int cx1;
        readline();
        Lexp();
        if(word.value == "then"){
            cx1 = Pcode.getCodePtr();       //用cx1记录jpc ，0，0在Pcode中的地址，用来回填
            Pcode.gen(Pcode.getJPC(),0,0);  //产生条件转移指令，条件的bool值为0时跳转，
                                            //跳转的目的地址暂时填为0
            readline();
            Statement();

            int cx2 = Pcode.getCodePtr();  //cx2记录jmp在Pcode中的地址，一会用来回填
            Pcode.gen(Pcode.getJMP(),0,0);
            Pcode.getPcodeArray()[cx1].setA(Pcode.getCodePtr()); 
            //地址回填，将jpc，0，0中的A回填
            Pcode.getPcodeArray()[cx2].setA(Pcode.getCodePtr());//考虑到没有else的情况
            if(word.value == "else"){
                readline();
                Statement();
                Pcode.getPcodeArray()[cx2].setA(Pcode.getCodePtr());
            }
        }
        else{
            Error(10);  //Missing 'then'
            Statement();
            if(word.value == "else"){
                readline();
                Statement();
            }
        }
    }
    
    else if(word.value == "while"){
        int cx1 = Pcode.getCodePtr(); //保存条件表达式在Pcode中的地址，便于do后的循环
        readline();
        Lexp();
        if(word.value == "do"){
            int cx2 = Pcode.getCodePtr();//保存条件跳转指令的地址，在回填时使用，仍是条件不符合是跳转
            Pcode.gen(Pcode.getJPC(),0,0);
            readline();
            Statement();
            Pcode.gen(Pcode.getJMP(),0,cx1); //完成DO后的相关语句后，需要跳转至条件表达式处，
                								//检查是否符合条件，即是否继续循环
            Pcode.getPcodeArray()[cx2].setA(Pcode.getCodePtr()); //回填JPC条件转移指令
        }
        else{    
            Error(11);  //Missing 'do'
            Statement();
        }
    }

    else if(word.value == "call"){
        int count = 0;//用来检验传入的参数和设定的参数是否相等
        TableUnit temp;
        readline();
        if(word.key == "identifier"){
            if(Table.isPreExistSTable(word.value,level)){        //符号表中存在该标识符
                temp = Table.getRow(Table.getNameRow(word.value));  
                    //获取该标识符所在行的所有信息，保存在tempRow中
                if(temp.getType() != Table.getProc()){       //cal类型不一致的错误
                    Error(23);
                }
            }       //if符号表中存在标识符
            readline();
            if(word.value == "("){
                readline();
                if(word.value == "+" || word.value == "-" || word.key == "identifier" || word.key == "constant" || word.value == "("){
                    Exp();
                    count++;
                    while(word.value == ","){
                        readline();
                        if(word.value == "+" || word.value == "-" || word.key == "identifier" || word.key == "constant" || word.value == "("){
                            Exp();
                            count++;
                        }
                        else
                            Error(12);  //Missing <exp>
                    }  
                    if(count != temp.getSize()){//参数个数是否匹配
                        Error(24);
                    }
                    //地址存在tempRow.getValue()
                    Pcode.gen(Pcode.getCAL(),level-temp.getLevel(),temp.getValue());                 
                }
                
                if(word.value == ")"){
                    Pcode.gen(Pcode.getCAL(),level-temp.getLevel(),temp.getValue());        //调用过程中的保存现场由解释程序完成，这里只产生目标代码,+3需详细说明
                    readline();
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

    else if(word.value == "begin"){
        Body();
    }

    else if(word.value == "read"){
        readline();
        if(word.value == "("){
            readline();
            if(word.key == "identifier"){
                if(!Table.isPreExistSTable(word.value,level)){//首先判断在符号表中在本层或本层之前是否有此变量
                    Error(21);
                }//if判断在符号表中是否有此变量
                else{        
                    TableUnit temp = Table.getRow(Table.getNameRow(word.value));
                    if(temp.getType() == Table.getVar()){       //该标识符是否为变量类型
                        Pcode.gen(Pcode.getRED(),level-temp.getLevel(),temp.getAddress());
                    }//if标识符是否为变量类型
                    else{     
                        Error(22);
                    }
                }
                readline();
                while(word.value == ","){
                    readline();
                    if(word.key == "identifier"){
                        if(!Table.isPreExistSTable(word.value,level)){//首先判断在符号表中在本层或本层之前是否有此变量
                            Error(21);
                        }//if判断在符号表中是否有此变量
                        else{        
                            TableUnit temp = Table.getRow(Table.getNameRow(word.value));
                            if(temp.getType() == Table.getVar()){       //该标识符是否为变量类型
                                Pcode.gen(Pcode.getRED(),level-temp.getLevel(),temp.getAddress());
                            }//if标识符是否为变量类型
                            else{       
                                Error(22);
                            }
                        }
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
            Pcode.gen(Pcode.getWRT(),0,0);
            while(word.value == ","){
                readline();
                Exp();
                Pcode.gen(Pcode.getWRT(),0,0);
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
        string oper = word.value;
        if(word.value == "=" || word.value == "<>" || word.value == "<" || word.value == "<=" || word.value == ">" || word.value == ">="){
            readline();
            Exp();
            if(oper == "=")
                Pcode.gen(Pcode.getOPR(),0,8);      //OPR 0 8	次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
            else if(oper == "<>")
                Pcode.gen(Pcode.getOPR(),0,9);      //OPR 0 9	次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
            else if(oper == "<")
                Pcode.gen(Pcode.getOPR(),0,10);     //OPR 0 10	次栈顶是否小于栈顶，退两个栈元素，结果值进栈
            else if(oper == ">=")
                Pcode.gen(Pcode.getOPR(),0,11);     //OPR 0 11	次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
            else if(oper == ">")
                Pcode.gen(Pcode.getOPR(),0,12);     //OPR 0 12	次栈顶是否大于栈顶，退两个栈元素，结果值进栈
            else if(oper == "<=")
                Pcode.gen(Pcode.getOPR(),0,13);     // OPR 0 13	次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
            
            
        }
        else
            Error(15);  //Missing <lop>
    }

    else if(word.value == "odd"){
        readline();
        Exp();
        Pcode.gen(Pcode.getOPR(),0,6);  //OPR 0 6	栈顶元素的奇偶判断，结果值在栈顶
    }

    else
        Error(14);  //Missing <lexp>

}

//<exp> → [+|-]<term>{<aop><term>}                                                        First(exp) = { + , - , l , d , ( }
//<aop> → +|-            
void Exp()
{
    string oper;
    if(word.value == "+" || word.value == "-"){
        oper = word.value;
        readline();
    }
    Term();
    
    if(oper == "-")
        Pcode.gen(Pcode.getOPR(),0,1); //  OPR 0 1	栈顶元素取反,说明是负数

    while(word.value == "+" || word.value == "-"){
        oper = word.value;
        readline();
        Term();
        if(oper == "+"){
            Pcode.gen(Pcode.getOPR(),0,2); //OPR 0 2	次栈顶与栈顶相加，退两个栈元素，结果值进栈
        }
        else if(oper == "-"){
            Pcode.gen(Pcode.getOPR(),0,3);  //OPR 0 3	次栈顶减去栈顶，退两个栈元素，结果值进栈
        }
    }
}

//<term> → <factor>{<mop><factor>}                                                        First(term) = { l , d , ( }
//<mop> → *|/ 
void Term()
{
    Factor();
    while(word.value == "*" || word.value == "/"){
        string oper = word.value;
        readline();
        Factor();
        if(oper == "*"){
            Pcode.gen(Pcode.getOPR(),0,4);       //OPR 0 4	次栈顶乘以栈顶，退两个栈元素，结果值进栈
        }
        else if(oper == "/")
        {
            Pcode.gen(Pcode.getOPR(),0,5);      // OPR 0 5	次栈顶除以栈顶，退两个栈元素，结果值进栈
        }
    }

}

//<factor>→<id>|<integer>|(<exp>)                                                         First(factor) = { l , d , ( }
void Factor()
{
    if(word.key == "identifier"){
        string name = word.value;
        if(!Table.isPreExistSTable(name,level)){     //判断因子中标识符在符号表中是否存在
            Error(25);
        }
        else{           //未定义变量的错误
            TableUnit temp = Table.getRow(Table.getNameRow(name));
            //通过 name ->行号 ->这一行所有的值
            if(temp.getType() == Table.getVar()){ //标识符是变量类型
                Pcode.gen(Pcode.getLOD(),level-temp.getLevel(),temp.getAddress());    
                //变量，LOD L  取变量（相对地址为a，层差为L）放到数据栈的栈顶
            }
            else if(temp.getType() == Table.getMyconst()){
                Pcode.gen(Pcode.getLIT(),0,temp.getValue());         //常量，LIT 0 a 取常量a放入数据栈栈顶
            }
            else{       //类型不一致的错误
                Error(22);
            }
        }
        readline();
    }

    else if(word.key == "constant"){
        Pcode.gen(Pcode.getLIT(),0,atoi(word.value.c_str()));
        readline();
    }

    else if(word.value == "("){
        readline();
        Exp();
        if(word.value == ")"){
            readline();
        }
        else
            Error(8);   //Missing ')'
    }

    else
        Error(16);  //Missing <factor>;
}



void Table_write()
{
	for(int i = 0; i < Table.getLength(); i++){
		TableUnit temp = Table.getRow(i);
		fp << temp.getType() << "  " << temp.getName() << "  " << temp.getLevel() << "  " << temp.getAddress() << "  " << temp.getValue() << "  " << temp.getSize() << endl;
	}
}

void Pcode_write()
{
    for(int i = 0; i < Pcode.getCodePtr(); i++){
        switch(Pcode.getPcodeArray()[i].getF()){
            case 0:
                fp << "LIT  ";
                break;
            case 1:
                fp << "OPR  ";
                break;
            case 2:
                fp << "LOD  ";
                break;
            case 3:
                fp << "STO  ";
                break;
            case 4:
                fp << "CAL  ";
                break;
            case 5:
                fp << "INT  ";
                break;
            case 6:
                fp << "JMP  ";
                break;
            case 7:
                fp << "JPC  ";
                break;
            case 8:
                fp << "RED  ";
                break;
            case 9:
                fp << "WRI  ";
                break;
        }
        fp << Pcode.getPcodeArray()[i].getL() << "  " << Pcode.getPcodeArray()[i].getA() << endl;
    }
}

void Pl0()
{
    Interpreter inter;
    inter.setPcode(Pcode);      //将目标代码传递给解释程序进行解释执行
    inter.interpreter();
}

int main()
{
    Prog();
    //Table_write();
    Pcode_write();
    Pl0();

    fp.close();
    fp1.close();
}

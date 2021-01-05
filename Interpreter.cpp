/*
    INT：为被调用的过程（包括主过程）在运行栈S中开辟数据区，这时A段为所需数据单元个数（包括三个连接数据）；L段恒为0。
    CAL：调用过程，这时A段为被调用过程的过程体（过程体之前一条指令）在目标程序区的入口地址。
    LIT：将常量送到运行栈S的栈顶，这时A段为常量值。
    LOD：将变量送到运行栈S的栈顶，这时A段为变量所在说明层中的相对位置。
    STO：将运行栈S的栈顶内容送入某个变量单元中，A段为变量所在说明层中的相对位置。
    JMP：无条件转移，这时A段为转向地址（目标程序）。
    JPC：条件转移，当运行栈S的栈顶的布尔值为假（0）时，则转向A段所指目标程序地址；否则顺序执行。
    OPR：关系或算术运算，A段指明具体运算，例如A=2代表算术运算“＋”；A＝12代表关系运算“>”等等。运算对象取自运行栈S的栈顶及次栈顶。

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
    OPR 0 10	次栈顶是否小于栈顶，退两个栈元素，结果值进栈
    OPR 0 11	次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
    OPR 0 12	次栈顶是否大于栈顶，退两个栈元素，结果值进栈
    OPR 0 13	次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
    OPR 0 14	栈顶值输出至屏幕,并且输出一个空格
    OPR 0 15	屏幕输出换行
    OPR 0 16	从命令行读入一个输入置于栈顶
    
    0   LIT 0 ，a 取常量a放入数据栈栈顶
    1   OPR 0 ，a 执行运算，a表示执行某种运算，具体是何种运算见上面的注释
    2   LOD L ，a 取变量（相对地址为a，层差为L）放到数据栈的栈顶
    3   STO L ，a 将数据栈栈顶的内容存入变量（相对地址为a，层次差为L）
    4   CAL L ，a 调用过程（转子指令）（入口地址为a，层次差为L）
    5   INT 0 ，a 数据栈栈顶指针增加a
    6   JMP 0 ，a无条件转移到地址为a的指令
    7   JPC 0 ，a 条件转移指令，转移到地址为a的指令
    8   RED L ，a 读数据并存入变量（相对地址为a，层次差为L）
    9   WRT 0 ，0 将栈顶内容输出
*/

#include <iostream>
#include <fstream>
#include <string>

#include "Pcode.cpp"

using namespace std;

class Interpreter 
{
    private:
        SPcode Instruction;      //指令寄存器I，存放当前要执行的代码
        APcode Pcode;    //存储器CODE，用来存放P的代码
        
        int Top;          //栈顶指示器T，指向数据栈STACK的栈，栈顶不存放元素
        int Base;          //基址寄存器B，存放当前运行过程的数据区在STACK中的起始地址
        int Ptr;          //程序地址寄存器，存放下一条要执行的指令的地址
        
        int Stack[1000];     //数据存储器STACK，初始值为1000

    public:
    
        void setPcode(APcode Pcode)
        {    //将Pcode中的代码用来初始化code
            this -> Pcode = Pcode;
        }
        
        APcode getCode()
        {
            return Pcode;
        }
        
        void interpreter()
        {
            Ptr = 0;//程序地址寄存器
            Base = 0;//基址寄存器
            Top = 0;//栈顶寄存器
            do{
                Instruction = Pcode.getPcodeArray()[Ptr];//Instruction表示每一行目标代码
                Ptr++;
                switch(Instruction.getF())//获取伪操作码
                {
                    case 0:		//LIT 0 a，取常量a放入数据栈栈顶
                        Stack[Top] = Instruction.getA();
                        Top++;
                        break;
                    case 1:     //OPR 0 a，执行运算，a表示执行某种运算
                        switch(Instruction.getA())
                        {
                            case 0:                 //opr,0,0 调用过程结束后，返回调用点并退栈
                                Top = Base;
                                Ptr = Stack[Base+2];//返回地址
                                Base = Stack[Base];//静态链
                                break;
                            case 1:                 //opr 0,1取反指令
                                Stack[Top-1] = -Stack[Top-1];
                                break;
                            case 2:                 //opr 0,2相加，将原来的两个元素退去，将结果置于栈顶
                                Stack[Top-2] = Stack[Top-1] + Stack[Top-2];
                                Top--;
                                break;
                            case 3:					//OPR 0,3 次栈顶减去栈顶，退两个栈元素，结果值进栈
                                Stack[Top-2] = Stack[Top-2] - Stack[Top-1];
                                Top--;
                                break;
                            case 4:    				//OPR 0,4次栈顶乘以栈顶，退两个栈元素，结果值进栈
                                Stack[Top-2] = Stack[Top-1] * Stack[Top-2];
                                Top--;
                                break;
                            case 5:					//OPR 0,5次栈顶除以栈顶，退两个栈元素，结果值进栈
                                Stack[Top-2] = Stack[Top-2] / Stack[Top-1];
                                Top--;
                                break;
                            case 6:                 //栈顶元素值奇偶判断，结果值进栈
                                Stack[Top-1] = Stack[Top-1] % 2;//(奇数为1)
                                break;
                            case 7:					
                                break;
                            case 8:					//次栈顶与栈项是否相等，退两个栈元素，结果值进栈
                                if(Stack[Top-1] == Stack[Top-2]){
                                    Stack[Top-2] = 1;
                                    Top--;
                                }
                                else{
                                    Stack[Top-2] = 0;
                                    Top--;
                                }
                                break;
                            case 9:					//次栈顶与栈项是否不等，退两个栈元素，结果值进栈
                                if(Stack[Top-1] != Stack[Top-2]){
                                    Stack[Top-2] = 1;
                                    Top--;
                                }
                                else{
                                    Stack[Top-2] = 0;
                                    Top--;
                                }
                                break;
                            case 10:				//次栈顶是否小于栈顶，退两个栈元素，结果值进栈
                                if(Stack[Top-2] < Stack[Top-1]){
                                    Stack[Top-2] = 1;
                                    Top--;
                                }
                                else{
                                    Stack[Top-2] = 0;
                                    Top--;
                                }
                                break;
                            case 11:				//次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
                                if(Stack[Top-2] >= Stack[Top-1]){
                                    Stack[Top-2] = 1;
                                    Top--;
                                }
                                else{
                                    Stack[Top-2] = 0;
                                    Top--;
                                }
                                break;
                            case 12:				//次栈顶是否大于栈顶，退两个栈元素，结果值进栈
                                if(Stack[Top-2] > Stack[Top-1]){
                                    Stack[Top-2] = 1;
                                    Top--;
                                }
                                else{
                                    Stack[Top-2] = 0;
                                    Top--;
                                }
                                break;
                            case 13:				//次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
                                if(Stack[Top-2] <= Stack[Top-1]){
                                    Stack[Top-2] = 1;
                                    Top--;
                                }
                                else{
                                    Stack[Top-2] = 0;
                                    Top--;
                                }
                                break;
                            case 14:				//栈顶值输出至屏幕
                                cout << Stack[Top-1] << "  ";     //便于观察，再输出一个空格
                                break;
                            case 15:				//屏幕输出换行
                                cout << endl;
                                break;
                            case 16:				//从命令行读入一个置于栈顶
                                cin >> Stack[Top];
                                Top++;
                                break;
                        }
                        break;
                    case 2:     //LOD L ，a 取变量（相对地址为a，层差为L）放到数据栈的栈顶
                        Stack[Top] = Stack[Instruction.getA() + getBase(Base,Instruction.getL())];
                        Top++; 
                        break;
                    case 3:     //STO L ，a 将数据栈栈顶的内容存入变量（相对地址为a，层次差为L）
                        Stack[Instruction.getA() + getBase(Base,Instruction.getL())] = Stack[Top-1];
                        break;
                    case 4:     //CAL L ，a 调用过程（转子指令）（入口地址为a，层次差为L）
                        Stack[Top] = Base;//静态链，直接外层过程
                        Stack[Top+1] = getBase(Base,Instruction.getL());//动态链，调用前运行过程
                        Stack[Top+2] = Ptr;//返回地址，下一条要执行的
                        Base = Top;
                        Ptr = Instruction.getA();
                        break;
                    case 5:     //INT 0 ，a 数据栈栈顶指针增加a
                        Top = Top + Instruction.getA();
                        break;
                    case 6:     //JMP 0 ，a无条件转移到地址为a的指令
                        Ptr = Instruction.getA();
                        break;
                    case 7:     //JPC 0 ，a 条件转移指令，转移到地址为a的指令
                        if(Stack[Top-1] == 0)
                            Ptr = Instruction.getA();
                        break;
                    case 8:     //RED L ，a 读数据并存入变量（相对地址为a，层次差为L）
                        cin >> Stack[Top];
                        Top++;
                        Stack[Instruction.getA() + getBase(Base,Instruction.getL())] = Stack[Top-1];
                        break;
                    case 9:     //WRT 0 ，0 将栈顶内容输出
                        cout << Stack[Top-1] << endl;
                        break;
                }
            }while(Ptr != 0);
        }
        
        int getBase(int now,int lev)
        {
            int pre = now;      
            while(lev > 0)//当存在层差时寻找非局部变量
            {
                pre = Stack[pre + 1];//直接外层的活动记录首地址
                lev--;
            }
            return pre;
        }
};

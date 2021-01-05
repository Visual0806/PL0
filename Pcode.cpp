#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SPcode
{
    private:
        // F段代表伪操作码
        // L段代表调用层与说明层的层差值
        // A段代表位移量（相对地址）
        int F;
        int L;
        int A;
    public:
        SPcode()
        {
            F=-1;
            L=-1;
            A=-1;
        }

        void Set(int f,int l,int a)
        {
            F=f;
            L=l;
            A=a;
        }

        void setF(int f)
        {
            F=f;
        }

        void setL(int l)
        {
            L=l;
        }
        
        void setA(int a)
        {
            A=a;
        }
        
        int getF()
        {
            return F;
        }
        
        int getL()
        {
            return L;
        }
        
        int getA()
        {
            return A;
        }
};

class APcode 
{
    private:
        int LIT = 0; //LIT 0 ，a 取常量a放入数据栈栈顶
        int OPR = 1; //OPR 0 ，a 执行运算，a表示执行某种运算，具体是何种运算见上面的注释
        int LOD = 2; //LOD L ，a 取变量（相对地址为a，层差为L）放到数据栈的栈顶
        int STO = 3; //STO L ，a 将数据栈栈顶的内容存入变量（相对地址为a，层次差为L）
        int CAL = 4; //CAL L ，a 调用过程（转子指令）（入口地址为a，层次差为L）
        int INT = 5; //INT 0 ，a 数据栈栈顶指针增加a
        int JMP = 6; //JMP 0 ，a 无条件转移到地址为a的指令
        int JPC = 7; //JPC 0 ，a 条件转移指令，转移到地址为a的指令
        int RED = 8; //RED L ，a 读数据并存入变量（相对地址为a，层次差为L）
        int WRT = 9; //WRT 0 ，0 将栈顶内容输出

        int MAX_PCODE=10000;
        int codePtr=0;          //指向下一条将要产生的代码的在APcode中的地址


        SPcode pcodeArray[10000];//建立一个目标代码数组

    public:
        APcode()
        {
            for(int i=0;i<MAX_PCODE;i++){
                pcodeArray[i].Set(-1,-1,-1);
            }
        }

        //产生目标代码
        void gen(int f,int l,int a)
        {
            pcodeArray[codePtr].setF(f);
            pcodeArray[codePtr].setL(l);
            pcodeArray[codePtr].setA(a);
            codePtr++;
        }

        int getCodePtr()
        {
            return codePtr;
        }

        int getOPR()
        {
            return OPR;
        }

        int getLIT() 
        {
            return LIT;
        }

        int getLOD() 
        {
            return LOD;
        }

        int getSTO()
        {
            return STO;
        }

        int getCAL() 
        {
            return CAL;
        }

        int getINT() 
        {
            return INT;
        }

        int getJMP() 
        {
            return JMP;
        }

        int getJPC() 
        {
            return JPC;
        }

        int getRED() 
        {
            return RED;
        }

        int getWRT() 
        {
            return WRT;
        }

        SPcode* getPcodeArray() 
        {
            return pcodeArray;
        }
};


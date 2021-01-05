#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class TableUnit    
{
    private:
        int type;       //表示常量、变量或过程
        int value;      //表示常量或变量的值
        int level;      //嵌套层次，最大应为3
        int address;    //相对于所在嵌套过程基地址的地址
        int size;       //表示常量，变量，过程所占的大小
        string name;    //变量、常量或过程名

    public:
    
        int getType()
        {
            return type;
        }

        int getValue()
        {
            return value;
        }

        int getLevel()
        {
            return level;
        }

        int getAddress()
        {
            return address;
        }

        int getSize()
        {
            return size;
        }

        string getName()
        {
            return name;
        }

        void setType(int t)
        {
            type=t;
        }

        void setValue(int v)
        {
            value=v;
        }

        void setLevel(int L)
        {
            level=L;
        }

        void setAddress(int a)
        {
            address=a;
        }

        void setSize(int s)
        {
            size=s;
        }

        void setName(string s)
        {
            name=s;
        }
};


class SymTable{      
    private:
        int rowMax=255;             //最大表长
        int valueMax=100000;        //最大常量或变量值
        int levelMax=3;             //最深嵌套层次
        int addressMax=255;         //最大地址数
    
        int myconst=1;              //常量类型用1表示
        int var=2;                  //变量类型用2表示
        int proc=3;                 //过程类型用3表示

        TableUnit table[255];       //TableUnit是符号表中的每一项 
        int tablePtr = 0;           //tablePtr指向符号表中已经填入值最后一项的下一项
        int length = 0;             //length表示符号表中填入了多少行数据

    public:
    
        void setTablePtr(int tablePtr) 
        {
            this -> tablePtr = tablePtr;
        }

        SymTable()
        {
            for(int i=0;i<rowMax;i++)
            {
                table[i].setAddress(0);
                table[i].setLevel(0);
                table[i].setSize(0);
                table[i].setType(0);
                table[i].setValue(0);
                table[i].setName("");

            }
        }
        
        int getMyconst() 
        {
            return myconst;
        }

        int getVar() 
        {
            return var;
        }

        int getProc() 
        {
            return proc;
        }

        int getLength()
        {
            return length;
        }

        TableUnit getRow(int i)     //获取符号表中第i个符号单元
        {
            return table[i];
        }
     
        void enterConst(string name,int level,string value,int address)
        {
            int value_int = atoi(value.c_str());
            table[tablePtr].setName(name);
            table[tablePtr].setLevel(level);
            table[tablePtr].setValue(value_int);
            table[tablePtr].setAddress(address);
            table[tablePtr].setType(myconst);
            table[tablePtr].setSize(4);
            tablePtr++;
            length++;
        }

        void enterVar(string name,int level,int address)
        {
            table[tablePtr].setName(name);
            table[tablePtr].setLevel(level);
            table[tablePtr].setAddress(address);
            table[tablePtr].setType(var);
            table[tablePtr].setSize(0);
            tablePtr++;
            length++;
        }

        void enterProc(string name,int level,int address)
        {
            table[tablePtr].setName(name);
            table[tablePtr].setLevel(level);
            table[tablePtr].setAddress(address);
            table[tablePtr].setType(proc);
            table[tablePtr].setSize(0);
            tablePtr++;
            length++;
        }

        //在lev层之前，包括lev层，名字为name的变量、常量、或过程是否被定义
        bool isPreExistSTable(string name,int lev)      
        {
            for(int i = 0; i < length; i++){
                if(table[i].getName() == name){
                    if(table[i].getLevel() == lev){
                        return true;
                    }
                    else{
                        while(lev >= 0){
                            lev = lev - 1;
                            if(table[i].getLevel() == lev){
                                return true;
                            }      
                        }
                    }
                }
            }
            return false;
        }

        //在lev层，名字为name的变量、常量、或过程是否被定义
        bool isNowExistSTable(string name,int lev)
        {
            for(int i = 0; i < length; i++){
                if(table[i].getName() == name && table[i].getLevel() == lev){
                    return true;
                }
            }
            return false;
        }

        //返回符号表中名字为name的符号表项的索引
        int  getNameRow(string name)
        {
            for(int i = length - 1; i >= 0; i--){
                if(table[i].getName() == name){
                    return i;
                }
            }
            return -1;        
        }

        int getTablePtr()
        {
            return tablePtr;
        }

        TableUnit* getAllTable()
        {
            return table;
        }

        //查找本层的过程在符号表中的位置
        int getLevelProc()
        {
            for(int i = length - 1; i >= 0; i--){
                if(table[i].getType() == proc){
                    return i;
                }
            }
            return -1;
        }

        int clearTheFowllowing(int begin)
        {
            for(int i = begin; i < 255; i++){
                table[i].setAddress(0);
                table[i].setLevel(0);
                table[i].setSize(0);
                table[i].setType(0);
                table[i].setValue(0);
                table[i].setName("");
            }
            return 0;
        }
};
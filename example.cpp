#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Unit {
public:
    std::string value;
    std::string key;
    int line;
    int column;

    void print();
};

void Unit::print() {
    std::cout << "-----------------" << std::endl;
    std::cout << "Value: " << value << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "Position: [" << line << "," << column << "]" << std::endl;
    std::cout << "-----------------" << std::endl;
}

Unit unit, former_unit;
int errorStack[100] = {-1};
int *errorType = errorStack;
bool error = false;
fstream gaSource;
fstream gaOutput;

string line;
string::iterator itLine;

/**
 * Read file line by line.
 * @return Global variables: unit.value & unit.key.
 */
void ReadLine();

/**
 * Throw an error.
 * @param type Error type.
 */
void ThrowError(int type);

/**
 * <factor>→<id>|<integer>|(<exp>)
 */
void Factor();

/**
 * <term> → <factor>{<mop><factor>}
 */
void Term();

/**
 * <exp> → [+|-]<term>{<aop><term>}
 */
void Exp();

/**
 * <lexp> → <exp> <lop> <exp>|odd <exp>
 */
void Lexp();

/**
 * <statement> → <id> := <exp>
               |if <lexp> then <statement>[else <statement>]
               |while <lexp> do <statement>
               |call <id>[（<exp>{,<exp>}）]
               |<body>
               |read (<id>{，<id>})
               |write (<exp>{,<exp>})
 */
void Statement();

/**
 * <body> → begin <statement>{;<statement>}end
 */
void Body();

/**
 * <proc> → procedure <id>（<id>{,<id>}）;<block>{;<proc>}
 */
void Proc();

/**
 * <vardecl> → var <id>{,<id>};
 * <id> → l{l|d}
 * l represent letter.
 */
void Vardecl();

/**
 * Const variables declaration.
 * <condecl> → const <const>{,<const>};
 * <const> → <id>:=<integer>
 * <id> → l{l|d}
 * l represent letter.
 */
void Condecl();

/**
 * <block> → [<condecl>][<vardecl>][<proc>]<body>
 */
void Block();

/**
 * <prog> → program <id>; <block>
 */
void Prog();

/**
 * Open IO files.
 */
void OpenFile();

/**
 * Close IO files.
 */
void CloseFile();

/**
 * Print the error stack out.
 */
void PrintErrorStack();

/**
 * Grammar analysis main program
 * @return
 */
int GA();

void ReadLine()
{
    if(getline(gaSource, line))
    {
        itLine = line.begin();
        while (*itLine == '#' || line.empty())
        {
            getline(gaSource, line);
            itLine = line.begin();
        }
        former_unit = unit;
        istringstream iss(line);
        if(*itLine == '^')
        {
            char ch;
            iss >> ch;
            iss >> unit.value;
            iss >> unit.key;
            iss >> unit.line;
            iss >> unit.column;
            getline(gaSource, line);
        }
        else
        {
            iss >> unit.value;
            iss >> unit.key;
            iss >> unit.line;
            iss >> unit.column;
        }
    }
}

void ThrowError(int type)
{
    error = true;
    *errorType = type;
    errorType++;
    switch (type)
    {
        case 0:
            cout << "[Grammar ERROR] " << " [" << unit.line << "," << unit.column << "] " << "Spell error \"program\"" << endl;
            break;
        case 1:
            cout << "[Grammar ERROR] " << " [" << unit.line << "," << unit.column << "] " << "Missing identifier after \"program\"" << endl;
            break;
        case 2:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " <<"Missing end character \";\"" << endl;
            break;
        case 3:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"const\"" << endl;
            break;
        case 4:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing assignment operation" << endl;
            break;
        case 5:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing assignment integer" << endl;
            break;
        case 6:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"var\"" << endl;
            break;
        case 7:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \",\"" << endl;
            break;
        case 8:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Cannot resolve type \"" << unit.value << "\"" << endl;
            break;
        case 9:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"procedure\"" << endl;
            break;
        case 10:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"procedure\"" << endl;
            break;
        case 11:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"procedure\"" << endl;
            break;
        case 12:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"begin\"" << endl;
            break;
        case 13:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"end\"" << endl;
            break;
        case 14:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing statement identifier" << endl;
            break;
        case 15:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"then\"" << endl;
            break;
        case 16:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"do\"" << endl;
            break;
        case 17:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"call\"" << endl;
            break;
        case 18:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"call\"" << endl;
            break;
        case 19:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"read\"" << endl;
            break;
        case 20:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"read\"" << endl;
            break;
        case 21:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"write\"" << endl;
            break;
        case 22:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing the compare operator" << endl;
            break;
        case 23:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses" << endl;
            break;
        case 24:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Wrong factor" << endl;
            break;
        case 25:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " << "Extra semicolon ';' before 'end'" << endl;
            break;
        case 26:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " << "Missing comma ',' in 'var'" << endl;
            break;
        case 27:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " << "Missing comma ',' in 'const'" << endl;
            break;
        default:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Unknown error" << endl;
            break;
    }
}

void Factor()
{
    if(unit.key == "ID" || *errorType == 24)
    {
        if(unit.key != "ID" && unit.key != "INT" && unit.value == "(" && *errorType == 24)
            errorType++;
        ReadLine();
    }
    else if(unit.key == "INT")
    {
        ReadLine();
    }
    else if(unit.key == "SOP" && unit.value == "(")
    {
        ReadLine();
        Exp();
        if(unit.key == "SOP" && unit.value == ")" || *errorType == 23)
        {
            if(unit.value != ")" && *errorType == 23)
                errorType++;
            ReadLine();
        }
        else
        {
            ThrowError(23);
        }
    }
    else
    {
        ThrowError(24);
    }
}

void Term()
{
    Factor();
    if(unit.value == "*" || unit.value == "/")
    {
        ReadLine();
        Factor();
    }
}

void Exp()
{
    if(unit.value == "+" || unit.value == "-")
        ReadLine();
    Term();
    if(unit.value == "+" || unit.value == "-")
    {
        ReadLine();
        Term();
    }
}

void Lexp()
{
    if(unit.value == "odd")
    {
        ReadLine();
        Exp();
    }
    else
    {
        Exp();
        if(unit.key == "COP" || *errorType == 22)
        {
            if(unit.key != "COP" && *errorType == 22)
                errorType++;
            ReadLine();
            Exp();
        }
        else
        {
            ThrowError(22);
        }
    }
}

void Statement()
{
    if(unit.key == "RESERVED" && unit.value == "if")
    {
        ReadLine();
        Lexp();
        if(unit.key == "RESERVED" && unit.value == "then" || *errorType == 15)
        {
            if(*errorType == 15 && unit.value != "then")
                errorType++;
            else
                ReadLine();
            Statement();
            if(unit.key == "RESERVED" && unit.value == "else")
            {
                ReadLine();
                Statement();
            }
        }
        else
        {
            ThrowError(16);
        }
    }
    else if(unit.key == "RESERVED" && unit.value == "while")
    {
        ReadLine();
        Lexp();
        if(unit.key == "RESERVED" && unit.value == "do" || *errorType == 16)
        {
            if(*errorType == 16 && unit.value != "do")
                errorType++;
            else
                ReadLine();
            Statement();
        }
        else
        {
            ThrowError(16);
        }
    }
    else if(unit.key == "RESERVED" && unit.value == "call")
    {
        ReadLine();
        if(unit.key == "ID" || *errorType == 17)
        {
            if(*errorType == 17 && unit.key != "ID")
                errorType++;
            ReadLine();
            if(unit.key == "SOP" && unit.value == "(")
            {
                ReadLine();
                Exp();
                while (unit.key == "SOP" && unit.value == ",")
                {
                    ReadLine();
                    Exp();
                }
                if(unit.value == ",")
                    ReadLine();
                if(unit.key == "SOP" && unit.value == ")" || *errorType == 18)
                {
                    if(*errorType == 18 && unit.value != ")")
                        errorType++;
                    else
                        ReadLine();
                }
                else
                {
                    ThrowError(18);
                }
            }
        }
        else
        {
            ThrowError(17);
        }
    }
    else if(unit.key == "RESERVED" && unit.value == "read")
    {
        ReadLine();
        if(unit.key == "SOP" && unit.value == "(" || *errorType == 20)
        {
            if(*errorType == 20 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            if(unit.key == "ID" || *errorType == 19)
            {
                if(*errorType == 19 && unit.key != "ID")
                    errorType++;
                else
                    ReadLine();
                while (unit.key == "SOP" && unit.value == ",")
                {
                    ReadLine();
                    if(unit.key == "ID" || *errorType == 19)
                    {
                        if (*errorType == 19 && unit.key != "ID")
                            errorType++;
                        else
                            ReadLine();
                    }
                    else
                    {
                        ThrowError(19);
                    }
                }
                if(unit.value == ",")
                    ReadLine();
                if(unit.key == "SOP" && unit.value == ")" || *errorType == 20)
                {
                    if(*errorType == 20 && unit.value != ")")
                        errorType++;
                    else
                        ReadLine();
                }
                else
                {
                    ThrowError(20);
                }
            }
            else
            {
                ThrowError(19);
            }
        }
        else
        {
            ThrowError(20);
        }
    }
    else if(unit.key == "RESERVED" && unit.value == "write")
    {
        ReadLine();
        if(unit.key == "SOP" && unit.value == "(" || *errorType == 21)
        {
            if(*errorType == 21 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            Exp();
            while (unit.key == "SOP" && unit.value == ",")
            {
                ReadLine();
                Exp();
            }
            if(unit.key == "SOP" && unit.value == ")" || *errorType == 21)
            {
                if(*errorType == 21 && unit.value != ")")
                    errorType++;
                else
                    ReadLine();
            }
            else
            {
                ThrowError(21);
            }
        }
        else
        {
            ThrowError(21);
        }
    }
    else if(unit.key == "ID")
    {
        ReadLine();
        if(unit.key == "AOP" && unit.value == ":=" || *errorType == 4)
        {
            if(*errorType == 4 && unit.value != ":=")
                errorType++;
            ReadLine();
            Exp();
        }
        else
        {
            ThrowError(4);
        }
    }
    else
    {
        Body();
    }
}

void Body()
{
    if (unit.key == "RESERVED" && unit.value == "begin" || *errorType == 12)
    {
        if (*errorType == 12 && unit.value != "begin")
            errorType++;
        ReadLine();
        Statement();
        while (unit.key == "EOP" && unit.value == ";" && !error)
        {
            ReadLine();
            if (unit.value == "end")
            {
                if (*errorType != 25)
                    ThrowError(25);
                break;
            }
            Statement();
        }
        if(*errorType == 25)
        {
            errorType++;
            ReadLine();
        }
        if (unit.key == "RESERVED" && unit.value == "end" || *errorType == 13 || error)
        {
            if (*errorType == 13 && unit.value != "end")
                errorType++;
            if (!error)
                ReadLine();
        }
        else
        {
            ThrowError(13);
        }
    }
    else
    {
        ThrowError(12);
    }
}

void Proc()
{
    if(unit.key == "ID" || *errorType == 9) {
        if(*errorType == 9 && unit.key != "ID") errorType++; else ReadLine();
//        ReadLine();
        if (unit.key == "SOP" && unit.value == "(" || *errorType == 10){
            if (*errorType == 10 && unit.value != "(") errorType++;
            ReadLine();
            if (unit.key == "ID" || *errorType == 11) {
                if (*errorType == 11 && unit.key != "ID") errorType++;
                ReadLine();

                while (unit.value == "," && unit.key == "SOP"){

                    ReadLine();
                    if (unit.key == "ID"|| *errorType == 11){
                        if(unit.key != "ID") errorType++;
                        ReadLine();

                    } else{
                        ThrowError(11); // Missing id
                    }
                    if(error) break;
                }
                if (unit.value == ",") ReadLine();
                if (unit.key == "SOP" && unit.value == ")" || *errorType == 10){
                    if (*errorType == 10 && unit.value != ")") errorType++;
                    ReadLine();

                    if (unit.key == "EOP" && unit.value == ";" || *errorType == 2) {
                        if (*errorType == 2 && unit.value != ";") errorType++; else ReadLine();
//                        ReadLine();
                        Block();

                        while (unit.key == "EOP" && unit.value == ";") {
                            ReadLine();
                            Proc();
                        }

//                        ReadLine();
                        // End Proc
                    } else {
                        ThrowError(2);
                    }
                } else {
                    ThrowError(10);
                }

            } else {
                ThrowError(11);
            }
        } else {
            ThrowError(10);
        }
    } else {
        ThrowError(9);
    }
}

void Vardecl()
{
    if (unit.key == "ID") {
        ReadLine();
        while (unit.value == "," && unit.key == "SOP" || *errorType == 26) {
            if (*errorType == 26 && unit.value != ",") errorType++; else ReadLine();
//            ReadLine();
            if (unit.key == "ID") {
                ReadLine();
            } else {
                ThrowError(7);// Missing ID
            }
        }
        if(unit.key == "ID") {
            ThrowError(26);
        }
        if (unit.value == ";" && unit.key == "EOP" || *errorType == 2) {
            if (*errorType == 2 && unit.value != ";") errorType++; else ReadLine();
//            ReadLine();
            // Finish var declaration.
        } else {
            ThrowError(2); // Missing EOP
        }
    } else {
        ThrowError(6); // Missing ID
    }
}


void Condecl() {
    if (unit.key == "ID" || *errorType == 3){
        if(unit.key != "ID") errorType++;
        ReadLine();
        if (unit.value == ":=" && unit.key == "AOP" || *errorType == 4) {
            if(!(unit.value == ":=" && unit.key == "AOP")) errorType++; else ReadLine();
//            ReadLine();
            if (unit.key == "INT" || *errorType == 5) {
                if(unit.key != "INT" && *errorType == 5) {
                    errorType++;
                    while (unit.value != "," && unit.value != ";") {
                        ReadLine();
                        if (unit.key == "ID") break;// For missing comma
                    }
                } else {
                    ReadLine();
                }
//                ReadLine();

                while (unit.value == "," && unit.key == "SOP" || *errorType == 27){
                    if (*errorType == 27 && unit.value != ",") errorType++; else ReadLine();
//                    ReadLine();
                    if (unit.key == "ID"|| *errorType == 3){
                        if(unit.key != "ID") errorType++; else ReadLine();
//                        ReadLine();
                        if (unit.value == ":=" && unit.key == "AOP"|| *errorType == 4){
                            if(!(unit.value == ":=" && unit.key == "AOP")) errorType++; else ReadLine();
//                            ReadLine();
                            if (unit.key == "INT"|| *errorType == 5) {
                                if(unit.key != "INT") {
                                    errorType++;
                                    while (unit.value != "," && unit.key != ";") {
                                        ReadLine();
                                    }
                                } else {
                                    ReadLine();
                                }
//                                ReadLine();
                            } else {
                                ThrowError(5); // Missing INT
                            }
                        } else {
                            ThrowError(4); // Missing AOP
                        }
                    } else{
                        ThrowError(3); // Missing id
                    }
                    if(error) break;
                }

                if(unit.key == "ID" && !error) {
                    ThrowError(27);// Missing comma
                }
                while (unit.value == ",") ReadLine();
                if (unit.value == ";" && unit.key == "EOP"|| *errorType == 2 || error) {
                    if(!(unit.value == ";" && unit.key == "EOP") && !error) errorType++; else ReadLine();
//                    ReadLine();
                    // Finish const declaration.
                } else {
                    ThrowError(2); // Missing EOP
                }
            } else {
                ThrowError(5); // Missing INT
            }
        } else {
            ThrowError(4); // Missing AOP
        }
    } else{
        ThrowError(3); // Missing id
    }
}

void Block() {
    if (unit.value == "const" && unit.key == "RESERVED" && !error) {
        ReadLine();
        Condecl();
    } else if (unit.key == "ID" && *errorType != 8 && !error){
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8 && !error) {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "var" && unit.key == "RESERVED" && !error) {
        ReadLine();
        Vardecl();
    } else if (unit.key == "ID" && *errorType != 8 && !error){
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8 && !error) {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "procedure" && unit.key == "RESERVED" && !error) {
        ReadLine();
        Proc();
    } else if (unit.key == "ID" && *errorType != 8 && !error){
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8 && !error) {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    }

    if (!error){
        Body();
    }
}

void Prog() {
    ReadLine();
    if ((unit.value == "program" && unit.key == "keyword") || *errorType == 0){
        if (*errorType == 0) errorType++;
        ReadLine();
        if (unit.key == "ID" || *errorType == 1) {
            if (*errorType == 1) errorType++; else ReadLine();
//            ReadLine();
            if ((unit.value == ";" && unit.key == "EOP") || *errorType == 2) {
                if (*errorType == 2 && unit.value != ";") errorType++; else ReadLine();
                Block();
            } else {
                ThrowError(2);
            }
        } else {
            ThrowError(1);
        }
    } else {
        ThrowError(0);
    }

}

void OpenFile() {
    gaSource.open("la_output", ios::in); // Read file
    gaOutput.open("ga_output", ios::out | ios::trunc); // Write file

    if (!gaSource.is_open()) {
        cout << "Cannot open the gaSource file!\a" << endl;
        exit(1);
    }
    if (!gaOutput.is_open()) {
        cout << "Cannot open the gaOutput file!\a" << endl;
    } else {
        // Header of the file (DateTime & File name & Lang set)

        time_t rawtime;
        struct tm * timeinfo;

        time (&rawtime);
        timeinfo = localtime (&rawtime);

        gaOutput << "# Grammar Analysis Result" << endl;
        gaOutput << "# Generated Time: " << asctime(timeinfo);
        gaOutput << "# Language Set: PL/0" << endl;
        gaOutput << endl;
    }
}

void CloseFile() {
    gaSource.close();
    gaOutput.close();
}

void PrintErrorStack() {
    cout << "[DEBUG] Error Stack" << endl;
    cout << endl << "|<<<< ERROR STACK <<<<<" << endl;
    int *p;
    cout << "|";
    for (p = errorStack; p != errorType; p++) {
        cout << *p << " ";
    }
    cout << endl << "|<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

int GA() {

    OpenFile();
    Prog();
    CloseFile();
    while(error){
        error = false;
        errorType = errorStack;
        OpenFile();
        Prog();
        CloseFile();
    }

//    PrintErrorStack();
    return 0;
}
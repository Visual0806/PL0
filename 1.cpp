/*                                                                                      First集
<prog> → program <id>;<block>                                                           First(prog) = { program }
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
               |write (<exp>{,<exp>})
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
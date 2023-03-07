# Грамматика языка
### Основные определения грамматики

***program*** ::= \<function> \<program> | \<function>

***letter*** ::= 'a' | ...| 'z' | 'A' | ... | 'Z' | '_'

***digit*** ::= '0' | ... |'9'

***name*** ::= \<letter> { \<letter> | \<digit> }

***number*** ::= \<digit> { \<digit> } { '.' \<digit> { \<digit> } }

***block*** ::= eps | \<action> \<block>

***action*** ::= \<if> | \<while> | \<for> | \<call function> ';' | \<equal> ';' | \<new variable> ';' | \<print> ';' | \<input> ';' | \<return> ';'

### Объявление переменных

***type*** ::= 'int' | 'bool' | 'float' | array '\<' \<type> '>'

***variable init*** ::= \<name> ( '=' \<expression> )

***array indexes*** ::= '\[' \<expression> '\]' | '\[' \<expression> '\]' \<array indexes>

***array init*** ::= \<name> \<array indexes> ( '=' \<expression> )

***new variable*** ::= \<type> \<variable init> {',' \<variable init>} | 'array' '\<' \<type> '>' \<array init>  { ',' \<array init> }

### Синтаксис функций

***function type*** ::= \<type> | 'void'

***function*** ::= \<function type> \<name>'(' (\<parameters> | eps) ')' '{' \<block> '}' 

***function value*** ::= \<type> \<name> {',' \<name>}

***parameters*** ::= \<function value> | \<function value> '.' \<parameters> 

### Выражения
  
***equal*** ::= \<variable> <=> \<expression>

***variable*** ::= \<name> | \<name> \<array indexes>

***call function*** ::= \<name> '(' eps | ( \<expression> { ',' \<expression> } ) ')'

***=*** ::= '=' | '+=' | '-=' | '\*=' | '/=' | %= | |= | &= | ^= 

***v*** ::= '=='  '!=' | '>' | '<' | '>=' | '<=' 

***expression*** ::= \<expression0> { 'and' \<expression0> }

***expression0*** ::= \<expression1> { 'or' \<expression1> }

***expression1*** ::= \<expression2> | \<expression2> \<v> \<expression2>

***expression2*** ::= \<expression3> { '&' \<expression3> | '|' \<expression3> | '^' \<expression3> }

***expression3*** ::= \<expression4> { '+' \<expression4> | '-' \<expression4> }

***expression4*** ::= \<expression5> { '\*' \<expression5> | '/' \<expression5> | '%' \<expression5> }

***expression5*** ::= \<expression6> | '-' \<expression6> | '!' \<expression6> | '~' \<expression6>

***expression6*** ::= '(' \<expression> ')' | \<variable> | \<number> | \<call function> | \<len> | \<deindexing of function>

***len*** ::= 'len' '(' \<name> \<array indexes> ')'

***deindexing of function*** ::= \<call function> \<array indexes>

### Основные конструкции

***if*** ::= 'if' '(' \<expression> ')' '{' \<block> '}' (eps | 'else' '{' \<block> '}')

***while*** ::= 'while' '(' \<expression> ')' '{' \<block> '}' 

***for variable init*** ::= \<new variable> | \<new variable> '.' \<for variable init>
  
***for*** ::= 'for' '(' eps | \<for  variable init> ';' \<expression> ';' eps | \<equal> {';' \<equal>} ')' '{' \<block> '}'

***input*** ::= 'input' '(' \<variable> { ',' \<variable> } ')'

***print*** ::= 'print' '(' \<expression> { ',' \<expression> } ')'

***return*** ::= 'return' '(' (eps | expression) ')'

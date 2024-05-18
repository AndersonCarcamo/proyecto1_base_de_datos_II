%{
#include <cstdlib>
#include "Parser1.hpp"
using namespace sql_parser;
%}

%option reentrant noyywrap

%%

"create"          return Parser::token::CREATE;
"table"           return Parser::token::TABLE;
"from"            return Parser::token::FROM;
"file"            return Parser::token::FILE;
"using"           return Parser::token::USING;
"index"           return Parser::token::INDEX;
"select"          return Parser::token::SELECT;
"where"           return Parser::token::WHERE;
"between"         return Parser::token::BETWEEN;
"insert"          return Parser::token::INSERT;
"into"            return Parser::token::INTO;
"values"          return Parser::token::VALUES;
"delete"          return Parser::token::DELETE;
"*"               return Parser::token::MULT;
"("               return Parser::token::LPAREN;
")"               return Parser::token::RPAREN;
"="               return Parser::token::EQ;
","               return Parser::token::COMMA;
[A-Za-z_][A-Za-z_0-9]* return Parser::token::IDENTIFIER;
\"[^\"]*\"        { yylval->assign(yytext + 1, yyleng - 2); return Parser::token::STRING_LITERAL; }
[0-9]+            { yylval->assign(yytext); return Parser::token::INTEGER_LITERAL; }
\n                return Parser::token::EOL;
[ \t\r]+          /* ignore whitespace */;
.                 { std::cerr << "Unexpected character: " << yytext << std::endl; return -1; }

%%

int main() {
    yyscan_t scanner;
    yylex_init(&scanner);
    sql_parser::Parser parser(scanner);
    parser.parse();
    yylex_destroy(scanner);
}